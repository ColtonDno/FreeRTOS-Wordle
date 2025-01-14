#include "key.h"

Key::Key()
{

}

void Key::setPosition(uint16_t row, uint16_t column)
{
  this->row = row;
  this->column = column;
  this->x = (column * KEY_SIZE) + X_CORRECTION + (row * X_OFFSET);
  this->y = Y_CORRECTION + KEY_SPACING + (row * Y_OFFSET);

  if (row == 2)
    this->x += 6;
}

uint16_t Key::getLetter()
{
  return this->letter;
}

void Key::setLetter(uint16_t letter)
{
  this->letter = letter;
}

//. Check for multiple letters
void Key::setResult(GuessType state) 
{
  if (state < guess_state)
    return;

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

void Key::drawKey(TFT_eSPI display, uint8_t selected)
{
  display.fillRect(x, y, BOX_SIZE, BOX_SIZE, fill_color);

  if (guess_state == NO_GUESS && !selected)
    display.drawRect(x, y, BOX_SIZE, BOX_SIZE, TFT_WHITE);

  if (guess_state != NO_GUESS && selected)
    display.drawRect(x, y, BOX_SIZE, BOX_SIZE, TFT_WHITE);

  if (letter != 0)
  {
    display.setTextColor(0xFFFF, fill_color);
    display.drawChar(letter, x + LETTER_X_OFFSET, y + LETTER_Y_OFFSET);
    display.setTextColor(0xFFFF, 0x0000);
  }
}


void Key::setNext(Key* next_key)
{
  this->next_key = next_key;
}

Key* Key::getNext()
{
  return next_key;
}

void Key::setPrev(Key* prev_key)
{
  this->prev_key = prev_key;
}

Key* Key::getPrev()
{
  return prev_key;
}

void Key::setUp(Key* up_key)
{
  this->up_key = up_key;
}

Key* Key::getUp()
{
  return up_key;
}

void Key::setDown(Key* down_key)
{
  this->down_key = down_key;
}

Key* Key::getDown()
{
  return down_key;
}