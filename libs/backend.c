#include "../includes/tetris.h"

GameInfo_t updateCurrentState() {
  // y = 0 x = 1
  GameInfo_t *game_info = initGameInfo();
  FigureInfo_t *figure_info = initTetris();
  int **figure = initFigure(false);
  UserAction_t *action = initUserAction();
  reset_if_overflow(false);
  SET_FIELD();
  SET_NEXT();
  SET_SCORE();
  SET_HIGHSCORE();
  SET_LEVEL();
  SET_SPEED();
  SET_PAUSE();

  return *game_info;
}

void reset_if_overflow(int forced_reset) {
  GameInfo_t *game_info = initGameInfo();
  FigureInfo_t *figure_info = initTetris();
  if (figure_info->overflow || forced_reset) {
    if (read_highscore() < game_info->score) {
      write_highscore(game_info->score);
    }
    figure_info->overflow = false;
    for (int i = 0; i < FIELD_HEIGHT; i++) {
      for (int j = 0; j < FIELD_WIDTH; j++) {
        game_info->field[i][j] = 0;
        game_info->next[i][j] = 0;
      }
    }
  }
}

int *init_ch(int free_ch) {
  static int *ch = NULL;
  if (ch == NULL) {
    ch = malloc(sizeof(int));
    if (ch == NULL) {
      return NULL;
    }
  }
  if (free_ch == true && ch != NULL) {
    free(ch);
    ch = NULL;
  }

  return ch;
}

void set_action(UserAction_t *action) {
  int *ch = init_ch(false);
  if (ch == NULL) {
    *action = Terminate;
  } else {
    *ch = getch();
    switch (*ch) {
      case KEY_LEFT:
      case 'a':
        *action = Left;
        break;
      case KEY_RIGHT:
      case 'd':
        *action = Right;
        break;
      case KEY_DOWN:
      case 's':
        *action = Down;
        break;
      case KEY_UP:
      case 'w':
        *action = Up;
        break;
      case 'q':
        *action = Terminate;
        break;
      case 'p':
        *action = Pause;
        break;
      case ' ':
        *action = Action;
        break;
      case '\n':
        *action = Start;
        break;
      default:
        *action = Action;
        break;
    }
  }
}

void set_next(UserAction_t *action) {
  GameInfo_t *game_info = initGameInfo();
  FigureInfo_t *figure_info = initTetris();
  int **figure = initFigure(false);
  set_figure(game_info->next, 0);
  if (*action == Left) {
    if (check(Left)) figure_info->x--;
  } else if (*action == Right) {
    if (check(Right)) figure_info->x++;
  } else if (*action == Down) {
    while (check(Down)) figure_info->y++;
    figure_info->step = game_info->speed;
  } else if (*action == Up) {
    turn();
  }
  if (figure_info->step >= game_info->speed) {
    figure_info->step = 0;
    if (!check(Down)) {
      set_figure(game_info->next, figure_info->type + 1);
      initFigure(true);
      while (check(Up)) {
        figure_info->y--;
      }
    } else {
      figure_info->y++;
    }
  } else {
    figure_info->step++;
  }
  for (int i = 0; i < 4; i++) {
    if (!check(Down) && (figure_info->y + figure[i][0] == 0 ||
                         figure_info->y + figure[i][0] == 1 ||
                         figure_info->y + figure[i][0] == 2)) {
      SET_FIELD();
      figure_info->overflow = true;
    }
  }
  set_figure(game_info->next, figure_info->type + 1);
}

void turn() {
  int **figure = initFigure(false);
  FigureInfo_t *figure_info = initTetris();
  GameInfo_t *game_info = initGameInfo();
  int x = 0, y = 0, reset = 0;
  int center[2] = {0, 0};
  int figure_old[4][2];

  for (int i = 0; i < 4; i++) {
    figure_old[i][0] = figure[i][0];
    figure_old[i][1] = figure[i][1];
  }

  for (int i = 0; i < 4; i++) {
    x = figure[i][1] - center[1];
    y = figure[i][0] - center[0];
    figure[i][0] = center[0] - x;
    figure[i][1] = center[1] + y;

    if (figure[i][1] + figure_info->x >= FIELD_WIDTH ||
        figure[i][1] + figure_info->x < 0 ||
        figure[i][0] + figure_info->y >= FIELD_HEIGHT ||
        figure[i][0] + figure_info->y < 0)
      reset = 1;

    if (!reset && game_info->next[figure_info->y + figure[i][0]]
                                 [figure_info->x + figure[i][1]] != 0) {
      reset = 1;
    }
  }

  if (reset) {
    for (int i = 0; i < 4; i++) {
      figure[i][0] = figure_old[i][0];
      figure[i][1] = figure_old[i][1];
    }
  }
}

int check(UserAction_t action) {
  int **figure = initFigure(false);
  GameInfo_t *game_info = initGameInfo();
  FigureInfo_t *figure_info = initTetris();
  int can = true, min_x = 0, max_x = 0, min_y = 0, max_y = 0;
  for (int i = 0; i < 4; i++) {
    if (figure[i][1] < min_x) min_x = figure[i][1];
    if (figure[i][1] > max_x) max_x = figure[i][1];
    if (figure[i][0] < min_y) min_y = figure[i][0];
    if (figure[i][0] > max_y) max_y = figure[i][0];
  }
  if (action == Left) {
    if (figure_info->x + min_x - 1 < 0)
      can = false;
    else
      for (int i = 0; i < 4; i++)
        if (game_info->next[figure_info->y + figure[i][0]]
                           [figure_info->x + figure[i][1] - 1] != 0)
          can = false;
  } else if (action == Right) {
    if (figure_info->x + max_x + 1 >= FIELD_WIDTH)
      can = false;
    else
      for (int i = 0; i < 4; i++)
        if (game_info->next[figure_info->y + figure[i][0]]
                           [figure_info->x + figure[i][1] + 1] != 0)
          can = false;
  } else if (action == Down) {
    if (figure_info->y + max_y + 1 >= FIELD_HEIGHT)
      can = false;
    else
      for (int i = 0; i < 4; i++)
        if (game_info->next[figure_info->y + figure[i][0] + 1]
                           [figure_info->x + figure[i][1]] != 0)
          can = false;
  } else if (action == Up) {
    if (figure_info->y + min_y - 1 < 0)
      can = false;
    else
      for (int i = 0; i < 4; i++)
        if (game_info->next[figure_info->y + figure[i][0] - 1]
                           [figure_info->x + figure[i][1]] != 0)
          can = false;
  }
  return can;
}

void set_figure(int **field, int type) {
  FigureInfo_t *figure_info = initTetris();
  int **figure = initFigure(false);
  field[figure_info->y + figure[0][0]][figure_info->x + figure[0][1]] = type;
  field[figure_info->y + figure[1][0]][figure_info->x + figure[1][1]] = type;
  field[figure_info->y + figure[2][0]][figure_info->x + figure[2][1]] = type;
  field[figure_info->y + figure[3][0]][figure_info->x + figure[3][1]] = type;
}

int **initFigure(int change) {
  FIGURS

  FigureInfo_t *figure_info = initTetris();
  static int **figure = NULL;
  if (figure == NULL) {
    figure = malloc(sizeof(int *) * 4 + sizeof(int) * 4 * 2);
    int *ptr = (int *)(figure + 4);
    for (int i = 0; i < 4; i++) {
      figure[i] = ptr + 2 * i;
    }
    if (figure == NULL) {
      return NULL;
    }
    for (int i = 0; i < 4; i++) {
      figure[i][0] = figurs[figure_info->type][i][0];
      figure[i][1] = figurs[figure_info->type][i][1];
    }
  }
  if (change) {
    figure_info->type = figure_info->type_next;
    figure_info->type_next = rand() % 7;
    for (int i = 0; i < 4; i++) {
      figure[i][0] = figurs[figure_info->type][i][0];
      figure[i][1] = figurs[figure_info->type][i][1];
    }
    figure_info->y = 2;
    figure_info->x = FIELD_WIDTH / 2 - 1;
  }

  return figure;
}

GameInfo_t *initGameInfo(void) {
  static GameInfo_t *game_info = NULL;
  if (game_info == NULL) {
    game_info = malloc(sizeof(GameInfo_t));
    if (game_info == NULL) {
      return NULL;
    }
    game_info->field = createMatrix(FIELD_HEIGHT, FIELD_WIDTH);
    if (game_info->field == NULL) {
      free(game_info);
      return NULL;
    }
    game_info->next = createMatrix(FIELD_HEIGHT, FIELD_WIDTH);
    if (game_info->next == NULL) {
      free(game_info->field);
      free(game_info);
      return NULL;
    }
    game_info->score = 0;
    game_info->high_score = read_highscore();
    game_info->level = 0;
    game_info->speed = 0;
    game_info->pause = false;
  }
  return game_info;
}

UserAction_t *initUserAction(void) {
  static UserAction_t *user_action = NULL;
  if (user_action == NULL) {
    user_action = malloc(sizeof(UserAction_t));
    if (user_action == NULL) {
      return NULL;
    } else {
      *user_action = Pause;
    }
  }
  return user_action;
}

FigureInfo_t *initTetris(void) {
  static FigureInfo_t *figure_info = NULL;
  if (figure_info == NULL) {
    figure_info = malloc(sizeof(FigureInfo_t));
    if (figure_info == NULL) {
      return NULL;
    }
    figure_info->x = FIELD_WIDTH / 2 - 1;
    GameInfo_t *game_info = initGameInfo();
    figure_info->y = 2;
    figure_info->type = rand() % 7;
    figure_info->type_next = rand() % 7;
    figure_info->overflow = false;
    figure_info->step = 0;
  }
  return figure_info;
}

int read_highscore(void) {
  int highscore = 0;
  FILE *f = fopen("highscore.txt", "r");
  if (f != NULL) {
    fscanf(f, "%d", &highscore);
    fclose(f);
  }
  return highscore;
}

void write_highscore(int score) {
  FILE *file = fopen("highscore.txt", "w");
  if (file != NULL) {
    fprintf(file, "%d", score);
    fclose(file);
  }
}

int scoring(int **matrix) {
  FigureInfo_t *figure_info = initTetris();
  int **figure = initFigure(false);
  for (int i = 0; i < 4; i++) {
    matrix[figure_info->y + figure[i][0]][figure_info->x + figure[i][1]] = 0;
  }
  int res = 0, gg = 0;
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    gg = 1;
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if (matrix[i][j] == 0) gg = 0;
    }
    if (gg) {
      res++;
      for (int j = 0; j < FIELD_WIDTH; j++) {
        matrix[i][j] = 0;
      }
      for (int k = i - 1; k >= 0; k--) {
        for (int j = 0; j < FIELD_WIDTH; j++) {
          matrix[k + 1][j] = matrix[k][j];
        }
      }
    }
  }
  if (res == 1) {
    res = 100;
  } else if (res == 2) {
    res = 300;
  } else if (res == 3) {
    res = 700;
  } else if (res == 4) {
    res = 1500;
  }
  return res;
}

int pause(void) {
  UserAction_t *action = initUserAction();
  int error = 0, q = 0;
  while (q == 0 && error == 0) {
    set_action(action);
    if (*action == Terminate) {
      error = 1;
    } else if (*action == Pause || *action == Start) {
      q = 1;
    }
  }
  return error;
}
