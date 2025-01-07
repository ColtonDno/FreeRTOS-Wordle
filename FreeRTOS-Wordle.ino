#include "variables.h"
#include "screen_tasks.h"
#include "button_tasks.h"

/* To-Do
 *---------------------
 * Keyboard
 * * Letter history
 * * Animation
 * Word list
 * Check for real word
 * Home page UI
 * Get today's word from web 
 * Allow user to input word
 * Tutorial?
 */

void setup() 
{
  Terminal.begin(115200);

  display.begin();
  resetScreen();
  delay(1000);

  digitalWrite(LED_BUILTIN, LOW);

  menu_semaphore = xSemaphoreCreateMutex();
  page_semaphore = xSemaphoreCreateMutex();

  srand(analogRead(A0));
  uint8_t index = rand() % 50;
  actual_word = words[index];
  
  for (int i = 0; i < 5; i++)
    if (actual_word[i] > 96)
      actual_word[i] = actual_word[i] - 97 + 65;

  for (int i = 0; i < 6; i++) 
  {
    rows[i].setPosition(i);
    rows[i].setDisplay(display);
    rows[i].drawRow();
  }

  // xTaskCreate(handleAlarm,   "Task F", 256, NULL, 5, &Handle_alarmTask);

  xTaskCreate(updateButtons, "Task A", 256, NULL, 4, &Handle_buttonTask);
  xTaskCreate(handleInput,   "Task B", 256, NULL, 3, &Handle_menuTask);
  xTaskCreate(drawScreen,    "Task C", 256, NULL, 2, &Handle_screenTask);

  vTaskStartScheduler();
  while(1);
}

void loop() {}
