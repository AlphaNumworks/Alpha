#ifndef SPRITES_SPRITE_PARAMETER_CONTROLLER_H
    #define SPRITES_SPRITE_PARAMETER_CONTROLLER_H
    #include "apps/sprites/sprite/sprite.h"
    #include <escher.h>
    #include <apps/i18n.h>

namespace Sprites {

    class MenuController;

    class SpriteParameterController : public ViewController, public SimpleListViewDataSource, public SelectableTableViewDataSource {
        public:
            SpriteParameterController(Responder * parentResponder, I18n::Message title, MenuController * menuController);

            void setSprite(Sprite sprite);
            void dismissSpriteParameterController();

            /* ViewController */
            View * view() override { return &m_selectableTableView; }
            const char * title() override;
            bool handleEvent(Ion::Events::Event event) override;
            void viewWillAppear() override;
            void didBecomeFirstResponder() override;
            TELEMETRY_ID("SpriteParameter");

            /* SimpleListViewDataSource */
            KDCoordinate cellHeight() override { return Metric::ParameterCellHeight; }
            HighlightCell * reusableCell(int index) override;
            int reusableCellCount() const override { return k_totalNumberOfCell; }
            int numberOfRows() const override { return k_totalNumberOfCell; }
            void willDisplayCellForIndex(HighlightCell * cell, int index) override;

        private:
            constexpr static int k_totalNumberOfCell = 4;
            StackViewController * stackViewController();
            I18n::Message m_pageTitle;

            MessageTableCell<> m_renameSprite;
            MessageTableCell<> m_duplicateSprite;
            MessageTableCell<> m_deleteSprite;

            MessageTableCellWithBuffer m_size;

            void GetSpriteSize(MessageTableCellWithBuffer * myCell);
            SelectableTableView m_selectableTableView;

            Sprite m_Sprite;

            MenuController * m_menuController;
            bool m_sizedisplaypercent = false;
    };

}

#endif /* SPRITES_SPRITE_PARAMETER_CONTROLLER_H */
