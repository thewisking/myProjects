#include "../include/window_context.h"
#include <iostream>

namespace {
struct FindWindowData {
    const wchar_t *needle;
    HWND hwnd = nullptr;
};

BOOL CALLBACK EnumWindowsProc(HWND h, LPARAM lParam) {
    FindWindowData *data = reinterpret_cast<FindWindowData *>(lParam);
    wchar_t title[512];
    int len = GetWindowTextW(h, title, 512);
    if (len <= 0)
        return TRUE;
    if (wcsstr(title, data->needle)) {
        data->hwnd = h;
        return FALSE;
    }
    return TRUE;
}
ScreenRect rect_to_screen_rect(const RECT &rc) {
    ScreenRect r;
    r.x = rc.left;
    r.y = rc.top;
    r.width = rc.right - rc.left;
    r.height = rc.bottom - rc.top;
    return r;
};

} // namespace

WindowContext WindowContext::from_title_substring(const std::wstring &substring) {
    FindWindowData data{substring.c_str(), nullptr};
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&data));
    return WindowContext(data.hwnd);
}

ScreenRect WindowContext::window_screen_rect() const {
    if (!is_valid()) {
        return {0, 0, 0, 0};
    }

    RECT rc{};
    if (!GetWindowRect(m_hwnd, &rc)) {
        std::cerr << "GetWindowRect failed\n";
        return {0, 0, 0, 0};
    }

    return rect_to_screen_rect(rc);
}

ScreenRect WindowContext::client_screen_rect() const {
    if (!is_valid()) {
        return {0, 0, 0, 0};
    }

    RECT client{};
    if (!GetClientRect(m_hwnd, &client)) {
        std::cerr << "GetClientRect failed\n";
        return {0, 0, 0, 0};
    }

    POINT origin{client.left, client.top};
    if (!ClientToScreen(m_hwnd, &origin)) {
        std::cerr << "ClientToScreen failed\n";
        return {0, 0, 0, 0};
    }

    ScreenRect r;
    r.x = origin.x;
    r.y = origin.y;
    r.width = client.right - client.left;
    r.height = client.bottom - client.top;
    return r;
}

bool WindowContext::is_foreground() const {
    if (!is_valid()) {
        return false;
    }
    return GetForegroundWindow() == m_hwnd;
}

bool WindowContext::is_client_fully_on_screen() const {
    if (!is_valid()) {
        return false;
    }

    ScreenRect client = client_screen_rect();

    int vx = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int vy = GetSystemMetrics(SM_YVIRTUALSCREEN);
    int vw = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int vh = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    int right = client.x + client.width;
    int bottom = client.y + client.height;

    bool inside =
        client.width > 0 &&
        client.height > 0 &&
        client.x >= vx &&
        client.y >= vy &&
        right <= vx + vw &&
        bottom <= vy + vh;

    return inside;
}

