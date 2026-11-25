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

    App::Descriptor::ExaminationLevel App::Descriptor::examinationLevel() {
        return App::Descriptor::ExaminationLevel::Basic;
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
        m_menuController(&m_modalViewController, this, snapshot->spriteStore(), nullptr),
        m_stackViewController(&m_modalViewController, nullptr)
    {
        // Pass
    }

    App::~App() {

    }

    bool App::handleEvent(Ion::Events::Event evt) {
        if (evt == Ion::Events::Home) {
            return true;
        }
        return false;
    }

    void App::willExitResponderChain(Responder * nextFirstResponder) {

    }

    bool App::prepareForExit()
    {
        return false;
    }

}
