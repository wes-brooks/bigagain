#include "pebble.h"
#include "effect_layer.h"

static Window *s_main_window;

static BitmapLayer *s_hr_1_layer, *s_hr_2_layer, *s_min_1_layer, *s_min_2_layer;
static GBitmap *s_hr_0_bmp, *s_hr_1_bmp, *s_hr_2_bmp, *s_hr_3_bmp, *s_hr_4_bmp, *s_hr_5_bmp, *s_hr_6_bmp, *s_hr_7_bmp, *s_hr_8_bmp, *s_hr_9_bmp;
static GBitmap *s_min_0_bmp, *s_min_1_bmp, *s_min_2_bmp, *s_min_3_bmp, *s_min_4_bmp, *s_min_5_bmp, *s_min_6_bmp, *s_min_7_bmp, *s_min_8_bmp, *s_min_9_bmp;
static GBitmap *s_null_bmp;
static bool solid_minutes;
static EffectLayer *effectlayer;


#define KEY_COLOR_A_RED     0
#define KEY_COLOR_A_GREEN   1
#define KEY_COLOR_A_BLUE    2
#define KEY_COLOR_B_RED     3
#define KEY_COLOR_B_GREEN   4
#define KEY_COLOR_B_BLUE    5
#define KEY_SOLID_MINUTES   6


GBitmap * get_digit(int value, bool hr) {
  // Pebble doesn't support fonts of the size we wish to use, so here we connect integers to the corresponding bitmaps
  GBitmap *mybitmap = s_null_bmp;
  
  switch(value) {
    case -1:
      mybitmap = s_null_bmp;
      break;
    case 0: 
      if (hr) mybitmap = s_hr_0_bmp;
      else mybitmap = s_min_0_bmp;
      break;
    case 1: 
      if (hr) mybitmap = s_hr_1_bmp;
      else mybitmap = s_min_1_bmp;
      break;
    case 2: 
      if (hr) mybitmap = s_hr_2_bmp;
      else mybitmap = s_min_2_bmp;
      break;
    case 3: 
      if (hr) mybitmap = s_hr_3_bmp;
      else mybitmap = s_min_3_bmp;
      break;
    case 4: 
      if (hr) mybitmap = s_hr_4_bmp;
      else mybitmap = s_min_4_bmp;
      break;
    case 5: 
      if (hr) mybitmap = s_hr_5_bmp;
      else mybitmap = s_min_5_bmp;
      break;
    case 6: 
      if (hr) mybitmap = s_hr_6_bmp;
      else mybitmap = s_min_6_bmp;
      break;
    case 7: 
      if (hr) mybitmap = s_hr_7_bmp;
      else mybitmap = s_min_7_bmp;
      break;
    case 8: 
      if (hr) mybitmap = s_hr_8_bmp;
      else mybitmap = s_min_8_bmp;
      break;
    case 9: 
      if (hr) mybitmap = s_hr_9_bmp;
      else mybitmap = s_min_9_bmp;
      break;
  }
  
  return(mybitmap);
}


static void draw_digit(int numeral, bool hr, BitmapLayer *layer)
{
  GBitmap *mybitmap = get_digit(numeral, hr);
  bitmap_layer_set_bitmap(layer, mybitmap);
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
  draw_digit(hr1, true, s_hr_1_layer);
  draw_digit(hr2, true, s_hr_2_layer);
  draw_digit(min1, solid_minutes, s_min_1_layer);
  draw_digit(min2, solid_minutes, s_min_2_layer);
  
  layer_mark_dirty(bitmap_layer_get_layer(s_hr_1_layer));
  layer_mark_dirty(bitmap_layer_get_layer(s_hr_2_layer));
  layer_mark_dirty(bitmap_layer_get_layer(s_min_1_layer));
  layer_mark_dirty(bitmap_layer_get_layer(s_min_2_layer));
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  // Save and apply the color settings from the phone
  
  // Extract the selected foreground color, save it and apply it:
  Tuple *color_a_red_t = dict_find(iter, KEY_COLOR_A_RED);
  Tuple *color_a_green_t = dict_find(iter, KEY_COLOR_A_GREEN);
  Tuple *color_a_blue_t = dict_find(iter, KEY_COLOR_A_BLUE);
  if(color_a_red_t && color_a_green_t && color_a_blue_t) {
    // Save the color
    int red_a = color_a_red_t->value->int32;
    int green_a = color_a_green_t->value->int32;
    int blue_a = color_a_blue_t->value->int32;

    // Persist values
    persist_write_int(KEY_COLOR_A_RED, red_a);
    persist_write_int(KEY_COLOR_A_GREEN, green_a);
    persist_write_int(KEY_COLOR_A_BLUE, blue_a);

    color_a = GColorFromRGB(red_a, green_a, blue_a).argb;
  }

  // Extract the selected color, save it and apply it:
  Tuple *color_b_red_t = dict_find(iter, KEY_COLOR_B_RED);
  Tuple *color_b_green_t = dict_find(iter, KEY_COLOR_B_GREEN);
  Tuple *color_b_blue_t = dict_find(iter, KEY_COLOR_B_BLUE);
  if(color_b_red_t && color_b_green_t && color_b_blue_t) {
    // Save the color
    int red_b = color_b_red_t->value->int32;
    int green_b = color_b_green_t->value->int32;
    int blue_b = color_b_blue_t->value->int32;

    // Persist values
    persist_write_int(KEY_COLOR_B_RED, red_b);
    persist_write_int(KEY_COLOR_B_GREEN, green_b);
    persist_write_int(KEY_COLOR_B_BLUE, blue_b);

    color_b = GColorFromRGB(red_b, green_b, blue_b).argb;
  }
  
  // Should minutes be shown in solid color?
  Tuple *solid_minutes_t = dict_find(iter, KEY_SOLID_MINUTES);
  if(solid_minutes_t) {
    int val = solid_minutes_t->value->int32;
    solid_minutes = (val == 0 ? false : true);
    persist_write_bool(KEY_SOLID_MINUTES, solid_minutes);
  }
  
  // Apply the new styling
  update_time();
}


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  // Handle the change of the minutes
  update_time();
}


static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(s_main_window);
  
  // Color defaults
  color_a = GColorBlack.argb;
  color_b = GColorWhite.argb;
  solid_minutes = false;
  
  if (persist_exists(KEY_COLOR_A_RED) && persist_exists(KEY_COLOR_A_GREEN) && persist_exists(KEY_COLOR_A_BLUE)) {
    // Use saved color setting
    int red_a = persist_read_int(KEY_COLOR_A_RED);
    int green_a = persist_read_int(KEY_COLOR_A_GREEN);
    int blue_a = persist_read_int(KEY_COLOR_A_BLUE);
    color_a = GColorFromRGB(red_a, green_a, blue_a).argb;
  } else { color_a = GColorBlack.argb; }
  
  if (persist_exists(KEY_COLOR_B_RED) && persist_exists(KEY_COLOR_B_GREEN) && persist_exists(KEY_COLOR_B_BLUE)) {
    // Use saved color setting
    int red_b = persist_read_int(KEY_COLOR_B_RED);
    int green_b = persist_read_int(KEY_COLOR_B_GREEN);
    int blue_b = persist_read_int(KEY_COLOR_B_BLUE);
    color_b = GColorFromRGB(red_b, green_b, blue_b).argb;
  } else { color_b = GColorWhite.argb; }
  
  if (persist_exists(KEY_SOLID_MINUTES)) {
    // Use saved setting
    solid_minutes = persist_read_bool(KEY_SOLID_MINUTES);
  }
  
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
  
  // Create an EffectLayer
  effectlayer = effect_layer_create(GRect(0, 0, 144, 168));    
  layer_add_child(window_get_root_layer(s_main_window), effect_layer_get_layer(effectlayer));
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
  
  // Destroy the EffectLayer
  effect_layer_destroy(effectlayer);
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
  
  // Register with TickTimerService for minutely updates
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Register handlers that interpret incoming communications
  app_message_register_inbox_received(inbox_received_handler);
  //app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  app_message_open(124, 16);
  
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
