#include "../includes/tetris.h"

void inclusion_of_colors(void) {
  start_color();
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_BLUE, COLOR_BLACK);
  init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(6, COLOR_RED, COLOR_BLACK);
  init_pair(7, COLOR_GREEN, COLOR_BLACK);
  init_pair(8, COLOR_BLACK, COLOR_GREEN);
  init_pair(9, COLOR_BLACK, COLOR_RED);
  init_pair(10, COLOR_CYAN, COLOR_BLACK);
  init_pair(11, COLOR_RED, COLOR_CYAN);
  init_pair(12, COLOR_RED, COLOR_BLACK);
}

void draw_lv(void) {
  GameInfo_t *game_info = initGameInfo();
  int lv = 1;
  if (game_info->score > 5399) {
    lv = 10;
  } else if (game_info->score > 4799) {
    lv = 9;
  } else if (game_info->score > 4199) {
    lv = 8;
  } else if (game_info->score > 3599) {
    lv = 7;
  } else if (game_info->score > 2999) {
    lv = 6;
  } else if (game_info->score > 2399) {
    lv = 5;
  } else if (game_info->score > 1799) {
    lv = 4;
  } else if (game_info->score > 1199) {
    lv = 3;
  } else if (game_info->score > 599) {
    lv = 2;
  }
  mvaddch(FIELD_HEIGHT - 4, FIELD_WIDTH * 2 + 2, ' ');
  attron(COLOR_PAIR(11));
  printw("lv: %d", lv);
}

void draw_start_screen_Tetris(void) {
  draw_phone();
  draw_field();
  draw_next_figure();
  draw_score();
  draw_lv();
  draw_highscore();
  attron(COLOR_PAIR(12));
  mvaddch(7, 8, ' ');
  printw("Tetris ");
  mvaddch(12, 6, 'P');
  printw("ress  Enter");
  mvaddch(13, 6, ' ');
  printw("to play!!! ");
}

void draw_button_Tetris(int color) {
  attron(COLOR_PAIR(color));
  mvaddch(2, 10, ' ');
  printw("               ");
  mvaddch(3, 10, ' ');
  printw("   Tetris      ");
  mvaddch(4, 10, ' ');
  printw("               ");
}

void draw_button_Empty(int color) {
  attron(COLOR_PAIR(color));
  mvaddch(8, 10, ' ');
  printw("               ");
  mvaddch(9, 10, ' ');
  printw("   Empty       ");
  mvaddch(10, 10, ' ');
  printw("               ");
}

void draw_button_Exit(int color) {
  attron(COLOR_PAIR(color));
  mvaddch(14, 10, ' ');
  printw("               ");
  mvaddch(15, 10, ' ');
  printw("   Exit        ");
  mvaddch(16, 10, ' ');
  printw("               ");
}

void draw_pause(void) {
  mvaddch(12, 27, ' ');
  attron(COLOR_PAIR(11));
  printw("Pause");
}

void draw_phone(void) {
  attron(COLOR_PAIR(10));
  for (int i = 0; i < FIELD_HEIGHT + 2; i++) {
    for (int j = 0; j < FIELD_WIDTH * 2 + 20; j++)
      mvaddch(i, j, ' ' | A_REVERSE);
  }
}

void draw_field(void) {
  GameInfo_t *game_info = initGameInfo();

  int k = 8, color = 0;
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if (game_info->field[i][j] == 0) {
        color = k;
      } else {
        color = game_info->field[i][j];
      }
      attron(COLOR_PAIR(color));
      mvaddch(i + 1, j * 2 + 2, ' ' | A_REVERSE);
      mvaddch(i + 1, j * 2 + 3, ' ' | A_REVERSE);
    }
  }
}

void draw_highscore(void) {
  GameInfo_t *game_info = initGameInfo();
  attron(COLOR_PAIR(10));
  mvaddch(FIELD_HEIGHT - 2, FIELD_WIDTH * 2 + 2, ' ' | A_REVERSE);
  attron(COLOR_PAIR(11));
  printw("record: %d", game_info->high_score);
}

void draw_next_figure(void) {
  FIGURS
  int **matrix = createMatrix(5, 5);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 5; j++) {
      matrix[i][j] = 8;
    }
  }
  FigureInfo_t *figure_info = initTetris();
  matrix[2 + figurs[figure_info->type_next][0][0]]
        [2 + figurs[figure_info->type_next][0][1]] = figure_info->type_next + 1;
  matrix[2 + figurs[figure_info->type_next][1][0]]
        [2 + figurs[figure_info->type_next][1][1]] = figure_info->type_next + 1;
  matrix[2 + figurs[figure_info->type_next][2][0]]
        [2 + figurs[figure_info->type_next][2][1]] = figure_info->type_next + 1;
  matrix[2 + figurs[figure_info->type_next][3][0]]
        [2 + figurs[figure_info->type_next][3][1]] = figure_info->type_next + 1;

  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 5; j++)
      if (matrix[i][j] != 0) {
        int color = matrix[i][j];
        attron(COLOR_PAIR(color));
        mvaddch(i + 3, j * 2 + FIELD_WIDTH * 2 + 5, ' ' | A_REVERSE);
        mvaddch(i + 3, j * 2 + FIELD_WIDTH * 2 + 6, ' ' | A_REVERSE);
      }
  free(matrix);

  attron(COLOR_PAIR(11));
  mvaddch(2, FIELD_WIDTH * 2 + 7, ' ');
  printw("next");
}

void draw_score(void) {
  GameInfo_t *game_info = initGameInfo();
  attron(COLOR_PAIR(10));
  mvaddch(FIELD_HEIGHT, FIELD_WIDTH * 2 + 2, ' ' | A_REVERSE);
  attron(COLOR_PAIR(11));
  printw("score: %d", game_info->score);
}
