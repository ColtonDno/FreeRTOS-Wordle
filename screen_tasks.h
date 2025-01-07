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

void setWord()
{
  static uint8_t jkl = 0;

  if (jkl) return;

  uint8_t index = rand() % 50;
  actual_word = words[index];

  jkl = 1;
  
  for (int i = 0; i < 5; i++)
    if (actual_word[i] > 96)
      actual_word[i] = actual_word[i] - 97 + 65;

  for (int i = 0; i < 5; i++) 
    Serial.print(actual_word[i]);
  Serial.println(".");
}

void drawWordle()
{
  static uint8_t prev_index;

  if (middle_button->getPress() == PRESSED)
  {
    // setWord();
    rows[current_row].setLetter(letter_input, current_index);
    
    if (current_index < 4)
      current_index++;
  }

  rows[current_row].blinkSelectedCell(current_index);
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

static void drawScreen(void* pvParameters)
{
  static TickType_t prev_wake_time = 0;
  // PageFunction pages[] = {homePage, temperaturePage, humidityPage, carbonMonoxidePage};

  while (1)
  {
    vTaskDelayUntil(&prev_wake_time, 50);

    if (!xSemaphoreTake(page_semaphore, portMAX_DELAY))
      continue;

    drawWordle();

    drawLetterInput();

    // pages[current_page]();
    // previous_page = current_page;

    xSemaphoreGive( page_semaphore );
  }

  vTaskDelete(NULL);
}

#endif