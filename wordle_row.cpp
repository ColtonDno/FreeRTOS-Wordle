#include "wordle_row.h"

WordleRow::WordleRow() {}

void WordleRow::getGuess()
{
  for (int i = 0; i < 5; i++)
    guess[i] = cells[i].getLetter();
}

void WordleRow::setDisplay(TFT_eSPI display)
{
  this->display = display;
}

void WordleRow::setPosition(uint8_t row_number)
{
  this->row_number = row_number;
  for (int i = 0; i < 5; i++)
    cells[i].setPosition(row_number, i);
}

uint8_t WordleRow::checkGuess(const char* actual_word)
{
  uint8_t total_correct = 0;
  getGuess();

  for (int i = 0; i < 5; i++)
  {
    if (guess[i] == actual_word[i])
    {
      cells[i].setResult(CORRECT);
      letter_count[i] = 0;
      total_correct++;
    }
    else
      cells[i].setResult(INCORRECT);
  }

  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 5; j++)
      if (guess[i] == actual_word[j]
           && letter_count[i] == 1 && letter_count[j] == 1)
      {
        Serial.printf("Found: %i, %i\n", i, j);
        cells[i].setResult(WRONG_SPOT);
        letter_count[j] = 0;
        j = 5;
      }

  drawRow();
  return (total_correct == 5);
}

void WordleRow::drawRow()
{
  for (int i = 0; i < 5; i++)
    cells[i].drawCell(display);
}

void WordleRow::blinkSelectedCell(uint8_t index)
{
  static uint8_t prev_index;
  static uint8_t draw_timer;
  static uint8_t draw_state;
  static uint8_t blink_state;

  if (prev_index != index)
  {
    cells[prev_index].drawCell(display);
    cells[index].drawCell(display, 1);
    prev_index = index;
  }

  if ((millis() / 500) % 2 == draw_state)
    return;

  draw_state = (millis() / 500) % 2;
  blink_state = !blink_state;
  cells[index].drawCell(display, blink_state);
}

void WordleRow::setLetter(char letter, uint8_t cell_index)
{
  cells[cell_index].setLetter(letter);
  cells[cell_index].drawCell(display);
}