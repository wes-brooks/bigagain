#pragma once
#include <pebble.h>  

// used to pass bimap info to get/set pixel accurately  
typedef struct {
   GBitmap *bitmap;  // actual bitmap for Chalk raw manipulation
   uint8_t *bitmap_data;
   int bytes_per_row;
   GBitmapFormat bitmap_format;
}  BitmapInfo;

// double colorize effect.
// Added by Wesley Brooks
void effect_double_colorize(GContext* ctx, GRect position, uint8_t col1, uint8_t col2);
