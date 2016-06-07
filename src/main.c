#include <pebble.h>
//Home Screen Variables
static Window *home_window;
Layer *home_background;
Layer *selection_layer;
static TextLayer *top_text_layer;
static TextLayer *s_text_home_layer_1, *s_text_home_layer_2,*s_text_home_layer_3,*s_text_home_layer_1_1,*s_text_home_layer_2_2,*s_text_home_layer_3_3;
static int home_selection = 0;
static char time_format[] = "00:00.";
static uint32_t segments[] = { 
  2000, 1000, 4000, 1000, 4000, 1000, 2000, 1000, 4000, 1000, 4000, 1000, 2000, 1000, 4000, 1000, 4000, 1000, 2000, 1000, 4000, 1000, 4000, 1000,
};
VibePattern pat = {
.durations = segments,
.num_segments = ARRAY_LENGTH(segments),
};

  //On-Off Window Specific Variables
  static Window *on_off_window;
  static TextLayer *on_off_layer, *on_off_on, *on_off_off;
  Layer *on_off_background;
  Layer *selection_on_off_layer;
  bool glb_alarm; //whether or not the alarm is on
  //On Off Window Functions
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
  static void on_off_select_click_handler( ClickRecognizerRef recognizer, void *context){
    window_stack_pop(true);  
    window_stack_push(home_window, true);
  }
  static void on_off_up_click_handler( ClickRecognizerRef recognizer, void *context){ 
    if(glb_alarm == true){
      glb_alarm = false;
    }
    else{
      glb_alarm = true;
    }
    refresh_on_off();
  }
  static void on_off_down_click_handler( ClickRecognizerRef recognizer, void *context){
    if(glb_alarm == true){
      glb_alarm = false;
    }
    else{
      glb_alarm = true;
    }
    refresh_on_off();
  }
  static void on_off_click_config_provider(void *context){
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
    // Destroy the text layers
    text_layer_destroy(on_off_layer);
    text_layer_destroy(on_off_on);
    text_layer_destroy(on_off_off);
};

  //Time Set Window Specific Variables
  static Window *time_window;
  static TextLayer *time_layer, *time_am_hour_input,*time_am_minute_input, *time_am_layer, *time_pm_hour_input, *time_pm_minute_input, *time_pm_layer;
  Layer *selection_time_layer;
  int glb_hours_1; //start hour
  int glb_hours_2; //end hour
  int glb_minutes_1; //start minute 
  int glb_minutes_2; //end minute
  char am_hour_buf[] = "00";
  char am_minute_buf[] = "000";
  char pm_hour_buf[] = "00";
  char pm_minute_buf[] = "000";
  int select_time;
  //Time Input Functions
  void time_selection_layer_1_proc(Layer *layer, GContext *ctx){
    graphics_draw_rect(ctx, GRect(25,84,33,30));
  }
  void time_selection_layer_2_proc(Layer *layer, GContext *ctx){
    graphics_draw_rect(ctx, GRect(56,84,34,30));
  }
  void time_selection_layer_3_proc(Layer *layer, GContext *ctx){
    graphics_draw_rect(ctx, GRect(25,124,33,30));
  }
  void time_selection_layer_4_proc(Layer *layer, GContext *ctx){
    graphics_draw_rect(ctx, GRect(56,124,34,30));
  }
  void refresh_time_window(){
    //am refresh
    if(select_time==0){
        layer_set_update_proc(selection_time_layer, time_selection_layer_1_proc);
        layer_add_child(window_get_root_layer(time_window), selection_time_layer);  
    }
    if(select_time==1){
        layer_set_update_proc(selection_time_layer, time_selection_layer_2_proc);
        layer_add_child(window_get_root_layer(time_window), selection_time_layer);  
    }
    if(select_time==2){
        layer_set_update_proc(selection_time_layer, time_selection_layer_3_proc);
        layer_add_child(window_get_root_layer(time_window), selection_time_layer);  
    }
    if(select_time==3){
        layer_set_update_proc(selection_time_layer, time_selection_layer_4_proc);
        layer_add_child(window_get_root_layer(time_window), selection_time_layer);  
    }
    snprintf(am_hour_buf, sizeof(am_hour_buf), "%d", glb_hours_1);
    text_layer_set_text(time_am_hour_input, am_hour_buf);
      if(glb_minutes_1 < 10){
        snprintf(am_minute_buf, sizeof(am_minute_buf), ":0%d", glb_minutes_1);
      }
      else{
        snprintf(am_minute_buf, sizeof(am_minute_buf), ":%d", glb_minutes_1);
      }
    text_layer_set_text(time_am_minute_input, am_minute_buf);
    //pm refresh
    snprintf(pm_hour_buf, sizeof(pm_hour_buf), "%d", glb_hours_2);
    text_layer_set_text(time_pm_hour_input, pm_hour_buf);
      if(glb_minutes_2 < 10){
        snprintf(pm_minute_buf, sizeof(pm_minute_buf), ":0%d", glb_minutes_2);
      }
      else{
        snprintf(pm_minute_buf, sizeof(am_minute_buf), ":%d", glb_minutes_2);
      }
    text_layer_set_text(time_pm_minute_input, pm_minute_buf);
   }
  static void time_input_select_click_handler( ClickRecognizerRef recognizer, void *context){
    select_time++;
      if(select_time == 4){
        select_time = 0;
      }
    refresh_time_window();
    }
  static void time_input_up_click_handler( ClickRecognizerRef recognizer, void *context){ 
    if(select_time == 0){
      glb_hours_1++;
        if(glb_hours_1 > 12){
          glb_hours_1 = 1;
        }
      }
      if(select_time == 1){
        glb_minutes_1++;
          if(glb_minutes_1 > 59){
            glb_minutes_1 = 0;
          }
      }
      if(select_time == 2){
      glb_hours_2++;
        if(glb_hours_2 > 12){
          glb_hours_2 = 1;
        }
      }
      if(select_time == 3){
        glb_minutes_2++;
          if(glb_minutes_2 > 59){
            glb_minutes_2 = 0;
          }
      }
    refresh_time_window();
    }
  static void time_input_down_click_handler( ClickRecognizerRef recognizer, void *context){
    if(select_time == 0){
      glb_hours_1--;
      if(glb_hours_1 < 1){
        glb_hours_1 = 12;
      }
    }
    if(select_time == 1){
      glb_minutes_1--;
        if(glb_minutes_1 < 0){
          glb_minutes_1 = 59;
        }
    }
    if(select_time == 2){
      glb_hours_2--;
      if(glb_hours_2 < 1){
        glb_hours_2 = 12;
      }
    }
    if(select_time == 3){
      glb_minutes_2--;
        if(glb_minutes_2 < 0){
          glb_minutes_2 = 59;
        }
    }
    refresh_time_window();
  }
  static void time_input_click_config_provider(void *context){
    uint16_t repeat_interval_ms = 100; 
    window_single_click_subscribe(BUTTON_ID_SELECT, time_input_select_click_handler);
    window_single_repeating_click_subscribe(BUTTON_ID_UP,repeat_interval_ms, time_input_up_click_handler);
    window_single_repeating_click_subscribe(BUTTON_ID_DOWN,repeat_interval_ms, time_input_down_click_handler);
  }
  void time_window_load(Window *w){
    //time input variables
      time_am_hour_input = text_layer_create(GRect(30,80,30,30));
      text_layer_set_background_color(time_am_hour_input, GColorClear);
      time_am_minute_input = text_layer_create(GRect(58,80,30,30)); 
      text_layer_set_background_color(time_am_minute_input, GColorClear);
      //printing am hour/minute variables
      snprintf(am_hour_buf, sizeof(am_hour_buf), "%d", glb_hours_1);
      text_layer_set_text(time_am_hour_input, am_hour_buf);
        if(glb_minutes_1 < 10){
          snprintf(am_minute_buf, sizeof(am_minute_buf), ":0%d", glb_minutes_1);
        }
        else{
          snprintf(am_minute_buf, sizeof(am_minute_buf), ":%d", glb_minutes_1);
        }
      text_layer_set_text(time_am_minute_input, am_minute_buf);
      //printing pm hour/minute variables
      time_pm_hour_input = text_layer_create(GRect(30,120,30,30));
      text_layer_set_background_color(time_pm_hour_input, GColorClear);
      time_pm_minute_input = text_layer_create(GRect(58,120,30,30)); 
      text_layer_set_background_color(time_pm_minute_input, GColorClear);
      snprintf(pm_hour_buf, sizeof(pm_hour_buf), "%d", glb_hours_2);
      text_layer_set_text(time_pm_hour_input, pm_hour_buf);
        if(glb_minutes_2 < 10){
          snprintf(pm_minute_buf, sizeof(pm_minute_buf), ":0%d", glb_minutes_2);
        }
        else{
          snprintf(pm_minute_buf, sizeof(am_minute_buf), ":%d", glb_minutes_2);
        }
      text_layer_set_text(time_pm_minute_input, pm_minute_buf);
    //time window variables
    time_layer = text_layer_create(GRect(0,10,144,168));
    time_am_layer = text_layer_create(GRect(35,80,144,30));
    time_pm_layer = text_layer_create(GRect(40,120,144,30));
    text_layer_set_text(time_layer, "Currently set to run from:");
    text_layer_set_text(time_am_layer, "AM");
    text_layer_set_text(time_pm_layer, "PM");
    text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_font(time_am_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
    text_layer_set_font(time_am_hour_input, fonts_get_system_font(FONT_KEY_GOTHIC_28));
    text_layer_set_font(time_am_minute_input, fonts_get_system_font(FONT_KEY_GOTHIC_28));
    text_layer_set_font(time_pm_hour_input, fonts_get_system_font(FONT_KEY_GOTHIC_28));
    text_layer_set_font(time_pm_minute_input, fonts_get_system_font(FONT_KEY_GOTHIC_28));  text_layer_set_font(time_pm_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
    text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
    text_layer_set_text_alignment(time_am_layer, GTextAlignmentCenter);
    text_layer_set_text_alignment(time_pm_layer, GTextAlignmentCenter);
    text_layer_enable_screen_text_flow_and_paging(time_layer, 10);
    //default background text added
    layer_add_child(window_get_root_layer(time_window), text_layer_get_layer(time_layer));
    layer_add_child(window_get_root_layer(time_window), text_layer_get_layer(time_am_layer));
    layer_add_child(window_get_root_layer(time_window), text_layer_get_layer(time_pm_layer));
    //time variables added
    layer_add_child(window_get_root_layer(time_window), text_layer_get_layer(time_am_hour_input));
    layer_add_child(window_get_root_layer(time_window), text_layer_get_layer(time_am_minute_input));
    layer_add_child(window_get_root_layer(time_window), text_layer_get_layer(time_pm_hour_input));
    layer_add_child(window_get_root_layer(time_window), text_layer_get_layer(time_pm_minute_input));
    window_set_click_config_provider(time_window, time_input_click_config_provider);  
    refresh_time_window();
  }
  void time_window_unload(Window *w){
    text_layer_destroy(time_layer);
    text_layer_destroy(time_pm_layer);
    text_layer_destroy(time_am_layer);
  }

  //Alert Style Window Specific Variables
  static Window *alert_style_window;
  static TextLayer *alert_style_vibrate_layer, *alert_style_phone_layer, *alert_style_both_layer;
  Layer *alert_style_background;
  Layer *selection_alert_style_layer;
  int glb_alert_style; //which of the three states the alarm is currently set to
  //Alert Style Window Specific Functions
  void alert_layer_proc(Layer *home_background, GContext *ctx){
    graphics_draw_rect(ctx, GRect(0,10,144,47));
    graphics_draw_rect(ctx, GRect(0,10,144,48));
    graphics_draw_rect(ctx, GRect(0,60,144,47));
    graphics_draw_rect(ctx, GRect(0,60,144,48));
    graphics_draw_rect(ctx, GRect(0,110,144,47));
    graphics_draw_rect(ctx, GRect(0,110,144,48));
  }
  void alert_selection_layer_1_proc(Layer *layer, GContext *ctx){
    graphics_draw_rect(ctx, GRect(0,10,140,43));
    graphics_draw_rect(ctx, GRect(0,10,141,44));
    graphics_draw_rect(ctx, GRect(0,10,142,45));
    graphics_draw_rect(ctx, GRect(0,10,143,46));
  }
  void alert_selection_layer_2_proc(Layer *layer, GContext *ctx){
    graphics_draw_rect(ctx, GRect(0,60,140,43));
    graphics_draw_rect(ctx, GRect(0,60,141,44));
    graphics_draw_rect(ctx, GRect(0,60,142,45));
    graphics_draw_rect(ctx, GRect(0,60,143,46));
  }
  void alert_selection_layer_3_proc(Layer *layer, GContext *ctx){
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
  static void alert_select_click_handler( ClickRecognizerRef recognizer, void *context){
    if(glb_alert_style == 0){
      window_stack_pop(true); 
      window_stack_push(home_window, true);
    } 
    if(glb_alert_style == 1){
      window_stack_pop(true);
      window_stack_push(home_window, true);
    }
    if(glb_alert_style == 2){
      window_stack_pop(true);
      window_stack_push(home_window, true);
    }
  }
  static void alert_up_click_handler( ClickRecognizerRef recognizer, void *context){ 
    glb_alert_style--;
    if (glb_alert_style < 0){
      glb_alert_style = 2;
    }
    refresh_alert_layer();
  }
  static void alert_down_click_handler( ClickRecognizerRef recognizer, void *context){
    glb_alert_style++;
    if(glb_alert_style > 2){
      glb_alert_style = 0;
    }
    refresh_alert_layer();
  }
  static void alert_click_config_provider(void *context){
    uint16_t repeat_interval_ms = 500;  // Fire every 200 ms while held down  
    window_single_click_subscribe(BUTTON_ID_SELECT, alert_select_click_handler);
    window_single_repeating_click_subscribe(BUTTON_ID_UP, repeat_interval_ms, alert_up_click_handler);
    window_single_repeating_click_subscribe(BUTTON_ID_DOWN, repeat_interval_ms, alert_down_click_handler);
  }
  void alert_style_window_load(Window *w){
    window_set_click_config_provider(alert_style_window, alert_click_config_provider);
    layer_set_update_proc(alert_style_background, alert_layer_proc);
    
    alert_style_vibrate_layer = text_layer_create(GRect(0,20,138,20));
    alert_style_phone_layer = text_layer_create(GRect(0,70,138,20));
    alert_style_both_layer = text_layer_create(GRect(0,110,138,40));
    text_layer_set_text(alert_style_vibrate_layer, "Vibrate Watch");
    text_layer_set_text(alert_style_phone_layer, "Phone Alert");
    text_layer_set_text(alert_style_both_layer, "Alert Both Watch + Phone");
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
    text_layer_destroy(alert_style_vibrate_layer);
    text_layer_destroy(alert_style_phone_layer);
    text_layer_destroy(alert_style_both_layer);
  }

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
    window_stack_push(on_off_window, true);
  } 
  if(home_selection == 1){
    window_stack_push(time_window, true);
  }
  if(home_selection == 2){
    window_stack_push(alert_style_window, true);
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
  /*
  //with this I now have the active time at all times. I can check the time_format variable against the global variable that I intend on saving
  if(!clock_is_24h_style()){
    strftime(time_format, sizeof(time_format), "%I:%M", t);
  }
  else{
    strftime(time_format, sizeof(time_format), "%H:%M", t);
  }
  text_layer_set_text(top_text_layer, time_format);
  */
  // Get an activities mask
  HealthActivityMask activities = health_service_peek_current_activities();
  int hours = t->tm_hour;
  int minutes = t->tm_min;
  // Determine which bits are set, and hence which activity is active
  if(activities & HealthActivitySleep) {
      //This is the code to make the alarm go off. It has to fall into the following use circumstance
      if(glb_alarm == true){
          if(hours >= glb_hours_1 && hours <= glb_hours_2){
              if(minutes >= glb_minutes_1 || minutes >=glb_minutes_2){
                if(glb_alert_style == 0){
                  vibes_enqueue_custom_pattern(pat);
                }
                if(glb_alert_style == 1){
                  //TODO, FIGURE OUT HOW TO DO PHONE ALERT
                }
                if(glb_alert_style == 2){
                  //TODO, FIGURE OUT HOW TO DO PHONE ALERT
                  vibes_enqueue_custom_pattern(pat);
                }
              }
          }
      }
      APP_LOG(APP_LOG_LEVEL_INFO, "The user is sleeping.");
  } 
  else if(activities & HealthActivityRestfulSleep) {
      //This is the code to make the alarm go off. It has to fall into the following use circumstance
      if(glb_alarm == true){
          if(hours >= glb_hours_1 && hours <= glb_hours_2){
              if(minutes >= glb_minutes_1 || minutes >=glb_minutes_2){
                if(glb_alert_style == 0){
                  vibes_enqueue_custom_pattern(pat);
                }
                if(glb_alert_style == 1){
                  //TODO, FIGURE OUT HOW TO DO PHONE ALERT
                }
                if(glb_alert_style == 2){
                  //TODO, FIGURE OUT HOW TO DO PHONE ALERT
                  vibes_enqueue_custom_pattern(pat);
                }
              }
          }
      }
      APP_LOG(APP_LOG_LEVEL_INFO, "The user is sleeping peacefully.");
  } 
  else {
    APP_LOG(APP_LOG_LEVEL_INFO, "The user is not currently sleeping.");
  }
}
static void init(void) {
	// Create a window and get information about the window
  home_window = window_create();
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  //layers creation
  home_background = layer_create(GRect(0,0,144,168));
  selection_layer = layer_create(GRect(0,0,144,168));
  selection_on_off_layer = layer_create(GRect(0,0,144,168));
  selection_time_layer = layer_create(GRect(0,0,144,168));
  selection_alert_style_layer = layer_create(GRect(0,0,144,168));

  on_off_background = layer_create(GRect(0,0,144,168));
  alert_style_background = layer_create(GRect(0,0,144,168));
  window_set_click_config_provider(home_window, click_config_provider);
  // Set Window Handlers
  window_set_window_handlers(home_window, (WindowHandlers){
    .load = home_window_load,
    .unload = home_window_unload,
  });
  window_stack_push(home_window, true);  
  //initialize on_off window
  on_off_window = window_create();
  window_set_window_handlers(on_off_window, (WindowHandlers){
    .load = on_off_window_load,
    .unload = on_off_window_unload,
  });
  //initialize time window
  time_window = window_create();
  window_set_window_handlers(time_window, (WindowHandlers){
    .load = time_window_load,
    .unload = time_window_unload,
  });
  //initialize alert style window
  alert_style_window = window_create();
  window_set_window_handlers(alert_style_window, (WindowHandlers){
    .load = alert_style_window_load,
    .unload = alert_style_window_unload,
  });
}
static void deinit(void) {
	// Destroy the window
	window_destroy(home_window);
  //destroy tick handler
  tick_timer_service_unsubscribe();
}


int main(void) {
	init();
  //above event loop is create
  app_event_loop();//app event loop is what happens in the middle
  //deinit is what happens when the user closes the app
	deinit();
}
