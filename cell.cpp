#include "cell.h"

Cell::Cell(){}

void Cell::setPosition(uint8_t row, uint8_t column)
{
  this->x = (column * CELL_SIZE) + CELL_SPACING + X_CORRECTION;
  this->y = (row * CELL_SIZE) + CELL_SPACING;
}

char Cell::getLetter()
{
  return letter;
}

void Cell::setLetter(char letter)
{
  this->letter = letter;
}

//. Check for multiple letters
void Cell::setResult(GuessType state) 
{
  guess_state = state;

  switch (guess_state)
  {
    case(INCORRECT):
      fill_color = W_GRAY;
      break;
    case(WRONG_SPOT):
      fill_color = W_YELLOW;
      break;
    case(CORRECT):
      fill_color = W_GREEN;
      break;
    default:
      fill_color = W_BLACK;
      break;
  };
}

void Cell::drawCell(TFT_eSPI display, uint8_t selected)
{
  display.fillRect(x, y, BOX_SIZE, BOX_SIZE, fill_color);

  if (guess_state == NO_GUESS && !selected)
    display.drawRect(x, y, BOX_SIZE, BOX_SIZE, TFT_WHITE);

  if (letter != 0)
  {
    display.setTextColor(0xFFFF, fill_color);
    display.drawChar(letter, x + LETTER_X_OFFSET, y + LETTER_Y_OFFSET);
    display.setTextColor(0xFFFF, 0x0000);
  }
}