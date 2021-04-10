#pragma once

#define SC static constexpr

SC int vwidth = 384, vheight = 288;
SC int pixel_scale = 2;
SC int fps = 15;

SC int horizon_y = 100;
SC int status_bar_height = 20;
SC int road_y_span = vheight - horizon_y - status_bar_height;

SC wchar_t const *font_root = L"../font/$.bmp";
SC wchar_t const *image_root = L"../images/$.bmp";