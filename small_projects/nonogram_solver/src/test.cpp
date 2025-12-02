#define NOMINMAX
#include <windows.h>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct FindWindowData {
    std::wstring needle;
    HWND hwnd = nullptr;
};

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    auto *data = reinterpret_cast<FindWindowData *>(lParam);

    wchar_t title[512];
    int len = GetWindowTextW(hwnd, title, static_cast<int>(std::size(title)));
    if (len <= 0) {
        return TRUE; // keep enumerating
    }

    if (wcsstr(title, data->needle.c_str()) != nullptr) {
        data->hwnd = hwnd;
        return FALSE; // found it, stop
    }

    return TRUE;
}

HWND FindWindowByTitleSubstring(const std::wstring &needle) {
    FindWindowData data;
    data.needle = needle;
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&data));
    return data.hwnd;
}


bool CaptureAreaToBMP(int srcX, int srcY, int width, int height, const std::string &filename) {
    if (width <= 0 || height <= 0) {
        std::cerr << "CaptureAreaToBMP: non-positive size w=" << width
                  << " h=" << height << "\n";
        return false;
    }

    HDC hScreenDC = GetDC(nullptr);
    if (!hScreenDC) {
        std::cerr << "GetDC(nullptr) failed\n";
        return false;
    }

    HDC hMemDC = CreateCompatibleDC(hScreenDC);
    if (!hMemDC) {

        std::cerr << "CreateCompatibleDC failed\n";
        ReleaseDC(nullptr, hScreenDC);
        return false;
    }

    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
    if (!hBitmap) {
        std::cerr << "CreateCompatibleBitmap failed\n";
        DeleteDC(hMemDC);
        ReleaseDC(nullptr, hScreenDC);
        return false;
    }

    HGDIOBJ oldObj = SelectObject(hMemDC, hBitmap);

    if (!BitBlt(hMemDC,
                0, 0, width, height, // dest
                hScreenDC,
                srcX, srcY, // src (screen coords)
                SRCCOPY | CAPTUREBLT)) {
        std::cerr << "BitBlt failed\n";
        SelectObject(hMemDC, oldObj);
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        ReleaseDC(nullptr, hScreenDC);
        return false;
    }

    // Prepare BITMAPINFO for 24-bit top-down DIB
    BITMAPINFO bi{};
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = width;
    bi.bmiHeader.biHeight = -height; // top-down
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 24;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biSizeImage = 0;

    int rowSize = ((width * 24 + 31) / 32) * 4;
    int imageSize = rowSize * height;

    std::vector<std::uint8_t> pixels(static_cast<std::size_t>(imageSize));

    if (!GetDIBits(hMemDC,
                   hBitmap,
                   0,
                   static_cast<UINT>(height),
                   pixels.data(),
                   &bi,
                   DIB_RGB_COLORS)) {
        std::cerr << "GetDIBits failed\n";
        SelectObject(hMemDC, oldObj);
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        ReleaseDC(nullptr, hScreenDC);
        return false;
    }

    // Clean up DCs before file I/O
    SelectObject(hMemDC, oldObj);
    DeleteDC(hMemDC);
    ReleaseDC(nullptr, hScreenDC);

    BITMAPFILEHEADER bfh{};
    bfh.bfType = 0x4D42; // 'BM'
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bfh.bfSize = bfh.bfOffBits + imageSize;
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;

    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for writing\n";
        DeleteObject(hBitmap);
        return false;
    }

    file.write(reinterpret_cast<const char *>(&bfh), sizeof(bfh));
    file.write(reinterpret_cast<const char *>(&bi.bmiHeader), sizeof(BITMAPINFOHEADER));
    file.write(reinterpret_cast<const char *>(pixels.data()), imageSize);

    if (!file) {
        std::cerr << "Failed while writing BMP data\n";
        DeleteObject(hBitmap);
        return false;
    }

    file.close();
    DeleteObject(hBitmap);

    return true;
}

bool CaptureWindowToBMP(HWND hwnd, const std::string &filename) {
    if (!IsWindow(hwnd)) {
        std::cerr << "Invalid HWND\n";
        return false;
    }

    // 1) Try client area
    RECT client{};
    if (GetClientRect(hwnd, &client)) {
        int cw = client.right - client.left;
        int ch = client.bottom - client.top;
        std::cout << "Client rect: w=" << cw << " h=" << ch << "\n";

        if (cw > 0 && ch > 0) {
            POINT origin{client.left, client.top};
            if (!ClientToScreen(hwnd, &origin)) {
                std::cerr << "ClientToScreen failed\n";
            } else {
                std::cout << "Using CLIENT area capture at screen ("
                          << origin.x << "," << origin.y << ")\n";
                return CaptureAreaToBMP(origin.x, origin.y, cw, ch, filename);
            }
        } else {
            std::cout << "Client area non-positive, will fall back to full window.\n";
        }
    } else {
        std::cerr << "GetClientRect failed, will fall back to full window.\n";
    }
    RECT rc{};
    if (!GetWindowRect(hwnd, &rc)) {
        std::cerr << "GetWindowRect failed\n";
        return false;
    }
    int ww = rc.right - rc.left;
    int wh = rc.bottom - rc.top;
    std::cout << "Window rect: w=" << ww << " h=" << wh
              << " at (" << rc.left << "," << rc.top << ")\n";
    if (ww <= 0 || wh <= 0) {
        std::cerr << "Window area also non-positive, giving up.\n";
        return false;
    }
    std::cout << "Using FULL WINDOW capture.\n";
    return CaptureAreaToBMP(rc.left, rc.top, ww, wh, filename);
}

int main() {
    if (!SetProcessDPIAware()) {
        std::cerr << "Warning: SetProcessDPIAware failed; capture may be scaled.\n";
    }
    std::wstring titleSubstr = L"Katana";
    HWND hwnd = FindWindowByTitleSubstring(titleSubstr);
    if (!hwnd) {
        std::wcerr << L"Could not find a window containing title: \""
                   << titleSubstr << L"\"\n";
        return 1;
    }
    std::wcout << L"Found window handle: " << hwnd << L"\n";
    std::string outputFile = "nonogram_katana_capture.bmp";
    if (!CaptureWindowToBMP(hwnd, outputFile)) {
        std::wcerr << L"Capture failed.\n";
        return 1;
    }
    std::wcout << L"Capture saved to: " << outputFile.c_str() << L"\n";
    return 0;
}
