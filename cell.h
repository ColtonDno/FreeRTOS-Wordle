#ifndef _CELL_H_
#define _CELL_H_

#include <Arduino.h>
#include "TFT_eSPI.h"

#define CELL_SIZE 40
#define ROW_COUNT 6
#define BOX_SIZE 30
#define X_CORRECTION 60
#define CELL_SPACING 5
#define LETTER_X_OFFSET 8
#define LETTER_Y_OFFSET 5

#define W_BLACK  0x0000
#define W_YELLOW 0xce8b
#define W_GREEN  0x6d4c
#define W_GRAY   0x7bef

enum GuessType
{
  NO_GUESS,
  INCORRECT,
  WRONG_SPOT,
  CORRECT
};

class Cell
{
  private:
    char letter;
    uint16_t x;
    uint16_t y;
    uint8_t column_num;
    uint8_t guess_state;
    uint16_t fill_color;

  public:
    Cell();
    void setPosition(uint8_t row, uint8_t column);

    void setLetter(char letter);
    char getLetter();
    void setResult(GuessType state);
    void drawCell(TFT_eSPI display, uint8_t selected = 0);
};

#endif