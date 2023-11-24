#pragma once

// viewport attributes
const float ASPECT_RATIO = 16.0 / 9.0;
const float SCR_HEIGHT = 600.0f;
const float SCR_WIDTH = ASPECT_RATIO * SCR_HEIGHT;


// samples per pixel
const int samples_per_pixel = 100;

// anti-aliasing
bool ANTI_ALIAS = true;
bool antiAliasUp = true;

// shading mode control
int SHADING_MODE = 1;
bool shadeModeUp = true;
