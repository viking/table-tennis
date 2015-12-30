#ifndef __GAME_H
#define __GAME_H

#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

typedef struct {
  int id;
  int player0_id;
  int player1_id;
  char *start;
  char *end;
  int duration;
  int player0_score;
  int player1_score;
  int score_delta;
  int winner_id;
} Game;

int games_count(sqlite3 *db);
Game *games_find(sqlite3 *db);
void games_free(Game *games, int count);

#endif
