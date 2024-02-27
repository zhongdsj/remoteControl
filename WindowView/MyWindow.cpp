#include "MyWindow.h"

MyWindow::MyWindow(DWORD _dwExStyle, const wchar_t* _className, const wchar_t* _windowName, DWORD _dwStyle, int _x,
    int _y, int _width, int _height, HWND _parent, HMENU _menu, HINSTANCE _instance):Element(_dwExStyle, _className, _windowName, _dwStyle, _x, _y, _width, _height, _parent, _menu, _instance, this)
{
}

void MyWindow::showWindow(int _nShow) const
{
    ShowWindow(this->m_hwnd, _nShow);
}

void MyWindow::updateWindow() const
{
    UpdateWindow(this->m_hwnd);
}
