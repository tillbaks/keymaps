/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "keycodes.h"
#include QMK_KEYBOARD_H

enum uint32_t { OS_MAC, OS_PC } OS_MODE;
typedef union {
    uint32_t raw;
    struct {
        uint32_t os_index; // 0 = MAC, 1 = PC (Windows/Linux)
    };
} user_config_t;

user_config_t user_config;
enum uint8_t { OFF, BLUE, GREEN } notification_rgb;
uint32_t notification_times;

enum my_keycodes {
    TG_OS_MODE = SAFE_RANGE,
    START_OF_MAPPED_KEYCODES,
    MAPPED_UNDO,             // CUSTOM(65) in VIA
    MAPPED_REDO,             // CUSTOM(66)
    MAPPED_CUT,              // CUSTOM(67)
    MAPPED_COPY,             // CUSTOM(68)
    MAPPED_PASTE,            // CUSTOM(69)
    MAPPED_WORD_PREV,        // CUSTOM(70)
    MAPPED_WORD_NEXT,        // CUSTOM(71)
    MAPPED_WORD_DELETE_PREV, // CUSTOM(72)
    MAPPED_WORD_DELETE_NEXT, // CUSTOM(73)
    MAPPED_UC_AO,            // CUSTOM(74)
    MAPPED_UC_AE,            // CUSTOM(75)
    MAPPED_UC_OE,            // CUSTOM(76)
    MAPPED_UC_UE,            // CUSTOM(77)
    MAPPED_BACK,             // CUSTOM(78)
    MAPPED_FORWARD,          // CUSTOM(79)
    MAPPED_TAB_NEW,          // CUSTOM(80)
    MAPPED_TAB_CLOSE,        // CUSTOM(81)
    MAPPED_TAB_PREV,         // CUSTOM(82)
    MAPPED_TAB_NEXT,         // CUSTOM(83)
    END_OF_MAPPED_KEYCODES
};

void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();
}

const uint16_t mapped_keys[][2] = {
    [MAPPED_UNDO - START_OF_MAPPED_KEYCODES]             = {G(KC_Z), C(KC_Z)},
    [MAPPED_REDO - START_OF_MAPPED_KEYCODES]             = {G(S(KC_Z)), C(KC_Y)},
    [MAPPED_CUT - START_OF_MAPPED_KEYCODES]              = {G(KC_X), S(KC_DELETE)},
    [MAPPED_COPY - START_OF_MAPPED_KEYCODES]             = {G(KC_C), C(KC_INS)},
    [MAPPED_PASTE - START_OF_MAPPED_KEYCODES]            = {G(KC_V), S(KC_INS)},
    [MAPPED_WORD_PREV - START_OF_MAPPED_KEYCODES]        = {A(KC_LEFT), C(KC_LEFT)},
    [MAPPED_WORD_NEXT - START_OF_MAPPED_KEYCODES]        = {A(KC_RIGHT), C(KC_RIGHT)},
    [MAPPED_WORD_DELETE_PREV - START_OF_MAPPED_KEYCODES] = {A(KC_BSPC), C(KC_BSPC)},
    [MAPPED_WORD_DELETE_NEXT - START_OF_MAPPED_KEYCODES] = {A(KC_DEL), C(KC_DEL)},
    [MAPPED_UC_AO - START_OF_MAPPED_KEYCODES]            = {UC(0xE4), UC(0xE4)},
    [MAPPED_UC_AE - START_OF_MAPPED_KEYCODES]            = {UC(0xE5), UC(0xE5)},
    [MAPPED_UC_OE - START_OF_MAPPED_KEYCODES]            = {UC(0xF6), UC(0xF6)},
    [MAPPED_UC_UE - START_OF_MAPPED_KEYCODES]            = {UC(0xFC), UC(0xFC)},
    [MAPPED_BACK - START_OF_MAPPED_KEYCODES]             = {G(KC_LEFT), A(KC_LEFT)},
    [MAPPED_FORWARD - START_OF_MAPPED_KEYCODES]          = {G(KC_RIGHT), A(KC_RIGHT)},
    [MAPPED_TAB_NEW - START_OF_MAPPED_KEYCODES]          = {G(KC_T), C(KC_T)},
    [MAPPED_TAB_CLOSE - START_OF_MAPPED_KEYCODES]        = {G(KC_W), C(KC_W)},
    [MAPPED_TAB_NEXT - START_OF_MAPPED_KEYCODES]         = {C(KC_TAB), C(KC_TAB)},
    [MAPPED_TAB_PREV - START_OF_MAPPED_KEYCODES]         = {C(S(KC_TAB)), C(S(KC_TAB))}

    // [SK_KILL - SK_KILL]    = {G(A(KC_ESC)), C(A(KC_DEL))},       // "KILL" OR Force quit /
    // ctrl-alt-del [SK_HENK - SK_KILL]    = {KC_INT4, KC_INT4},                 // 変換 [SK_MHEN -
    // SK_KILL]    = {KC_INT5, KC_INT5},                 // 無変換 [SK_HENT - SK_KILL]    =
    // {G(KC_ENT), C(KC_ENT)},             // Hard ENTER [SK_PSTM - SK_KILL]    = {G(S(A(KC_V))),
    // C(S(A(KC_V)))},     // paste_match [SK_SALL - SK_KILL]    = {G(KC_A), C(KC_A)}, // select all
    // [SK_CLOZ - SK_KILL]    = {G(KC_W), C(KC_W)},                 // close
    // [SK_QUIT - SK_KILL]    = {G(KC_Q), C(KC_Q)},                 // quit
    // [SK_NEW - SK_KILL]     = {G(KC_N), C(KC_N)},                 // new
    // [SK_OPEN - SK_KILL]    = {G(KC_O), C(KC_O)},                 // open
    // [SK_FIND - SK_KILL]    = {G(KC_F), C(KC_F)},                 // find
    // [SK_FAGN - SK_KILL]    = {G(KC_G), KC_F3},                   // find again
    // [SK_SCAP - SK_KILL]    = {S(G(KC_4)), KC_PSCR},              // Screen Capture
    // [SK_SCLP - SK_KILL]    = {C(S(G(KC_4))), A(KC_PSCR)},        // Selection Capture))
    // [SK_DOCBEG - SK_KILL]  = {G(KC_UP), C(KC_HOME)},             // Go to start of document
    // [SK_DOCEND - SK_KILL]  = {G(KC_DOWN), C(KC_END)},            // Go to end of document
    // [SK_LINEBEG - SK_KILL] = {G(KC_DOWN), C(KC_END)},            // Go to beg of line
    // [SK_LINEEND - SK_KILL] = {G(KC_DOWN), C(KC_END)},            // Go to end of line
    // [SK_PARAPRV - SK_KILL] = {A(KC_UP), C(KC_UP)},               // Go to previous paragraph
    // [SK_PARANXT - SK_KILL] = {A(KC_DOWN), C(KC_DOWN)},           // Go to next paragraph
    // [SK_HISTPRV - SK_KILL] = {G(KC_LBRC), A(KC_LEFT)},           // BROWSER BACK
    // [SK_HISTNXT - SK_KILL] = {G(KC_RBRC), A(KC_RIGHT)},          // BROWSER FWD
    // [SK_ZOOMIN - SK_KILL]  = {G(KC_EQL), C(KC_EQL)},             // ZOOM IN
    // [SK_ZOOMOUT - SK_KILL] = {G(KC_MINS), C(KC_MINS)},           // ZOOM OUT
    // [SK_ZOOMRST - SK_KILL] = {G(KC_0), C(KC_0)},                 // ZOOM RESET
    // [SK_APPNXT - SK_KILL]  = {RGUI(KC_TAB), A(KC_TAB)},          // APP switcher Next (last used)
    // [SK_APPPRV - SK_KILL]  = {RGUI(RSFT(KC_TAB)), A(S(KC_TAB))}, // APP switcher Prev (least
    // recently used) [SK_WINNXT - SK_KILL]  = {RCTL(KC_TAB), C(KC_TAB)},          // Window/tab
    // switcher Next [SK_WINPRV - SK_KILL]  = {RCTL(RSFT(KC_TAB)), C(S(KC_TAB))}, // Window/tab
    // switcher Prev [SK_SECT - SK_KILL]    = {A(KC_5), 0xE167},                  // § ** SAMPLE OF
    // GLYPH. REALLY NEED UNICODE. [SK_ENYE - SK_KILL]    = {A(KC_N), A(KC_N)},                 //
    // ñ/Ñ ** SAMPLE OF GLYPH. REALLY NEED UNICODE? [SK_SQUL - SK_KILL]    = {A(KC_RBRC),
    // A(KC_RBRC)},           // ’ ** Left single quote UNICODE? [SK_SQUR - SK_KILL]    =
    // {S(A(KC_RBRC)), S(A(KC_RBRC))},     // ’ ** Right single quote UNICODE? [SK_SDQL - SK_KILL]
    // = {A(KC_LBRC), A(KC_LBRC)},           // ’ ** Left double quote UNICODE? [SK_SDQR - SK_KILL]
    // = {A(S(KC_LBRC)), A(S(KC_LBRC))},     // ’ ** Right double quote UNICODE?

};

#define tap_mapped_key(keycode) \
    tap_code16(mapped_keys[keycode - START_OF_MAPPED_KEYCODES][user_config.os_index])
#define register_mapped_key(keycode) \
    register_code16(mapped_keys[keycode - START_OF_MAPPED_KEYCODES][user_config.os_index])
#define unregister_mapped_key(keycode) \
    unregister_code16(mapped_keys[keycode - START_OF_MAPPED_KEYCODES][user_config.os_index])

bool process_mapped_key(uint16_t keycode, const keyrecord_t *record) {
    if (record->event.pressed) {
        if (keycode >= START_OF_MAPPED_KEYCODES && keycode < END_OF_MAPPED_KEYCODES) {
            register_mapped_key(keycode);
            return false;
        }
        switch (keycode) {
            case KC_COPY:
                register_mapped_key(MAPPED_COPY);
                return false;
            case KC_PASTE:
                register_mapped_key(MAPPED_PASTE);
                return false;
            case KC_CUT:
                register_mapped_key(MAPPED_CUT);
                return false;
            case KC_UNDO:
                register_mapped_key(MAPPED_UNDO);
                return false;
            case KC_AGAIN:
                register_mapped_key(MAPPED_REDO);
                return false;
            case KC_WWW_BACK:
                register_mapped_key(MAPPED_BACK);
                return false;
            case KC_WWW_FORWARD:
                register_mapped_key(MAPPED_FORWARD);
                return false;
        }
    } else {
        if (keycode >= START_OF_MAPPED_KEYCODES && keycode < END_OF_MAPPED_KEYCODES) {
            unregister_mapped_key(keycode);
            return false;
        }
        switch (keycode) {
            case KC_COPY:
                unregister_mapped_key(MAPPED_COPY);
                return false;
            case KC_PASTE:
                unregister_mapped_key(MAPPED_PASTE);
                return false;
            case KC_CUT:
                unregister_mapped_key(MAPPED_CUT);
                return false;
            case KC_UNDO:
                unregister_mapped_key(MAPPED_UNDO);
                return false;
            case KC_AGAIN:
                unregister_mapped_key(MAPPED_REDO);
                return false;
            case KC_WWW_BACK:
                unregister_mapped_key(MAPPED_BACK);
                return false;
            case KC_WWW_FORWARD:
                unregister_mapped_key(MAPPED_FORWARD);
                return false;
        }
    }
    return true;
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_mapped_key(keycode, record)) {
        return false;
    }
    switch (keycode) {
        case TG_OS_MODE:
            if (record->event.pressed) {
                if (user_config.os_index == OS_MAC) {
                    set_unicode_input_mode(QK_UNICODE_MODE_LINUX);
                    user_config.os_index = OS_PC;
                    notification_rgb     = GREEN;
                    notification_times   = 150;
                } else {
                    set_unicode_input_mode(QK_UNICODE_MODE_MACOS);
                    user_config.os_index = OS_MAC;
                    notification_rgb     = BLUE;
                    notification_times   = 150;
                }
                eeconfig_update_user(user_config.raw);
            }
            return false;
    }
    return true;
}

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

enum charybdis_keymap_layers {
    LAYER_BASE = 0,
    LAYER_LOWER,
    LAYER_RAISE,
    LAYER_POINTER,
};

/** \brief Automatically enable sniping-mode on the pointer layer. */
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#define LOWER MO(LAYER_LOWER)
#define RAISE MO(LAYER_RAISE)
#define PT_Z LT(LAYER_POINTER, KC_Z)
#define PT_SLSH LT(LAYER_POINTER, KC_SLSH)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_ESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_LCTL,    PT_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M, KC_COMM,  KC_DOT, PT_SLSH, KC_LALT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                   KC_LGUI, KC_SPC,   LOWER,      RAISE,  KC_ENT,
                                           KC_LALT, KC_BSPC,     KC_DEL
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_LOWER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       KC_TILD, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,    KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RGB_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_LBRC,   KC_P7,   KC_P8,   KC_P9, KC_RBRC, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       RGB_TOG, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    KC_PPLS,   KC_P4,   KC_P5,   KC_P6, KC_PMNS, KC_PEQL,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
      RGB_RMOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_PAST,   KC_P1,   KC_P2,   KC_P3, KC_PSLS, KC_PDOT,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  XXXXXXX, XXXXXXX, _______,    XXXXXXX, _______,
                                           XXXXXXX, XXXXXXX,      KC_P0
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_RAISE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_F12,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,      KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MNXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLU,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MPLY, KC_LEFT,   KC_UP, KC_DOWN, KC_RGHT, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, KC_MUTE,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       KC_MPRV, KC_HOME, KC_PGUP, KC_PGDN,  KC_END, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLD,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  _______, _______, XXXXXXX,    _______, XXXXXXX,
                                           _______, _______,    XXXXXXX
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),

  [LAYER_POINTER] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,    S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,    XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
       XXXXXXX, _______, DRGSCRL, SNIPING,  EE_CLR, QK_BOOT,    QK_BOOT,  EE_CLR, SNIPING, DRGSCRL, _______, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                  KC_BTN2, KC_BTN1, KC_BTN3,    KC_BTN3, KC_BTN1,
                                           XXXXXXX, KC_BTN2,    KC_BTN2
  //                            ╰───────────────────────────╯ ╰──────────────────╯
  ),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD ||
        abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
#        endif // RGB_MATRIX_ENABLE
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >=
                                             CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#        endif // RGB_MATRIX_ENABLE
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);

// LED Color for each layer
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (notification_times == 0) {
        notification_rgb = 0;
    } else {
        notification_times -= 1;
    }
    uint8_t layer = get_highest_layer(layer_state);

    for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
        for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
            uint8_t index = g_led_config.matrix_co[row][col];

            if (index >= led_min && index < led_max && index != NO_LED &&
                keymap_key_to_keycode(layer, (keypos_t){col, row}) > KC_TRNS) {
                if (notification_rgb == BLUE) {
                    rgb_matrix_set_color(index, RGB_BLUE);
                    continue;
                } else if (notification_rgb == GREEN) {
                    rgb_matrix_set_color(index, RGB_GREEN);
                    continue;
                }
                switch (layer) {
                    case 1:
                        rgb_matrix_set_color(index, RGB_RED);
                        break;
                    case 2:
                        rgb_matrix_set_color(index, RGB_ORANGE);
                        break;
                    case 3:
                        rgb_matrix_set_color(index, RGB_MAGENTA);
                        break;
                    case 4:
                        rgb_matrix_set_color(index, RGB_SPRINGGREEN);
                        break;
                    case 5:
                        rgb_matrix_set_color(index, RGB_TEAL);
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return false;
}
#endif

void shutdown_user(void) {
#ifdef RGBLIGHT_ENABLE
    rgblight_enable_noeeprom();
    rgblight_mode_noeeprom(1);
    rgblight_setrgb(RGB_RED);
#endif // RGBLIGHT_ENABLE
#ifdef RGB_MATRIX_ENABLE
    rgb_matrix_set_color_all(RGB_RED);
    rgb_matrix_update_pwm_buffers();
#endif // RGB_MATRIX_ENABLE
}

const key_override_t questionmark_key_override =
    ko_make_with_layers_and_negmods(0,             // Trigger modifiers
                                    KC_SLASH,      // Trigger key
                                    S(KC_SLASH),   // Replacement key
                                    ~0,            // Activate on layer
                                    MOD_MASK_SHIFT // Do not activate when mods are pressed
    );

// This globally defines all key overrides to be used
const key_override_t **key_overrides = (const key_override_t *[]){
    &questionmark_key_override,
    &ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL),
    &ko_make_basic(MOD_MASK_SHIFT, KC_DOT, S(KC_SEMICOLON)),
    &ko_make_basic(MOD_MASK_SHIFT, KC_COMMA, KC_SEMICOLON),
    &ko_make_basic(MOD_MASK_SHIFT, KC_BACKSLASH, KC_SLASH),
    &ko_make_basic(MOD_MASK_SHIFT, KC_SLASH, S(KC_1)),
    NULL // Null terminate the array of overrides!
};
