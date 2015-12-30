#ifndef __PLAYER_H
#define __PLAYER_H

#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>

typedef struct {
  int id;
  int elo;
  int play_count;
  int guest;
  char *rfid;
  char *name;
  char *image;
  enum {
    NO_GENDER,
    FEMALE,
    MALE
  } gender;
} Player;

int players_count(sqlite3 *db);
Player *players_find(sqlite3 *db);
void players_free(Player *players, int count);

#endif
