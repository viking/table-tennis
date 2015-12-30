#include "stats.h"

typedef struct {
  Player *player;
  int best_win_streak;
  int win_streak;
  int worst_loss_streak;
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
  int loss;
  int games;
} Indices;

static void
player_stat_add_win(stat)
  PlayerStat *stat;
{
  stat->game_count++;
  if (stat->loss_streak > stat->worst_loss_streak) {
    stat->worst_loss_streak = stat->loss_streak;
  }
  stat->loss_streak = 0;
  stat->win_streak++;
}

static void
player_stat_add_loss(stat)
  PlayerStat *stat;
{
  stat->game_count++;
  if (stat->win_streak > stat->best_win_streak) {
    stat->best_win_streak = stat->win_streak;
  }
  stat->win_streak = 0;
  stat->loss_streak++;
}

static void
player_stat_finish(stat)
  PlayerStat *stat;
{
  if (stat->loss_streak > stat->worst_loss_streak) {
    stat->worst_loss_streak = stat->loss_streak;
  }
  if (stat->win_streak > stat->best_win_streak) {
    stat->best_win_streak = stat->win_streak;
  }
}

Stats *
stats_new(players, player_count, games, game_count)
  Player *players;
  int player_count;
  Game *games;
  int game_count;
{
  Stats *stats = NULL;
  PlayerStat *player_stat, *player_stats = NULL;
  GameStat *game_stat, *game_stats = NULL;
  Indices indices;
  indices.delta = indices.win = indices.loss = indices.games = -1;

  /* Calculate stats */
  if (player_count > 0) {
    player_stats = (PlayerStat *)malloc(sizeof(PlayerStat) * player_count);
    for (int i = 0; i < player_count; i++) {
      player_stats[i].player = players + i;
      player_stats[i].best_win_streak = 0;
      player_stats[i].win_streak = 0;
      player_stats[i].worst_loss_streak = 0;
      player_stats[i].loss_streak = 0;
      player_stats[i].game_count = 0;
    }
  }

  if (game_count > 0) {
    game_stats = (GameStat *)malloc(sizeof(GameStat) * game_count);

    for (int i = 0; i < game_count; i++) {
      game_stats[i].game = games + i;
      game_stats[i].winner = NULL;
      game_stats[i].loser = NULL;

      for (int j = 0; j < player_count; j++) {
        if (games[i].player0_id == player_stats[j].player->id) {
          if (games[i].winner_id == games[i].player0_id) {
            game_stats[i].winner = player_stats + j;
            game_stats[i].winning_score = games[i].player0_score;
          } else {
            game_stats[i].loser = player_stats + j;
            game_stats[i].losing_score = games[i].player0_score;
          }
        } else if (games[i].player1_id == player_stats[j].player->id) {
          if (games[i].winner_id == games[i].player1_id) {
            game_stats[i].winner = player_stats + j;
            game_stats[i].winning_score = games[i].player1_score;
          } else {
            game_stats[i].loser = player_stats + j;
            game_stats[i].losing_score = games[i].player1_score;
          }

        }
      }

      if (game_stats[i].winner == NULL || game_stats[i].loser == NULL || game_stats[i].winner == game_stats[i].loser) {
        /* Ignore bad games */
        continue;
      }

      if (game_stats[i].winner->player->guest == 1 || game_stats[i].loser->player->guest == 1) {
        /* Ignore games with guests */
        continue;
      }

      player_stat_add_win(game_stats[i].winner);
      player_stat_add_loss(game_stats[i].loser);

      if (indices.delta < 0 || games[i].score_delta > games[indices.delta].score_delta) {
        indices.delta = i;
      }
    }
  }

  for (int i = 0; i < player_count; i++) {
    player_stat_finish(player_stats + i);

    if (player_stats[i].player->guest == 1) {
      /* Skip guests */
      continue;
    }

    if ((indices.win < 0 && player_stats[i].best_win_streak > 0) ||
        player_stats[i].best_win_streak > player_stats[indices.win].best_win_streak) {
      indices.win = i;
    }
    if ((indices.loss < 0 && player_stats[i].worst_loss_streak > 0) ||
        player_stats[i].worst_loss_streak > player_stats[indices.loss].worst_loss_streak) {
      indices.loss = i;
    }
    if ((indices.games < 0 && player_stats[i].game_count > 0) ||
        player_stats[i].game_count > player_stats[indices.games].game_count) {
      indices.games = i;
    }
  }

  /* Put them together */
  stats = (Stats *)malloc(sizeof(Stats));
  if (indices.delta >= 0) {
    game_stat = game_stats + indices.delta;
    stats->largest_margin = (LargestMargin *)malloc(sizeof(LargestMargin));
    stats->largest_margin->game = game_stat->game;
    stats->largest_margin->winner = game_stat->winner->player;
    stats->largest_margin->loser = game_stat->loser->player;
    stats->largest_margin->winning_score = game_stat->winning_score;
    stats->largest_margin->losing_score = game_stat->losing_score;
  } else {
    stats->largest_margin = NULL;
  }

  if (indices.win >= 0) {
    player_stat = player_stats + indices.win;
    stats->best_win_streak = (PlayerNum *)malloc(sizeof(PlayerNum));
    stats->best_win_streak->player = player_stat->player;
    stats->best_win_streak->num = player_stat->best_win_streak;
  } else {
    stats->best_win_streak = NULL;
  }

  if (indices.loss >= 0) {
    player_stat = player_stats + indices.loss;
    stats->worst_loss_streak = (PlayerNum *)malloc(sizeof(PlayerNum));
    stats->worst_loss_streak->player = player_stat->player;
    stats->worst_loss_streak->num = player_stat->worst_loss_streak;
  } else {
    stats->worst_loss_streak = NULL;
  }

  if (indices.games >= 0) {
    player_stat = player_stats + indices.games;
    stats->most_games = (PlayerNum *)malloc(sizeof(PlayerNum));
    stats->most_games->player = player_stat->player;
    stats->most_games->num = player_stat->game_count;
  } else {
    stats->most_games = NULL;
  }

  if (player_stats != NULL) {
    free(player_stats);
  }

  if (game_stats != NULL) {
    free(game_stats);
  }

  return stats;
}

void
stats_free(stats)
  Stats *stats;
{
  if (stats == NULL) {
    return;
  }

  if (stats->largest_margin != NULL) {
    free(stats->largest_margin);
  }
  if (stats->best_win_streak != NULL) {
    free(stats->best_win_streak);
  }
  if (stats->worst_loss_streak != NULL) {
    free(stats->worst_loss_streak);
  }
  if (stats->most_games != NULL) {
    free(stats->most_games);
  }
  free(stats);
}
