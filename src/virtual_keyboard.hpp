#pragma once

#include "../calc.hpp"
#include "../lib/functions/shapes.hpp"
#include <vector>
#include <string>
#include <cstring>
#include "internal.hpp"

// Theme colors (Light theme from cinput.py)
#define C_WHITE 0xFFFF
#define C_BLACK 0x0000
#define C_LIGHT 0xCE59 // Grey
#define C_DARK  0x0000

// Theme dictionary equivalent
struct Theme {
    uint16_t modal_bg = C_WHITE;
    uint16_t kbd_bg = C_WHITE;
    uint16_t key_bg = C_WHITE;
    uint16_t key_spec = 0xCE59; // Secondary (Light Grey) ~ RGB(200, 200, 200)
    uint16_t key_out = C_BLACK;
    uint16_t txt = 0x0000;
    uint16_t txt_dim = 0x4208; // Dark Grey
    uint16_t accent = 0x0010; // Dark Blue-ish
    uint16_t txt_acc = C_WHITE;
    uint16_t hl = 0xCE59;
};

struct KeyRect {
    int16_t x, y, w, h;
    const char* label;
    const char* val; // if NULL, use label
    bool is_spec;
    bool is_acc;
};

class VirtualKeyboard {
public:
    bool visible = true;
    int current_tab = 0; // 0: ABC, 1: Sym, 2: Math
    bool shift = false;
    const char* last_key = nullptr;
    uint8_t* font;
    Theme theme;

    // Layouts
    const char* tabs[3] = {"ABC", "Sym", "Math"};

    // QWERTY Layout
    const char* layout_alpha[4] = {
        "1234567890",
        "qwertyuiop",
        "asdfghjkl:",
        "zxcvbnm,._"
    };

    const char* layout_sym[4] = {
        "1234567890",
        "@#$_&-+()/",
        "=\\<*\"':;!?",
        "{}[]^~`|<>"
    };

    VirtualKeyboard() {
        // Initialize theme
    }

    void SetFont(uint8_t* f) {
        font = f;
    }

    void draw_rect_border(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t border_col) {
        drawRectangle(x, y, w, h, border_col);
    }

    void draw_text_centered(int16_t cx, int16_t cy, const char* text, uint16_t color) {
        if (!font) return;
        // Estimate width (8px per char approx)
        int len = strlen(text);
        int w = len * 8;
        int h = 10;
        DRAW_FONT(font, text, cx - w/2, cy - h/2, color, 0);
    }

    void draw_key(int16_t x, int16_t y, int16_t w, int16_t h, const char* label, bool is_special, bool is_pressed, bool is_accent) {
        uint16_t bg;
        if (is_pressed) bg = theme.hl;
        else if (is_accent) bg = theme.accent;
        else if (is_special) bg = theme.key_spec;
        else bg = theme.key_bg;

        uint16_t txt_col = is_accent ? theme.txt_acc : theme.txt;
        uint16_t border_col = theme.key_spec;

        drawFilledRectangle(x + 1, y + 1, w - 2, h - 2, bg);
        draw_rect_border(x, y, w, h, border_col);
        draw_text_centered(x + w/2, y + h/2, label, txt_col);
    }

    void draw_tabs() {
        int16_t y_pos = SCREEN_H - KBD_H;
        int16_t tab_w = SCREEN_W / 3;
        uint16_t border_col = theme.key_spec;

        for (int i = 0; i < 3; i++) {
            int16_t tx = i * tab_w;
            bool is_active = (i == current_tab);
            uint16_t bg = is_active ? theme.kbd_bg : theme.key_spec;

            drawFilledRectangle(tx, y_pos, tab_w, TAB_H, bg);
            draw_rect_border(tx, y_pos, tab_w, TAB_H, border_col);

            // "Active" look (cover bottom border)
            if (is_active) {
               // drawFilledRectangle(tx + 1, y_pos + TAB_H - 1, tab_w - 2, 2, theme.kbd_bg);
            }

            const char* name = tabs[i];
            // If tab 0 and not qwerty (not implemented yet), change name?
            draw_text_centered(tx + tab_w/2, y_pos + TAB_H/2, name, theme.txt);
        }
    }

    void draw_grid() {
        int16_t grid_y = (SCREEN_H - KBD_H) + TAB_H;
        int16_t row_h = 45;

        const char** layout = (current_tab == 1) ? layout_sym : layout_alpha;

        char labelBuf[2] = {0, 0};

        for (int r = 0; r < 4; r++) {
            const char* row = layout[r];
            int count = strlen(row);
            int16_t kw = SCREEN_W / count;

            for (int c = 0; c < count; c++) {
                int16_t kx = c * kw;
                int16_t ky = grid_y + r * row_h;

                char char_code = row[c];
                if (current_tab == 0 && shift) {
                    if (char_code >= 'a' && char_code <= 'z') char_code -= 32;
                }

                labelBuf[0] = char_code;

                // Check if pressed
                bool is_pressed = false;
                // We need to know if this specific key is pressed.
                // For simplicity, we can rely on immediate redraw or state.
                // Assuming last_key stores the char string.
                if (last_key && last_key[0] == char_code && last_key[1] == 0) is_pressed = true;

                draw_key(kx, ky, kw, row_h, labelBuf, false, is_pressed, false);
            }
        }

        // Bottom Control Row
        int16_t bot_y = grid_y + 4 * row_h;
        int16_t bot_h = row_h;

        bool caps_pressed = shift;
        bool back_pressed = (last_key && strcmp(last_key, "BACKSPACE") == 0);
        bool space_pressed = (last_key && strcmp(last_key, " ") == 0);
        bool enter_pressed = (last_key && strcmp(last_key, "ENTER") == 0);

        draw_key(0, bot_y, 50, bot_h, "CAPS", true, caps_pressed, false);
        draw_key(50, bot_y, 50, bot_h, "<-", true, back_pressed, false);
        draw_key(100, bot_y, 160, bot_h, "Space", false, space_pressed, false);
        draw_key(260, bot_y, 60, bot_h, "EXE", false, enter_pressed, true);
    }

    // Helper for Math layout (simplified from cinput.py)
    void draw_math() {
        int16_t start_y = (SCREEN_H - KBD_H) + TAB_H;
        int16_t total_h = KBD_H - TAB_H;
        int16_t row_h = total_h / 4;
        int16_t side_w = 50;
        int16_t center_w = SCREEN_W - (side_w * 2);
        int16_t numpad_w = center_w / 3;

        // Left Col: + - * /
        const char* ops[] = {"+", "-", "*", "/"};
        for (int i=0; i<4; i++) {
            bool pressed = (last_key && strcmp(last_key, ops[i]) == 0);
            draw_key(0, start_y + i*row_h, side_w, row_h, ops[i], false, pressed, false);
        }

        // Right Col
        // %, " ", <-, EXE
        // const char* r_labels[] = {"%", " ", "<-", "EXE"};
        // const char* r_vals[] = {"%", " ", "BACKSPACE", "ENTER"};
        // is_spec, is_acc

        // %
        draw_key(SCREEN_W - side_w, start_y, side_w, row_h, "%", false, (last_key && strcmp(last_key, "%") == 0), false);
        // Space
        draw_key(SCREEN_W - side_w, start_y + row_h, side_w, row_h, " ", false, (last_key && strcmp(last_key, " ") == 0), false);
        // Backspace
        draw_key(SCREEN_W - side_w, start_y + 2*row_h, side_w, row_h, "<-", true, (last_key && strcmp(last_key, "BACKSPACE") == 0), false);
        // Enter
        draw_key(SCREEN_W - side_w, start_y + 3*row_h, side_w, row_h, "EXE", false, (last_key && strcmp(last_key, "ENTER") == 0), true);

        // Numpad 1-9
        const char* nums[3][3] = {{"1","2","3"}, {"4","5","6"}, {"7","8","9"}};
        for (int r=0; r<3; r++) {
            for (int c=0; c<3; c++) {
                bool pressed = (last_key && strcmp(last_key, nums[r][c]) == 0);
                draw_key(side_w + c*numpad_w, start_y + r*row_h, numpad_w, row_h, nums[r][c], false, pressed, false);
            }
        }

        // Bottom Row: , # 0 = .
        int16_t y_bot = start_y + 3*row_h;
        int16_t unit_w = center_w / 6;
        const char* bot_row[] = {",", "#", "0", "=", "."};
        int widths[] = {1, 1, 2, 1, 1};
        int16_t cur_x = side_w;

        for (int i=0; i<5; i++) {
            int16_t w = widths[i] * unit_w;
            if (i == 4) w = (side_w + center_w) - cur_x;
            bool pressed = (last_key && strcmp(last_key, bot_row[i]) == 0);
            draw_key(cur_x, y_bot, w, row_h, bot_row[i], false, pressed, false);
            cur_x += w;
        }
    }

    void Render() {
        if (!visible) return;

        int16_t y_pos = SCREEN_H - KBD_H;
        // Background
        drawFilledRectangle(0, y_pos, SCREEN_W, KBD_H, theme.kbd_bg);
        line(0, y_pos, SCREEN_W, y_pos, theme.key_spec); // Top border line

        draw_tabs();

        if (current_tab == 2) {
            draw_math();
        } else {
            draw_grid();
        }
    }

    // Returns the key string if pressed, or NULL
    const char* Update(uint16_t touch_x, uint16_t touch_y, uint32_t type) {
        if (!visible) return nullptr;
        int16_t kbd_y = SCREEN_H - KBD_H;
        if (touch_y < kbd_y) return nullptr;

        // Reset last key on touch down
        if (type == TOUCH_DOWN) last_key = nullptr;

        // Tabs
        if (touch_y < kbd_y + TAB_H) {
            if (type == TOUCH_DOWN) {
                int16_t tab_w = SCREEN_W / 3;
                current_tab = touch_x / tab_w;
                if (current_tab > 2) current_tab = 2;
                Render(); // Redraw immediately
            }
            return nullptr;
        }

        const char* ret = nullptr;

        if (current_tab == 2) {
            // Math Layout Hit Test
            int16_t start_y = kbd_y + TAB_H;
            int16_t total_h = KBD_H - TAB_H;
            int16_t row_h = total_h / 4;
            int16_t side_w = 50;
            int16_t center_w = SCREEN_W - (side_w * 2);
            int16_t numpad_w = center_w / 3;

            int row = (touch_y - start_y) / row_h;
            if (row < 0 || row > 3) return nullptr;

            // Left Col
            if (touch_x < side_w) {
                const char* ops[] = {"+", "-", "*", "/"};
                ret = ops[row];
            }
            // Right Col
            else if (touch_x >= SCREEN_W - side_w) {
                const char* vals[] = {"%", " ", "BACKSPACE", "ENTER"};
                ret = vals[row];
            }
            // Center
            else {
                if (row < 3) {
                    int col = (touch_x - side_w) / numpad_w;
                    const char* nums[3][3] = {{"1","2","3"}, {"4","5","6"}, {"7","8","9"}};
                    if (col >= 0 && col < 3) ret = nums[row][col];
                } else {
                    // Bottom row
                    int16_t rel_x = touch_x - side_w;
                    int16_t unit_w = center_w / 6;
                    // , # 0 = . widths: 1 1 2 1 1
                    if (rel_x < unit_w) ret = ",";
                    else if (rel_x < unit_w*2) ret = "#";
                    else if (rel_x < unit_w*4) ret = "0";
                    else if (rel_x < unit_w*5) ret = "=";
                    else ret = ".";
                }
            }

        } else {
            // Grid Layout Hit Test
            int16_t grid_y = kbd_y + TAB_H;
            int16_t row_h = 45;
            int row_idx = (touch_y - grid_y) / row_h;

            if (row_idx >= 0 && row_idx < 4) {
                const char** layout = (current_tab == 1) ? layout_sym : layout_alpha;
                const char* row_str = layout[row_idx];
                int count = strlen(row_str);
                int16_t kw = SCREEN_W / count;
                int col_idx = touch_x / kw;
                if (col_idx >= count) col_idx = count - 1;

                static char charStr[2] = {0, 0};
                char char_code = row_str[col_idx];
                if (current_tab == 0 && shift && char_code >= 'a' && char_code <= 'z') {
                    char_code -= 32;
                }
                charStr[0] = char_code;
                ret = charStr;

            } else if (row_idx == 4) {
                if (touch_x < 50) {
                    if (type == TOUCH_DOWN) {
                        shift = !shift;
                        Render();
                    }
                    return nullptr; // Shift toggle doesn't return key char
                } else if (touch_x < 100) {
                    ret = "BACKSPACE";
                } else if (touch_x < 260) {
                    ret = " ";
                } else {
                    ret = "ENTER";
                }
            }
        }

        if (type == TOUCH_DOWN) {
            last_key = ret;
            Render(); // Visual feedback
        } else if (type == TOUCH_UP) {
            last_key = nullptr;
            Render(); // Clear feedback
        }

        return ret;
    }

    // Toggle Visibility
    void Toggle() {
        visible = !visible;
        // Need to clear screen area if hidden?
        // The main loop usually redraws everything, or we might need to trigger a full refresh.
    }
};
