#ifndef SPRITES_MENU_CONTROLLER_H
    #define SPRITES_MENU_CONTROLLER_H

#include "apps/sprites/app.h"
#include "escher/button_row_controller.h"
#include "escher/selectable_table_view_data_source.h"
#include "escher/table_view_data_source.h"
#include "escher/text_field_delegate.h"
#include "escher/view_controller.h"
namespace Sprites
{

    class MenuController :
        public ViewController,
        public TableViewDataSource,
        public SelectableTableViewDataSource,
        public SelectableTableViewDelegate,
        public TextFieldDelegate,
        public ButtonRowDelegate
    {
        public:
            MenuController(
                Responder * parentResponder,
                App * spritesDelegate,
                ButtonRowController * footer
            );

            StackViewController * stackViewController();

            void willExitResponderChain(Responder * nextFirstResponder) override;

            /* ViewController */
            View * view() override;
            bool   handleEvent(Ion::Events::Event event) override;
            void   didBecomeFirstResponder() override;
            void   viewWillAppear() override;
            TELEMETRY_ID("Menu");

            /* TableViewDataSource */
            int numberOfRows() const override;
            int numberOfColumns() const override { return 2; };
            void willDisplayCellAtLocation(HighlightCell * cell, int i, int j) override;
            KDCoordinate columnWidth(int i) override;
            KDCoordinate rowHeight(int j) override { return Metric::StoreRowHeight; }
            KDCoordinate cumulatedWidthFromIndex(int i) override;
            KDCoordinate cumulatedHeightFromIndex(int j) override;
            int indexFromCumulatedWidth(KDCoordinate offsetX) override;
            int indexFromCumulatedHeight(KDCoordinate offsetY) override;
            HighlightCell * reusableCell(int index, int type) override;
            int reusableCellCount(int type) override;
            int typeAtLocation(int i, int j) override;
            void willDisplayScriptTitleCellForIndex(HighlightCell * cell, int index);

            /* SelectableTableViewDelegate */
            void tableViewDidChangeSelection(SelectableTableView * t, int previousSelectedCellX, int previousSelectedCellY, bool withinTemporarySelection) override;

            /* TextFieldDelegate */
            bool textFieldShouldFinishEditing(TextField * textField, Ion::Events::Event event) override;
            bool textFieldDidReceiveEvent(TextField * textField, Ion::Events::Event event) override { return false; };
            bool textFieldDidFinishEditing(TextField * textField, const char * text, Ion::Events::Event event) override;
            bool textFieldDidAbortEditing(TextField * textField) override;
            bool textFieldDidHandleEvent(TextField * textField, bool returnValue, bool textSizeDidChange) override;

            /* ButtonRowDelegate */
            int numberOfButtons(ButtonRowController::Position position) const override { return 1; };
            Button * buttonAtIndex(int index, ButtonRowController::Position position) const override;

        private:
            static constexpr int k_maxNumberOfDisplayableSpriteCells = 5;
            static constexpr int k_parametersColumbWidth = Metric::EllipsisCellWidth;
            static constexpr int AddSpriteCellType = 0;
            static constexpr int SpriteCellType = 1;
            static constexpr int SpriteParameterCellType = 2;
            static constexpr int EmptyCellType = 3;

            void addSprite();
    };

}

#endif /* SPRITES_MENU_CONTROLLER_H */
