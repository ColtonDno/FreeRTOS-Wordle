#ifndef _KEY_H_
#define _KEY_H_

#include "cell.h"
#include "TFT_eSPI.h"

#define KEY_SIZE 30
#define ROW_COUNT 6
#define BOX_SIZE 28
#define KEY_SPACING 5
#define X_CORRECTION 11
#define Y_CORRECTION 120
#define LETTER_X_OFFSET 7
#define LETTER_Y_OFFSET 3
#define X_OFFSET 10
#define Y_OFFSET 35

#define W_BLACK  0x0000
#define W_YELLOW 0xce8b
#define W_GREEN  0x6d4c
#define W_GRAY   0x7bef

class Key
{
  private:
    uint16_t letter;
    uint16_t x;
    uint16_t y;
    uint8_t y_offset;
    uint8_t row;
    uint8_t column;
    uint8_t guess_state = NO_GUESS;
    uint16_t fill_color = 0x0000;
    Key* up_key;
    Key* down_key;
    Key* next_key;
    Key* prev_key;

  //. Arrange functions
  public:
    Key();
    void setPosition(uint16_t row, uint16_t column);

    void setLetter(uint16_t letter);
    uint16_t getLetter();
    void setResult(GuessType state);
    void drawKey(TFT_eSPI display, uint8_t selected = 0);
    void setNext(Key* next_key);
    Key* getNext();
    void setPrev(Key* prev_key);
    Key* getPrev();
    void setUp(Key* up_key);
    Key* getUp();
    void setDown(Key* down_key);
    Key* getDown();
};

#endif