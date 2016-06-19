#include <pebble.h>
#include "alarm_screen.h"
Window *alarm_screen_window;
BitmapLayer *bitmapLayer;
GBitmap *alarm_image;
TextLayer *alarm_screen_layer;
Layer *alarm_screen_background;
static uint32_t segments[] = { 
  2000, 1000, 4000, 1000, 4000, 1000, 2000, 1000, 4000, 1000, 4000, 1000, 2000, 1000, 4000, 1000, 4000, 1000, 2000, 1000, 4000, 1000, 4000, 1000,
};
VibePattern pat = {
.durations = segments,
.num_segments = ARRAY_LENGTH(segments),
};

void window_alarm_screen_layer_create(){
    bitmapLayer = bitmap_layer_create(GRect(0,0,144,168));
    alarm_screen_background = layer_create(GRect(0,0,144,168));
}

void window_push_alarm_screen(){
    window_stack_push(alarm_screen_window, true);
}

void window_alarm_screen_create(){
    alarm_screen_window = window_create();
    window_set_window_handlers(alarm_screen_window, (WindowHandlers){
      .load = alarm_screen_window_load,
      .unload = alarm_screen_window_unload,
    });
}

void alarm_screen_window_load(Window *w){
    //draw new screen
  layer_add_child(window_get_root_layer(alarm_screen_window), bitmap_layer_get_layer(bitmapLayer));
  alarm_image = gbitmap_create_with_resource(321);
  APP_LOG(APP_LOG_LEVEL_INFO, "image created %p", alarm_image);
  bitmap_layer_set_bitmap(bitmapLayer, alarm_image);
  
  alarm_screen_layer = text_layer_create(GRect(0,10,144,168));
  text_layer_set_text(alarm_screen_layer, "WAKE UP BUTTERCUP! ANTI-NAP IS ON!");
  text_layer_set_font(alarm_screen_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(alarm_screen_layer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(alarm_screen_window), text_layer_get_layer(alarm_screen_layer));
  vibes_enqueue_custom_pattern(pat);
  vibes_enqueue_custom_pattern(pat);
}

void alarm_screen_window_unload(Window *w){
  text_layer_destroy(alarm_screen_layer);
  window_stack_pop(true);  
}