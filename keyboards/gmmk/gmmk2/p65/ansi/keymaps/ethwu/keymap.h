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

enum Layers {
	// Base layer for macOS.
	MAC,
	// Alternate base layer for Windows.
	WIN,
	// Discord control overlay.
	DSC,
	// Function layer on FN hold.
	HFN,
	// Utility layer for Windows.
	HFN_WIN,
	// Function layer on FN tap.
	TFN,
	// Replacement functions for Windows.
	TFN_WIN,
	// Function layer on FN held after tap.
	SFN,
};

enum Keycodes {
	// Activate TFN on tap and momentarily activate HFN when held.
	LT_FN = SAFE_RANGE,
	// Momentarily activate SFN while held and deactivate both FN and SFN when
	// released.
	MF_SFN,
	// Deactivate the function layers.
	ESC_FN,
	// Leave function layers and hold control.
	FN_CTRL,
	REDO,
	CLOSE,
	QUIT,
	SW_NEXT,
	SW_PREV,
	// Toggle function layer RGB overlay.
	TG_FOV
};

#define SW_WIN TG(WIN)
#define SW_DSC TG(DSC)

// The bottommost function layer.
#define BOTTOM_FN_LAYER DSC
#define TOP_FN_LAYER SFN

// Colors of the Windows logo.

// #define WIN_RED RGB_RED
// #define WIN_GREEN RGB_GREEN
// #define WIN_BLUE RGB_BLUE 
// #define WIN_YELLOW RGB_YELLOW

#define WIN_RED 0xf2, 0x50, 0x22
#define WIN_GREEN 0x7f, 0xba, 0x00
#define WIN_BLUE 0x00, 0xa4, 0xef
#define WIN_YELLOW 0xff, 0xb9, 0x00

// Discord brand color.

#define DSC_BLURPLE 0x58, 0x65, 0xf2

// Function layer colors.

// #define UTIL_COLOR 0xff, 0xb5, 0x11
// #define HFN_COLOR 0x00, 0xb0, 0xda 

// alternate color palette
// #define HFN_COLOR 0xEF, 0x56, 0x45
// #define TFN_COLOR 0xfe, 0xbc, 0x11
// #define SFN_COLOR 0x00, 0x36, 0x60

// share the windows colors
#define HFN_COLOR WIN_BLUE
#define TFN_COLOR WIN_GREEN
#define SFN_COLOR WIN_RED

// Timeout for RGB lights in minutes.
#define RGB_TIMEOUT 1

// vim: noexpandtab
