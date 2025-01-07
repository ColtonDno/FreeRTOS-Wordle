#include "delay.h"
#ifndef _BUTTON_TASKS_H_
#define _BUTTON_TASKS_H_

#include "button.h"

// static void updateMenu()
// {
//   if (!xSemaphoreTake(menu_semaphore, portMAX_DELAY))
//     return;

  

//   xSemaphoreGive( menu_semaphore );
// }

static void handleInput(void* pvParameters)
{
  static TickType_t prev_wake_time = 0;

  static uint32_t hold_timer;
  uint8_t hold_state;

  while (1)
  {
    vTaskDelayUntil(&prev_wake_time, 25);

    if (!xSemaphoreTake(page_semaphore, portMAX_DELAY))
      continue;

    if (up_button->getState() == PRESSED && millis() - hold_timer > 200)
    {
      hold_timer = millis();

      if (letter_input == 65)
        letter_input = 90;
      else
        letter_input--;
    }

    if (down_button->getState() == PRESSED && millis() - hold_timer > 200)
    {
      hold_timer = millis();

      if (letter_input == 90)
        letter_input = 65;
      else
        letter_input++;
    }

    if (left_button->getPress() == PRESSED)
    {
      if (current_index != 0)
        current_index--;
    }

    if (right_button->getPress() == PRESSED)
    {
      if (current_index < 4)
        current_index++;
    }

    if (submit_button->getPress() == PRESSED)
    {
      uint8_t game_result = rows[current_row].checkGuess(actual_word); 
      if (game_result)
        while(1);
      else if (current_row == 5)
      {
        resetScreen();
        display.print(actual_word);
      }
      else
        current_index = 0;
        
      current_row++;
    }

    xSemaphoreGive( page_semaphore );
  }

  vTaskDelete(NULL);
}

static void updateButtons(void* pvParameters)
{
  static TickType_t prev_wake_time = 0;
  while (1)
  {
    for (int i = 0; i < 6; i++)
      buttons[i]->updateButton();
      
    vTaskDelayUntil(&prev_wake_time, 25);
  }

  vTaskDelete(NULL);
}

#endif