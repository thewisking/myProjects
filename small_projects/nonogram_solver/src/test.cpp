#include <iostream>
#include <windows.h>

#include "../include/screenshot.h"
#include "../include/window_context.h"

// Pull implementation into this translation unit
#include "screenshot.cpp"
#include "window_context.cpp"

int main() {
    // Make sure our process sees real pixels (not scaled)
    if (!SetProcessDPIAware()) {
        std::cerr << "Warning: SetProcessDPIAware failed; coordinates may be scaled.\n";
    }

    // 1. Locate the Nonogram Katana window by title substring
    WindowContext win = WindowContext::from_title_substring(L"Katana");
    if (!win.is_valid()) {
        std::wcerr << L"Nonogram window not found (title substring \"Katana\")\n";
        return 1;
    }

    // 2. Get client area in screen coordinates
    ScreenRect client = win.client_screen_rect();
    std::cout << "Client SCREEN rect: x=" << client.x
              << " y=" << client.y
              << " w=" << client.width
              << " h=" << client.height << "\n";

    // Optional safety check: warn if client isn’t fully on screen
    if (!win.is_client_fully_on_screen()) {
        std::cout << "Warning: client area is not fully on screen; "
                     "capture may miss part of the puzzle.\n";
    }

    // 3. Capture that area into a Screenshot
    ScreenCaptureService capturer;
    Screenshot shot = capturer.capture(client);

    if (!shot.valid()) {
        std::cerr << "Capture failed.\n";
        return 1;
    }

    std::cout << "Captured screenshot: "
              << shot.width << "x" << shot.height << " pixels\n";

    // 4. Dump to BMP for debugging
    const std::string filename = "nonogram_debug_capture.bmp";
    if (!save_bmp(shot, filename)) {
        std::cerr << "save_bmp failed\n";
        return 1;
    }

    std::cout << "Saved " << filename << "\n";
    return 0;
}
