#include "leader_list.h"

enum {
  RANK_COLUMN,
  PLAYER_COLUMN,
  N_COLUMNS
};

GtkWidget *
leader_list_new(players, player_count)
  Player *players;
  int player_count;
{
  GtkWidget *leader_list;
  GtkListStore *leaders;
  GtkTreeIter iter;
  GtkTreeViewColumn *rank_column, *player_column;
  GtkCellRenderer *rank_renderer, *player_renderer;

  leaders = gtk_list_store_new(2, G_TYPE_INT, G_TYPE_STRING);
  for (int i = 0; i < player_count; i++ ){
    gtk_list_store_append(leaders, &iter);
    gtk_list_store_set(leaders, &iter,
        RANK_COLUMN, i + 1,
        PLAYER_COLUMN, players[i].name,
        -1);
  }

  leader_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(leaders));
  g_object_unref(G_OBJECT(leaders));

  rank_renderer = gtk_cell_renderer_text_new();
  rank_column = gtk_tree_view_column_new_with_attributes("Rank",
      rank_renderer, "text", RANK_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(leader_list), rank_column);

  player_renderer = gtk_cell_renderer_text_new();
  player_column = gtk_tree_view_column_new_with_attributes("Player",
      player_renderer, "text", PLAYER_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(leader_list), player_column);

  return leader_list;
}
