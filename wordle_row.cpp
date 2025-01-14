#include "wordle_row.h"
// #include "word_list.h"
// char** word_list;

uint8_t alpha_to_keyboard[] = { 10, 23, 21, 12, 2, 13, 14, 15, 7, 16, 17, 18, 25, 24, 8, 9, 0, 3, 11, 4, 6, 22, 1, 20, 5, 19 };
// uint16_t letter_indexes[] = {0, 737, 909, 922, 685, 303, 598, 638, 489, 165, 202, 376, 577, 693, 325, 262, 859, 78, 628, 1565, 815, 189, 242, 413, 16, 181, 105};
uint32_t letter_indexes[] = { 0, 737, 1646, 2568, 3253, 3556, 4154, 4792, 5281, 5446, 5648, 6024, 6601, 7294, 7619, 7881, 8740, 8818, 9446, 11011, 11826, 12015, 12257, 12670, 12686, 12867, 12972 };

WordleRow::WordleRow() {}

void WordleRow::getGuess() {
  for (int i = 0; i < 5; i++)
    guess[i] = cells[i].getLetter();
}

void WordleRow::setDisplay(TFT_eSPI display) {
  this->display = display;
}

void WordleRow::setPosition(uint8_t row_number) {
  this->row_number = row_number;
  for (int i = 0; i < 5; i++)
    cells[i].setPosition(row_number, i);
}

uint8_t WordleRow::validateWord(char word_list[12972][5]) {
  uint8_t first_letter = guess[0] - 65 + 1;
  uint32_t start = letter_indexes[first_letter - 1];
  uint32_t stop = letter_indexes[first_letter];

  // Serial.printf("a: %d, o: %d\n", start, stop);

  for (uint32_t i = start; i < stop; i++) {
    uint8_t succeed = 1;

    for (int j = 1; j < 5; j++)
      if (guess[j] != word_list[i][j]) {
        succeed = 0;
        break;
      }

    if (succeed)
      return succeed;
  }

  return false;
}

uint8_t WordleRow::checkGuess(const char* actual_word, Key* keyboard, char word_list[12972][5]) {
  uint8_t total_correct = 0;
  getGuess();

  if (!validateWord(word_list))
    return -1;

  for (int i = 0; i < 5; i++) {
    if (guess[i] == actual_word[i]) {
      // Serial.printf("Correct: %d: %c == %c\n", i, guess[i], actual_word[i]);
      cells[i].setResult(CORRECT);
      keyboard[alpha_to_keyboard[guess[i] - 65]].setResult(CORRECT);
      letter_count[i] = -1;
      total_correct++;
    } else {
      // Serial.printf("%d: %c != %c\n", i, guess[i], actual_word[i]);

      cells[i].setResult(INCORRECT);
      keyboard[alpha_to_keyboard[guess[i] - 65]].setResult(INCORRECT);
    }
  }

  for (int i = 0; i < 5; i++)
    for (int j = 0; j < 5; j++)
      if (guess[i] == actual_word[j]
          && letter_count[i] != -1 && letter_count[j] > 0) {
        cells[i].setResult(WRONG_SPOT);
        keyboard[alpha_to_keyboard[guess[i] - 65]].setResult(WRONG_SPOT);
        letter_count[j] -= 1;
        j = 5;
      }

  drawRow();
  return (total_correct == 5);
}

void WordleRow::drawRow() {
  for (int i = 0; i < 5; i++)
    cells[i].drawCell(display);
}

void WordleRow::blinkSelectedCell(uint8_t index) {
  static uint8_t prev_index;
  static uint8_t draw_timer;
  static uint8_t draw_state;
  static uint8_t blink_state;

  if (prev_index != index) {
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

void WordleRow::setLetter(char letter, uint8_t cell_index) {
  cells[cell_index].setLetter(letter);
  cells[cell_index].drawCell(display);
}