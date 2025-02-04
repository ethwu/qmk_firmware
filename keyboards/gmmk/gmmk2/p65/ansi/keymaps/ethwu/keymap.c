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

static const int FN_COLORS[TOP_FN_LAYER - BOTTOM_FN_LAYER + 1][3] = {
    { DSC_BLURPLE },
    { HFN_COLOR },
    { HFN_COLOR },
    { TFN_COLOR },
    { TFN_COLOR },
    { SFN_COLOR }
};

// Timer for the tap/held fn layer change key LT_FN.
static uint16_t fn_layer_tap_timer = 0;
// Timer for shutting off the lights after keyboard is idle.
static uint16_t idle_timer = 0;
// Minutes elapsed (for tracking when to shut off the lights).
static uint8_t idle_minutes_elapsed = 0;
/* // Whether the RGB matrix has been disabled by the idle timer. */
/* static bool rgb_matrix_idled = false; */
// Whether RGB function layer overlays are enabled.
static bool rgb_fn_overlay = true;

// Get the index of the corresponding color array for the given function layer.
// Returns -1 if the given function layer does not have a corresponding color
// array.
int fn_color_index(uint8_t layer) {
	if (layer >= BOTTOM_FN_LAYER && layer <= TOP_FN_LAYER) {
		return layer - BOTTOM_FN_LAYER;
	} else {
		return -1;
	}
}

// Set the color of a given LED to the appropriate function layer color.
void set_led_color_for_fn_layer(uint8_t led, uint8_t layer) {
	int color_index = fn_color_index(layer);
	if (color_index > -1) {
		rgb_matrix_set_color(led, FN_COLORS[color_index][0],
			FN_COLORS[color_index][1], FN_COLORS[color_index][2]);
	}
}

// Set led colors for a range.
void set_led_colors(uint8_t start, uint8_t end, uint8_t r, uint8_t g, uint8_t b) {
	for (uint8_t i = start; i < end; i++) rgb_matrix_set_color(i, r, g, b);
}

// Set led colors for a range according to the function layer color.
void set_led_colors_for_fn_layer(uint8_t start, uint8_t end, uint8_t layer) {
	int color_index = fn_color_index(layer);
	if (color_index > -1) {
		set_led_colors(start, end, FN_COLORS[color_index][0],
			FN_COLORS[color_index][1], FN_COLORS[color_index][2]);
	}
}

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
	if (timer_elapsed(idle_timer) > 60000 && rgb_matrix_is_enabled()) {
		// Reset the idle timer.
		idle_timer = timer_read();
		if (idle_minutes_elapsed >= RGB_TIMEOUT) {
			// It is time to turn the matrix off.
			rgb_matrix_disable_noeeprom();
			idle_minutes_elapsed = 0;
		} else {
			idle_minutes_elapsed++;
		}
	}
}

// // Run after all QMK processing.
// void housekeeping_task_user(void) {}

/* // Run repeatedly while system board is idled. */
/* void suspend_power_down_user(void) { */
/* 	/1* rgb_matrix_disable_noeeprom(); *1/ */
/* 	rgb_matrix_set_suspend_state(true); */
/* } */

/* // Run on keyboard wakeup. */
/* void suspend_wakeup_init_user(void) { */
/* 	/1* rgb_matrix_enable_noeeprom(); *1/ */
/* 	rgb_matrix_set_suspend_state(false); */
/* } */

// // Run whenever the layer changes.
layer_state_t layer_state_set_user(layer_state_t state) {
	if (rgb_matrix_get_mode() == RGB_MATRIX_TYPING_HEATMAP) set_led_colors(67, 87, RGB_OFF);
	return state;
}

void disable_fn_layers(void) {
	layer_and(((layer_state_t)1 << WIN) | ((layer_state_t)1 << DSC));
}

/* // Send a different string in Windows vs macOS mode. */
/* void send_string_win_mac(const char *win_str, const char *mac_str) { */
/* 	if (IS_LAYER_ON(WIN)) { */
/* 		send_string_P(win_str); */
/* 	} else { */
/* 		send_string_P(mac_str); */
/* 	} */
/* } */
/* #define SS_WM(win, mac) send_string_win_mac(PSTR(win), PSTR(mac)) */

/* // Send a basic keyboard shortcut that has shared letter btw windows and mac. */
/* #define SBS_WM(shortcut) SS_WM(SS_LCTL(shortcut), SS_LGUI(shortcut)) */

// Run whenever a key is pressed or released, before the key event is handled. 
// Returns true if QMK should handle the key event normally. 
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed) {
		/* // key pressed */
		/* if (rgb_matrix_idled) rgb_matrix_enable_noeeprom(); */
		if (!rgb_matrix_is_enabled()) rgb_matrix_enable_noeeprom();
		reset_idle_timer();

		switch (keycode) {
		case LT_FN:
			fn_layer_tap_timer = timer_read();
			layer_on(HFN);
			if (IS_LAYER_ON(WIN)) layer_on(HFN_WIN);
			break;
		case MF_SFN:
			layer_on(SFN);
			break;
		case FN_CTRL: // When held, disable the function layers and leave the ctrl modifier down.
			disable_fn_layers();
			register_code(KC_LCTL);
			break;
		}
	} else {
		// key released
		switch (keycode) {
		case TG_FOV:
			rgb_fn_overlay = !rgb_fn_overlay;
			break;
		case FN_CTRL:
			unregister_code(KC_LCTL);
			break;
		case LT_FN:
			if (timer_elapsed(fn_layer_tap_timer) < TAPPING_TERM) {
				layer_on(TFN);
				if (IS_LAYER_ON(WIN)) layer_on(TFN_WIN);
				break;
			}
			// fall through
		case MF_SFN:
		case ESC_FN:
			disable_fn_layers();
			break;
		// Don't disable function layer when changing volume.
		case KC_VOLU:
		case KC_VOLD:
		case KC_BRIU:
		case KC_BRID:
			break;
		default:
			disable_fn_layers();
			break;
		}
	}
	return true;
}

// // Run after a keystroke is sent.
// void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
// }

// Set RGB indicators.
void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
	uint8_t highest_layer = get_highest_layer(layer_state | default_layer_state);

	// Highlight the caps lock key in white if caps lock or caps word are active.
	if (host_keyboard_led_state().caps_lock || is_caps_word_on()) {
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

	if (rgb_fn_overlay) {
		// Highlight function layer keys in their colors.
		if (highest_layer >= BOTTOM_FN_LAYER) {
			for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
				for (uint8_t col = 0; col < MATRIX_COLS; col++) {
					uint8_t index = g_led_config.matrix_co[row][col];
					keypos_t pos = { col, row };

					uint8_t which_layer = layer_switch_get_layer(pos);
					uint16_t kc = keymap_key_to_keycode(which_layer, pos);

					if (index != NO_LED && which_layer >= BOTTOM_FN_LAYER &&
						IS_LAYER_ON(which_layer)) {
						switch (kc) {
						case KC_NO:
							rgb_matrix_set_color(index, RGB_OFF);
							break;
						case SW_WIN:
							rgb_matrix_set_color(index, WIN_YELLOW);
							break;
						case SW_DSC:
							rgb_matrix_set_color(index, DSC_BLURPLE);
							break;
						default:
							set_led_color_for_fn_layer(index, which_layer);
							break;
						}
					}
				}
			}
		}
		set_led_colors_for_fn_layer(67, 87, highest_layer);
	} else {
		set_led_color_for_fn_layer(59, highest_layer);
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
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,            	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	        	_______,	_______,
KC_LGUI,	LT_FN  ,	KC_LALT,	        	        	        	_______,	        	        	        	KC_RALT,	KC_RCTL,	_______,	_______,	_______),
	[DSC] = LAYOUT(
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,            	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	        	_______,	_______,
_______,	_______,	_______,	        	        	        	_______,	        	        	        	KC_F23 ,	KC_F24 ,	_______,	_______,	_______),
	[HFN] = LAYOUT(
_______,	KC_F1  ,	KC_F2  ,	KC_F3  ,	KC_F4  ,	KC_F5  ,	KC_F6  ,	KC_F7  ,	KC_F8  ,	KC_F9  ,	KC_F10  ,	KC_F11 ,	KC_F12 ,	_______,	_______,
C(KC_TAB),	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	KC_INS ,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,            	KC_HOME,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	        	_______,	KC_END ,
_______,	LT_FN  ,	_______,	        	        	        	_______,	        	        	        	_______,	KC_APP ,	_______,	_______,	_______),
	[HFN_WIN] = LAYOUT(
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______ ,	_______,	_______,	_______,	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,            	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	        	_______,	_______,
_______,	LT_FN  ,	_______,	        	        	        	_______,	        	        	        	_______,	_______,	_______,	_______,	_______),
	[TFN] = LAYOUT(
XXXXXXX,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	XXXXXXX,	ESC_FN,
C(KC_F4),	G(KC_Q),	G(KC_W),	XXXXXXX,	G(S(KC_Z)),	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	KC_VOLD,	KC_VOLU,	KC_MUTE,	_______,
FN_CTRL,	XXXXXXX,	G(KC_S),	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	XXXXXXX,	KC_BRID,	KC_BRIU,	_______,	        	_______,
_______,	G(KC_Z),	G(KC_X),	G(KC_C),	G(KC_V),	XXXXXXX,	XXXXXXX,	XXXXXXX,	KC_MPRV,	KC_MNXT,	KC_MPLY,	_______,	        	_______,	_______,
_______,	MF_SFN ,	XXXXXXX,	        	        	        	XXXXXXX,	        	        	        	C(KC_F3),	C(KC_F2),	_______,	_______,	_______),
	[TFN_WIN] = LAYOUT(
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,
A(KC_ESC),	A(KC_F4),	C(KC_F4),	_______,	C(KC_R),	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,
_______,	_______,	C(KC_S),	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	        	_______,
_______,	C(KC_Z),	S(KC_DEL),	C(KC_INS),	S(KC_INS),	_______,	_______,	_______,	_______,	_______,	_______,	_______,	        	_______,	_______,
_______,	MF_SFN ,	_______,	        	        	        	_______,	        	        	        	KC_RGUI,	KC_RALT,	_______,	_______,	_______),
	[SFN] = LAYOUT(
QK_BOOT,	KC_F13 ,	KC_F14 ,	KC_F15 ,	KC_F16 ,	KC_F17 ,	KC_F18 ,	KC_F19 ,	KC_F20 ,	KC_F21 ,	KC_F22 ,	KC_F23       ,	KC_F24       ,	_______,	_______,
_______,	TG_FOV ,	SW_WIN ,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	S(A(_______)),	S(A(_______)),	_______,	KC_PSCR,
KC_CAPS,	_______,	_______,	SW_DSC ,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______      ,	_______      ,	        	KC_SCRL,
_______,	RGB_TOG,	RGB_MOD,	RGB_HUI,	RGB_SAI,	RGB_VAI,	_______,	_______,	_______,	_______,	_______,	_______      ,	              	_______,	KC_PAUS,
_______,	MF_SFN ,	_______,	        	        	        	_______,	        	        	        	QK_RBT ,	_______      ,	_______      ,	_______,	_______),
	[31] = LAYOUT(
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,            	_______,
_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	_______,	        	_______,	_______,
_______,	_______,	_______,	        	        	        	_______,	        	        	        	_______,	_______,	_______,	_______,	_______)
};

// vim: noexpandtab
