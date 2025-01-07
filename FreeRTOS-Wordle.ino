#include "variables.h"
#include "screen_tasks.h"
#include "button_tasks.h"

/* To-Do
 *---------------------
 * Keyboard
 * * Animation
 * Home page UI
 * Get today's word from web 
 * Allow user to input word
 * Tutorial
 */

inline void initializeKeyboard()
{
  for (int i = 0; i < 26; i++)
    keyboard_layout[i] = keyboard_layout[i] - 97 + 65;

  keyboard[0].setPrev(&keyboard[25]);
  keyboard[25].setNext(&keyboard[0]);

  keyboard[9].setDown(&keyboard[18]);
  keyboard[14].setDown(&keyboard[22]);

  for (int i = 0; i < 25; i++)
  {
    keyboard[i].setNext(&keyboard[i + 1]);
    keyboard[25 - i].setPrev(&keyboard[25 - (i + 1)]);
  }
  
  for (int x = 0; x < 10; x++)
  {
    keyboard[x].setPosition(0, x);
    keyboard[x].setLetter(keyboard_layout[x]);
  }

  for (int x = 0; x < 9; x++)
  {
    keyboard[x].setDown(&keyboard[x + 10]);
    keyboard[x + 10].setUp(&keyboard[x]);

    keyboard[x + 10].setPosition(1, x);
    keyboard[x + 10].setLetter(keyboard_layout[x + 10]);
  }

  for (int x = 0; x < 7; x++)
  { 
    keyboard[x + 19].setPosition(2, x);
    keyboard[x + 19].setLetter(keyboard_layout[x + 19]);
  }

  for (int x = 0; x < 4; x++)
  {
    keyboard[x + 10].setDown(&keyboard[x + 19]);
    keyboard[x + 19].setUp(&keyboard[x + 10]);
  }

  //. Redo or push bottom row over
  for (int x = 0; x < 3; x++)
  {
    keyboard[x + 15].setDown(&keyboard[x + 23]);
    keyboard[x + 23].setUp(&keyboard[x + 15]);
  }
}

void setup() 
{
  Terminal.begin(115200);

  initializeKeyboard();

  display.begin();
  resetScreen();
  delay(3000);

  digitalWrite(LED_BUILTIN, LOW);

  menu_semaphore = xSemaphoreCreateMutex();
  page_semaphore = xSemaphoreCreateMutex();

  for (int i = 0; i < 6; i++) 
  {
    rows[i].setPosition(i);
    rows[i].setDisplay(display);
    rows[i].drawRow();
  }

  xTaskCreate(updateButtons, "Task A", 256, NULL, 4, &Handle_buttonTask);
  xTaskCreate(handleInput,   "Task B", 256, NULL, 3, &Handle_menuTask);
  xTaskCreate(drawScreen,    "Task C", 256, NULL, 2, &Handle_screenTask);

  vTaskStartScheduler();
  while(1);
}

void loop() {}
