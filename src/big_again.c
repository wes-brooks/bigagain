#include "pebble.h"

static Window *s_main_window;

static BitmapLayer *s_hr_1_layer, *s_hr_2_layer, *s_min_1_layer, *s_min_2_layer;
static GBitmap *s_hr_0_bmp, *s_hr_1_bmp, *s_hr_2_bmp, *s_hr_3_bmp, *s_hr_4_bmp, *s_hr_5_bmp, *s_hr_6_bmp, *s_hr_7_bmp, *s_hr_8_bmp, *s_hr_9_bmp;
static GBitmap *s_min_0_bmp, *s_min_1_bmp, *s_min_2_bmp, *s_min_3_bmp, *s_min_4_bmp, *s_min_5_bmp, *s_min_6_bmp, *s_min_7_bmp, *s_min_8_bmp, *s_min_9_bmp;
static GBitmap *s_null_bmp;


GBitmap * get_digit(int value, bool hr) {
  // Pebble doesn't support fonts of the size we wish to use, so here we connect integers to the corresponding bitmaps
  switch(value) {
    case -1:
      return(s_null_bmp);
    case 0: 
      if (hr) return(s_hr_0_bmp);
      else return(s_min_0_bmp);
    case 1: 
      if (hr) return(s_hr_1_bmp);
      else return(s_min_1_bmp);
    case 2: 
      if (hr) return(s_hr_2_bmp);
      else return(s_min_2_bmp);
    case 3: 
      if (hr) return(s_hr_3_bmp);
      else return(s_min_3_bmp);
    case 4: 
      if (hr) return(s_hr_4_bmp);
      else return(s_min_4_bmp);
    case 5: 
      if (hr) return(s_hr_5_bmp);
      else return(s_min_5_bmp);
    case 6: 
      if (hr) return(s_hr_6_bmp);
      else return(s_min_6_bmp);
    case 7: 
      if (hr) return(s_hr_7_bmp);
      else return(s_min_7_bmp);
    case 8: 
      if (hr) return(s_hr_8_bmp);
      else return(s_min_8_bmp);
    case 9: 
      if (hr) return(s_hr_9_bmp);
      else return(s_min_9_bmp);
  }
  return(s_null_bmp);
}


static void update_time() {
  // This is the function that writes the current time on the watchface. It is called upon opening the watchface, and every minute thereafter.
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Get the digits of the current hour, in 12 or 24 hour format, as appropriate
  int hr1 = clock_is_24h_style() ? tick_time->tm_hour / 10 : (tick_time->tm_hour % 12) / 10;
  int hr2 = clock_is_24h_style() ? tick_time->tm_hour % 10 : (tick_time->tm_hour % 12) % 10;
  
  // Get the digits of the current minute
  int min1 = tick_time->tm_min / 10;
  int min2 = tick_time->tm_min % 10;
  
  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_hr_1_layer, get_digit(hr1, true));
  bitmap_layer_set_bitmap(s_hr_2_layer, get_digit(hr2, true));
  bitmap_layer_set_bitmap(s_min_1_layer, get_digit(min1, false));
  bitmap_layer_set_bitmap(s_min_2_layer, get_digit(min2, false));
}


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  // Handle the change of the minutes
  update_time();
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(s_main_window);

  // Null bitmap:
  s_null_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_null);
  
  // Initialize white GBitmaps
  s_hr_0_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_0_wh);
  s_hr_1_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_1_wh);
  s_hr_2_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_2_wh);
  s_hr_3_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_3_wh);
  s_hr_4_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_4_wh);
  s_hr_5_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_5_wh);
  s_hr_6_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_6_wh);
  s_hr_7_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_7_wh);
  s_hr_8_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_8_wh);
  s_hr_9_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_9_wh);
  
  // Initialize gray GBitmaps
  s_min_0_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_0_gr);
  s_min_1_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_1_gr);
  s_min_2_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_2_gr);
  s_min_3_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_3_gr);
  s_min_4_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_4_gr);
  s_min_5_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_5_gr);
  s_min_6_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_6_gr);
  s_min_7_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_7_gr);
  s_min_8_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_8_gr);
  s_min_9_bmp = gbitmap_create_with_resource(RESOURCE_ID_BIG_9_gr);
  
  // Create BitmapLayers to display the digits
  s_hr_1_layer = bitmap_layer_create(GRect(0,0,72,84));
  s_hr_2_layer = bitmap_layer_create(GRect(72,0,72,84));
  s_min_1_layer = bitmap_layer_create(GRect(0,84,72,84));
  s_min_2_layer = bitmap_layer_create(GRect(72,84,72,84));
  
  // Add the bitmap layers to the window
  layer_add_child(window_layer, bitmap_layer_get_layer(s_hr_1_layer));
  layer_add_child(window_layer, bitmap_layer_get_layer(s_hr_2_layer));
  layer_add_child(window_layer, bitmap_layer_get_layer(s_min_1_layer));
  layer_add_child(window_layer, bitmap_layer_get_layer(s_min_2_layer));
}


static void main_window_unload(Window *window) {
  // Destroy the numeral bitmaps:
  // Begin with the null bitmap
  gbitmap_destroy(s_null_bmp);
  
  // Destroy white GBitmaps
  gbitmap_destroy(s_hr_0_bmp);
  gbitmap_destroy(s_hr_1_bmp);
  gbitmap_destroy(s_hr_2_bmp);
  gbitmap_destroy(s_hr_3_bmp);
  gbitmap_destroy(s_hr_4_bmp);
  gbitmap_destroy(s_hr_5_bmp);
  gbitmap_destroy(s_hr_6_bmp);
  gbitmap_destroy(s_hr_7_bmp);
  gbitmap_destroy(s_hr_8_bmp);
  gbitmap_destroy(s_hr_9_bmp);
  
  // Destroy gray GBitmaps
  gbitmap_destroy(s_min_0_bmp);
  gbitmap_destroy(s_min_1_bmp);
  gbitmap_destroy(s_min_2_bmp);
  gbitmap_destroy(s_min_3_bmp);
  gbitmap_destroy(s_min_4_bmp);
  gbitmap_destroy(s_min_5_bmp);
  gbitmap_destroy(s_min_6_bmp);
  gbitmap_destroy(s_min_7_bmp);
  gbitmap_destroy(s_min_8_bmp);
  gbitmap_destroy(s_min_9_bmp);
  
  // Destroy the bitmap layers
  bitmap_layer_destroy(s_hr_1_layer);
  bitmap_layer_destroy(s_hr_2_layer);
  bitmap_layer_destroy(s_min_1_layer);
  bitmap_layer_destroy(s_min_2_layer);
}


static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Show the current time
  update_time();
}


static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}


int main(void) {
  init();
  app_event_loop();
  deinit();
}
