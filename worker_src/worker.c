#include <pebble_worker.h>


void worker_message_handler(uint16_t key, AppWorkerMessage *message){

}

void tick_handler_worker(struct tm *t, TimeUnits units){
  //load in old variables
  int alarm, start_hour, end_hour, start_minute, end_minute; 
  /*Add Alarm Style When Feature Becomes Necessary*/
  //int alarm_stlye
  alarm = persist_read_int(0);
  start_hour = persist_read_int(1);
  end_hour = persist_read_int(2);
  start_minute = persist_read_int(3);
  end_minute = persist_read_int(4);
  //alarm_style = persist_read_int(5);
  // Get an activities mask
  HealthActivityMask activities = health_service_peek_current_activities();
  int hours = t->tm_hour;
  int minutes = t->tm_min;    
  if(alarm == 1){
  // Determine which bits are set, and hence which activity is active
    if(activities & HealthActivitySleep) {
          //This is the code to make the alarm go off. It has to fall into the following use circumstance
              if((hours >= start_hour) && (hours <= end_hour)){
                  if((minutes >= start_minute) || (minutes >=end_minute)){
                  APP_LOG(APP_LOG_LEVEL_INFO, "The user is sleeping.");
                  persist_write_int(6, 321);  
                  worker_launch_app();
                  }
              }
      } 
     else if(activities & HealthActivityRestfulSleep){
          //This is the code to make the alarm go off. It has to fall into the following use circumstance
              if(hours >= start_hour){
                if(hours <= end_hour){
                  if((minutes >= start_minute) || (minutes >=end_minute)){
                    APP_LOG(APP_LOG_LEVEL_INFO, "The user is sleeping peacefully.");
                    persist_write_int(6, 321);
                    worker_launch_app();
                  }
                }
              }       
      } 
      else {
        APP_LOG(APP_LOG_LEVEL_INFO, "The user is not currently sleeping.");    
      }       
  }
}

void worker_init(){
  app_worker_message_subscribe(worker_message_handler);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler_worker);
}

void worker_deinit(){

}

int main(void){
  worker_init();
  worker_event_loop();
  worker_deinit();
}