#include "menu_controller.h"

#include "app.h"

namespace Sprites
{

    MenuController::MenuController(
        Responder * parentResponder,
        App * spriteDelegate,
        ButtonRowController * footer
    ) : 
        ViewController(parentResponder),
        ButtonRowDelegate(nullptr, footer)
    {
        // Init
    }

}
