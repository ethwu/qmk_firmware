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
    HFN,
    TFN,
    SFN
};

/* #define WIN_RED RGB_RED */
/* #define WIN_GREEN RGB_GREEN */
/* #define WIN_BLUE RGB_BLUE */
/* #define WIN_YELLOW RGB_YELLOW */

#define WIN_RED 0xf2, 0x50, 0x22
#define WIN_GREEN 0x7f, 0xba, 0x00
#define WIN_BLUE 0x00, 0xa4, 0xef
#define WIN_YELLOW 0xff, 0xb9, 0x00

#define DSC_BLURPLE 0x58, 0x65, 0xf2

#define HFN_COLOR 0x30, 0x99, 0x75
#define TFN_COLOR 0x58, 0xb3, 0x68
#define SFN_COLOR 0xda, 0xd8, 0x73

enum custom_keycodes {
    LT_FN = SAFE_RANGE,
    MF_SFN,
    SW_WIN,
    SW_DSC
};

/* Timers for tap/hold fn layer changes */
static uint16_t fn_layer_tap_timer;

/* Disable lights after 10 minutes. */
#define RGB_TIMEOUT 10 * 10 * 1000
static uint16_t idle_timer = 0;
static bool rgb_matrix_enabled = true;

void matrix_scan_user(void) {
    if (idle_timer == 0) idle_timer = timer_read();
    if (timer_elapsed(idle_timer) > RGB_TIMEOUT && rgb_matrix_enabled) {
        rgb_matrix_enabled = false;
        rgb_matrix_disable_noeeprom();
    }
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        if (!rgb_matrix_enabled) rgb_matrix_enable_noeeprom();
        idle_timer = timer_read();
    } 
    switch (keycode) {
        case LT_FN:
            if (record->event.pressed) {
                fn_layer_tap_timer = timer_read();
                layer_on(HFN);
            } else {
                layer_off(HFN);
                if (timer_elapsed(fn_layer_tap_timer) < TAPPING_TERM) {
                    layer_invert(TFN);
                }
            }
            break;
        case MF_SFN:
            if (record->event.pressed) {
                layer_on(SFN);
            } else {
                layer_off(SFN);
                layer_off(TFN);
            }
            break;
        case SW_WIN:
            if (!record->event.pressed) {
                layer_invert(WIN);
                layer_off(SFN);
            }
            break;
        case SW_DSC:
            if (!record->event.pressed) {
                layer_invert(DSC);
                layer_off(SFN);
            }
            break;
    }
    return true;
}

static bool caps_word = false;

void caps_word_set_user(bool active) {
    caps_word = active;
}

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state | default_layer_state);
    if (layer > DSC) {
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                uint8_t index = g_led_config.matrix_co[row][col];
                if (index >= 0 && index <= 88 && index != NO_LED &&
                    keymap_key_to_keycode(layer, (keypos_t){col, row}) > KC_TRNS) {
                    switch (layer) {
                        case TFN:
                            rgb_matrix_set_color(index, TFN_COLOR);
                            break;
                        case HFN:
                            rgb_matrix_set_color(index, HFN_COLOR);
                            break;
                        case SFN:
                            rgb_matrix_set_color(index, SFN_COLOR);
                            break;
                    }
                }
            }
        }
    }

    if (host_keyboard_led_state().caps_lock || caps_word) {
        rgb_matrix_set_color(30, RGB_WHITE);
    }

    if (IS_LAYER_ON(HFN)) {
        rgb_matrix_set_color(59, HFN_COLOR);
    } else if (IS_LAYER_ON(TFN)) {
        rgb_matrix_set_color(59, TFN_COLOR);
    } else if (IS_LAYER_ON(SFN)) {
        rgb_matrix_set_color(59, SFN_COLOR);
    }

    if (IS_LAYER_ON(WIN)) {
        /* Show Windows logo in the bottom right. */

        /* Red in the top left. */
        rgb_matrix_set_color(56, WIN_RED);
        /* Green in the top right. */
        rgb_matrix_set_color(57, WIN_GREEN);
        /* Blue in the bottom left. */
        rgb_matrix_set_color(65, WIN_BLUE);
        /* Yellow in the bottom right. */
        rgb_matrix_set_color(66, WIN_YELLOW);
    }

    if (IS_LAYER_ON(DSC)) {
        /* Show Discord color over programmable buttons. */
        rgb_matrix_set_color(29, DSC_BLURPLE);  // DEL
        rgb_matrix_set_color(43, DSC_BLURPLE);  // PGUP
        rgb_matrix_set_color(57, DSC_BLURPLE);  // PGDN
        rgb_matrix_set_color(58, DSC_BLURPLE);  // Bottom left modifier.
    }
}

void suspend_power_down_kb(void) {
    rgb_matrix_set_suspend_state(true);
}

void suspend_wakeup_init_kb(void) {
    rgb_matrix_set_suspend_state(false);
}

/* bool led_update_user(led_t led_state) { */
/*     if (led_state.caps_lock) { */
/*         rgb_matrix_set_color(4, 0xff, 0xff, 0x00); */
/*     } */
/*     return true; */
/* } */

// LEADER_EXTERNS();

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
  KC_LALT, LT_FN,    KC_LGUI,                                KC_SPC,                                 KC_RGUI,  KC_RALT,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[WIN] = LAYOUT(
  KC_GRV,  KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_ESC,
  KC_TAB,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,
  KC_LCTL, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_ENT,             KC_PGUP,
  KC_LSFT, KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,            KC_UP,    KC_PGDN,
  KC_LGUI, LT_FN,    KC_LALT,                                KC_SPC,                                 KC_RALT,  KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[DSC] = LAYOUT(
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_F20,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_F21,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  KC_F22,
  KC_F23,   _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______),

[HFN] = LAYOUT(
  _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______, _______,
  C(KC_TAB),_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______, KC_INS,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,           KC_HOME,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______, KC_END,
  _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______, _______),

[TFN] = LAYOUT(
  _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,  TG(TFN),
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_VOLD,  KC_VOLU,  KC_MUTE,  _______,
  KC_CAPS,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_BRID,  KC_BRIU,  _______,            _______,
  _______,  RGB_TOG,  RGB_MOD,  RGB_HUI,  RGB_SAI,  RGB_VAI,  _______,  _______,  KC_MPRV,  KC_MNXT,  KC_MPLY,  _______,            _______,  _______,
  _______,  MF_SFN,   _______,                                _______,                                _______,  _______,  _______,  _______,  _______),

[SFN] = LAYOUT(
  QK_BOOT,  KC_F13,   KC_F14,   KC_F15,   KC_F16,   KC_F17,   KC_F18,   KC_F19,   KC_F20,   KC_F21,   KC_F22,   KC_F23,   KC_F23,   _______,  _______,
  _______,  _______,  SW_WIN,   _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
  _______,  _______,  _______,  SW_DSC,   _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
  _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______),

[31] = LAYOUT(
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,             _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,  _______,
  _______,  _______,  _______,                                _______,                                _______,  _______,  _______,   _______,  _______)
};
