//
//  libVCT用のメニュープログラム 
//
#ifndef __VCT_MENU_H__
#define __VCT_MENU_H__

#include "screen.h"

typedef int MenuResult;
#define VCT_MAX_MENU_ITEM  10

class MenuItem;

class Menu
{
public:
    Menu(void *user_data);
    ~Menu();

    int         GoPrev();
    int         GoNext();
    
    void        AddItem(MenuItem *item);
    MenuResult  ShowMenu();
    void        ChangeMenu(int index);

private:
    MenuItem*   _item[VCT_MAX_MENU_ITEM];
    int         _mode;
    int         _index;
    int         _numItem;

    void*       _userData;
};

class MenuItem
{
public:
    MenuItem();
    virtual ~MenuItem();

    virtual const char* title() = 0;
    virtual int         numItems() = 0;
    virtual const char* menuElement(int index, int cursor_position, u8* outColor) = 0;
    
    virtual MenuResult  keyDown(int index, int key);
    virtual int         showExInfo(int scrrenY, int cursor_position);

protected:
    void*   _userData;
    int     _index;

    friend class Menu;
};

#endif //__VCT_MENU_H__
