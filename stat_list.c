#include "stat_list.h"

GtkWidget *
stat_list_new(stats)
  Stats *stats;
{
  GtkWidget *stat_list;
  GtkListStore *store;
  GtkTreeIter iter;
  GtkTreeViewColumn *name_column, *info_column;
  GtkCellRenderer *name_renderer, *info_renderer;
  LargestMargin *margin;
  PlayerNum *num;
  char *text;

  store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

  /* Largest margin of victory */
  if (stats->largest_margin == NULL) {
    asprintf(&text, "None");
  } else {
    margin = stats->largest_margin;
    asprintf(&text, "%s vs %s, %d to %d", margin->winner->name,
        margin->loser->name, margin->winning_score,
        margin->losing_score);
  }
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter,
      STAT_LIST_NAME_COLUMN, "Largest Margin of Victory",
      STAT_LIST_INFO_COLUMN, text,
      -1);
  free(text);

  /* Best winning streak */
  if (stats->best_win_streak == NULL) {
    asprintf(&text, "None");
  } else {
    num = stats->best_win_streak;
    asprintf(&text, "%s - %d", num->player->name,
        num->num);
  }
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter,
      STAT_LIST_NAME_COLUMN, "Largest Winning Streak",
      STAT_LIST_INFO_COLUMN, text,
      -1);
  free(text);

  /* Worst losing streak */
  if (stats->worst_loss_streak == NULL) {
    asprintf(&text, "None");
  } else {
    num = stats->worst_loss_streak;
    asprintf(&text, "%s - %d", num->player->name,
        num->num);
  }
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter,
      STAT_LIST_NAME_COLUMN, "Largest Losing Streak",
      STAT_LIST_INFO_COLUMN, text,
      -1);
  free(text);

  /* Most games */
  if (stats->most_games == NULL) {
    asprintf(&text, "None");
  } else {
    num = stats->most_games;
    asprintf(&text, "%s - %d", num->player->name,
        num->num);
  }
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter,
      STAT_LIST_NAME_COLUMN, "Most Games Played",
      STAT_LIST_INFO_COLUMN, text,
      -1);
  free(text);

  stat_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  gtk_widget_set_name(stat_list, "stat-list");
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(stat_list), FALSE);
  g_object_unref(G_OBJECT(store));

  name_renderer = gtk_cell_renderer_text_new();
  name_column = gtk_tree_view_column_new_with_attributes("Name",
      name_renderer, "text", STAT_LIST_NAME_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(stat_list), name_column);

  info_renderer = gtk_cell_renderer_text_new();
  info_column = gtk_tree_view_column_new_with_attributes("Info",
      info_renderer, "text", STAT_LIST_INFO_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(stat_list), info_column);

  return stat_list;
}
