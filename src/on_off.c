#include <pebble.h>
#include "on_off.h"
//On-Off Window Specific Variables
Window *on_off_window;
TextLayer *on_off_layer, *on_off_on, *on_off_off;
Layer *on_off_background;
Layer *selection_on_off_layer;

  //On Off Window Functions
  void window_on_off_layer_create(){
      selection_on_off_layer = layer_create(GRect(0,0,144,168));
      on_off_background = layer_create(GRect(0,0,144,168));
   }
  void window_push_on_off(){
        
        window_stack_push(on_off_window, true);
  }
   void window_on_off_create(){
      //initialize on_off window
      on_off_window = window_create();
      window_set_window_handlers(on_off_window, (WindowHandlers){
        .load = on_off_window_load,
        .unload = on_off_window_unload,
      });
   }
  void on_off_layer_proc(Layer *layer, GContext *ctx){
    //DRAWS RECTANGLES
    graphics_draw_rect(ctx, GRect(52,84,40,31));
    graphics_draw_rect(ctx, GRect(52,84,40,32));
    graphics_draw_rect(ctx, GRect(52,115,40,31));
    graphics_draw_rect(ctx, GRect(52,115,40,32));
  }
  void on_off_selection_layer_1_proc(Layer *layer, GContext *ctx){
    graphics_draw_rect(ctx, GRect(52,84,36,27));
    graphics_draw_rect(ctx, GRect(52,84,37,28));
    graphics_draw_rect(ctx, GRect(52,84,38,29));
    graphics_draw_rect(ctx, GRect(52,84,39,30));
  }
  void on_off_selection_layer_2_proc(Layer *layer, GContext *ctx){
    graphics_draw_rect(ctx, GRect(52,115,36,27));
    graphics_draw_rect(ctx, GRect(52,115,37,28));
    graphics_draw_rect(ctx, GRect(52,115,38,29));
    graphics_draw_rect(ctx, GRect(52,115,39,30));
  }
  void refresh_on_off(){
    if(glb_alarm == true){
        layer_set_update_proc(selection_on_off_layer, on_off_selection_layer_1_proc);
        layer_add_child(window_get_root_layer(on_off_window), selection_on_off_layer);  
    } 
    if(glb_alarm == false){
        layer_set_update_proc(selection_on_off_layer, on_off_selection_layer_2_proc);
        layer_add_child(window_get_root_layer(on_off_window), selection_on_off_layer);
    } 
  }
  void on_off_select_click_handler( ClickRecognizerRef recognizer, void *context){
    window_stack_pop(true);  
  }
  void on_off_up_click_handler( ClickRecognizerRef recognizer, void *context){ 
    if(glb_alarm == true){
      glb_alarm = false;
    }
    else{
      glb_alarm = true;
    }
    refresh_on_off();
  }
  void on_off_down_click_handler( ClickRecognizerRef recognizer, void *context){
    if(glb_alarm == true){
      glb_alarm = false;
    }
    else{
      glb_alarm = true;
    }
    refresh_on_off();
  }
  void on_off_click_config_provider(void *context){
    window_single_click_subscribe(BUTTON_ID_SELECT, on_off_select_click_handler);
    window_single_click_subscribe(BUTTON_ID_UP, on_off_up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, on_off_down_click_handler);
  }
  void on_off_window_load(Window *w){
    //initialize rectangles
    layer_set_update_proc(on_off_background, on_off_layer_proc);
    //place all the appropriate text
    on_off_layer = text_layer_create(GRect(0,10,144,168));
    on_off_on = text_layer_create(GRect(0,80,144,30));
    on_off_off = text_layer_create(GRect(0,110,144,30));
    text_layer_set_text(on_off_layer, "Anti-Nap is Currently:");
    text_layer_set_text(on_off_on, "ON");
    text_layer_set_text(on_off_off, "OFF");
    text_layer_set_font(on_off_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_font(on_off_on, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_font(on_off_off, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(on_off_layer, GTextAlignmentCenter);
    text_layer_set_text_alignment(on_off_on, GTextAlignmentCenter);
    text_layer_set_text_alignment(on_off_off, GTextAlignmentCenter);
    text_layer_enable_screen_text_flow_and_paging(on_off_layer, 10);
    layer_add_child(window_get_root_layer(on_off_window), text_layer_get_layer(on_off_layer));
    layer_add_child(window_get_root_layer(on_off_window), text_layer_get_layer(on_off_on));
    layer_add_child(window_get_root_layer(on_off_window), text_layer_get_layer(on_off_off));
    //add Rectangles
    layer_add_child(window_get_root_layer(on_off_window), on_off_background);
    //selection rectangles
    refresh_on_off();
    window_set_click_config_provider(on_off_window, on_off_click_config_provider); 
  };

  void on_off_window_unload(Window *w){
    persist_write_int(0, glb_alarm);
    // Destroy the text layers
    text_layer_destroy(on_off_layer);
    text_layer_destroy(on_off_on);
    text_layer_destroy(on_off_off);
};