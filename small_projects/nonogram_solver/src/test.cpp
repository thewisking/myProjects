#define NOMINMAX
#include <iostream>
#include <windows.h>

struct ScreenRect {
    int x;      // top-left x in SCREEN coords
    int y;      // top-left y in SCREEN coords
    int width;  // width in pixels
    int height; // height in pixels
};

ScreenRect GetClientScreenRect(HWND hwnd) {
    RECT client{};
    if (!GetClientRect(hwnd, &client)) {
        std::cerr << "GetClientRect filed \n";
        return {0, 0, 0, 0};
    }

    POINT origin{client.left, client.top};
    if (!ClientToScreen(hwnd, &origin)) {
        std::cerr << "ClientToScreen failed\n";
        return {0, 0, 0, 0};
    }

    ScreenRect r;
    r.x = origin.x;
    r.y = origin.y;
    r.width = client.right - client.left;
    r.height = client.bottom - client.top;
    return r;
};

struct FindWindowData {
    const wchar_t *needle;
    HWND hwnd = nullptr;
};

BOOL CALLBACK EnumWindowsProc(HWND h, LPARAM lParam) {
    FindWindowData *data = reinterpret_cast<FindWindowData *>(lParam);
    wchar_t title[512];
    int len = GetWindowTextW(h, title, 512);
    if (len <= 0)
        return true;
    if (wcsstr(title, data->needle)) {
        std::wcout << L"Found: \"" << title << L"\" HWND=" << h << L"\n";
        data->hwnd = h;
        return FALSE;
    }
    return TRUE;
}

int main() {
    system("cls");
    SetProcessDPIAware();

    FindWindowData data{L"Katana", nullptr};

    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&data));

    if (!data.hwnd) {
        std::wcerr << L"Window not found\n";
        return 1;
    }
    HWND hwnd = data.hwnd;

    RECT windowRect{};

    bool escape = true;

    while (escape) {
        Sleep(200);
        system("cls");

        // 1. Update window rect each tick
        GetWindowRect(hwnd, &windowRect);

        std::cout << "Window rect (SCREEN coords):\n"
                  << "left=" << windowRect.left
                  << "  top=" << windowRect.top
                  << "  right=" << windowRect.right
                  << "  bottom=" << windowRect.bottom
                  << "\n\n";

        // 2. Get client rect in SCREEN coords via your helper
        ScreenRect r = GetClientScreenRect(hwnd);

        std::cout << "Client area in SCREEN coords:\n"
                  << "x=" << r.x << "  y=" << r.y
                  << "  width=" << r.width
                  << "  height=" << r.height << "\n\n";

        // 3. Optional: also show raw client rect (client coords)
        RECT clientRect{};
        GetClientRect(hwnd, &clientRect);

        std::cout << "Client rect (CLIENT coords):\n"
                  << "left=" << clientRect.left
                  << "  top=" << clientRect.top
                  << "  right=" << clientRect.right
                  << "  bottom=" << clientRect.bottom
                  << "\n\n";

        // 4. Exit on ESC
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            escape = false;
        }
    }
}