#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static GFont s_time_font;
static GFont s_small_font;
static GFont s_large_font;
static TextLayer *s_aka_layer;
static TextLayer *s_slogan_layer;

static void update_time() {

  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  static char buffer[] = "00:00";

  if(clock_is_24h_style() == true) {
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  
  text_layer_set_text(s_time_layer, buffer);
}
  
static void main_window_load(Window *window) {
  
  window_set_background_color(window, GColorBlack);
  
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 140, 144, 20));
  text_layer_set_background_color(s_time_layer, GColorBlack);
  text_layer_set_text_color(s_time_layer, GColorClear);
  
  s_aka_layer = text_layer_create(GRect(0, 130, 144, 10));
  text_layer_set_background_color(s_aka_layer, GColorBlack);
  text_layer_set_text_color(s_aka_layer, GColorClear);
  
  s_slogan_layer = text_layer_create(GRect(0, 10, 144, 100));
  text_layer_set_background_color(s_slogan_layer, GColorBlack);
  text_layer_set_text_color(s_slogan_layer, GColorClear);

  // Improve the layout to be more like a watchface
  // Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SQUARE_20));
  s_small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SQUARE_10));
  s_large_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_SQUARE_30));

  // Apply to TextLayer
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  text_layer_set_font(s_aka_layer, s_small_font);
  text_layer_set_text_alignment(s_aka_layer, GTextAlignmentCenter);
  
  text_layer_set_font(s_slogan_layer, s_large_font);
  text_layer_set_text_alignment(s_slogan_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(s_slogan_layer, GTextOverflowModeWordWrap);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_aka_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_slogan_layer));
  
  text_layer_set_text(s_aka_layer, "aka");
  text_layer_set_text(s_slogan_layer, "\nAmateur Hour");
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  // Unload GFont
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_small_font);
  fonts_unload_custom_font(s_large_font);
  
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_aka_layer);
  text_layer_destroy(s_slogan_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}

static void deinit() {
  animation_unschedule_all();
  
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}