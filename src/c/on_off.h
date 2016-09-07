#pragma once
bool glb_alarm; //whether or not the alarm is on
//On Off Window Functions
void on_off_layer_proc(Layer *layer, GContext *ctx);
void on_off_selection_layer_1_proc(Layer *layer, GContext *ctx);
void on_off_selection_layer_2_proc(Layer *layer, GContext *ctx);
void refresh_on_off();
void on_off_select_click_handler( ClickRecognizerRef recognizer, void *context);
void on_off_up_click_handler( ClickRecognizerRef recognizer, void *context);
void on_off_down_click_handler( ClickRecognizerRef recognizer, void *context);
void on_off_click_config_provider(void *context);
void on_off_window_load(Window *w);
void on_off_window_unload(Window *w);
void window_on_off_create();
void window_on_off_layer_create();
void window_push_on_off();