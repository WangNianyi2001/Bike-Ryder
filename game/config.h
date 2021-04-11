#pragma once

#define SC static constexpr

SC int vwidth = 384, vheight = 288;
SC int pixel_scale = 2;
SC int fps = 15;

SC int horizon_y = 100;
SC int status_bar_height = 20;
SC int road_y_span = vheight - horizon_y - status_bar_height;

SC float far_clip_z = 6;

SC float player_z = 1.0f;
SC float crash_z_tolerance = 0.1f;
SC float crash_x_tolerance = 10.0f;
SC float kick_x_range = 50.0f;
SC float kick_z_range = 0.4f;

SC wchar_t const *font_root = L"../font/$.bmp";
SC wchar_t const *image_root = L"../images/$.bmp";