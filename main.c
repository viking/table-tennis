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
  GdkScreen *screen;
  GtkCssProvider *css;
  int player_count = 0, game_count = 0, result = 0;
  Player *players = NULL;
  Game *games = NULL;
  Stats *stats = NULL;
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
  if (player_count > 0 && players == NULL) {
    g_print("Error fetching players\n");
    result = 1;
    goto exit;
  }
  players_sort_by_elo(players, player_count);

  /* Get games */
  game_count = games_count(db);
  if (game_count < 0) {
    g_print("Error counting games\n");
    result = 1;
    goto exit;
  }
  games = games_find(db);
  if (game_count > 0 && games == NULL) {
    g_print("Error fetching games\n");
    result = 1;
    goto exit;
  }

  /* Calculate stats */
  stats = stats_new(players, player_count, games, game_count);

  /* Setup main window */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Table Tennis");
  gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
  gtk_window_set_default_size(GTK_WINDOW(window), WWIDTH, WHEIGHT);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  /* Setup styles */
  screen = gtk_widget_get_screen(window);
  css = gtk_css_provider_new();
  if (gtk_css_provider_load_from_path(css, "table-tennis.css", NULL) == TRUE) {
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css), GTK_STYLE_PROVIDER_PRIORITY_USER);
  } else {
    g_print("Failed to load CSS\n");
  }

  /* Setup leaderboard */
  leaderboard = gtk_grid_new();

  leader_list = leader_list_new(players, player_count);
  gtk_widget_set_hexpand(leader_list, TRUE);
  gtk_widget_set_vexpand(leader_list, TRUE);
  gtk_widget_set_halign(leader_list, GTK_ALIGN_FILL);
  gtk_widget_set_valign(leader_list, GTK_ALIGN_FILL);
  gtk_grid_attach(GTK_GRID(leaderboard), leader_list, 0, 0, 1, 1);
  gtk_widget_show(leader_list);

  stat_list = stat_list_new(stats);
  gtk_widget_set_hexpand(stat_list, TRUE);
  gtk_widget_set_vexpand(stat_list, TRUE);
  gtk_widget_set_halign(stat_list, GTK_ALIGN_FILL);
  gtk_widget_set_valign(stat_list, GTK_ALIGN_FILL);
  gtk_grid_attach(GTK_GRID(leaderboard), stat_list, 1, 0, 1, 1);
  gtk_widget_show(stat_list);

  gtk_container_add(GTK_CONTAINER(window), leaderboard);
  gtk_widget_show(window);
  gtk_widget_show(leaderboard);
  gtk_main();

exit:
  players_free(players, player_count);
  games_free(games, game_count);
  stats_free(stats);
  sqlite3_close(db);
  return result;
}
