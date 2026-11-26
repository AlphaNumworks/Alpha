#ifndef SPRITES_CANVA_VIEW_H
    #define SPRITES_CANVA_VIEW_H
    #include "escher/view.h"
#include <cstdint>

namespace Sprites
{

    class CanvaView : public View
    {
        public:
            static constexpr int k_pixelSize = 16;
            static constexpr int k_cellSize  = 12; // (222px // 16)

            CanvaView(const uint8_t * pxs);

            void drawRect(KDContext * ctx, KDRect rect) const override;

            void moveCursorByOffset(int offsetX, int offsetY);

            KDRect cellAt(int x, int y) const;

        private:
            // NOTE: Given by controlleeer
            const uint8_t * m_buff;
            int m_x{0}, m_y{0};
    };

}

#endif /* SPRITES_CANVA_VIEW_H */
