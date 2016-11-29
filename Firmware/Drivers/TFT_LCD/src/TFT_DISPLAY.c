#include "TFT_DISPLAY.h"

typedef enum _tftDisplayButtonStatus {
    INACTIVE,
    ACTIVE,
    PRESSED
} tftDisplayButtonStatus;

typedef struct _tftButtons {
    uint16_t _w1;
    uint16_t _h1;
    uint16_t _w2;
    uint16_t _h2;
    uint8_t buttonHandle;
    uint8_t status;
    const char* stringVal;
    uint8_t font;
    uint16_t color;
    uint8_t debouncer;
} tftButtons;

static tftButtons activeButtons[10] = {};

static uint8_t numberActiveButtons = 0;

uint8_t checkRange(uint16_t x, uint16_t y, uint16_t target_x1, uint16_t target_x2, uint16_t target_y1, uint16_t target_y2);

void animatePress(tftButtons thisButton);

uint8_t TFT_DISPLAY_place_button(const char* string, uint8_t x, uint8_t y, uint16_t color, uint8_t size) {
    activeButtons[numberActiveButtons]._w1 = (TFT_LCD_width() / 4)*(x - 1) + 10;
    activeButtons[numberActiveButtons]._h1 = (TFT_LCD_height() / 4)*(y - 1) + 10;
    activeButtons[numberActiveButtons]._w2 = (TFT_LCD_width() / 4) * x - 10;
    activeButtons[numberActiveButtons]._h2 = (TFT_LCD_height() / 4) * y - 10;
    activeButtons[numberActiveButtons].buttonHandle = numberActiveButtons;
    activeButtons[numberActiveButtons].status = ACTIVE;
    activeButtons[numberActiveButtons].stringVal = string;
    activeButtons[numberActiveButtons].font = size;
    activeButtons[numberActiveButtons].color = color;
    animatePress(activeButtons[numberActiveButtons++]);
    return numberActiveButtons;
}

uint8_t TFT_DISPLAY_button_handler() {
    static uint8_t i = 0;
    uint8_t returnVal = 0xFF;
    TFT_TOUCH_touchData_S currentData = TFT_TOUCH_get_data();
    if (currentData.status == TOUCHED) {
        for (i = 0; i < numberActiveButtons; i++) {
            if (activeButtons[i].status == ACTIVE) {
                if (checkRange(currentData.xPos, currentData.yPos,
                        activeButtons[i]._w2, activeButtons[i]._w1,
                        activeButtons[i]._h2, activeButtons[i]._h1)) {
                    activeButtons[i].status = PRESSED;
                    animatePress(activeButtons[i]);
                    break;
                }
            }
        }
    } else {
        for (i = 0; i < numberActiveButtons; i++) {
            if (activeButtons[i].status == PRESSED) {
                returnVal = i;
                activeButtons[i].status = ACTIVE;
                animatePress(activeButtons[i]);
            }
        }
    }

    return returnVal;
}

void TFT_DISPLAY_destroy_buttons() {
    numberActiveButtons = 0;
}

uint8_t checkRange(uint16_t x, uint16_t y, uint16_t target_x1, uint16_t target_x2, uint16_t target_y1, uint16_t target_y2) {
    if (y < (target_x1) && y > target_x2) {
        if (x < (target_y1) && x > target_y2) {
            return 1;
        }
    }
    return 0;
}

void TFT_DISPLAY_draw(uint16_t color) {
    TFT_TOUCH_touchData_S thisScreenData = TFT_TOUCH_get_data();
    TFT_LCD_drawRect(thisScreenData.yPos, thisScreenData.xPos, thisScreenData.yPos + 3, thisScreenData.xPos + 3, color);
}

void animatePress(tftButtons thisButton) {
    uint16_t color = thisButton.color;
    if (thisButton.status == PRESSED) {
        color = TFT_LCD_MAGENTA;
    }
    uint16_t len = strlen(thisButton.stringVal);
    TFT_LCD_drawRect(thisButton._w1, thisButton._h1, thisButton._w2,
            thisButton._h2, color);
    len = thisButton._w1 + ((thisButton._w2 - thisButton._w1) / 2) -
            len * ASCII_FONT_WIDTH * thisButton.font / 2;
    TFT_LCD_writeString(thisButton.stringVal, len, thisButton._h1 +
            ((thisButton._h2 - thisButton._h1) / 2) - thisButton.font * 7 / 2,
            color, TFT_LCD_BLACK, thisButton.font);
}
