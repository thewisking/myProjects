#include "../include/screenshot.h"

#include <fstream>
#include <iostream>

Screenshot ScreenCaptureService::capture(const ScreenRect &area) const {
    Screenshot shot{};

    if (area.width <= 0 || area.height <= 0) {
        std::cerr << "ScreenCaptureService::capture: non-positive size\n";
        return shot;
    }

    HDC hScreenDC = GetDC(nullptr);
    if (!hScreenDC) {
        std::cerr << "GetDC(nullptr) failed\n";
        return shot;
    }

    HDC hMemDC = CreateCompatibleDC(hScreenDC);
    if (!hMemDC) {
        std::cerr << "CreateCompatibleDC failed\n";
        ReleaseDC(nullptr, hScreenDC);
        return shot;
    }

    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, area.width, area.height);
    if (!hBitmap) {
        std::cerr << "CreateCompatibleBitmap failed\n";
        DeleteDC(hMemDC);
        ReleaseDC(nullptr, hScreenDC);
        return shot;
    }

    HGDIOBJ oldObj = SelectObject(hMemDC, hBitmap);

    if (!BitBlt(hMemDC,
                0, 0, area.width, area.height,
                hScreenDC,
                area.x, area.y,
                SRCCOPY | CAPTUREBLT)) {
        std::cerr << "BitBlt failed\n";
        SelectObject(hMemDC, oldObj);
        DeleteObject(hBitmap);
        DeleteDC(hMemDC);
        ReleaseDC(nullptr, hScreenDC);
        return shot;
    }

    BITMAPINFO bi{};
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = area.width;
    bi.bmiHeader.biHeight = -area.height;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biSizeImage = 0;

    const int stride = area.width * 4;
    const int imageSize = stride * area.height;

    shot.width = area.width;
    shot.height = area.height;
    shot.stride = stride;
    shot.pixels.resize(static_cast<std::size_t>(imageSize));

    if (!GetDIBits(hMemDC,
                   hBitmap,
                   0,
                   static_cast<UINT>(area.height),
                   shot.pixels.data(),
                   &bi,
                   DIB_RGB_COLORS)) {
        std::cerr << "GetDIBits failed\n";
        shot = Screenshot{};
    }

    SelectObject(hMemDC, oldObj);
    DeleteObject(hBitmap);
    DeleteDC(hMemDC);
    ReleaseDC(nullptr, hScreenDC);

    return shot;
}

bool save_bmp(const Screenshot &shot, const std::string &filename) {
    if (!shot.valid()) {
        std::cerr << "save_bmp: invalid screenshot\n";
        return false;
    }

    const int width = shot.width;
    const int height = shot.height;

    const int outBpp = 24;
    const int outStride = ((width * outBpp + 31) / 32) * 4;
    const int imageSize = outStride * height;

    BITMAPFILEHEADER bfh{};
    bfh.bfType = 0x4D42;
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bfh.bfSize = bfh.bfOffBits + imageSize;

    BITMAPINFOHEADER bih{};
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biWidth = width;
    bih.biHeight = -height;
    bih.biPlanes = 1;
    bih.biBitCount = 24;
    bih.biCompression = BI_RGB;
    bih.biSizeImage = imageSize;

    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "save_bmp: failed to open file\n";
        return false;
    }

    file.write(reinterpret_cast<const char *>(&bfh), sizeof(bfh));
    file.write(reinterpret_cast<const char *>(&bih), sizeof(bih));

    std::vector<std::uint8_t> row(outStride);

    for (int y = 0; y < height; ++y) {
        const std::uint8_t *srcRow = shot.row_ptr(y);
        std::uint8_t *dstRow = row.data();

        for (int x = 0; x < width; ++x) {
            const std::uint8_t *px = srcRow + x * 4;
            dstRow[x * 3 + 0] = px[0]; // B
            dstRow[x * 3 + 1] = px[1]; // G
            dstRow[x * 3 + 2] = px[2]; // R
        }

        file.write(reinterpret_cast<const char *>(dstRow), outStride);
    }

    return static_cast<bool>(file);
}