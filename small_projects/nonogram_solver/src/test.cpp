#define NOMINMAX
#include <iostream>
#include <windows.h>

#include "../include/screenshot.h"
#include "../include/window_context.h"
#include "screenshot.cpp"
#include "window_context.cpp"

int main() {
    SetProcessDPIAware();
    bool escape = false;

    while (!escape) {
        Sleep(5000);
        system("cls");
        WindowContext win = WindowContext::from_title_substring(L"Katana");
        if (!win.is_valid()) {
            std::wcerr << L"Nonogram window not found\n";
            return 1;
        }

        if (!win.is_client_fully_on_screen()) {
            std::cout << "Warning: client area is not fully on screen.\n";
        }

        ScreenRect client = win.client_screen_rect();
        std::cout << "Capturing client area at x=" << client.x
                  << " y=" << client.y
                  << " w=" << client.width
                  << " h=" << client.height << "\n";

        ScreenCaptureService capturer;
        Screenshot shot = capturer.capture(client);

        if (!shot.valid()) {
            std::cerr << "Capture failed.\n";
            return 1;
        }

        if (!save_bmp(shot, "nonogram_debug_capture.bmp")) {
            std::cerr << "save_bmp failed\n";
            return 1;
        }

        std::cout << "Saved nonogram_debug_capture.bmp\n";

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            escape = true;
        }
    }
}