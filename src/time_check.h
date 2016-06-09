#pragma once

int glb_hours_1; //start hour
int glb_hours_2; //end hour
int glb_minutes_1; //start minute 
int glb_minutes_2; //end minute

void time_selection_layer_1_proc(Layer *layer, GContext *ctx);
void time_selection_layer_2_proc(Layer *layer, GContext *ctx);
void time_selection_layer_3_proc(Layer *layer, GContext *ctx);
void time_selection_layer_4_proc(Layer *layer, GContext *ctx);
void refresh_time_window();
void time_input_select_click_handler( ClickRecognizerRef recognizer, void *context);
void time_input_up_click_handler( ClickRecognizerRef recognizer, void *context);
void time_input_down_click_handler( ClickRecognizerRef recognizer, void *context);
void time_input_click_config_provider(void *context);
void time_window_load(Window *w);
void time_window_unload(Window *w);
void window_time_create();
void window_time_layer_create();
void window_push_time();