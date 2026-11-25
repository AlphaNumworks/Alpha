#include "apps/sprites/controllers/menu_controller.h"
#include "escher/button_row_controller.h"
#include "escher/responder.h"
#include "escher/text_field.h"
#include <cstddef>

namespace Sprites
{

    MenuController::MenuController(
        Responder * parentResponder,
        App * spriteDelegate,
        SpriteStore * store,
        ButtonRowController * footer
    ) : 
        ViewController(parentResponder),
        ButtonRowDelegate(nullptr, footer),
        m_spriteStore(store)
    {
        // Init
    }

    StackViewController * MenuController::stackViewController(void)
    {
        // TODO: Return actual stackview controller
        return nullptr;
    }

    void MenuController::willExitResponderChain(Responder * nfr)
    {
        // FEGNoepzp kzv
    }

    /* ViewController */
    View * MenuController::view(void) 
    {
        // TODO: C'mon, do smth
        return nullptr;
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
        // TODO: Do smth
        return;
    }

    /* Table view datasource */

    int MenuController::numberOfRows(void) const
    {
        // TODO: Return actual number of rows
        return 0;
    }

    void MenuController::willDisplayCellAtLocation(HighlightCell * cell, int i, int j)
    {
        // TODO: Do smtkj
        return;
    }

    KDCoordinate MenuController::columnWidth(int i)
    {
        // TODO: Do smth
        return 0;
    }

    KDCoordinate MenuController::cumulatedWidthFromIndex(int i)
    {
        // TODO: Do smth
        return 0;
    }

    KDCoordinate MenuController::cumulatedHeightFromIndex(int j)
    {
        // TODO: Do smth
        return 0;
    }

    int MenuController::indexFromCumulatedWidth(KDCoordinate offsetX)
    {
        // TODO: Do smth
        return 0;
    }

    int MenuController::indexFromCumulatedHeight(KDCoordinate offsetY)
    {
        // TODO: Do smth
        return 0;
    }

    HighlightCell * MenuController::reusableCell(int index, int type)
    {
        // TODO: Do smth
        return nullptr;
    }

    int MenuController::reusableCellCount(int type)
    {
        // TODO: Do smth
        return 0;
    }

    int MenuController::typeAtLocation(int i, int j)
    {
        // TODO: Do smth
        return 0;
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

    Button * MenuController::buttonAtIndex(
        int index,
        ButtonRowController::Position pos
    ) const
    {
        // TODO: Do smth
        return nullptr;
    }

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
