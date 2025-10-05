/*
Copyright 2018 Mattia Dal Ben <matthewdibi@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "print.h"

enum layers {
   _QWERTY,
   _NUMBER,
   _SYMB
};

enum custom_keycodes {
   LANG_ENG = SAFE_RANGE, // Ensure these don't conflict with existing keycodes
   LANG_RUS,
   LT_SYM_ENT,
   LT_NUM_SPC,
  // ...
};

// Define a structure to hold layer-tap information
typedef struct {
   bool     is_active;
   bool     command_sent;
   uint16_t timer;
} layer_tap_t;

// Initialize structure instance for holding state
layer_tap_t my_lt = {false, false, 0};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT(
     KC_NO ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,                                            KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO ,
     KC_NO  ,KC_Q    ,KC_W    ,KC_E    ,KC_R    ,KC_T    ,KC_NO   ,                          KC_NO   ,KC_Y    ,KC_U    ,KC_I    ,KC_O    ,KC_P    ,	KC_LBRC  ,
     KC_NO ,LGUI_T(KC_A),LALT_T(KC_S),LSFT_T(KC_D),LCTL_T(KC_F),KC_G,KC_LBRC,                KC_RBRC,KC_H,RCTL_T(KC_J), RSFT_T(KC_K),LALT_T(KC_L),RGUI_T(KC_SCLN),KC_QUOT,
     KC_NO ,KC_Z    ,KC_X    ,KC_C    ,KC_V    ,KC_B    ,KC_NO ,KC_NO ,        KC_NO ,KC_NO ,KC_N    ,KC_M    ,KC_COMM ,KC_DOT  ,KC_SLSH ,KC_NO ,
     KC_NO ,KC_NO ,KC_NO ,KC_NO ,     	KC_ESC ,    	LT_NUM_SPC ,KC_TAB  ,        LT_SYM_ENT  ,KC_BSPC,    	KC_DEL ,     KC_LEFT ,KC_DOWN ,KC_UP   ,KC_RGHT 
  ),

  [_NUMBER] = LAYOUT(
     KC_NO ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,                                            KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO ,
     KC_NO  ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO   ,                          KC_NO   ,KC_NO    ,KC_7    ,KC_8    ,KC_9    ,KC_NO    ,	KC_NO  ,
     KC_NO ,KC_LGUI,KC_LALT,	KC_LSFT,KC_LCTL,KC_NO,KC_NO,                                             KC_NO,KC_NO,KC_4,     KC_5,    KC_6,     KC_NO,      KC_NO,
     KC_NO ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO ,KC_NO ,        KC_NO ,KC_NO ,KC_NO    ,KC_1    ,KC_2 ,KC_3  ,KC_NO ,KC_NO ,
     KC_NO ,KC_NO ,KC_NO ,KC_NO ,     	KC_ESC ,    	KC_SPC ,KC_TAB  ,        KC_ENT  ,KC_BSPC,    	KC_DEL ,     KC_0 ,KC_NO ,KC_NO   ,KC_NO 
  ),

  [_SYMB] = LAYOUT(
     KC_NO ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,                                            KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO ,
     KC_NO  ,KC_EXLM    ,KC_LT    ,KC_GT    ,KC_NO    ,KC_NO    ,KC_NO   ,                          KC_NO   ,KC_CIRC    ,KC_LCBR    ,KC_RCBR    ,KC_DLR    ,KC_QUES    ,	KC_NO  ,
     KC_NO ,KC_LGUI,KC_LALT,	KC_LSFT,KC_LCTL,KC_NO,KC_NO,                KC_RBRC,KC_HASH,KC_LPRN, KC_RPRN,	KC_SCLN,KC_DQT,KC_NO,
     KC_NO ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO ,KC_NO ,        KC_NO ,KC_NO ,KC_NO    ,KC_AT    ,KC_COLON ,	KC_COMM  ,KC_DOT ,KC_QUOT ,
     KC_NO ,KC_NO ,KC_NO ,KC_NO ,     	KC_ESC ,    	KC_SPC ,KC_TAB  ,        KC_ENT  ,KC_BSPC,    	KC_DEL ,     KC_0 ,KC_NO ,KC_NO   ,KC_NO 
  )
};

// Function to handle layer-tap logic
void process_layer_tap(uint16_t keycode, uint16_t layer, keyrecord_t *record) {
   if (record->event.pressed) {
       // Key was pressed - start timer.
       my_lt.timer = timer_read();
       my_lt.is_active = true;
       layer_on(layer);
       dprintf("lt: pressed!\n");
   } else { // Key was released
       // Was it a tap?
       if (my_lt.is_active && timer_elapsed(my_lt.timer) < TAPPING_TERM) {
           tap_code(keycode);
           dprintf("lt: tapped!\n");
       }
       layer_off(layer);
       // Only retore win kbd layout if it was switched.
       if (my_lt.command_sent) {
         // Tell Windows we're still in Cyrillic
         SEND_STRING(SS_LSFT(SS_LALT("1")));
         // Clear flag.
         my_lt.command_sent = false;
         dprintf("lt: sent SA(1) to windows!\n");
       }
       my_lt.is_active = false;
       dprintf("lt: released!\n");
   }
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
   // check for specific keycodes
   switch (keycode) {
    //
     case LT_SYM_ENT:
       process_layer_tap(KC_ENT, _SYMB, record);
       return false; // Skip all further processing of this key
       //
     case LT_NUM_SPC:
       process_layer_tap(KC_SPC, _NUMBER, record);
       return false; // Skip all further processing of this key
       //
     default:
       if (my_lt.is_active && record->event.pressed) {
         // Only switch win kbd layout if it has not been switched.
         if (!my_lt.command_sent) {
           // Custom function called when any key is pressed while MY_LT is active
           my_lt.command_sent = true;
           SEND_STRING(SS_LSFT(SS_LALT("0")));
           dprintf("lt: sent SA(0) to windows!\n");
         }
         return true;  // Allow the key press to be handled normally by QMK
       }
       break;
   } // done with specific keys
   //
   return true;
 }