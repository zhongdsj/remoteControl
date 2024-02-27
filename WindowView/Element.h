#pragma once
#include "Windows.h"

class Element
{
public:
    Element(DWORD _dwExStyle, const wchar_t* _className, const wchar_t* _windowName, DWORD _dwStyle, int _x, int _y, int _width, int _height, HWND _parent, HMENU _menu, HINSTANCE _instance, LPVOID _lpParam);
    void changeStyle(DWORD _dwStyle) const;
    void changeExStyle(DWORD _dwExStyle) const;
    HWND hwnd() const;
protected:
    HWND m_hwnd = nullptr;
};
