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
#define CUSTOM_SAFE_RANGE SAFE_RANGE

#include "lang_shift/include.h"
#include "print.h"

enum layers {
   _QWERTY,
   _NUMBER,
   _SYMB
};

enum custom_keycodes {
  KC_AHASH = CUSTOM_SAFE_RANGE,
  KC_KEY2
  // ...
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT(
     KC_NO ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,                                            KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO ,
     KC_NO  ,KC_Q    ,KC_W    ,KC_E    ,KC_R    ,KC_T    ,KC_NO   ,                          KC_NO   ,KC_Y    ,KC_U    ,KC_I    ,KC_O    ,KC_P    ,	KC_LBRC  ,
     KC_NO ,LGUI_T(KC_A),LALT_T(KC_S),LSFT_T(KC_D),LCTL_T(KC_F),KC_G,KC_LBRC,                KC_RBRC,KC_H,RCTL_T(KC_J), RSFT_T(KC_K),LALT_T(KC_L),RGUI_T(KC_SCLN),KC_QUOT,
     KC_NO ,KC_Z    ,KC_X    ,KC_C    ,KC_V    ,KC_B    ,KC_NO ,LA_SYNC ,        KC_NO ,KC_NO ,KC_N    ,KC_M    ,KC_COMM ,KC_DOT  ,KC_SLSH ,KC_NO ,
     KC_NO ,KC_NO ,KC_NO ,KC_NO ,     	KC_ESC ,    	LT(_NUMBER, KC_SPC) ,KC_TAB  ,        LT(_SYMB, KC_ENT)  ,KC_BSPC,    	KC_DEL ,     KC_LEFT ,KC_DOWN ,KC_UP   ,KC_RGHT 
  ),

  [_NUMBER] = LAYOUT(
     KC_NO ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,                                            KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO ,
     KC_NO  ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO   ,                          KC_NO   ,KC_NO    ,KC_7    ,KC_8    ,KC_9    ,KC_NO    ,	KC_NO  ,
     KC_NO ,KC_LGUI,KC_LALT,	KC_LSFT,KC_LCTL,KC_NO,KC_NO,                                             KC_NO,KC_NO,KC_4,     KC_5,    KC_6,     KC_NO,      KC_NO,
     KC_NO ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO ,LA_SYNC ,        KC_NO ,KC_NO ,KC_NO    ,KC_1    ,KC_2 ,KC_3  ,KC_NO ,KC_NO ,
     KC_NO ,KC_NO ,KC_NO ,KC_NO ,     	KC_ESC ,    	KC_SPC ,KC_TAB  ,        KC_ENT  ,KC_BSPC,    	KC_DEL ,     KC_0 ,KC_NO ,KC_NO   ,KC_NO 
  ),

  [_SYMB] = LAYOUT(
     KC_NO ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,                                            KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO ,
     KC_NO  ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO   ,                          KC_NO   ,KC_Y    ,KC_1    ,KC_2    ,KC_3    ,KC_NO    ,	KC_NO  ,
     KC_NO ,KC_LGUI,KC_LALT,	KC_LSFT,KC_LCTL,KC_NO,KC_NO,                KC_RBRC,KC_AHASH,AG_LPRN, AG_RPRN,KC_6,KC_NO,KC_NO,
     KC_NO ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO    ,KC_NO ,LA_SYNC ,        KC_NO ,KC_NO ,KC_NO    ,KC_7    ,KC_8 ,KC_9  ,KC_NO ,KC_NO ,
     KC_NO ,KC_NO ,KC_NO ,KC_NO ,     	KC_ESC ,    	KC_SPC ,KC_TAB  ,        KC_ENT  ,KC_BSPC,    	KC_DEL ,     KC_0 ,KC_NO ,KC_NO   ,KC_NO 
  )
};


bool process_record_user(uint16_t key, keyrecord_t *record) {
  if (!lang_shift_process_record(key, record)){
      return false;
   }

   switch (key) {
      case KC_AHASH:
         if (record->event.pressed) {
            print("AHASH pressed");
            lang_shift_tap_key(EN_HASH);
         }
         return false;
         break;
      }

   return true;
}

void user_timer(void) {
  lang_shift_user_timer();
}

void matrix_scan_user(void) {
  user_timer();
}