#include "DISPLAY_INTERFACE.h"

typedef enum _tftDisplayButtonStatus {
    NOT_PRESSED,
    PRESSED
} tftDisplayButtonStatus;

typedef enum _tftDisplayButtonCaptureState {
    CLEARED,
    SELECTED,
    RELEASED
} tftDisplayButtonCaptureState;

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
static uint8_t enableDrawing = 0;
static uint16_t drawingColor = 0xFFFF;
static tftDisplayButtonCaptureState buttonCaptureState = CLEARED;

uint8_t checkRange(TOUCH_SCREEN_touchData_S currentData, tftButtons thisButton);
void animatePress(tftButtons thisButton);

uint8_t DISPLAY_INTERFACE_place_button(const char* string, uint8_t x, uint8_t y, uint16_t color, uint8_t size) {
    activeButtons[numberActiveButtons]._w1 = (TFT_LCD_width() / 4)*(x - 1) + 10;
    activeButtons[numberActiveButtons]._h1 = (TFT_LCD_height() / 4)*(y - 1) + 10;
    activeButtons[numberActiveButtons]._w2 = (TFT_LCD_width() / 4) * x - 10;
    activeButtons[numberActiveButtons]._h2 = (TFT_LCD_height() / 4) * y - 10;
    activeButtons[numberActiveButtons].buttonHandle = numberActiveButtons;
    activeButtons[numberActiveButtons].status = NOT_PRESSED;
    activeButtons[numberActiveButtons].stringVal = string;
    activeButtons[numberActiveButtons].font = size;
    activeButtons[numberActiveButtons].color = color;
    animatePress(activeButtons[numberActiveButtons++]);
    return numberActiveButtons;
}

uint8_t DISPLAY_INTERFACE_button_handler() {
    static uint8_t i = 0;
    static uint8_t active_button = 0;
    uint8_t returnVal = 0xFF;
    TOUCH_SCREEN_touchData_S currentData = TOUCH_SCREEN_get_data();
    switch(buttonCaptureState){
        case CLEARED:
            if (currentData.status == TOUCHED) {
                for (i = 0; i < numberActiveButtons; i++) {
                    if (checkRange(currentData, activeButtons[i])) {
                        active_button = i;
                        buttonCaptureState = SELECTED;
                        activeButtons[active_button].status = PRESSED;
                        animatePress(activeButtons[active_button]);
                        break;
                    }
                }
            }
            break;
        case SELECTED:
            if (currentData.status == TOUCHED) {
                if (!checkRange(currentData, activeButtons[active_button])) {
                    activeButtons[active_button].status = NOT_PRESSED;
                    animatePress(activeButtons[active_button]);
                    buttonCaptureState = CLEARED;
                }
            } else {
                if (activeButtons[active_button].status == PRESSED){
                    activeButtons[active_button].status = NOT_PRESSED;
                    animatePress(activeButtons[active_button]);
                    buttonCaptureState = RELEASED;
                }
            }
            break;
        case RELEASED:
            buttonCaptureState = CLEARED;
            returnVal = active_button;
            //put queue of button presses here....
            break;
        default:
            break;
    }

    return returnVal;
}

void DISPLAY_INTERFACE_destroy_buttons() {
    numberActiveButtons = 0;
}

void DISPLAY_INTERFACE_enable_drawing(uint8_t enable, uint16_t color) {
    enableDrawing = enable;
    drawingColor = color;
}

void DISPLAY_INTERFACE_draw(void) {
    if (enableDrawing){
        TOUCH_SCREEN_touchData_S thisScreenData = TOUCH_SCREEN_get_data();
        if (thisScreenData.status == TOUCHED) {
            TFT_LCD_drawRect(thisScreenData.yPos, thisScreenData.xPos, thisScreenData.yPos + 3, thisScreenData.xPos + 3, drawingColor);
        }
    }
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

uint8_t checkRange(TOUCH_SCREEN_touchData_S currentData, tftButtons thisButton){
    if (currentData.yPos > thisButton._w1 && currentData.yPos < thisButton._w2) {
        if (currentData.xPos > thisButton._h1 && currentData.xPos < thisButton._h2){
            return 1;
        }
    }
    return 0;
}