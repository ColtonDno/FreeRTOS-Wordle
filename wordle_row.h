#ifndef _WORDLE_ROW_
#define _WORDLE_ROW_

#include "cell.h"

class WordleRow
{
  private:
    Cell cells[5];
    uint8_t row_number;
    char actual_word[6];
    uint8_t letter_count[5] = {1,1,1,1,1};
    char guess[6];
    TFT_eSPI display;

    void getGuess();
  
  public:
    WordleRow();
    void setDisplay(TFT_eSPI display);
    void setPosition(uint8_t row_number);
    uint8_t checkGuess(const char* actual_word);
    void drawRow();
    void blinkSelectedCell(uint8_t index);
    void setLetter(char letter, uint8_t cell_index);
};

#endif