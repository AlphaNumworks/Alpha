#ifndef SPRITES_CANVA_VIEW_H
    #define SPRITES_CANVA_VIEW_H
    #include "escher/view.h"

namespace Sprites
{

    class CanvaView : public View
    {
        public:
            static constexpr int k_pixelSize = 16;
            static constexpr int k_cellSize  = 13; // (222px // 16)

            CanvaView();

            void drawRect(KDContext * ctx, KDRect rect) const override;

            void moveCursorByOffset(int offsetX, int offsetY);

            KDRect cellAt(int x, int y) const;

        private:
            int m_x{0}, m_y{0};
    };

}

#endif /* SPRITES_CANVA_VIEW_H */
