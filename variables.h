#include "variant.h"
#ifndef _VARIABLES_H_
#define _VARIABLES_H_

#include <Seeed_Arduino_FreeRTOS.h>
#include "TFT_eSPI.h"
#include "button.h"
#include "wordle_row.h"
#include "words.h"

#define Terminal Serial
#define BUZZERPIN 12

TaskHandle_t Handle_screenTask;
TaskHandle_t Handle_buttonTask;
TaskHandle_t Handle_menuTask;

TaskHandle_t Handle_alarmTask;

SemaphoreHandle_t menu_semaphore = NULL;
uint8_t menu_option;
#define MENU_OPTION_COUNT 3

SemaphoreHandle_t page_semaphore = NULL;
uint8_t previous_page, current_page;

Button* up_button     = new Button(WIO_5S_UP);
Button* down_button   = new Button(WIO_5S_DOWN);
Button* left_button   = new Button(WIO_5S_LEFT);
Button* right_button  = new Button(WIO_5S_RIGHT);
Button* middle_button = new Button(WIO_5S_PRESS);
Button* submit_button = new Button(WIO_KEY_A);
Button* buttons[] = {up_button, down_button, left_button, right_button, middle_button, submit_button};

TFT_eSPI display;

typedef void (*PageFunction)();

char* actual_word;
// uint8_t actual_words[2][5] = {'L','E','M','U','R','T','E','S','T','S'};
char letter_input = 'A';

WordleRow rows[6];

uint8_t current_row;
uint8_t current_index;
uint8_t end_game = 0;

#endif