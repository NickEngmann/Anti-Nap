#include <pebble_worker.h>

//bool tryme;
int alarm, start_hour, end_hour, start_minute, end_minute; 
void back_worker_message_handler(uint16_t key, AppWorkerMessage *data){
  alarm = data->data0;
  start_hour = (data->data1)/1000;
  end_hour = (data->data2)/1000;
  start_minute = (data->data1)%1000;
  end_minute = (data->data2)%1000;
  }
void tick_handler_worker(struct tm *t, TimeUnits units){ 
  // Get an activities mask
  HealthActivityMask activities = health_service_peek_current_activities();
  AppWorkerMessage message_back = {
    .data0 = 0,
    .data1 = 0,
    .data2 = 0,
  };
  int hours = t->tm_hour;
  int minutes = t->tm_min;
  if(alarm == 1){
  // Determine which bits are set, and hence which activity is active
    if(activities & HealthActivitySleep) {
          //This is the code to make the alarm go off. It has to fall into the following use circumstance
              if(hours >= start_hour && hours <= end_hour){
                  if(minutes >= start_minute || minutes >=end_minute){
                  //message_back.data0 = 1;
                  //message_back.data1 = (start_hour*1000)+start_minute;
                  //message_back.data2 = (end_hour*1000)+end_minute;
                  //message_back.data2 = 1;
                  app_worker_send_message(1, &message_back);
                  worker_launch_app();
                  }
              }/*
              if(hours >= start_hour){
                  if(minutes >= start_minute){
                  //message_back.data0 = 1;
                  //message_back.data1 = (start_hour*1000)+start_minute;
                  //message_back.data2 = (end_hour*1000)+end_minute;
                  message_back.data2 = 1;
                  app_worker_send_message(1, &message_back);
                  worker_launch_app();
                  }
              }*/
            //APP_LOG(APP_LOG_LEVEL_INFO, "The user is sleeping.");
      } 
     if(activities & HealthActivityRestfulSleep) {
          //This is the code to make the alarm go off. It has to fall into the following use circumstance
              if(hours >= start_hour && hours <= end_hour){
                  if(minutes >= start_minute || minutes >=end_minute){
                  //message_back.data0 = 1;
                  //message_back.data1 = (start_hour*1000)+start_minute;
                  //message_back.data2 = (end_hour*1000)+end_minute;
                  //message_back.data2 = 1;
                  app_worker_send_message(1, &message_back);
                  worker_launch_app();
                  }
              }
              /*if(hours >= start_hour){
                  if(minutes >= start_minute){
                  //message_back.data0 = 1;
                  //message_back.data1 = (start_hour*1000)+start_minute;
                  //message_back.data2 = (end_hour*1000)+end_minute;
                  message_back.data2 = 0;
                  app_worker_send_message(1, &message_back);
                  worker_launch_app();
                  }
              }*/
                //APP_LOG(APP_LOG_LEVEL_INFO, "The user is sleeping peacefully.");
      } 
      else {
        //This is the code to make the alarm go off. It has to fall into the following use circumstance
              /*if(hours >= start_hour && hours <= end_hour){
                  if(minutes >= start_minute || minutes >=end_minute){
                  message_back.data0 = 1;
                  app_worker_send_message(1, &message_back);
                  worker_launch_app();
                  }
              }
              if(hours >= start_hour){
                  if(minutes >= start_minute){
                  //message_back.data0 = 1;
                  //message_back.data1 = (start_hour*1000)+start_minute;
                  message_back.data2 = 1;
                  app_worker_send_message(1, &message_back);
                  worker_launch_app();
                  }
              }*/
          }
        //APP_LOG(APP_LOG_LEVEL_INFO, "The user is not currently sleeping.");
  }
}
void worker_init(){
    AppWorkerMessage message_back = {
    .data0 = 0,
    .data1 = 0,
    .data2 = 0,
  };
  app_worker_message_subscribe(back_worker_message_handler);
  app_worker_send_message(1, &message_back);
  //tick_timer_service_subscribe(MINUTE_UNIT, tick_handler_worker);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler_worker);
  
}

void worker_deinit(){
    AppWorkerMessage message_back = {
    .data0 = 0,
    .data1 = 0,
    .data2 = 0,
  };
}

int main(void){
  worker_init();
  worker_event_loop();
  worker_deinit();
}