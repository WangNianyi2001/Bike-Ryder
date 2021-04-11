#pragma once

#define SC static constexpr

// Game settings

// Display settings
SC int vwidth = 384, vheight = 288;
SC int pixel_scale = 2;
SC int fps = 15;

// Map settings
SC int horizon_y = 100;
SC int road_y_span = vheight - horizon_y;
SC int runway_width = vwidth;
SC float far_clip_z = 6.0f;

// NPC generation
SC float avg_npc_speed = 0.2f;
SC float npc_speed_tolerance = 0.1f;
SC float generate_z = 5.0f;

// Player settings
SC float player_z = 1.0f;
SC float crash_z_tolerance = 0.2f;
SC float crash_x_tolerance = 15.0f;
SC float kick_x_range = 50.0f;
SC float kick_z_range = 0.4f;

// Physics
SC float speed_decay = 0.99f;

// Resource directories
SC wchar_t const *font_root = L"../resource/font/$.bmp";
SC wchar_t const *image_root = L"../resource/image/$.bmp";
SC wchar_t const *bgm = L"../resource/bgm.wav";