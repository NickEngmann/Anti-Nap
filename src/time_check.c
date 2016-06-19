#include <pebble.h>
#include "time_check.h"

Window *time_window;
TextLayer *time_layer, *time_am_hour_input,*time_am_minute_input, *time_am_layer, *time_pm_hour_input, *time_pm_minute_input, *time_pm_layer;
Layer *selection_time_layer;

char am_hour_buf[] = "00";
char am_minute_buf[] = "000";
char pm_hour_buf[] = "00";
char pm_minute_buf[] = "000";
int select_time;

  //Time Input Functions
  void window_time_layer_create(){
      selection_time_layer = layer_create(GRect(0,0,144,168));
  }
void window_push_time(){
      window_stack_push(time_window, true);
  }
  void window_time_create(){
      time_window = window_create();
      window_set_window_handlers(time_window, (WindowHandlers){
    .load = time_window_load,
    .unload = time_window_unload,
    });
  }

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

  void time_input_select_click_handler( ClickRecognizerRef recognizer, void *context){
    select_time++;
      if(select_time == 4){
        select_time = 0;
      }
    refresh_time_window();
    }

  void time_input_up_click_handler( ClickRecognizerRef recognizer, void *context){ 
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

  void time_input_down_click_handler( ClickRecognizerRef recognizer, void *context){
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

  void time_input_click_config_provider(void *context){
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
    //save hours and minutes for later
    persist_write_int(1, glb_hours_1);
    persist_write_int(2, glb_hours_2);
    persist_write_int(3, glb_minutes_1);
    persist_write_int(4, glb_minutes_2);
    //destroy text layers
    text_layer_destroy(time_layer);
    text_layer_destroy(time_pm_layer);
    text_layer_destroy(time_am_layer);
  }


