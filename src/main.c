#include <pebble.h>

static Window *s_window;
static TextLayer *s_text_layer;
static char time_format[] = "00:00.";
static const uint32_t const segments[] = { 
  2000, 1000, 4000, 1000, 4000
};
VibePattern pat = {
.durations = segments,
.num_segments = ARRAY_LENGTH(segments),
};
bool glb_alarm;

static void select_click_handler( ClickRecognizerRef recognizer, void *context){
  text_layer_set_text(s_text_layer, "Select");
  vibes_double_pulse();
}
static void up_click_handler( ClickRecognizerRef recognizer, void *context){
  text_layer_set_text(s_text_layer, "Up");
  vibes_long_pulse();
}
static void down_click_handler( ClickRecognizerRef recognizer, void *context){
  text_layer_set_text(s_text_layer, "Down");
  vibes_enqueue_custom_pattern(pat);
}
static void click_config_provider(void *context){
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

void tick_handler(struct tm *t, TimeUnits units_changed){
  //with this I now have the active time at all times. I can check the time_format variable against the global variable that I intend on saving
  if(!clock_is_24h_style()){
    strftime(time_format, sizeof(time_format), "%I:%M", t);
  }
  else{
    strftime(time_format, sizeof(time_format), "%H:%M", t);
  }
  text_layer_set_text(s_text_layer, time_format);
  int hours = t->tm_hour;
  int minutes = t->tm_min;
  /*//This is the code to make the alarm go off. It has to fall into the following use circumstance
    if(glb_alarm = true){
        if(hours >= glb_hours_1 && hours <= glb_hours_2){
            if(minutes >= glb_minutes_1 || minutes >=glb_minutes_2){
            
            }
        }
    }
  */
}

static void init(void) {
	// Create a window and get information about the window
	s_window = window_create();
  window_set_click_config_provider(s_window, click_config_provider);
  Layer *window_layer = window_get_root_layer(s_window);
  GRect bounds = layer_get_bounds(window_layer);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);	
  // Create a text layer and set the text
	s_text_layer = text_layer_create(bounds);
	//Old Hello World Text
  //text_layer_set_text(s_text_layer, "Hi, Making Sure I'm Running!");
  
  // Set the font and text alignment
	text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);

	// Add the text layer to the window
	layer_add_child(window_get_root_layer(s_window), text_layer_get_layer(s_text_layer));
  
  // Enable text flow and paging on the text layer, with a slight inset of 10, for round screens
  text_layer_enable_screen_text_flow_and_paging(s_text_layer, 10);

	// Push the window, setting the window animation to 'true'
	window_stack_push(s_window, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}

static void deinit(void) {
	// Destroy the text layer
	text_layer_destroy(s_text_layer);
	
	// Destroy the window
	window_destroy(s_window);
  //destroy tick handler
  tick_timer_service_unsubscribe();
}

int main(void) {
	init();
  //above event loop is create
	app_event_loop();
  //app event loop is what happens in the middle
  //deinit is what happens when the user closes the app
	deinit();
}
