#include <pebble.h>
#include "alert.h"

//Alert Style Window Specific Variables
Window *alert_style_window;
TextLayer *alert_style_vibrate_layer, *alert_style_phone_layer, *alert_style_both_layer;
Layer *alert_style_background;
Layer *selection_alert_style_layer;

//Alert Style Window Specific Functions
void window_alert_create(){
  alert_style_window = window_create();
  window_set_window_handlers(alert_style_window, (WindowHandlers){
    .load = alert_style_window_load,
    .unload = alert_style_window_unload,
  });
}
void window_alert_layer_create(){
  selection_alert_style_layer = layer_create(GRect(0,0,144,168));
  alert_style_background = layer_create(GRect(0,0,144,168));
}
void window_push_alert(){
  window_stack_push(alert_style_window, true);
}
void alert_layer_proc(Layer *home_background, GContext *ctx){
  graphics_draw_rect(ctx, GRect(0,10,144,47));
  graphics_draw_rect(ctx, GRect(0,10,144,48));
  graphics_draw_rect(ctx, GRect(0,60,144,47));
  graphics_draw_rect(ctx, GRect(0,60,144,48));
  graphics_draw_rect(ctx, GRect(0,110,144,47));
  graphics_draw_rect(ctx, GRect(0,110,144,48));
}
void alert_selection_layer_1_proc(Layer *home_background, GContext *ctx){
  graphics_draw_rect(ctx, GRect(0,10,140,43));
  graphics_draw_rect(ctx, GRect(0,10,141,44));
  graphics_draw_rect(ctx, GRect(0,10,142,45));
  graphics_draw_rect(ctx, GRect(0,10,143,46));
}
void alert_selection_layer_2_proc(Layer *home_background, GContext *ctx){
  graphics_draw_rect(ctx, GRect(0,60,140,43));
  graphics_draw_rect(ctx, GRect(0,60,141,44));
  graphics_draw_rect(ctx, GRect(0,60,142,45));
  graphics_draw_rect(ctx, GRect(0,60,143,46));
}
void alert_selection_layer_3_proc(Layer *home_background, GContext *ctx){
  graphics_draw_rect(ctx, GRect(0,110,140,43));
  graphics_draw_rect(ctx, GRect(0,110,141,44));
  graphics_draw_rect(ctx, GRect(0,110,142,45));
  graphics_draw_rect(ctx, GRect(0,110,143,46));
}
void refresh_alert_layer(){
  if(glb_alert_style == 0){
      layer_set_update_proc(selection_alert_style_layer, alert_selection_layer_1_proc);
      layer_add_child(window_get_root_layer(alert_style_window), selection_alert_style_layer);
  } 
  if(glb_alert_style == 1){
      layer_set_update_proc(selection_alert_style_layer, alert_selection_layer_2_proc);
      layer_add_child(window_get_root_layer(alert_style_window), selection_alert_style_layer);  
  }
  if(glb_alert_style == 2){
      layer_set_update_proc(selection_alert_style_layer, alert_selection_layer_3_proc);
      layer_add_child(window_get_root_layer(alert_style_window), selection_alert_style_layer);  
  }
}
void alert_select_click_handler( ClickRecognizerRef recognizer, void *context){
  if(glb_alert_style == 0){
    window_stack_pop(true); 
  } 
  if(glb_alert_style == 1){
    window_stack_pop(true);
  }
  if(glb_alert_style == 2){
    window_stack_pop(true);
  }
}
void alert_up_click_handler( ClickRecognizerRef recognizer, void *context){ 
  glb_alert_style--;
  if (glb_alert_style < 0){
    glb_alert_style = 2;
  }
  refresh_alert_layer();
}
void alert_down_click_handler( ClickRecognizerRef recognizer, void *context){
  glb_alert_style++;
  if(glb_alert_style > 2){
    glb_alert_style = 0;
  }
  refresh_alert_layer();
}
void alert_click_config_provider(void *context){
  uint16_t repeat_interval_ms = 500;  // Fire every 200 ms while held down  
  window_single_click_subscribe(BUTTON_ID_SELECT, alert_select_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_UP, repeat_interval_ms, alert_up_click_handler);
  window_single_repeating_click_subscribe(BUTTON_ID_DOWN, repeat_interval_ms, alert_down_click_handler);
}
void alert_style_window_load(Window *w){
  window_set_click_config_provider(alert_style_window, alert_click_config_provider);
  layer_set_update_proc(alert_style_background, alert_layer_proc);
  alert_style_vibrate_layer = text_layer_create(GRect(0,20,138,20));
  alert_style_phone_layer = text_layer_create(GRect(0,60,138,40));
  alert_style_both_layer = text_layer_create(GRect(0,110,138,40));
  text_layer_set_text(alert_style_vibrate_layer, "Vibrate Watch");
/* Phone Alert and Alert Both Watch + Phone to come*/
//   text_layer_set_text(alert_style_phone_layer, "Phone Alert");
//   text_layer_set_text(alert_style_both_layer, "Alert Both Watch + Phone");
  text_layer_set_text(alert_style_phone_layer, "Vibrate Phone");
  text_layer_set_text(alert_style_both_layer, "Vibrate Watch + Phone");
  text_layer_set_font(alert_style_vibrate_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_font(alert_style_phone_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_font(alert_style_both_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(alert_style_vibrate_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(alert_style_phone_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(alert_style_both_layer, GTextAlignmentCenter);
  text_layer_enable_screen_text_flow_and_paging(alert_style_both_layer, 10);
  layer_add_child(window_get_root_layer(alert_style_window), text_layer_get_layer(alert_style_vibrate_layer));  
  layer_add_child(window_get_root_layer(alert_style_window), text_layer_get_layer(alert_style_phone_layer));  
  layer_add_child(window_get_root_layer(alert_style_window), text_layer_get_layer(alert_style_both_layer));  
  //add Rectangles
  layer_add_child(window_get_root_layer(alert_style_window), alert_style_background);
  refresh_alert_layer();
}
void alert_style_window_unload(Window *w){
  persist_write_int(5, glb_alert_style);
  text_layer_destroy(alert_style_vibrate_layer);
  text_layer_destroy(alert_style_phone_layer);
  text_layer_destroy(alert_style_both_layer);
}
