#ifndef __LEADER_LIST_H
#define __LEADER_LIST_H

#include <gtk/gtk.h>
#include "player.h"

enum {
  LEADER_LIST_RANK_COLUMN,
  LEADER_LIST_PLAYER_COLUMN,
  LEADER_LIST_N_COLUMNS
};


GtkWidget *leader_list_new(Player *players, int player_count);

#endif
