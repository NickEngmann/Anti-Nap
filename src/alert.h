#pragma once

int glb_alert_style; //which of the three states the alarm is currently set to

//Alert Style Window Specific Functions
void alert_layer_proc(Layer *home_background, GContext *ctx);
void alert_selection_layer_1_proc(Layer *home_background, GContext *ctx);
void alert_selection_layer_2_proc(Layer *home_background, GContext *ctx);
void alert_selection_layer_3_proc(Layer *home_background, GContext *ctx);
void refresh_alert_layer();
void alert_select_click_handler( ClickRecognizerRef recognizer, void *context);
void alert_up_click_handler( ClickRecognizerRef recognizer, void *context);
void alert_down_click_handler( ClickRecognizerRef recognizer, void *context);
void alert_click_config_provider(void *context);
void alert_style_window_load(Window *w);
void alert_style_window_unload(Window *w);
void window_alert_create();
void window_alert_layer_create();
void window_push_alert();