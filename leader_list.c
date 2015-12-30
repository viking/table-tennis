#include "leader_list.h"

GtkWidget *
leader_list_new(players, player_count)
  Player *players;
  int player_count;
{
  GtkWidget *leader_list;
  GtkListStore *leaders;
  GtkTreeIter iter;
  GtkTreeViewColumn *rank_column, *player_column, *elo_column;
  GtkCellRenderer *rank_renderer, *player_renderer, *elo_renderer;
  int rank = 1;

  leaders = gtk_list_store_new(LEADER_LIST_N_COLUMNS, G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT);
  for (int i = 0; i < player_count; i++ ){
    if (players[i].guest == 1) {
      continue;
    }

    gtk_list_store_append(leaders, &iter);
    gtk_list_store_set(leaders, &iter,
        LEADER_LIST_RANK_COLUMN, rank++,
        LEADER_LIST_PLAYER_COLUMN, players[i].name,
        LEADER_LIST_ELO_COLUMN, players[i].elo,
        -1);
  }

  leader_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(leaders));
  g_object_unref(G_OBJECT(leaders));

  rank_renderer = gtk_cell_renderer_text_new();
  rank_column = gtk_tree_view_column_new_with_attributes("Rank",
      rank_renderer, "text", LEADER_LIST_RANK_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(leader_list), rank_column);

  player_renderer = gtk_cell_renderer_text_new();
  player_column = gtk_tree_view_column_new_with_attributes("Player",
      player_renderer, "text", LEADER_LIST_PLAYER_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(leader_list), player_column);

  elo_renderer = gtk_cell_renderer_text_new();
  elo_column = gtk_tree_view_column_new_with_attributes("Rating",
      elo_renderer, "text", LEADER_LIST_ELO_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(leader_list), elo_column);

  return leader_list;
}
