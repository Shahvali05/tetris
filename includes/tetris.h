#ifndef TETRIS_H
#define TETRIS_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// min FIELD_WIDTH 7
// min FIELD_HEIGHT 7
#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

#define FIGURS                                                 \
  int figurs[7][4][2] = {{{-1, 0}, {-2, 0}, {0, 0}, {1, 0}},   \
                         {{0, -1}, {-1, -1}, {-1, 0}, {0, 0}}, \
                         {{-1, 0}, {-1, 1}, {0, 0}, {0, -1}},  \
                         {{0, 0}, {-1, 0}, {0, 1}, {-1, -1}},  \
                         {{0, 0}, {0, -1}, {0, 1}, {-1, -1}},  \
                         {{0, 0}, {0, -1}, {0, 1}, {1, -1}},   \
                         {{0, 0}, {0, -1}, {0, 1}, {-1, 0}}};

#define INITNCURSES()    \
  initscr();             \
  cbreak();              \
  noecho();              \
  keypad(stdscr, TRUE);  \
  nodelay(stdscr, TRUE); \
  curs_set(0);

#define SET_FIELD()                                   \
  for (int i = 0; i < FIELD_HEIGHT; i++) {            \
    for (int j = 0; j < FIELD_WIDTH; j++) {           \
      game_info->field[i][j] = game_info->next[i][j]; \
    }                                                 \
  }                                                   \
  set_figure(game_info->field, figure_info->type + 1);

#define SET_LEVEL()                     \
  if (game_info->score > 5399) {        \
    game_info->level = 10;              \
  } else if (game_info->score > 4799) { \
    game_info->level = 9;               \
  } else if (game_info->score > 4199) { \
    game_info->level = 8;               \
  } else if (game_info->score > 3599) { \
    game_info->level = 7;               \
  } else if (game_info->score > 2999) { \
    game_info->level = 6;               \
  } else if (game_info->score > 2399) { \
    game_info->level = 5;               \
  } else if (game_info->score > 1799) { \
    game_info->level = 4;               \
  } else if (game_info->score > 1199) { \
    game_info->level = 3;               \
  } else if (game_info->score > 599) {  \
    game_info->level = 2;               \
  }

#define SET_SPEED()                   \
  if (game_info->level == 10) {       \
    game_info->speed = 10;            \
  } else if (game_info->level == 9) { \
    game_info->speed = 12;            \
  } else if (game_info->level == 8) { \
    game_info->speed = 14;            \
  } else if (game_info->level == 7) { \
    game_info->speed = 16;            \
  } else if (game_info->level == 6) { \
    game_info->speed = 18;            \
  } else if (game_info->level == 5) { \
    game_info->speed = 20;            \
  } else if (game_info->level == 4) { \
    game_info->speed = 22;            \
  } else if (game_info->level == 3) { \
    game_info->speed = 24;            \
  } else if (game_info->level == 2) { \
    game_info->speed = 30;            \
  } else {                            \
    game_info->speed = 40;            \
  }

#define SET_PAUSE()             \
  if (*action == Pause) {       \
    game_info->pause = true;    \
  } else if (action == Start) { \
    game_info->pause = false;   \
  }

#define SET_NEXT() set_next(action);

#define SET_HIGHSCORE()                         \
  if (game_info->score > game_info->high_score) \
    game_info->high_score = game_info->score;

#define SET_SCORE() \
  if (figure_info->y <= 2) game_info->score += scoring(game_info->next);

#define ENDNCURSES() endwin();

#define FREE_ALL()                           \
  int **figure2 = initFigure(false);         \
  free(figure2);                             \
  GameInfo_t *game_info2 = initGameInfo();   \
  free(game_info2->next);                    \
  free(game_info2->field);                   \
  free(game_info2);                          \
  FigureInfo_t *figure_info2 = initTetris(); \
  free(figure_info2);                        \
  UserAction_t *action2 = initUserAction();  \
  free(action2);                             \
  init_ch(true);

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef struct {
  int x;
  int y;
  int type;
  int type_next;
  int overflow;
  int step;
} FigureInfo_t;

// tetris
int menu(void);
void gameMenuTetris(void);
int gameStartTetris(void);

// matrix
int **createMatrix(int rows, int cols);
void freeMatrix(int **matrix);
void printMatrix(int **matrix, int rows, int cols);

// backend
GameInfo_t updateCurrentState();
void reset_if_overflow(int forced_reset);
int *init_ch(int free_ch);
void set_action(UserAction_t *action);
void set_next(UserAction_t *action);
void turn();
int check(UserAction_t action);
void set_figure(int **field, int type);
int **initFigure(int change);
GameInfo_t *initGameInfo(void);
UserAction_t *initUserAction(void);
FigureInfo_t *initTetris(void);
int read_highscore(void);
void write_highscore(int score);
int scoring(int **matrix);
int pause(void);

// frontend
void userInput(UserAction_t action, bool hold);

// draw
void inclusion_of_colors(void);
void draw_lv(void);
void draw_start_screen_Tetris(void);
void draw_pause(void);
void draw_button_Tetris(int color);
void draw_button_Empty(int color);
void draw_button_Exit(int color);
void draw_phone(void);
void draw_field(void);
void draw_highscore(void);
void draw_next_figure(void);
void draw_score(void);

#endif
