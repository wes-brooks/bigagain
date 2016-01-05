#include <pebble.h>
#include "effects.h"
#include "math.h"
  
  
// { ********* Graphics utility functions (probablu should be seaparated into anothe file?) *********
  
  
// set pixel color at given coordinates 
void set_pixel(BitmapInfo bitmap_info, int y, int x, uint8_t color) {
  
#ifndef PBL_PLATFORM_APLITE  
  if (bitmap_info.bitmap_format == GBitmapFormat1BitPalette) { // for 1bit palette bitmap on Basalt --- verify if it needs to be different
     bitmap_info.bitmap_data[y*bitmap_info.bytes_per_row + x / 8] ^= (-color ^ bitmap_info.bitmap_data[y*bitmap_info.bytes_per_row + x / 8]) & (1 << (x % 8)); 
#else
  if (bitmap_info.bitmap_format == GBitmapFormat1Bit) { // for 1 bit bitmap on Aplite  --- verify if it needs to be different
     bitmap_info.bitmap_data[y*bitmap_info.bytes_per_row + x / 8] ^= (-color ^ bitmap_info.bitmap_data[y*bitmap_info.bytes_per_row + x / 8]) & (1 << (x % 8)); 
#endif
  } else { // othersise (assuming GBitmapFormat8Bit) going byte-wise
      
     #ifndef PBL_PLATFORM_CHALK
       bitmap_info.bitmap_data[y*bitmap_info.bytes_per_row + x] = color;
     #else
       GBitmapDataRowInfo info = gbitmap_get_data_row_info(bitmap_info.bitmap, y);
       if ((x >= info.min_x) && (x <= info.max_x)) info.data[x] = color;
     #endif  
  
  }
      
}

// get pixel color at given coordinates 
uint8_t get_pixel(BitmapInfo bitmap_info, int y, int x) {

#ifndef PBL_PLATFORM_APLITE  
  if (bitmap_info.bitmap_format == GBitmapFormat1BitPalette) { // for 1bit palette bitmap on Basalt shifting left to get correct bit
    return (bitmap_info.bitmap_data[y*bitmap_info.bytes_per_row + x / 8] << (x % 8)) & 128;
#else
  if (bitmap_info.bitmap_format == GBitmapFormat1Bit) { // for 1 bit bitmap on Aplite - shifting right to get bit
    return (bitmap_info.bitmap_data[y*bitmap_info.bytes_per_row + x / 8] >> (x % 8)) & 1;
#endif
  } else {  // othersise (assuming GBitmapFormat8Bit) going byte-wise
    
    #ifndef PBL_PLATFORM_CHALK
       return bitmap_info.bitmap_data[y*bitmap_info.bytes_per_row + x]; 
     #else
       GBitmapDataRowInfo info = gbitmap_get_data_row_info(bitmap_info.bitmap, y);
       if ((x >= info.min_x) && (x <= info.max_x))
         return info.data[x];
       else 
         return -1;
     #endif  
  }
  
}  
  

//  ********* Graphics utility functions (probablu should be seaparated into anothe file?) ********* }


// colorize effect - given a target color, replace it with a new color
// Added by Martin Norland (@cynorg)
// Parameter:  GColor firstColor, GColor secondColor
void effect_double_colorize(GContext* ctx,  GRect position, uint8_t col1, uint8_t col2) {
  //capturing framebuffer bitmap
  GBitmap *fb = graphics_capture_frame_buffer(ctx);
  
  BitmapInfo bitmap_info;
  bitmap_info.bitmap = fb;
  bitmap_info.bitmap_data =  gbitmap_get_data(fb);
  bitmap_info.bytes_per_row = gbitmap_get_bytes_per_row(fb);
  bitmap_info.bitmap_format = gbitmap_get_format(fb);
  
  uint8_t pixel;
#ifdef PBL_COLOR  
  uint8_t black = GColorBlack.argb;
  uint8_t white = GColorWhite.argb;
#else
  // On black and white Pebbles, interpret not-white as black and use 1-bit color:
  uint8_t black = 0;
  uint8_t white = 1;
  
  col1 = (col1 == 255 ? 1 : 0);
  col2 = (col2 == 255 ? 1 : 0);
#endif
  
  for (int y = 0; y < position.size.h; y++) {
      for (int x = 0; x < position.size.w; x++) {
        pixel = get_pixel(bitmap_info, y + position.origin.y, x + position.origin.x);
        
        if (pixel == white)
           set_pixel(bitmap_info, y + position.origin.y, x + position.origin.x, col1);
          
        if (pixel == black)
           set_pixel(bitmap_info, y + position.origin.y, x + position.origin.x, col2);
      }
  }
  graphics_release_frame_buffer(ctx, fb);
}
  