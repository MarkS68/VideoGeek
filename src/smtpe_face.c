#include "smtpe_face.h"
#include "ctype.h"
#include <pebble.h>
#define KEY_USESECONDS 0
#define KEY_DATEFORMAT 1
  
// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GBitmap *s_res_image_smpte_face;
static GFont s_res_gothic_28_bold;
static GFont s_res_gothic_14;
static GBitmap *s_res_image_bluetooth_connected;
static BitmapLayer *s_bitmaplayer_background;
static TextLayer *s_textlayer_time;
static TextLayer *s_textlayer_UB;
static TextLayer *s_textlayer_1;
static BitmapLayer *s_bitmaplayer_bluetooth_connected;

static void initialise_ui(void) {
  s_window = window_create();
  
  s_res_image_smpte_face = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SMPTE_FACE);
  s_res_gothic_28_bold = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  s_res_gothic_14 = fonts_get_system_font(FONT_KEY_GOTHIC_14);
  s_res_image_bluetooth_connected = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTH_CONNECTED);
  // s_bitmaplayer_background
  s_bitmaplayer_background = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_bitmaplayer_background, s_res_image_smpte_face);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_background);
  
  // s_textlayer_time
  s_textlayer_time = text_layer_create(GRect(2, 49, 137, 36));
  text_layer_set_background_color(s_textlayer_time, GColorBlack);
  text_layer_set_text_color(s_textlayer_time, GColorWhite);
  text_layer_set_text(s_textlayer_time, "TCR: 00:00:00");
  text_layer_set_text_alignment(s_textlayer_time, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_time, s_res_gothic_28_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_time);
  
  // s_textlayer_UB
  s_textlayer_UB = text_layer_create(GRect(3, 146, 75, 18));
  text_layer_set_background_color(s_textlayer_UB, GColorBlack);
  text_layer_set_text_color(s_textlayer_UB, GColorWhite);
  text_layer_set_text(s_textlayer_UB, "UB:00-00-00");
  text_layer_set_text_alignment(s_textlayer_UB, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_UB);
  
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(2, 85, 39, 18));
  text_layer_set_background_color(s_textlayer_1, GColorBlack);
  text_layer_set_text_color(s_textlayer_1, GColorWhite);
  text_layer_set_text(s_textlayer_1, "LOCK");
  text_layer_set_font(s_textlayer_1, s_res_gothic_14);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_bitmaplayer_bluetooth_connected
  s_bitmaplayer_bluetooth_connected = bitmap_layer_create(GRect(29, 89, 8, 14));
  bitmap_layer_set_bitmap(s_bitmaplayer_bluetooth_connected, s_res_image_bluetooth_connected);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_bitmaplayer_bluetooth_connected);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  bitmap_layer_destroy(s_bitmaplayer_background);
  text_layer_destroy(s_textlayer_time);
  text_layer_destroy(s_textlayer_UB);
  text_layer_destroy(s_textlayer_1);
  bitmap_layer_destroy(s_bitmaplayer_bluetooth_connected);
  gbitmap_destroy(s_res_image_smpte_face);
  gbitmap_destroy(s_res_image_bluetooth_connected);
}
// END AUTO-GENERATED UI CODE

static int gUseSeconds = 1;
static int gDateFormat = 1;

//function to convert to upper case
char *upcase(char *str)
{
    char *s = str;

    while (*s)
    {
        *s++ = toupper((int)*s);
    }

    return str;
}

//bluetooth monitoring
void bt_handler(bool connected) {
  if (connected) {
    layer_set_hidden((Layer *)s_textlayer_1, false);
    layer_set_hidden((Layer *)s_bitmaplayer_bluetooth_connected, false);
  } else {
   layer_set_hidden((Layer *)s_textlayer_1, true);
    layer_set_hidden((Layer *)s_bitmaplayer_bluetooth_connected, true);
    vibes_long_pulse();
  }
}

static void update_time(){
  //GET A TM STRUCTURE
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  //CREATE LONG LIVED BUFFERS FOR DATE AND TIME
  static char buffer[] = "              ";
  static char datebuffer[] = "           ";
  
  //WRITE THE CURRENT HOURS, MINUTES AND SECONDS INTO THE TIME BUFFER
  if(clock_is_24h_style() == true) {
    //USE 24 HOUR FORMAT
    if (gUseSeconds == 1){
      strftime(buffer,sizeof("              "), "TCR: %H:%M:%S", tick_time);
  } else {
      strftime(buffer,sizeof("              "), "TCR: %H:%M:00", tick_time);
  }
  } else {
    //USE 12 HOUR FORMAT
    if (gUseSeconds == 1){
      strftime(buffer,sizeof("              "), "TCR: %l:%M:%S", tick_time);
    } else {
      strftime(buffer,sizeof("              "), "TCR: %l:%M:00", tick_time);
    }
  }
  
  // WRITE THE CURRENT DATE TO THE DATE BUFFER
  switch(gDateFormat){
    case 2:
      //DD-MM-YY
      strftime(datebuffer, sizeof("           "), "UB:%d-%m-%y", tick_time);
      break;
    case 3:
      //YY-MM-DD
      strftime(datebuffer, sizeof("           "), "UB:%y-%m-%d", tick_time);
      break;
    case 4:
      //short DAY-DD
      strftime(datebuffer, sizeof("         "), "UB:%a-%d", tick_time);
      break;
    case 5:
      //short MONTH-DD
      strftime(datebuffer, sizeof("         "), "UB:%b-%d", tick_time);
      break;
    default:
      //MM-DD-YY
      strftime(datebuffer, sizeof("           "), "UB:%m-%d-%y", tick_time);
  }
  
  //CONVERT DATE BUFFER TO UPPER CASE
  upcase(datebuffer);
  
  //DISPLAY THE TIME ON THE TIME TEXT LAYER (TCR:)
  text_layer_set_text(s_textlayer_time, buffer);
  
  //DISPLAY THE DATE ON THE DATE TEXT LAYER (UB:)
  text_layer_set_text(s_textlayer_UB,datebuffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  update_time();
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context){
  APP_LOG(APP_LOG_LEVEL_INFO, "Inbox received message.");
  Tuple *t = dict_read_first(iterator);
  while (t != NULL) {
    switch(t->key) {
      case KEY_USESECONDS:
        //set gUseSeconds and persistent storage
        gUseSeconds =  t->value->int8;
        persist_write_int(KEY_USESECONDS, gUseSeconds);
        break;
      case KEY_DATEFORMAT:
        //set gDateFormat and persistent storage
        gDateFormat = t->value->int8;
        persist_write_int(KEY_DATEFORMAT, gDateFormat);
        break;
      default:
        APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
        break;
    }
    t = dict_read_next(iterator);
  }
   tick_timer_service_unsubscribe();
   if (gUseSeconds == 1){
     tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
   } else {
     tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);  
   }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context){
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context){
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context){
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}


//static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
//  update_time();
//}
void init(void){
  if (persist_exists(KEY_USESECONDS)){
    APP_LOG(APP_LOG_LEVEL_INFO, "KEY_USESECONDS Exists!");
    APP_LOG(APP_LOG_LEVEL_INFO, "Value %d", (int) persist_read_int(KEY_USESECONDS));
  }
  if (persist_exists(KEY_DATEFORMAT)){
    APP_LOG(APP_LOG_LEVEL_INFO, "KEY_DATEFORMAT Exists!");
  }
  gUseSeconds = persist_exists(KEY_USESECONDS) ? persist_read_int(KEY_USESECONDS) : 1;
  gDateFormat = (int)persist_exists(KEY_DATEFORMAT) ? persist_read_int(KEY_DATEFORMAT) : 1;
  APP_LOG(APP_LOG_LEVEL_INFO, "---Value %d", (int)gDateFormat);
}
void show_smtpe_face(void) {
  initialise_ui();
  init();
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
  update_time();
  if (gUseSeconds == 1){
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  } else {
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  }
  bluetooth_connection_service_subscribe(bt_handler);
}

void hide_smtpe_face(void) {
  window_stack_remove(s_window, true);
}

 
int main(void){
  show_smtpe_face();
  app_event_loop();
  hide_smtpe_face();
}








