#pragma once

#include <string>
#include <windows.h>

struct ScreenRect {
    int x;
    int y;
    int width;
    int height;
};

class WindowContext {
  public:
    static WindowContext from_title_substring(const std::wstring &substring);

    bool is_valid() const { return m_hwnd != nullptr; }
    HWND handle() const { return m_hwnd; }
    ScreenRect window_screen_rect() const;
    ScreenRect client_screen_rect() const;

    bool is_foreground() const;

    bool is_client_fully_on_screen() const;

  private:
    explicit WindowContext(HWND hwnd) : m_hwnd(hwnd) {}
    HWND m_hwnd = nullptr;
};