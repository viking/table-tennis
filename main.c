#include <gtk/gtk.h>
#include <sqlite3.h>
#include "player.h"
#include "game.h"
#include "leader_list.h"
#include "stat_list.h"

int
main (argc, argv)
  int argc;
  char **argv;
{
  GtkWidget *window, *leaderboard, *leader_list, *stat_list;
  int player_count = 0, game_count = 0, result = 0;
  Player *players = NULL;
  Game *games = NULL;
  sqlite3 *db;

  gtk_init(&argc, &argv);

  /* Setup database */
  if (sqlite3_open("table-tennis.db", &db) != SQLITE_OK) {
    g_print("Error opening database: %s\n", sqlite3_errmsg(db));
    result = 1;
    goto exit;
  }

  /* Get players */
  player_count = players_count(db);
  if (player_count < 0) {
    g_print("Error counting players\n");
    result = 1;
    goto exit;
  }
  players = players_find(db);

  /* Get games */
  game_count = games_count(db);
  if (game_count < 0) {
    g_print("Error counting games\n");
    result = 1;
    goto exit;
  }
  games = games_find(db);

  /* Setup main window */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Table Tennis");
  gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
  gtk_window_set_default_size(GTK_WINDOW(window), WWIDTH, WHEIGHT);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  /* Setup leaderboard */
  leaderboard = gtk_grid_new();

  leader_list = leader_list_new(players, player_count);
  gtk_grid_attach(GTK_GRID(leaderboard), leader_list, 0, 0, 1, 1);
  gtk_widget_show(leader_list);

  stat_list = stat_list_new(players, player_count, games, game_count);
  gtk_grid_attach(GTK_GRID(leaderboard), stat_list, 1, 0, 1, 1);
  gtk_widget_show(stat_list);

  gtk_container_add(GTK_CONTAINER(window), leaderboard);
  gtk_widget_show(window);
  gtk_widget_show(leaderboard);
  gtk_main();

exit:
  players_free(players, player_count);
  games_free(games, game_count);
  sqlite3_close(db);
  return result;
}
