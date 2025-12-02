#pragma once

#include <windows.h>

#include <cstdint>
#include <string>
#include <vector>

#include "window_context.h"

struct Screenshot {
    int width = 0;  // in pixels
    int height = 0; // in pixels
    int stride = 0; // bytes per row (for us: width * 4)

    std::vector<std::uint8_t> pixels;
    bool valid() const { return width > 0 && height > 0 && !pixels.empty(); }
    const std::uint8_t *row_ptr(int y) const {
        return pixels.data() + static_cast<std::size_t>(y) * stride;
    }
    const std::uint8_t *pixel_ptr(int x, int y) const {
        return row_ptr(y) + static_cast<std::size_t>(x) * 4;
    }
};

class ScreenCaptureService {
  public:
    Screenshot capture(const ScreenRect &area) const;
};

bool save_bmp(const Screenshot &shot, const std::string &filename);
