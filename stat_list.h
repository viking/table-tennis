#ifndef __STAT_LIST_H
#define __STAT_LIST_H

#define _GNU_SOURCE
#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include "stats.h"

enum {
  STAT_LIST_NAME_COLUMN,
  STAT_LIST_INFO_COLUMN,
  STAT_LIST_N_COLUMNS
};

GtkWidget *stat_list_new(Stats *stats);

#endif
