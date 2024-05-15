#include "includes/tetris.h"

int main(void) {
  menu();
  return 0;
}

int menu(void) {
  INITNCURSES();
  UserAction_t *action = initUserAction();
  inclusion_of_colors();
  int gg = 1, tetris = 1, empty = 2, exit = 3;
  while (*action != Terminate) {
    set_action(action);
    draw_phone();
    draw_button_Tetris(gg == tetris ? 9 : 8);
    draw_button_Empty(gg == empty ? 9 : 8);
    draw_button_Exit(gg == exit ? 9 : 8);
    if (*action == Start) {
      if (gg == tetris) {
        gameMenuTetris();
      } else if (gg == empty) {
        ;
      } else if (gg == exit) {
        *action = Terminate;
      }
    } else if (*action == Down) {
      gg++;
      if (gg == 4) gg = 1;
    } else if (*action == Up) {
      gg--;
      if (gg == 0) gg = 3;
    }
  }

  FREE_ALL();
  ENDNCURSES();

  return 0;
}

void gameMenuTetris(void) {
  UserAction_t *action = initUserAction();
  while (*action != Terminate) {
    set_action(action);
    if (*action == Start) {
      gameStartTetris();
    }
    draw_start_screen_Tetris();
  }
  reset_if_overflow(true);
  *action = Action;
}

void and_game(void) {
  GameInfo_t *game_info = initGameInfo();
  UserAction_t *action = initUserAction();
  for (int i = FIELD_HEIGHT - 1; i >= 0; i--) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if (*action == Terminate) {
        return;
      }
      game_info->field[i][j] = 2;
      set_action(action);
      draw_phone();
      draw_field();
      draw_next_figure();
      draw_score();
      draw_lv();
      draw_highscore();
      refresh();
      napms(5);
    }
  }
  reset_if_overflow(true);
}

int gameStartTetris(void) {
  UserAction_t *action = initUserAction();
  FigureInfo_t *figure_info = initTetris();
  while (*action != Terminate && !figure_info->overflow) {
    set_action(action);
    updateCurrentState();
    draw_phone();
    draw_field();
    draw_next_figure();
    draw_score();
    draw_lv();
    draw_highscore();
    if (*action == Pause) {
      pause();
    }
    napms(20);
  }
  if (figure_info->overflow) {
    and_game();
  }

  return 0;
}

int factorial(int n) {
  int res = 1;
  for (int i = 1; i <= n; i++) {
    res *= i;
  }
  return res;
}

int combination(int n, int k) {
  return factorial(n) / (factorial(k) * factorial(n - k));
}
