#ifndef SPRITES_APP_H
    #define SPRITES_APP_H
    #include "apps/shared/input_event_handler_delegate_app.h"
    #include "apps/shared/shared_app.h"
    #include "apps/sprites/store/sprite_store.hpp"
    #include "apps/sprites/controllers/menu_controller.h"
    #include <escher.h>

namespace Sprites
{

class App : public Shared::InputEventHandlerDelegateApp
{

    public:
        class Descriptor : public Shared::InputEventHandlerDelegateApp::Descriptor
        {
            public:
                I18n::Message                     name() override;
                I18n::Message                     upperName() override;
                const Image *                     icon() override;
        };

        class Snapshot : public SharedApp::Snapshot {
            public:
                Snapshot();
                App *        unpack(Container * container) override;
                Descriptor * descriptor() override;
                SpriteStore * spriteStore() { return &m_store; };
            private:
                SpriteStore m_store;
        };

        static App * app() {
            return static_cast<App *>(Container::activeApp());
        }

        TELEMETRY_ID("Sprites");

        bool prepareForExit() override;

        MenuController * menuController(void) { return &m_menuController; }
        StackViewController * stackViewController(void) { return &m_stackViewController; }

        bool handleEvent(Ion::Events::Event event) override;
        void willExitResponderChain(Responder * nextFirstResponder) override;

    private:
        App(Snapshot * snapshot);
        MenuController               m_menuController;
        StackViewController          m_stackViewController;
};

}

#endif /* SPRITES_APP_H */
