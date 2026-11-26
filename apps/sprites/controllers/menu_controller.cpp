#include "apps/sprites/controllers/menu_controller.h"
#include "apps/apps_container.h"
#include "apps/code/script_name_cell.h"
#include "apps/sprites/controllers/cells/sprite_name_cell.h"
#include "escher/button_row_controller.h"
#include "escher/container.h"
#include "escher/i18n.h"
#include "escher/metric.h"
#include "escher/responder.h"
#include "escher/text_field.h"
#include <apps/i18n.h>

namespace Sprites
{

    MenuController::MenuController(
        Responder * parentResponder,
        App * spriteDelegate,
        SpriteStore * store
        // ButtonRowController * footer
    ) : 
        ViewController(parentResponder),
        // ButtonRowDelegate(nullptr, footer),
        m_spriteStore(store),
        m_selectableTableView(this, this, this, this),
        m_paramController(nullptr, I18n::Message::SpriteOptions, this)
    {
        m_selectableTableView.setMargins(0);
        m_selectableTableView.setDecoratorType(ScrollView::Decorator::Type::None);
        // FIXME: Change for add sprite
        m_addNewSpriteCell.setMessage(I18n::Message::AddSprite);
        for (size_t i = 0; i < k_maxNumberOfDisplayableSpriteCells; ++i) {
            m_spriteCells[i].setParentResponder(&m_selectableTableView);
            m_spriteCells[i].textField()->setDelegates(nullptr, this);
        }
    }

    StackViewController * MenuController::stackViewController(void)
    {
        // TODO: Return actual stackview controller
        return static_cast<StackViewController *>(parentResponder());
    }

    void MenuController::willExitResponderChain(Responder * nfr)
    {
        int selectedRow = m_selectableTableView.selectedRow();
        int selectedColumn = m_selectableTableView.selectedColumn();
        if (selectedRow >= 0 && selectedRow < m_spriteStore->NumberOfSprites() && selectedColumn == 0) {
            TextField * tf = static_cast<SpriteNameCell *>(m_selectableTableView.selectedCell())->textField();
            if (tf->isEditing()) {
                tf->setEditing(false);
                privateTextFieldDidAbortEditing(tf, false);
            }
        }
    }

    /* ViewController */
    View * MenuController::view(void) 
    {
        return &m_selectableTableView;
    }

    bool MenuController::handleEvent(Ion::Events::Event evt)
    {
        if (evt == Ion::Events::OK || evt == Ion::Events::EXE) {
            int selectedRow = m_selectableTableView.selectedRow();
            int selectedColumn = m_selectableTableView.selectedColumn();
            if (selectedRow >= 0 && selectedRow < m_spriteStore->NumberOfSprites()) {
                if (selectedColumn == 1) {
                    this->configureSprite();
                    return true;
                }
                assert(selectedColumn == 0);
                return true;
            } else if (selectedColumn == 0 && selectedRow == m_spriteStore->NumberOfSprites()) {
                addSprite();
                return true;
            }
        }
        return false;
    }

    void MenuController::didBecomeFirstResponder(void)
    {
        // TODO: Do smth
        if (m_selectableTableView.selectedRow() < 0) {
            m_selectableTableView.selectCellAtLocation(0, 0);
        }
        assert(m_selectableTableView.selectedRow() < m_spriteStore->NumberOfSprites() + 1);
        Container::activeApp()->setFirstResponder(&m_selectableTableView);
    }

    void MenuController::viewWillAppear(void)
    {
        ViewController::viewWillAppear();
        updateAddSpriteRowDisplay();
    }

    /* Table view datasource */

    int MenuController::numberOfRows(void) const
    {
        // TODO: Return actual number of rows
        return m_spriteStore->NumberOfSprites() + m_shouldDisplayAddSpriteRow;  // Add sprite
    }

    void MenuController::willDisplayCellAtLocation(HighlightCell * cell, int i, int j)
    {
        if (i == 0 && j < m_spriteStore->NumberOfSprites()) {
            assert(j >= 0 && j < m_spriteStore->NumberOfSprites());
            (static_cast<SpriteNameCell *>(cell))->textField()->setText(m_spriteStore->spriteAtIdx(j).fullName());
        }
        static_cast<EvenOddCell *>(cell)->setEven(j%2 == 0);
        cell->setHighlighted(i == selectedColumn() && j == selectedRow());
    }

    KDCoordinate MenuController::columnWidth(int i)
    {
        switch (i) {
            case 0:
                return m_selectableTableView.bounds().width() - k_parametersColumbWidth;
            case 1:
                return k_parametersColumbWidth;
            default:
                assert(false);
                return 0;
        }
    }

    KDCoordinate MenuController::cumulatedWidthFromIndex(int i)
    {
        switch (i) {
            case 0:
                return 0;
            case 1:
                return m_selectableTableView.bounds().width() - k_parametersColumbWidth;
            case 2:
                return m_selectableTableView.bounds().width();
            default:
                assert(false);
                return 0;
        }
    }

    KDCoordinate MenuController::cumulatedHeightFromIndex(int j)
    {
        return Metric::StoreRowHeight * j;
    }

    int MenuController::indexFromCumulatedWidth(KDCoordinate offsetX)
    {
        if (offsetX <= m_selectableTableView.bounds().width() - k_parametersColumbWidth) {
            return 0;
        }
        if (offsetX <= m_selectableTableView.bounds().width()) {
            return 1;
        }
        else {
            return 2;
        }
        assert(false);
        return 0;
    }

    int MenuController::indexFromCumulatedHeight(KDCoordinate offsetY)
    {
        if (Metric::StoreRowHeight == 0) {
            return 0;
        }
        return (offsetY - 1) / Metric::StoreRowHeight;
    }

    HighlightCell * MenuController::reusableCell(int index, int type)
    {
        assert(index >= 0);
        if (type == SpriteCellType) {
            assert(index >=0 && index < k_maxNumberOfDisplayableSpriteCells);
            return &m_spriteCells[index];
        }
        if (type == SpriteParameterCellType) {
            assert(index >=0 && index < k_maxNumberOfDisplayableSpriteCells);
            return &m_spriteParameterCells[index];
        }
        if (type == AddSpriteCellType) {
            assert(index == 0);
            return &m_addNewSpriteCell;
        }
        if(type == EmptyCellType) {
            return &m_emptyCell;
        }
        assert(false);
        return nullptr;
    }

    int MenuController::reusableCellCount(int type)
    {
        switch (type) {
            case EmptyCellType:
            case AddSpriteCellType:
                return 1;
            case SpriteCellType:
            case SpriteParameterCellType:
                return k_maxNumberOfDisplayableSpriteCells;
        }
        assert(false);
        return 0;
    }

    int MenuController::typeAtLocation(int i, int j)
    {
        assert(i >= 0 && i < numberOfColumns());
        assert(j >= 0 && j < numberOfRows());
        if (i == 0) {
            if (j == numberOfRows()-1 && m_shouldDisplayAddSpriteRow) {
                return AddSpriteCellType;
            }
            return SpriteCellType;
        }
        assert(i == 1);
        if (j == numberOfRows()-1 && m_shouldDisplayAddSpriteRow) {
            return EmptyCellType;
        }
        return SpriteParameterCellType;
    }

    /* SelectableTableViewDelegate */

    void MenuController::tableViewDidChangeSelection(
        SelectableTableView * t,
        int previousSelectedCellX,
        int previousSelectedCellY,
        bool withinTemporarySelection
    )
    {
        if (selectedRow() == numberOfRows() - 1 && selectedColumn() == 1 && m_shouldDisplayAddSpriteRow) {
            t->selectCellAtLocation(0, numberOfRows()-1);
        }
    }

    /* TextFieldDelegate */

    bool MenuController::textFieldShouldFinishEditing(
        TextField * field,
        Ion::Events::Event event
    )
    {
        return event == Ion::Events::OK || event == Ion::Events::EXE || event == Ion::Events::Down || event == Ion::Events::Up;
    }

    bool MenuController::textFieldDidAbortEditing(TextField * field)
    {
        return false;
    }

    bool MenuController::textFieldDidHandleEvent(
        TextField * field,
        bool returnValue,
        bool textSizeDidChange
    )
    {
        int scriptExtensionLength = 1 + strlen(SpriteStore::k_spriteExtension);
        if (field->isEditing()) {
            const char * maxPointerLocation = field->text() + field->draftTextLength() - scriptExtensionLength;
            if (field->cursorLocation() > maxPointerLocation) {
                field->setCursorLocation(maxPointerLocation);
            }
        }
        return returnValue;
    }

    /* ButtonRowDelegate */

    // Button * MenuController::buttonAtIndex(
    //     int index,
    //     ButtonRowController::Position pos
    // ) const
    // {
    //     // TODO: Do smth
    //     return nullptr;
    // }

    /* Private */

    void MenuController::addSprite(void)
    {
        Sprite::ErrorStatus err = m_spriteStore->AddNewSprite();
        if (err == Sprite::ErrorStatus::None) {
            updateAddSpriteRowDisplay();
            renameSelectedSprite();
            return;
        }
        assert(false);
    }

    void MenuController::editSpriteAtIndex(size_t idx)
    {
        // TODO: Do smth
    }

    void MenuController::configureSprite(void)
    {
        assert(m_selectableTableView.selectedRow() >= 0);
        assert(m_selectableTableView.selectedRow() < m_spriteStore->NumberOfSprites());
        m_paramController.setSprite(m_spriteStore->spriteAtIdx(m_selectableTableView.selectedRow()));
        stackViewController()->push(&m_paramController);
    }

    void MenuController::updateAddSpriteRowDisplay(void)
    {
        m_shouldDisplayAddSpriteRow = !m_spriteStore->isFull();
        m_selectableTableView.reloadData();
    }

    void MenuController::renameSelectedSprite(void)
    {
        assert(m_selectableTableView.selectedRow() >= 0);
        assert(m_selectableTableView.selectedRow() < m_spriteStore->NumberOfSprites());
        AppsContainer::sharedAppsContainer()->setShiftAlphaStatus(Ion::Events::ShiftAlphaStatus::AlphaLock);
        m_selectableTableView.selectCellAtLocation(0, (m_selectableTableView.selectedRow()));
        SpriteNameCell * myCell = static_cast<SpriteNameCell *>(m_selectableTableView.selectedCell());
        Container::activeApp()->setFirstResponder(myCell);
        myCell->setHighlighted(false);
        TextField * tf = myCell->textField();
        const char * previousText = tf->text();
        tf->setEditing(true);
        tf->setText(previousText);
        tf->setCursorLocation(tf->text() + strlen(previousText));
    }

    bool MenuController::textFieldDidFinishEditing(
        TextField * textField,
        const char * text,
        Ion::Events::Event event
    ) {
        const char * newName;
        static constexpr int bufferSize = Sprite::k_defaultSpriteNameMaxSize + 1 + SpriteStore::k_spriteExtensionLength; //"script99" + "." + "py"
        char numberedDefaultName[bufferSize];

        if (strlen(text) > 1 + SpriteStore::k_spriteExtensionLength) {
            newName = text;
        } else {
            // The user entered an empty name. Use a numbered default script name.
            bool foundDefaultName = Sprite::DefaultName(numberedDefaultName, Sprite::k_defaultSpriteNameMaxSize);
            int defaultNameLength = strlen(numberedDefaultName);
            assert(UTF8Decoder::CharSizeOfCodePoint('.') == 1);
            numberedDefaultName[defaultNameLength++] = '.';
            assert(defaultNameLength < bufferSize);
            strlcpy(numberedDefaultName + defaultNameLength, SpriteStore::k_spriteExtension, bufferSize - defaultNameLength);
            /* If there are already scripts named script1.py, script2.py,... until
            * Script::k_maxNumberOfDefaultScriptNames, we want to write the last tried
            * default name and let the user modify it. */
            if (!foundDefaultName) {
            textField->setText(numberedDefaultName);
            textField->setCursorLocation(textField->draftTextBuffer() + defaultNameLength);
            }
            newName = const_cast<const char *>(numberedDefaultName);
        }
        Sprite::ErrorStatus error = Sprite::nameCompliant(newName) ? m_spriteStore->spriteAtIdx(m_selectableTableView.selectedRow()).setName(newName) : Sprite::ErrorStatus::NonCompliantName;
        if (error == Sprite::ErrorStatus::None) {
            updateAddSpriteRowDisplay();
            textField->setText(newName);
            int currentRow = m_selectableTableView.selectedRow();
            if (event == Ion::Events::Down && currentRow < numberOfRows() - 1) {
                m_selectableTableView.selectCellAtLocation(m_selectableTableView.selectedColumn(), currentRow + 1);
            } else if (event == Ion::Events::Up && currentRow > 0) {
                m_selectableTableView.selectCellAtLocation(m_selectableTableView.selectedColumn(), currentRow - 1);
            }
            m_selectableTableView.selectedCell()->setHighlighted(true);
            // reloadConsole();
            Container::activeApp()->setFirstResponder(&m_selectableTableView);
            AppsContainer::sharedAppsContainer()->setShiftAlphaStatus(Ion::Events::ShiftAlphaStatus::Default);
            return true;
        } else if (error == Sprite::ErrorStatus::NameTaken) {
            Container::activeApp()->displayWarning(I18n::Message::NameTaken);
        } else if (error == Sprite::ErrorStatus::NonCompliantName) {
            Container::activeApp()->displayWarning(I18n::Message::AllowedCharactersaz09, I18n::Message::NameCannotStartWithNumber);
        } else {
            assert(error == Sprite::ErrorStatus::NotEnoughSpaceAvailable);
            Container::activeApp()->displayWarning(I18n::Message::NameTooLong);
        }
        return false;
    }

    bool MenuController::privateTextFieldDidAbortEditing(
        TextField * textField,
        bool menuControllerStaysInResponderChain
    ) {
        /* If menuControllerStaysInResponderChain is false, we do not want to use
        * methods that might call setFirstResponder, because we might be in the
        * middle of another setFirstResponder call. */
        Sprite script = m_spriteStore->spriteAtIdx(m_selectableTableView.selectedRow());
        const char * scriptName = script.fullName();
        if (strlen(scriptName) <= 1 + strlen(SpriteStore::k_spriteExtension)) {
            // The previous text was an empty name. Use a numbered default script name.
            char numberedDefaultName[Sprite::k_defaultSpriteNameMaxSize];
            bool foundDefaultName = Sprite::DefaultName(numberedDefaultName, Sprite::k_defaultSpriteNameMaxSize);
            if (!foundDefaultName) {
                // If we did not find a default name, delete the script
                // TODO: deleteSprite(script);
                return true;
            }
            Sprite::ErrorStatus error = script.setBaseNameWithExtension(numberedDefaultName, SpriteStore::k_spriteExtension);
            scriptName = m_spriteStore->spriteAtIdx(m_selectableTableView.selectedRow()).fullName();
            /* Because we use the numbered default name, the name should not be
            * already taken. Plus, the script could be added only if the storage has
            * enough available space to add a script named 'script99.py' */
            (void) error; // Silence the "variable unused" warning if assertions are not enabled
            assert(error == Sprite::ErrorStatus::None);
            if (menuControllerStaysInResponderChain) {
                updateAddSpriteRowDisplay();
            }
        }
        textField->setText(scriptName);
        if (menuControllerStaysInResponderChain) {
            m_selectableTableView.selectCellAtLocation(m_selectableTableView.selectedColumn(), m_selectableTableView.selectedRow());
            Container::activeApp()->setFirstResponder(&m_selectableTableView);
        }
        AppsContainer::sharedAppsContainer()->setShiftAlphaStatus(Ion::Events::ShiftAlphaStatus::Default);
        return true;
    }

}
