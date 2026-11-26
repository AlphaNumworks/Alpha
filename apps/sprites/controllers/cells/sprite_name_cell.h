#ifndef SPRITES_SPRITE_NAME_CELL_H
#define SPRITES_SPRITE_NAME_CELL_H

#include <escher/even_odd_cell.h>
#include <escher/metric.h>
#include <escher/responder.h>
#include <escher/text_field_delegate.h>
#include <apps/shared/text_field_with_extension.h>
#include "apps/sprites/store/sprite_store.hpp"

namespace Sprites {

    class SpriteNameCell : public EvenOddCell, public Responder {
        public:
            SpriteNameCell(Responder * parentResponder = nullptr, TextFieldDelegate * delegate = nullptr) :
                EvenOddCell(),
                Responder(parentResponder),
                m_textField(k_extensionLength, this, m_textBody, TextField::maxBufferSize(), TextField::maxBufferSize(), nullptr, delegate)
            {
                m_textBody[0] = 0;
            }

            Shared::TextFieldWithExtension * textField() { return &m_textField; }

            // EvenOddCell
            void setEven(bool even) override;
            // HighlightCell
            void setHighlighted(bool highlight) override;
            Responder * responder() override {
                if (m_textField.isEditing()) {
                    return this;
                }
                return nullptr;
            }
            const char * text() const override;
            // View
            KDSize minimalSizeForOptimalDisplay() const override;
            // Responder
            void didBecomeFirstResponder() override;

        private:
            constexpr static size_t       k_extensionLength = 1 + SpriteStore::k_spriteExtensionLength; // '.' + "spr"
            constexpr static KDCoordinate k_leftMargin = Metric::CommonLargeMargin;

            // View
            int numberOfSubviews() const override { return 1; }
            View * subviewAtIndex(int index) override {
                assert(index == 0);
                return &m_textField;
            }
            void layoutSubviews(bool force = false) override;

            Shared::TextFieldWithExtension m_textField;
            char m_textBody[TextField::maxBufferSize()];
        };

}

#endif /* SPRITES_SPRITE_NAME_CELL_H */
