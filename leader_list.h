#ifndef __LEADER_LIST_H
#define __LEADER_LIST_H

#include <gtk/gtk.h>
#include "player.h"

GtkWidget *leader_list_new(Player *players, int player_count);

#endif
