#include "app.h"
#include "ion/events.h"
#include "sprites_icon.h"
#include <apps/i18n.h>

namespace Sprites
{

    I18n::Message App::Descriptor::name() {
        return I18n::Message::SpritesApp;
    }

    I18n::Message App::Descriptor::upperName() {
        return I18n::Message::SpritesAppCapital;
    }

    const Image * App::Descriptor::icon() {
        return ImageStore::SpritesIcon;
    }

    App::Snapshot::Snapshot()
    {

    }

    App * App::Snapshot::unpack(Container * container) {
        return new (container->currentAppBuffer()) App(this);
    }

    App::Descriptor * App::Snapshot::descriptor() {
        static Descriptor descriptor;
        return &descriptor;
    }

    App::App(Snapshot * snapshot) :
        Shared::InputEventHandlerDelegateApp(snapshot, &m_stackViewController),
        // m_footer(&m_stackViewController, &m_menuController, &m_menuController),
        m_menuController(&m_modalViewController, this, snapshot->spriteStore()),
        m_stackViewController(&m_modalViewController, &m_menuController)
    {
        // Pass
    }

    bool App::handleEvent(Ion::Events::Event evt) {
        return false;
    }

    void App::willExitResponderChain(Responder * nextFirstResponder) {

    }

    bool App::prepareForExit()
    {
        // NOTE: Since we are NOT running a console controller with input loopp ?
        return true;
    }

}
