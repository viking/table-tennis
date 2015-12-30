#ifndef __STAT_LIST_H
#define __STAT_LIST_H

#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include "player.h"
#include "game.h"

enum {
  STAT_LIST_NAME_COLUMN,
  STAT_LIST_INFO_COLUMN,
  STAT_LIST_N_COLUMNS
};

typedef struct {
  Player *player;
  int best_win_streak;
  int win_streak;
  int best_loss_streak;
  int loss_streak;
  int game_count;
} PlayerStat;

typedef struct {
  Game *game;
  PlayerStat *winner;
  PlayerStat *loser;
  int winning_score;
  int losing_score;
} GameStat;

typedef struct {
  int delta;
  int win;
  int lose;
  int games;
} Superlative;

GtkWidget *stat_list_new(Player *players, int player_count, Game *games, int game_count);

#endif
