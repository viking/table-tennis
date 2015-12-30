#ifndef __STATS_H
#define __STATS_H

#include "player.h"
#include "game.h"

typedef struct {
  Game *game;
  Player *winner;
  Player *loser;
  int winning_score;
  int losing_score;
} LargestMargin;

typedef struct {
  Player *player;
  int num;
} PlayerNum;

typedef struct {
  LargestMargin *largest_margin;
  PlayerNum *best_win_streak;
  PlayerNum *worst_loss_streak;
  PlayerNum *most_games;
} Stats;

Stats *stats_new(Player *players, int player_count, Game *games, int game_count);
void stats_free(Stats *stats);

#endif
