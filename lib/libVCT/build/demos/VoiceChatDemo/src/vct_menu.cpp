//
//
//

#include "vct_menu.h"
#include "utility.h"

#pragma profile off

#define Y_OFFSET    2

extern KeyControl stKeyCnt;

Menu::Menu(void *user_data)
    : _mode(0), _index(0), _numItem(0), _userData(user_data)
{
    for (int i = 0; i < VCT_MAX_MENU_ITEM; ++i) {
        _item[i] = NULL;
    }
}

Menu::~Menu()
{
    for (int i = 0; i < VCT_MAX_MENU_ITEM; ++i) {
        delete _item[i];
    }
}

void Menu::AddItem(MenuItem *item)
{
    SDK_ASSERT(_numItem < VCT_MAX_MENU_ITEM);

    _item[_numItem++] = item;

    item->_userData = _userData;
}

MenuResult Menu::ShowMenu()
{
    for (int i = 0; i < 20; ++i) {
        PutMainScreen(0, (s16)(i + Y_OFFSET), 0, "                                ");
    }
    
    MenuItem *item = _item[_mode];

    item->_index = _index;
    
    if (stKeyCnt.trg & PAD_KEY_UP) {
        if (--_index < 0) _index = item->numItems() - 1;
    }
    if (stKeyCnt.trg & PAD_KEY_DOWN) {
        if (++_index > item->numItems() - 1) _index = 0;
    }

    PutMainScreen(0, Y_OFFSET, 15, "+------------------------------+");
    PutMainScreen(2, Y_OFFSET, 15, " %s ", item->title());

    int i;
    for (i = 0; i < item->numItems(); ++i) {
        u8 color = FONT_COLOR_WHITE;
        const char *msg = item->menuElement(i, _index, &color);

        if (i == _index) {
            PutMainScreen(1, (s16)(Y_OFFSET+i+1), 15, ">");
        }
        
        PutMainScreen(3, (s16)(Y_OFFSET+i+1), color, "%s", msg);
    }

    i += item->showExInfo(Y_OFFSET+i+1, _index);
    
    PutMainScreen(0, (s16)(Y_OFFSET+i+1), 15, "+------------------------------+");

    int result = stKeyCnt.trg;
    if (stKeyCnt.trg & ~(PAD_KEY_UP | PAD_KEY_DOWN)) {
        result = item->keyDown(_index, stKeyCnt.cont);
    }
    return result;
}

int Menu::GoPrev()
{
    if (--_mode < 0) {
        _mode = _numItem - 1;
    }
    _index = _item[_mode]->_index;
    return _mode;
}

int Menu::GoNext()
{
    if (++_mode >= _numItem) {
        _mode = 0;
    }
    _index = _item[_mode]->_index;
    return _mode;
}

void Menu::ChangeMenu(int index)
{
    _mode  = index;
    if (_mode < 0) _mode = 0;
    if (_mode >= _numItem) _mode = _numItem - 1;

    _index = _item[_mode]->_index;
    
    
}


MenuItem::MenuItem()
    : _index(0)
{
}

MenuItem::~MenuItem()
{
}

MenuResult MenuItem::keyDown(int index, int key)
{
#pragma unused(index)

    return key;
}

int MenuItem::showExInfo(int screenY, int cursor_position)
{
#pragma unused(screenY, cursor_position)

    return 0;
}
