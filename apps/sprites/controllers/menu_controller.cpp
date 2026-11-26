#include "apps/sprites/controllers/menu_controller.h"
#include "escher/button_row_controller.h"
#include "escher/container.h"
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
        m_selectableTableView(this, this, this, this)
    {
        m_selectableTableView.setMargins(0);
        m_selectableTableView.setDecoratorType(ScrollView::Decorator::Type::Arrows);
        // FIXME: Change for add sprite
        m_addNewSpriteCell.setMessage(I18n::Message::AddScript);
        for (size_t i = 0; i < k_maxNumberOfDisplayableSpriteCells; ++i) {
            m_spriteCells[i].setParentResponder(&m_selectableTableView);
            m_spriteCells[i].textField()->setDelegates(nullptr, this);
        }
    }

    StackViewController * MenuController::stackViewController(void)
    {
        // TODO: Return actual stackview controller
        return static_cast<StackViewController *>(parentResponder()->parentResponder());
    }

    void MenuController::willExitResponderChain(Responder * nfr)
    {
        Container::activeApp()->setFirstResponder(&m_selectableTableView);
    }

    /* ViewController */
    View * MenuController::view(void) 
    {
        return &m_selectableTableView;
    }

    bool MenuController::handleEvent(Ion::Events::Event evt)
    {
        // TODO: Handle event
        return false;
    }

    void MenuController::didBecomeFirstResponder(void)
    {
        // TODO: Do smth
        return;
    }

    void MenuController::viewWillAppear(void)
    {
        ViewController::viewWillAppear();
    }

    /* Table view datasource */

    int MenuController::numberOfRows(void) const
    {
        // TODO: Return actual number of rows
        return m_spriteStore->NumberOfSprites() + 1;  // Add sprite
    }

    void MenuController::willDisplayCellAtLocation(HighlightCell * cell, int i, int j)
    {
        // TODO: Do smtkj
        return;
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
            if (j == numberOfRows()-1) {// && m_shouldDisplayAddScriptRow) {
                return AddSpriteCellType;
            }
            return SpriteCellType;
        }
        assert(i == 1);
        if (j == numberOfRows()-1) {// && m_shouldDisplayAddScriptRow) {
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
        // TODO: Do smth
    }

    /* TextFieldDelegate */

    bool MenuController::textFieldShouldFinishEditing(
        TextField * field,
        Ion::Events::Event evt
    )
    {
        // TODO: Do smth
        return false;
    }

    bool MenuController::textFieldDidFinishEditing(
        TextField * field,
        const char * text,
        Ion::Events::Event evt
    )
    {
        // TODO: Do smth
        return false;
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
        // TODO: Do smth
        return false;
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
        // TODO: Do smth
    }

    void MenuController::editSpriteAtIndex(size_t idx)
    {
        // TODO: Do smth
    }

}
