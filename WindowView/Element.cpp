#include "Element.h"

Element::Element(DWORD _dwExStyle, const wchar_t* _className, const wchar_t* _windowName, DWORD _dwStyle, int _x, int _y, int _width, int _height, HWND _parent, HMENU _menu, HINSTANCE _instance, LPVOID _lpParam)
{
    this->m_hwnd = CreateWindowExW(
        _dwExStyle, _className, _windowName, _dwStyle,
        _x, _y, _width, _height,
        _parent, _menu, _instance, _lpParam
        );
}

void Element::changeStyle(DWORD _dwStyle) const
{
    SetWindowLong(this->m_hwnd, GWL_STYLE, _dwStyle);
}

void Element::changeExStyle(DWORD _dwExStyle) const
{
    SetWindowLong(this->m_hwnd, GWL_EXSTYLE, _dwExStyle);
}

HWND Element::hwnd() const
{
    return this->m_hwnd;
}
