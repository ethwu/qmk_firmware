/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>

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

enum LayerNames {
    MACOS,
    WIN,
    DSC,
    FN
};

void matrix_init_user(void) {
    rgb_matrix_disable();
}

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (host_keyboard_led_state().caps_lock) {
        rgb_matrix_set_color(30, RGB_WHITE);
        /* for (uint8_t i = led_min; i <= led_max; i++) { */
        /*     if (g_led_config.flags[i] & LED_FLAG_KEYLIGHT) { */
        /*         rgb_matrix_set_color(i, RGB_RED); */
        /*     } */
        /* } */
    }

    if (host_keyboard_led_state().caps_lock) rgb_matrix_set_color(30, RGB_WHITE);
    if (IS_LAYER_ON(FN)) rgb_matrix_set_color(58, RGB_WHITE);
    if (IS_LAYER_ON(WIN)) {
        /* Show Windows logo in the bottom right. */

        /* Red in the top left. */
        rgb_matrix_set_color(56, 0xf2, 0x50, 0x22);
        /* Green in the top right. */
        rgb_matrix_set_color(57, 0x7f, 0xba, 0x00);
        /* Blue in the bottom left. */
        rgb_matrix_set_color(65, 0x00, 0xa4, 0xef);
        /* Yellow in the bottom right. */
        rgb_matrix_set_color(66, 0xff, 0xb9, 0x00);
    }
}


bool led_update_user(led_t led_state) {
    if (led_state.caps_lock) {
        rgb_matrix_set_color(4, 0xff, 0xff, 0x00);
    }
    return true;
}

LEADER_EXTERNS();

/* void matrix_scan_user(void) { */
/*     LEADER_DICTIONARY() { */
/*         leading = false; */
/*         leader_end(); */

/*         SEQ_ONE_KEY(KC_1) { SEND_STRING(SS_TAP(X_F13)); } */
/*         SEQ_ONE_KEY(KC_2) { SEND_STRING(SS_TAP(X_F14)); } */
/*         SEQ_ONE_KEY(KC_3) { SEND_STRING(SS_TAP(X_F15)); } */
/*         SEQ_ONE_KEY(KC_4) { SEND_STRING(SS_TAP(X_F16)); } */
/*         SEQ_ONE_KEY(KC_5) { SEND_STRING(SS_TAP(X_F17)); } */
/*         SEQ_ONE_KEY(KC_6) { SEND_STRING(SS_TAP(X_F18)); } */
/*         SEQ_ONE_KEY(KC_7) { SEND_STRING(SS_TAP(X_F19)); } */
/*         SEQ_ONE_KEY(KC_8) { SEND_STRING(SS_TAP(X_F20)); } */
/*         SEQ_ONE_KEY(KC_9) { SEND_STRING(SS_TAP(X_F21)); } */
/*         SEQ_ONE_KEY(KC_0) { SEND_STRING(SS_TAP(X_F22)); } */
/*         SEQ_ONE_KEY(KC_MINUS) { SEND_STRING(SS_TAP(X_F23)); } */
/*         SEQ_ONE_KEY(KC_EQUAL) { SEND_STRING(SS_TAP(X_F24)); } */

/*         /1* SEQ_TWO_KEYS(KC_RALT, KC_RGHT) { SEND_STRING(SS_TAP(X_MEDIA_NEXT_TRACK)); } *1/ */
/*         /1* SEQ_TWO_KEYS(KC_RALT, KC_LEFT) { SEND_STRING(SS_TAP(X_MEDIA_PREV_TRACK)); } *1/ */
/*         /1* SEQ_TWO_KEYS(KC_RALT, KC_RALT) { SEND_STRING(SS_TAP(X_MEDIA_PLAY_PAUSE)); } *1/ */
/*         /1* SEQ_TWO_KEYS(KC_RALT, KC_UP) { SEND_STRING(SS_TAP(X_BRIGHTNESS_UP)); } *1/ */
/*         /1* SEQ_TWO_KEYS(KC_RALT, KC_DOWN) { SEND_STRING(SS_TAP(X_BRIGHTNESS_DOWN)); } *1/ */
/*     } */
/* } */

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[MACOS] = LAYOUT(
  KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_ESC,
  KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,
  KC_LCTL, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_ENT,             KC_PGUP,
  KC_LSFT, KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,            KC_UP,    KC_PGDN,
  TT(FN),  KC_LALT,  KC_LGUI,                                KC_SPC,                                 KC_RGUI,  KC_RALT,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[WIN] = LAYOUT(
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
  _______,  KC_LGUI,  KC_LALT,                                _______,                                KC_RALT,  KC_RCTL,  _______,  _______,  _______),

[DSC] = LAYOUT(
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  PB_1,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
  _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______),

[FN] = LAYOUT(
  QK_BOOT,    KC_F1,      KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,     KC_F11,     KC_F12,     _______,    _______,
  C(KC_TAB ), RGB_TOG,    RGB_HUI,  _______,  KC_INS,   KC_PSCR,  _______,  _______,  _______,  KC_PGUP,  _______,    KC_VOLD,    KC_VOLU,    KC_MUTE,    _______,
  KC_CAPS,    RGB_MOD,    RGB_SAI,  _______,  _______,  KC_SCRL,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_BRID,    KC_BRIU,    C(KC_ENT ),             KC_HOME,
  C(KC_LSFT), C(KC_Z),    C(KC_X),  C(KC_C),  C(KC_V),  KC_PAUS,  KC_PGDN,  _______,  KC_MPRV,  KC_MNXT,  KC_MPLY,    C(KC_RSFT),             C(KC_UP  ), KC_END,
  TG(FN),     C(KC_LALT), C(KC_LGUI),                             TG(WIN),                                C(KC_RGUI), C(KC_RALT), C(KC_LEFT), C(KC_DOWN), C(KC_RGHT)),

[31] = LAYOUT(
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,             _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,
  _______,  _______,  _______,                                _______,                                _______,  _______,  _______,   _______,  _______)
};
