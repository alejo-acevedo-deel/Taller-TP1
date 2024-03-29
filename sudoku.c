#include "sudoku.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "board.h"

int sudoku_init(sudoku_t *self) {
  if (board_init(&(self->board))) return 1;
  self->plays = malloc(sizeof(list_t));
  list_init(self->plays);
  return 0;
}

static void _generate_row(sudoku_t *self, char *row_representation, int row) {
  int offset = 0;
  for (int column = 0; column < 9; column = column + 3) {
    char valor0 = board_get(&(self->board), row, column);
    char valor1 = board_get(&(self->board), row, column + 1);
    char valor2 = board_get(&(self->board), row, column + 2);
    snprintf(row_representation + offset,
      DIVISOR_LEN - offset,
      ROW_REPRESENTATION, valor0,
      valor1, valor2);
    offset += (ROW_REPRESENTATION_LEN - 1);
  }
  snprintf(row_representation+ strlen(row_representation),
    DIVISOR_LEN - strlen(row_representation),
    "%s", "U\n");
}

char *sudoku_show(sudoku_t *self, char *boardRepresentation) {
  int offset = 0;
  for (int row = 0; row < 9; row++) {
    char line_representation[39] = "\0";
    if (row % 3 == 0) {
      snprintf(boardRepresentation + offset,
        BOARD_REPRESENTATION_LEN - offset, "%s",
        SECTOR_DIVISOR);
      offset += (DIVISOR_LEN - 1);
    } else {
      snprintf(boardRepresentation + offset,
        BOARD_REPRESENTATION_LEN - offset,
        "%s", ROW_DIVISOR);
      offset += (DIVISOR_LEN - 1);
    }
    _generate_row(self, line_representation, row);
    snprintf(boardRepresentation + offset,
      BOARD_REPRESENTATION_LEN - offset,
      "%s", line_representation);
    offset += (DIVISOR_LEN - 1);
  }
  snprintf(boardRepresentation + offset,
    BOARD_REPRESENTATION_LEN - offset,
    "%s", SECTOR_DIVISOR);
  return boardRepresentation;
}


int sudoku_set(sudoku_t *self, char row, char column, char value) {
  int row_int = (int) (row) - '0' - 1;
  int column_int = (int) (column) - '0' - 1;
  if (!board_is_ini(&(self->board), row_int, column_int)) {
    board_set(&(self->board), row_int, column_int, value);
    play_t *play = malloc(sizeof(play_t));
    play->column = column_int;
    play->row = row_int;
    list_append(self->plays, play);
    return 0;
  }
  return 1;
}

static int _validate_row(sudoku_t *self, int column, char value) {
  int apariciones = 0;
  for (int row = 0; row < BOARD_SIZE; row++) {
    if (board_get(&(self->board), row, column) == value) {
      apariciones++;
      if (apariciones == 2) return 0;
    }
  }
  return 1;
}

static int _validate_column(sudoku_t *self, int row, char value) {
  int apariciones = 0;
  for (int column = 0; column < BOARD_SIZE; column++) {
    if (board_get(&(self->board), row, column) == value) {
      apariciones++;
      if (apariciones == 2) return 0;
    }
  }
  return 1;
}

static int _validate_sector(sudoku_t *self,
                            int row_ini,
                            int column_ini,
                            char value) {
  int row_offset = row_ini / 3;
  row_offset = row_offset * 3;
  int column_offset = column_ini / 3;
  column_offset = column_offset * 3;
  int apariciones = 0;
  for (int row = 0; row < 3; row++) {
    for (int column = 0; column < 3; column++) {
      if (board_get(&(self->board),
          row + row_offset, column + column_offset) == value) {
        apariciones++;
        if (apariciones == 2) return 0;
      }
    }
  }
  return 1;
}

int sudoku_validate(sudoku_t *self) {
  list_iter_reset(self->plays);
  play_t *play = list_iter_next(self->plays);
  for (; play != NULL; play = list_iter_next(self->plays)) {
    char value = board_get(&(self->board), play->row, play->column);
    int column = _validate_column(self, play->row, value);
    int row = _validate_row(self, play->column, value);
    int sector = _validate_sector(self, play->row, play->column, value);
    if (!row || !column || !sector) {
      return 0;
    }
  }
  return 1;
}

static int _reset_plays(sudoku_t *self) {
  list_iter_reset(self->plays);
  play_t *play = list_iter_next(self->plays);
  for (; play != NULL; play = list_iter_next(self->plays)) {
    free(play);
  }
  list_uninit(self->plays);
  list_init(self->plays);
  return 0;
}

int sudoku_reset(sudoku_t *self) {
  board_reset(&(self->board));
  _reset_plays(self);
  return 0;
}

void sudoku_uninit(sudoku_t *self) {
  _reset_plays(self);
  free(self->plays);
}
