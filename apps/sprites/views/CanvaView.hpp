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

            // NOTE: 8x8 = 64
            static constexpr int k_paletteNumberOfCells = 8;
            static constexpr int k_paletteOffsetX       = 200;
            static constexpr int k_paletteCellSize      = 14;

            static KDColor toKDColor(uint8_t p)
            {
                return KDColor::RGB888((p >> 6) * 85, ((p >> 4) & 3) * 85, ((p >> 2) & 3) * 85);
            }

            CanvaView(const uint8_t * pxs);

            void drawRect(KDContext * ctx, KDRect rect) const override;

            void moveCursorByOffset(int offsetX, int offsetY);

            KDRect cellAt(int x, int y) const;

            int cursorX(void) const { return m_x; }
            int cursorY(void) const { return m_y; }
            void reloadOnCursor(void) {
                markRectAsDirty(cellAt(m_x, m_y));
            }

            void movePaletteByOffset(int offX, int offY);

            KDRect paletteAt(int x, int y) const;

            int paletteX(void) const { return m_px; }
            int paletteY(void) const { return m_py; }
            // void reloadOnPalette(void);

        private:
            static constexpr uint8_t k_paletteOrder[64] = {
                16, 32, 48, 36, 37, 52, 53, 58,
                20, 40, 41, 56, 57, 60, 61, 62,
                4,  8, 24, 25, 12, 28, 44, 45,
                5,  9, 10, 13, 14, 29, 30, 46,
                6,  7, 11, 26, 27, 15, 31, 47,
                1,  2,  3, 18, 19, 22, 23, 43,
                17, 34, 35, 51, 38, 39, 55, 59,
                33, 49, 50, 54,  0, 21, 42, 63,
            };

            // NOTE: Given by controlleeer
            const uint8_t * m_buff;
            int m_x{0}, m_y{0};
            // NOTE: vvv Palette cursor !!
            int m_px{0}, m_py{0};
    };

}

#endif /* SPRITES_CANVA_VIEW_H */
