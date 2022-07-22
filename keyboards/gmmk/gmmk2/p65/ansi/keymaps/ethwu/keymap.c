/* Copyright 2022 Ethan Wu <ethanwu@cs.ucsb.edu>

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
#include "keymap.h"

// Timer for the tap/held fn layer change key LT_FN.
static uint16_t fn_layer_tap_timer;
// Timer for shutting off the lights after keyboard is idle.
static uint16_t idle_timer;
// Minutes elapsed (for tracking when to shut off the lights).
static uint8_t idle_minutes_elapsed;
// Whether the RGB matrix has been disabled by the idle timer.
static bool rgb_matrix_idled = false;

// Whether caps word is enabled.
static bool caps_word = false;

// Reset the idle timer.
void reset_idle_timer(void) {
	idle_timer = timer_read();
	idle_minutes_elapsed = 0;
}

// // Run very early (before USB is started). Primarily for initing hardware.
// void keyboard_pre_init_user(void) {}
// // Run when matrix is initialized, before features.
// void matrix_init_user(void) {}
// Run after features.
void keyboard_post_init_user(void) {
	reset_idle_timer();
}

// Run every matrix scan (as often as possible).
void matrix_scan_user(void) {
	if (timer_elapsed(idle_timer) > 60000) {
		idle_timer = timer_read();
		idle_timer++;

		if (idle_minutes_elapsed > RGB_TIMEOUT && !rgb_matrix_idled) {
			reset_idle_timer();
			rgb_matrix_disable_noeeprom();
		}
	}
}

// // Run after all QMK processing.
// void housekeeping_task_user(void) {}

// Run repeatedly while system board is idled.
void suspend_power_down_user(void) {
	rgb_matrix_set_suspend_state(true);
}

// Run on keyboard wakeup.
void suspend_wakeup_init_user(void) {
	rgb_matrix_set_suspend_state(false);
}

// // Run whenever the layer changes.
// layer_state_t layer_state_set_user(layer_state_t state) {
// 	return state;
// }

// Run whenever a key is pressed or released, before the key event is handled. 
// Returns true if QMK should handle the key event normally. 
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {
		// key pressed
		if (rgb_matrix_idled) rgb_matrix_enable_noeeprom();
		reset_idle_timer();

		switch (keycode) {
		case LT_FN:
			fn_layer_tap_timer = timer_read();
			layer_on(HFN);
			break;
		case MF_SFN:
			layer_on(SFN);
			break;
		}
	} else {
		// key released
		switch (keycode) {
		case LT_FN:
			layer_off(HFN);
			if (timer_elapsed(fn_layer_tap_timer) < TAPPING_TERM) layer_invert(TFN);
			break;
		case MF_SFN:
			layer_off(SFN);
			layer_off(TFN);
			break;
		}
	}
	return true;
}

// Run when caps word state changes.
void caps_word_set_user(bool active) {
	caps_word = active;
}

// Set RGB indicators.
void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
	uint8_t highest_layer = get_highest_layer(layer_state | default_layer_state);
	uint8_t rgb_mode = rgb_matrix_get_mode();

	// Highlight the caps lock key in white if caps lock or caps word are active.
	if (host_keyboard_led_state().caps_lock || caps_word) {
		rgb_matrix_set_color(30, RGB_WHITE);
	}

	// Windows layer indicator.
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

		/* Highlight switched modifiers. */
		/* rgb_matrix_set_color(58, WIN_RED); */
		/* rgb_matrix_set_color(60, WIN_GREEN); */
		/* rgb_matrix_set_color(62, WIN_BLUE); */
		/* rgb_matrix_set_color(63, WIN_YELLOW); */
	}

	// Highlight function layer keys in their colors.
	if (highest_layer >= BOTTOM_FN_LAYER) {
		for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
			for (uint8_t col = 0; col < MATRIX_COLS; col++) {
				uint8_t index = g_led_config.matrix_co[row][col];
				keypos_t pos = {col, row};

				uint8_t which_layer;
				int kc;
				for (which_layer = highest_layer; which_layer >= BOTTOM_FN_LAYER; which_layer--) {
					if (IS_LAYER_ON(which_layer)) {
						kc = keymap_key_to_keycode(which_layer, pos);
						if (kc > KC_TRNS || index == 59) break;
					}
				}
				if (index >= 0 && index <= 66 && index != NO_LED && which_layer >= BOTTOM_FN_LAYER) {
					switch (which_layer) {
						case DSC:
							if (index != 59) rgb_matrix_set_color(index, DSC_BLURPLE);
							break;
						case HFN:
							rgb_matrix_set_color(index, HFN_COLOR);
							break;
						case TFN:
							rgb_matrix_set_color(index, TFN_COLOR);
							break;
						case SFN:
							rgb_matrix_set_color(index, SFN_COLOR);
							break;
					}
				}
			}
		}
	}

	for (uint8_t index = 69; index <= 88; index++) {
		switch (highest_layer) {
		case DSC:
			rgb_matrix_set_color(index, DSC_BLURPLE);
			break;
		case HFN:
			rgb_matrix_set_color(index, HFN_COLOR);
			break;
		case TFN:
			rgb_matrix_set_color(index, TFN_COLOR);
			break;
		case SFN:
			rgb_matrix_set_color(index, SFN_COLOR);
			break;
		default:
			if (rgb_mode == RGB_MATRIX_TYPING_HEATMAP) {
				rgb_matrix_set_color(index, RGB_OFF);
			}
			break;
		}
	}
}


// Layer definitions.
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[MAC] = LAYOUT(
KC_GRV ,	KC_1 ,	KC_2   ,	KC_3,	KC_4,	KC_5,	KC_6  ,	KC_7,	KC_8   ,	KC_9  ,	KC_0   ,	KC_MINS,	KC_EQL ,	KC_BSPC,	KC_ESC,
KC_TAB ,	KC_Q ,	KC_W   ,	KC_E,	KC_R,	KC_T,	KC_Y  ,	KC_U,	KC_I   ,	KC_O  ,	KC_P   ,	KC_LBRC,	KC_RBRC,	KC_BSLS,	KC_DEL,
KC_LCTL,	KC_A ,	KC_S   ,	KC_D,	KC_F,	KC_G,	KC_H  ,	KC_J,	KC_K   ,	KC_L  ,	KC_SCLN,	KC_QUOT,	KC_ENT ,	        	KC_PGUP,
KC_LSFT,	KC_Z ,	KC_X   ,	KC_C,	KC_V,	KC_B,	KC_N  ,	KC_M,	KC_COMM,	KC_DOT,	KC_SLSH,	KC_RSFT,            	KC_UP  ,	KC_PGDN,
KC_LALT,	LT_FN,	KC_LGUI,	     	     	     	KC_SPC,	     	        	       	KC_RGUI,	KC_RALT,	KC_LEFT,	KC_DOWN,	KC_RGHT),
	[WIN] = LAYOUT(
KC_GRV ,	KC_1 ,	KC_2   ,	KC_3,	KC_4,	KC_5,	KC_6  ,	KC_7,	KC_8   ,	KC_9  ,	KC_0   ,	KC_MINS,	KC_EQL ,	KC_BSPC,	KC_ESC,
KC_TAB ,	KC_Q ,	KC_W   ,	KC_E,	KC_R,	KC_T,	KC_Y  ,	KC_U,	KC_I   ,	KC_O  ,	KC_P   ,	KC_LBRC,	KC_RBRC,	KC_BSLS,	KC_DEL,
KC_LCTL,	KC_A ,	KC_S   ,	KC_D,	KC_F,	KC_G,	KC_H  ,	KC_J,	KC_K   ,	KC_L  ,	KC_SCLN,	KC_QUOT,	KC_ENT ,	        	KC_PGUP,
KC_LSFT,	KC_Z ,	KC_X   ,	KC_C,	KC_V,	KC_B,	KC_N  ,	KC_M,	KC_COMM,	KC_DOT,	KC_SLSH,	KC_RSFT,            	KC_UP  ,	KC_PGDN,
KC_LGUI,	LT_FN,	KC_LALT,	     	     	     	KC_SPC,	     	        	       	KC_RALT,	KC_RCTL,	KC_LEFT,	KC_DOWN,	KC_RGHT),
	[DSC] = LAYOUT(
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	KC_F20 ,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,            	KC_F21 ,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	        	_______,	KC_F22 ,
KC_F23 ,	_______,	_______,	        	        	        	_______,	        	        	        	_______,	_______,	_______,	_______,	_______),
	[HFN] = LAYOUT(
_______,	KC_F1  ,	KC_F2  ,	KC_F3  ,	KC_F4  ,	KC_F5  ,	KC_F6  ,	KC_F7  ,	KC_F8  ,	KC_F9  ,	KC_F10  ,	KC_F11 ,	KC_F12 ,	_______,	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	KC_INS ,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,            	KC_HOME,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	        	_______,	KC_END ,
_______,	_______,	_______,	        	        	        	_______,	        	        	        	_______,	_______,	_______,	_______,	_______),
	[TFN] = LAYOUT(
XXXXXXX,	KC_F1  ,	KC_F2  ,	KC_F3  ,	KC_F4  ,	KC_F5  ,	KC_F6  ,	KC_F7  ,	KC_F8  ,	KC_F9  ,	KC_F10  ,	KC_F11 ,	KC_F12 ,	XXXXXXX,	TG(TFN),
XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	KC_VOLD,	KC_VOLU,	KC_MUTE,	XXXXXXX,
XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	KC_BRID,	KC_BRIU,	XXXXXXX,            	XXXXXXX,
XXXXXXX,	RGB_TOG,	RGB_MOD,	RGB_HUI,	RGB_SAI,	RGB_VAI,	XXXXXXX,	XXXXXXX,	KC_MPRV,	KC_MNXT,	KC_MPLY,	XXXXXXX,	        	XXXXXXX,	XXXXXXX,
XXXXXXX,	MF_SFN ,	XXXXXXX,	        	        	        	XXXXXXX,	        	        	        	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX),
	[SFN] = LAYOUT(
QK_BOOT,	KC_F13 ,	KC_F14 ,	KC_F15 ,	KC_F16 ,	KC_F17 ,	KC_F18 ,	KC_F19 ,	KC_F20 ,	KC_F21 ,	KC_F22 ,	KC_F23 ,	KC_F24 ,	_______,	_______,
_______,	_______,	TG(WIN),	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	KC_PSCR,
KC_CAPS,	_______,	_______,	TG(DSC),	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,            	KC_SCRL,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	        	_______,	KC_PAUS,
_______,	_______,	_______,	        	        	        	_______,	        	        	        	_______,	_______,	_______,	_______,	_______),
	[31] = LAYOUT(
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,            	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	        	_______,	_______,
_______,	_______,	_______,	        	        	        	_______,	        	        	        	_______,	_______,	_______,	_______,	_______)
};

// vim: noexpandtab
