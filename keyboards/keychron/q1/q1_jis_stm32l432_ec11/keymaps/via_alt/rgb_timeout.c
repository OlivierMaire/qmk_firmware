/* RGB Timeout behaves similar to OLED Timeout where it turns off underglow 
lighting on your QMK board after X seconds of inactivity. The logic is 
best described by this reddit comment - https://bit.ly/3zCYIRl  */

#include "test.h"


static uint16_t key_timer; // timer to track the last keyboard activity
static void refresh_rgb(void); // refreshes the activity timer and RGB, invoke whenever activity happens
static void check_rgb_timeout(void); // checks if enough time has passed for RGB to timeout
bool is_rgb_timeout = false; // store if RGB has timed out or not in a boolean


void refresh_rgb() {
  key_timer = timer_read(); // store time of last refresh
  if (is_rgb_timeout) { // only do something if rgb has timed out
    print("Activity detected, removing timeout\n");
    is_rgb_timeout = false;
    // rgblight_wakeup();
    rgb_matrix_set_suspend_state(false); 

  }
}

void check_rgb_timeout() {
  if (!is_rgb_timeout && timer_elapsed(key_timer) > RGBLIGHT_TIMEOUT) {
    // rgblight_suspend();
    rgb_matrix_set_suspend_state(true); 
    is_rgb_timeout = true;
  }
}


/* Runs at the end of each scan loop, check if RGB timeout has occured */
void housekeeping_task_user(void) {
  #ifdef RGBLIGHT_TIMEOUT
  check_rgb_timeout();
  #endif
  
  /* rest of the function code here */
}

/* Runs after each key press, check if activity occurred */
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
  #ifdef RGBLIGHT_TIMEOUT
  if (record->event.pressed) refresh_rgb();
  #endif

  /* rest of the function code here */
}


/* Runs after each encoder tick, check if activity occurred */
void post_encoder_update_user(uint8_t index, bool clockwise) {
  #ifdef RGBLIGHT_TIMEOUT
  refresh_rgb();
  #endif
  
  /* rest of the function code here */
}