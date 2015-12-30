#include "stat_list.h"

static void
player_stat_add_win(stat)
  PlayerStat *stat;
{
  stat->game_count++;
  if (stat->loss_streak > stat->best_loss_streak) {
    stat->best_loss_streak = stat->loss_streak;
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
  if (stat->loss_streak > stat->best_loss_streak) {
    stat->best_loss_streak = stat->loss_streak;
  }
  if (stat->win_streak > stat->best_win_streak) {
    stat->best_win_streak = stat->win_streak;
  }
}

GtkWidget *
stat_list_new(players, player_count, games, game_count)
  Player *players;
  int player_count;
  Game *games;
  int game_count;
{
  GtkWidget *stat_list;
  GtkListStore *stats;
  GtkTreeIter iter;
  GtkTreeViewColumn *name_column, *info_column;
  GtkCellRenderer *name_renderer, *info_renderer;
  PlayerStat *player_stats = NULL, *player_stat;
  GameStat *game_stats = NULL, *game_stat;
  Superlative super;
  super.delta = super.win = super.lose = super.games = -1;
  char *text;

  /* Calculate stats */
  if (player_count > 0) {
    player_stats = (PlayerStat *)malloc(sizeof(PlayerStat) * player_count);
    for (int i = 0; i < player_count; i++) {
      player_stats[i].player = players + i;
      player_stats[i].best_win_streak = 0;
      player_stats[i].win_streak = 0;
      player_stats[i].best_loss_streak = 0;
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

        if (super.delta < 0 || games[i].score_delta > games[super.delta].score_delta) {
          super.delta = i;
        }
      }
    }
  }

  for (int i = 0; i < player_count; i++) {
    player_stat_finish(player_stats + i);
    if ((super.win < 0 && player_stats[i].best_win_streak > 0) ||
        player_stats[i].best_win_streak > player_stats[super.win].best_win_streak) {
      super.win = i;
    }
    if ((super.lose < 0 && player_stats[i].best_loss_streak > 0) ||
        player_stats[i].best_loss_streak > player_stats[super.lose].best_loss_streak) {
      super.lose = i;
    }
    if ((super.games < 0 && player_stats[i].game_count > 0) ||
        player_stats[i].game_count > player_stats[super.games].game_count) {
      super.games = i;
    }
  }

  stats = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

  /* Biggest whooping */
  if (super.delta < 0) {
    asprintf(&text, "None");
  } else {
    game_stat = game_stats + super.delta;
    asprintf(&text, "%s vs %s, %d to %d", game_stat->winner->player->name,
        game_stat->loser->player->name, game_stat->winning_score,
        game_stat->losing_score);
  }
  gtk_list_store_append(stats, &iter);
  gtk_list_store_set(stats, &iter,
      STAT_LIST_NAME_COLUMN, "Biggest Whooping",
      STAT_LIST_INFO_COLUMN, text,
      -1);
  free(text);

  /* Biggest winning streak */
  if (super.win < 0) {
    asprintf(&text, "None");
  } else {
    player_stat = player_stats + super.win;
    asprintf(&text, "%s - %d", player_stat->player->name,
        player_stat->best_win_streak);
  }
  gtk_list_store_append(stats, &iter);
  gtk_list_store_set(stats, &iter,
      STAT_LIST_NAME_COLUMN, "Biggest Winning Streak",
      STAT_LIST_INFO_COLUMN, text,
      -1);
  free(text);

  /* Biggest losing streak */
  if (super.lose < 0) {
    asprintf(&text, "None");
  } else {
    player_stat = player_stats + super.lose;
    asprintf(&text, "%s - %d", player_stat->player->name,
        player_stat->best_loss_streak);
  }
  gtk_list_store_append(stats, &iter);
  gtk_list_store_set(stats, &iter,
      STAT_LIST_NAME_COLUMN, "Biggest Losing Streak",
      STAT_LIST_INFO_COLUMN, text,
      -1);
  free(text);

  /* Most games */
  if (super.games < 0) {
    asprintf(&text, "None");
  } else {
    player_stat = player_stats + super.games;
    asprintf(&text, "%s - %d", player_stat->player->name,
        player_stat->game_count);
  }
  gtk_list_store_append(stats, &iter);
  gtk_list_store_set(stats, &iter,
      STAT_LIST_NAME_COLUMN, "Most Games Played",
      STAT_LIST_INFO_COLUMN, text,
      -1);
  free(text);

  stat_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(stats));
  g_object_unref(G_OBJECT(stats));

  name_renderer = gtk_cell_renderer_text_new();
  name_column = gtk_tree_view_column_new_with_attributes("Name",
      name_renderer, "text", STAT_LIST_NAME_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(stat_list), name_column);

  info_renderer = gtk_cell_renderer_text_new();
  info_column = gtk_tree_view_column_new_with_attributes("Info",
      info_renderer, "text", STAT_LIST_INFO_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(stat_list), info_column);

  if (player_stats != NULL) {
    free(player_stats);
  }

  if (game_stats != NULL) {
    free(game_stats);
  }

  return stat_list;
}
