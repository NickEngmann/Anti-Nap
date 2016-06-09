#include <pebble.h>
#include "main.h"
#include "on_off.h"
#include "time_check.h"
#include "alert.h"

//Background and Selection for Home Layer
void home_layer_proc(Layer *home_background, GContext *ctx){
  graphics_draw_rect(ctx, GRect(0,10,144,47));
  graphics_draw_rect(ctx, GRect(0,10,144,48));
  graphics_draw_rect(ctx, GRect(0,60,144,47));
  graphics_draw_rect(ctx, GRect(0,60,144,48));
  graphics_draw_rect(ctx, GRect(0,110,144,47));
  graphics_draw_rect(ctx, GRect(0,110,144,48));
}
void selection_layer_1_proc(Layer *layer, GContext *ctx){
  graphics_draw_rect(ctx, GRect(0,10,140,43));
  graphics_draw_rect(ctx, GRect(0,10,141,44));
  graphics_draw_rect(ctx, GRect(0,10,142,45));
  graphics_draw_rect(ctx, GRect(0,10,143,46));
}
void selection_layer_2_proc(Layer *layer, GContext *ctx){
  graphics_draw_rect(ctx, GRect(0,60,140,43));
  graphics_draw_rect(ctx, GRect(0,60,141,44));
  graphics_draw_rect(ctx, GRect(0,60,142,45));
  graphics_draw_rect(ctx, GRect(0,60,143,46));
}
void selection_layer_3_proc(Layer *layer, GContext *ctx){
  graphics_draw_rect(ctx, GRect(0,110,140,43));
  graphics_draw_rect(ctx, GRect(0,110,141,44));
  graphics_draw_rect(ctx, GRect(0,110,142,45));
  graphics_draw_rect(ctx, GRect(0,110,143,46));
}
//Home Window Specific Functions
void refresh_home_layer(){
  if(home_selection == 0){
      layer_set_update_proc(selection_layer, selection_layer_1_proc);
      layer_add_child(window_get_root_layer(home_window), selection_layer);
  } 
  if(home_selection == 1){
      layer_set_update_proc(selection_layer, selection_layer_2_proc);
      layer_add_child(window_get_root_layer(home_window), selection_layer);  
  }
  if(home_selection == 2){
      layer_set_update_proc(selection_layer, selection_layer_3_proc);
      layer_add_child(window_get_root_layer(home_window), selection_layer);  
  }
}
void home_window_load(Window *home_window){
  layer_set_update_proc(home_background, home_layer_proc);	
  // Create text layer
	s_text_home_layer_1 = text_layer_create(GRect(0,10,138,20));
	s_text_home_layer_1_1 = text_layer_create(GRect(0,30,138,20));
  s_text_home_layer_2 = text_layer_create(GRect(0,60,138,20));
  s_text_home_layer_2_2 = text_layer_create(GRect(0,80,138,20));
  s_text_home_layer_3 = text_layer_create(GRect(0,110,138,20));
  s_text_home_layer_3_3 = text_layer_create(GRect(0,130,138,20));
  //Set Home Window Text
  text_layer_set_text(s_text_home_layer_1, "Currently On/Off");
  text_layer_set_text(s_text_home_layer_1_1, "Turn App On or Off");
  text_layer_set_text(s_text_home_layer_2, "Time Set");
  text_layer_set_text(s_text_home_layer_2_2, "Set Time Frame");
  text_layer_set_text(s_text_home_layer_3, "Alert Style");
  text_layer_set_text(s_text_home_layer_3_3, "Change Alert Style");
  
  // Set the font and text alignment for main text
	text_layer_set_font(s_text_home_layer_1, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
	text_layer_set_font(s_text_home_layer_2, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_font(s_text_home_layer_3, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
	text_layer_set_text_alignment(s_text_home_layer_1, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_text_home_layer_2, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_text_home_layer_3, GTextAlignmentCenter);
  // Set the font and text alignment for sub text
	text_layer_set_font(s_text_home_layer_1_1, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_font(s_text_home_layer_2_2, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_font(s_text_home_layer_3_3, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_text_alignment(s_text_home_layer_1_1, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_text_home_layer_2_2, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_text_home_layer_3_3, GTextAlignmentCenter);
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(home_window), text_layer_get_layer(s_text_home_layer_1));
	layer_add_child(window_get_root_layer(home_window), text_layer_get_layer(s_text_home_layer_1_1));
  layer_add_child(window_get_root_layer(home_window), text_layer_get_layer(s_text_home_layer_2));
  layer_add_child(window_get_root_layer(home_window), text_layer_get_layer(s_text_home_layer_2_2));
  layer_add_child(window_get_root_layer(home_window), text_layer_get_layer(s_text_home_layer_3));
  layer_add_child(window_get_root_layer(home_window), text_layer_get_layer(s_text_home_layer_3_3));
  
  //add Rectangles
  layer_add_child(window_get_root_layer(home_window), home_background);
  // Enable text flow and paging on the text layer, with a slight inset of 10, for round screens
  text_layer_enable_screen_text_flow_and_paging(s_text_home_layer_1, 10);
  text_layer_enable_screen_text_flow_and_paging(s_text_home_layer_2, 10);
  text_layer_enable_screen_text_flow_and_paging(s_text_home_layer_3, 10);
	// Push the window, setting the window animation to 'true'
  refresh_home_layer();
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}
void home_window_unload(Window *home_window){
  // Destroy the text layers
	text_layer_destroy(s_text_home_layer_1);
	text_layer_destroy(s_text_home_layer_2);
  text_layer_destroy(s_text_home_layer_3);
  text_layer_destroy(s_text_home_layer_1_1);
	text_layer_destroy(s_text_home_layer_2_2);
  text_layer_destroy(s_text_home_layer_3_3);
}
static void select_click_handler( ClickRecognizerRef recognizer, void *context){
  if(home_selection == 0){
    window_push_on_off();
  } 
  if(home_selection == 1){
    window_push_time();
  }
  if(home_selection == 2){
    window_push_alert();
  }
}
static void up_click_handler( ClickRecognizerRef recognizer, void *context){ 
  if (home_selection > 0){
    home_selection--;
  }
  refresh_home_layer();
}
static void down_click_handler( ClickRecognizerRef recognizer, void *context){
  if(home_selection < 2){
    home_selection++;
  }
  refresh_home_layer();
}
static void click_config_provider(void *context){
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  
  //On Off Screen Click handlers
  //window_single_click_subscribe(BUTTON_ID_UP, on-off_up_click_handler);
  //window_single_click_subscribe(BUTTON_ID_DOWN, on-off_down_click_handler);
}
void tick_handler(struct tm *t, TimeUnits units_changed){
  if(glb_alarm == 1){
    int time_start = (glb_hours_1*1000)+glb_minutes_1;
    int time_end = (glb_hours_2*1000)+glb_minutes_2;
    AppWorkerMessage message_fore = {
      .data0 = glb_alarm,
      .data1 = time_start,
      .data2 = time_end,
    };
  app_worker_send_message(1, &message_fore);
  }
}

void worker_message_handler(uint16_t key, AppWorkerMessage *data){
  int checker = data->data2;
  //glb_alarm = checker;
  //glb_hours_1 = (data->data1)/1000;
  //glb_hours_2 = (data->data2)/1000;
  //glb_minutes_1 = (data->data1)%1000;
  //glb_minutes_2 = (data->data2)%1000;
  if (checker == 1){
    //transfer the alarm variables back from the backgroun
    //activate alarm
    //create a BitMap Screen?
      vibes_enqueue_custom_pattern(pat); 
  }
}
static void init(void) {
	// Create a window and get information about the window
  home_window = window_create();
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  glb_hours_1 = 7;
  glb_hours_2 = 11;
  glb_minutes_2 = 59;
  //all things worker
  AppWorkerResult result;
  result = app_worker_launch();
  APP_LOG(APP_LOG_LEVEL_INFO, "Result: %d", result);
  app_worker_message_subscribe(worker_message_handler);

  //layers creation
  window_on_off_layer_create();//Create layers for on_off
  window_time_layer_create(); //Create layers for time
  window_alert_layer_create(); //create layer for alerts
  home_background = layer_create(GRect(0,0,144,168));
  selection_layer = layer_create(GRect(0,0,144,168));
  window_set_click_config_provider(home_window, click_config_provider); //Click Fig Provider
  // Set Window Handlers
  window_set_window_handlers(home_window, (WindowHandlers){
    .load = home_window_load,
    .unload = home_window_unload,
  });
  window_stack_push(home_window, true);  
  //initialize on_off_window
  window_on_off_create();
  //initialize time window
  window_time_create();
  //initialize alert style window
  window_alert_create();
  
}

static void deinit(void) {
	// Destroy the window
	window_destroy(home_window);
  //destroy tick handler
  tick_timer_service_unsubscribe();
  //destroy the BackGround Worker
  //if(glb_alarm == 0){
   // app_worker_kill();
  //}
}

int main(void) {
	init();
  //above event loop is create
  app_event_loop();//app event loop is what happens in the middle
  //deinit is what happens when the user closes the app
	deinit();
}
