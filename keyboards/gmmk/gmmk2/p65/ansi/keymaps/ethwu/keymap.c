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

void matrix_init_user(void) {
    rgb_matrix_disable();
}

LEADER_EXTERNS();

/* void matrix_scan_user(void) { */
/*     LEADER_DICTIONARY() { */
/*         leading = false; */
/*         leader_end(); */

/*         SEQ_ONE_KEY(KC_F1) { SEND_STRING(SS_TAP(X_F1)); } */
/*         SEQ_ONE_KEY(KC_F2) { SEND_STRING(SS_TAP(X_F2)); } */
/*         SEQ_ONE_KEY(KC_F3) { SEND_STRING(SS_TAP(X_F3)); } */
/*         SEQ_ONE_KEY(KC_F4) { SEND_STRING(SS_TAP(X_F4)); } */
/*         SEQ_ONE_KEY(KC_F5) { SEND_STRING(SS_TAP(X_F5)); } */
/*         SEQ_ONE_KEY(KC_F6) { SEND_STRING(SS_TAP(X_F6)); } */
/*         SEQ_ONE_KEY(KC_F7) { SEND_STRING(SS_TAP(X_F7)); } */
/*         SEQ_ONE_KEY(KC_F8) { SEND_STRING(SS_TAP(X_F8)); } */
/*         SEQ_ONE_KEY(KC_F9) { SEND_STRING(SS_TAP(X_F9)); } */
/*         SEQ_ONE_KEY(KC_F10) { SEND_STRING(SS_TAP(X_F10)); } */
/*         SEQ_ONE_KEY(KC_F11) { SEND_STRING(SS_TAP(X_F11)); } */
/*         SEQ_ONE_KEY(KC_F12) { SEND_STRING(SS_TAP(X_F12)); } */

/*         SEQ_ONE_KEY(KC_F1) { SEND_STRING(SS_TAP(X_F13)); } */
/*         SEQ_ONE_KEY(KC_F2) { SEND_STRING(SS_TAP(X_F14)); } */
/*         SEQ_ONE_KEY(KC_F3) { SEND_STRING(SS_TAP(X_F15)); } */
/*         SEQ_ONE_KEY(KC_F4) { SEND_STRING(SS_TAP(X_F16)); } */
/*         SEQ_ONE_KEY(KC_F5) { SEND_STRING(SS_TAP(X_F17)); } */
/*         SEQ_ONE_KEY(KC_F6) { SEND_STRING(SS_TAP(X_F18)); } */
/*         SEQ_ONE_KEY(KC_F7) { SEND_STRING(SS_TAP(X_F19)); } */
/*         SEQ_ONE_KEY(KC_F8) { SEND_STRING(SS_TAP(X_F20)); } */
/*         SEQ_ONE_KEY(KC_F9) { SEND_STRING(SS_TAP(X_F21)); } */
/*         SEQ_ONE_KEY(KC_F10) { SEND_STRING(SS_TAP(X_F22)); } */
/*         SEQ_ONE_KEY(KC_F11) { SEND_STRING(SS_TAP(X_F23)); } */
/*         SEQ_ONE_KEY(KC_F12) { SEND_STRING(SS_TAP(X_F24)); } */

/*         SEQ_TWO_KEYS(KC_RALT, KC_RGHT) { SEND_STRING(SS_TAP(X_MEDIA_NEXT_TRACK)); } */
/*         SEQ_TWO_KEYS(KC_RALT, KC_LEFT) { SEND_STRING(SS_TAP(X_MEDIA_PREV_TRACK)); } */
/*         SEQ_TWO_KEYS(KC_RALT, KC_RALT) { SEND_STRING(SS_TAP(X_MEDIA_PLAY_PAUSE)); } */
/*         SEQ_TWO_KEYS(KC_RALT, KC_UP) { SEND_STRING(SS_TAP(X_BRIGHTNESS_UP)); } */
/*         SEQ_TWO_KEYS(KC_RALT, KC_DOWN) { SEND_STRING(SS_TAP(X_BRIGHTNESS_DOWN)); } */
/*     } */
/* } */

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.

enum LayerNames {
    MACOS,
    WIN,
    DSC,
    FN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[MACOS] = LAYOUT(
  KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_ESC,
  KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,
  KC_LCTL, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_ENT,             KC_PGUP,
  KC_LSFT, KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,            KC_UP,    KC_PGDN,
  KC_LALT, OSL(FN),  KC_LGUI,                                KC_SPC,                                 KC_RGUI,  KC_RALT,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[WIN] = LAYOUT(
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
  KC_LGUI,  _______,  KC_LALT,                                _______,                                KC_RALT,  KC_RCTL,  _______,  _______,  _______),

[DSC] = LAYOUT(
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  PB_1,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
  _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______),

[FN] = LAYOUT(
  QK_BOOT,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,  _______,
  C(KC_TAB),RGB_TOG,  RGB_HUI,  _______,  KC_INS,   KC_PSCR,  _______,  _______,  _______,  KC_PGUP,  _______,  KC_VOLD,  KC_VOLU,  KC_MUTE,  _______,
  _______,  RGB_MOD,  RGB_SAI,  _______,  _______,  KC_SCRL,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  _______,  _______,  _______,            KC_HOME,
 C(KC_LSFT),TG(WIN),  TG(DSC),  _______,  _______,  KC_PAUS,  KC_PGDN,  _______,  KC_MPRV,  KC_MNXT,  KC_MPLY,  C(KC_RSFT),         _______,  KC_END,
CAPS_WORD,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______),

[31] = LAYOUT(
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,             _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,
  _______,  _______,  _______,                                _______,                                _______,  _______,  _______,   _______,  _______)
};
