#ifndef _BUTTON_TASKS_H_
#define _BUTTON_TASKS_H_

#include "button.h"

void chooseWord()
{
  uint16_t index = micros() % 2315;
  Serial.printf("Seed: %d\n",index);
  // index = 3;
  actual_word = possible_words[index];
}

void checkForWin()
{
  int8_t game_result = rows[current_row].checkGuess(actual_word, keyboard, word_list);

  if (game_result == -1)
    return;
  else if (game_result)
    while(1);
  else if (current_row == 5)
  {
    resetScreen();
    for (int i = 0; i < 5; i++)
      display.print(actual_word[i]);
    while(1);
  }
  else
    current_index = 0;
    
  current_row++;
}

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

      if (curr_key->getUp() != nullptr)
        curr_key = curr_key->getUp();
    }

    if (down_button->getState() == PRESSED && millis() - hold_timer > 200)
    {
      hold_timer = millis();

      if (letter_input == 90)
        letter_input = 65;
      else
        letter_input++;

      if (curr_key->getDown() != nullptr)
        curr_key = curr_key->getDown();
    }

    if (left_button->getState() == PRESSED && millis() - hold_timer > 200)
    {
      hold_timer = millis();

      if (current_page == 1 && current_index != 0)
        current_index--;
      else if (current_page == 2)
        curr_key = curr_key->getPrev();
    }

    if (right_button->getState() == PRESSED && millis() - hold_timer > 200)
    {
      hold_timer = millis();

      if (current_page == 1 && current_index < 4)
        current_index++;
      else if (current_page == 2)
        curr_key = curr_key->getNext();
    }

    if (middle_button->getPress() == PRESSED)
    {
      if (game_state == 0)
      {
        chooseWord();
        game_state = 1;
      }

      if (current_page == 1)
      {
        resetScreen();
        drawKeyboard();
        current_page = 2;
      }
      else if (current_page == 2)
      {
        rows[current_row].setLetter(curr_key->getLetter(), current_index);

        resetScreen();
        drawWordle();

        current_page = 1;
        
        if (current_index < 4)
          current_index++;
      }
    }

    if (submit_button->getPress() == PRESSED)
    {
      if (game_state == 1 && current_page == 1)
      {
        Serial.println("Checking for win");
        checkForWin();
      }
    }
    
    if (back_button->getPress() == PRESSED)
    {
      if (current_page == 2)
      {
        resetScreen();
        drawWordle();
        current_page = 1;
      }
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
    for (int i = 0; i < 7; i++)
      buttons[i]->updateButton();
      
    vTaskDelayUntil(&prev_wake_time, 25);
  }

  vTaskDelete(NULL);
}

#endif