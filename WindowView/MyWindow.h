#pragma once
#include "Element.h"

class MyWindow: public Element
{
public:
    MyWindow(DWORD _dwExStyle, const wchar_t* _className, const wchar_t* _windowName, DWORD _dwStyle, int _x, int _y, int _width, int _height, HWND _parent, HMENU _menu, HINSTANCE _instance);
    void showWindow(int _nShow) const;
    void updateWindow() const;
private:
};
