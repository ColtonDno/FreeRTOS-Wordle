#include <stdlib.h>
#ifndef _SCREEN_TASKS_H_
#define _SCREEN_TASKS_H_

#include "variables.h"

void resetScreen()
{
  display.setRotation(3);
  display.fillScreen(TFT_BLACK);
  display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.setTextSize(3);
}

void homePage()
{
  static uint8_t prev_option = 1;

  if (!xSemaphoreTake(menu_semaphore, portMAX_DELAY))
    return;
  
  if (prev_option == menu_option && previous_page == current_page)
  {
    xSemaphoreGive( menu_semaphore );
    return;
  }
  prev_option = menu_option;

  if (previous_page != 0)
    resetScreen();
  
  xSemaphoreGive( menu_semaphore );
}

void blinkWordle()
{
  rows[current_row].blinkSelectedCell(current_index);
}

void drawWordle()
{
  for (int i = 0; i < 6; i++)
    rows[i].drawRow();
}

void drawLetterInput()
{
  static char prev_letter;

  if (prev_letter == letter_input)
    return;

  display.setTextSize(4);

  display.drawChar(letter_input, 250, 90);
  prev_letter = letter_input;

  display.setTextSize(3);
}

void blinkKey()
{
  static Key* prev_key;
  static uint8_t draw_timer;
  static uint8_t draw_state;
  static uint8_t blink_state;

  if (prev_key != curr_key)
  {
    prev_key->drawKey(display);
    curr_key->drawKey(display, 1);
    prev_key = curr_key;
  }

  if ((millis() / 500) % 2 == draw_state)
    return;

  draw_state = (millis() / 500) % 2;
  blink_state = !blink_state;
  curr_key->drawKey(display, blink_state);
}

void drawKeyboard()
{
  for (int i = 0; i < 26; i++)
    keyboard[i].drawKey(display);
}

static void drawScreen(void* pvParameters)
{
  static TickType_t prev_wake_time = 0;
  // PageFunction pages[] = {homePage, temperaturePage, humidityPage, carbonMonoxidePage};

  while (1)
  {
    vTaskDelayUntil(&prev_wake_time, 50);

    if (!xSemaphoreTake(page_semaphore, portMAX_DELAY))
      continue;

    if (current_page == 1)
      blinkWordle();
    else
      blinkKey();
    // drawLetterInput();

    // pages[current_page]();
    previous_page = current_page;

    xSemaphoreGive( page_semaphore );
  }

  vTaskDelete(NULL);
}

#endif