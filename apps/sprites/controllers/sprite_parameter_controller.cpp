#include "sprite_parameter_controller.h"
#include "escher/i18n.h"
#include "menu_controller.h"
#include <poincare/integer.h>

namespace Sprites {

    SpriteParameterController::SpriteParameterController(
        Responder * parentResponder,
        I18n::Message title,
        MenuController * menuController
    ) :
        ViewController(parentResponder),
        m_pageTitle(title),
        m_renameSprite(I18n::Message::Rename),
        m_deleteSprite(I18n::Message::DeleteSprite),
        m_duplicateSprite(I18n::Message::DuplicateSprite),
        m_size(I18n::Message::SpriteSize),
        m_selectableTableView(this),
        m_Sprite(Ion::Storage::Record()),
        m_menuController(menuController)
    {
        // Init goes brrr
    }

    void SpriteParameterController::setSprite(Sprite Sprite)
    {
        m_Sprite = Sprite;
    }

    void SpriteParameterController::dismissSpriteParameterController(void)
    {
        m_Sprite = Sprite(Ion::Storage::Record());
        stackViewController()->pop();
    }

    const char * SpriteParameterController::title(void)
    {
        return I18n::translate(m_pageTitle);
    }

    bool SpriteParameterController::handleEvent(Ion::Events::Event event)
    {
        if (event == Ion::Events::OK || event == Ion::Events::EXE) {
            Sprite s = m_Sprite;
            switch (this->selectedRow()) {
            case 0:
                dismissSpriteParameterController();
                m_menuController->renameSelectedSprite();
                return true;
            case 1:{
                MessageTableCellWithBuffer * myCell = (MessageTableCellWithBuffer *)m_selectableTableView.selectedCell();
                m_sizedisplaypercent = !m_sizedisplaypercent;
                GetSpriteSize(myCell);
                return true;
            }
            case 2:
                dismissSpriteParameterController();
                m_menuController->duplicateSprite(s);
                return true;
            case 3:
                dismissSpriteParameterController();
                m_menuController->deleteSprite(s);
                return true;
            default:
                assert(false);
                return false;
            }
        }
        return false;
    }

    void SpriteParameterController::viewWillAppear(void)
    {
        ViewController::viewWillAppear();
        m_selectableTableView.reloadData();
        m_selectableTableView.selectCellAtLocation(0,0);
    }

    void SpriteParameterController::didBecomeFirstResponder(void)
    {
        selectCellAtLocation(0, 0);
        Container::activeApp()->setFirstResponder(&m_selectableTableView);
    }

    HighlightCell * SpriteParameterController::reusableCell(int index)
    {
        assert(index >= 0);
        assert(index < k_totalNumberOfCell);
        HighlightCell * cells[] = {&m_renameSprite, &m_size, &m_duplicateSprite, &m_deleteSprite};
        return cells[index];
    }

    void SpriteParameterController::willDisplayCellForIndex(HighlightCell * cell, int index) 
    {
        // if (cell == (HighlightCell *)&m_size) {
        //     MessageTableCellWithBuffer * myCell = (MessageTableCellWithBuffer *)cell;
        //     GetSpriteSize(myCell);
        //     myCell->setAccessoryFont(KDFont::SmallFont);
        //     myCell->setAccessoryTextColor(Palette::SecondaryText);
        // }
    }

    void SpriteParameterController::GetSpriteSize(MessageTableCellWithBuffer* myCell)
    {
        if(m_sizedisplaypercent){
            char size[18];
            int sizelen = Poincare::Integer((int)m_Sprite.value().size).serialize(size, 6);
            size[sizelen] = ' ';
            size[sizelen+1] = 'o';
            size[sizelen+2] = ' ';
            size[sizelen+3] = '/';
            size[sizelen+4] = ' ';
            int sizelen2 = Poincare::Integer((int)Ion::Storage::k_storageSize).serialize(size+sizelen+5, 6) + sizelen + 5;
            size[sizelen2] = ' ';
            size[sizelen2+1] = 'o';
            size[sizelen2+2] = '\0';
            myCell->setAccessoryText(size);
        }else{
            char size[18];
            int sizelen = Poincare::Integer((int)(((float)((int)m_Sprite.value().size)/((int)Ion::Storage::k_storageSize)) * 100.f)).serialize(size, 3);
            size[sizelen] = ' ';
            size[sizelen+1] = '%';
            size[sizelen+2] = '\0';
            myCell->setAccessoryText(size);
        }
    }

    StackViewController * SpriteParameterController::stackViewController()
    {
        return static_cast<StackViewController *>(parentResponder());
    }

}
