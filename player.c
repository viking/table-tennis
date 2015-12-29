#include "player.h"

static int
setup_table(db)
  sqlite3 *db;
{
  sqlite3_stmt *stmt;
  int count, result;

  /* Check for existance of table */
  result = sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM sqlite_master WHERE type = 'table' AND name = 'players';", -1, &stmt, NULL);
  if (result != SQLITE_OK) {
    return result;
  }

  result = sqlite3_step(stmt);
  if (result != SQLITE_ROW) {
    return result;
  }

  count = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);
  if (count == 1) {
    return SQLITE_OK;
  }

  result = sqlite3_prepare_v2(db, "CREATE TABLE players (id INTEGER PRIMARY KEY, rfid TEXT, name TEXT, gender INTEGER, elo INTEGER, image TEXT, play_count INTEGER);", -1, &stmt, NULL);
  if (result != SQLITE_OK) {
    return result;
  }

  result = sqlite3_step(stmt);
  sqlite3_finalize(stmt);
  if (result != SQLITE_DONE) {
    return result;
  }

  return SQLITE_OK;
}

int
players_count(db)
  sqlite3 *db;
{
  sqlite3_stmt *stmt;
  int count, result;

  result = setup_table(db);
  if (result != SQLITE_OK) {
    return -1;
  }

  result = sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM players;", -1, &stmt, NULL);
  if (result != SQLITE_OK) {
    return -1;
  }

  result = sqlite3_step(stmt);
  if (result != SQLITE_ROW) {
    sqlite3_finalize(stmt);
    return -1;
  }

  count = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);

  return count;
}

Player *
players_find(db)
  sqlite3 *db;
{
  sqlite3_stmt *stmt;
  int count, result;
  char *text;
  Player *values;

  count = players_count(db);
  if (count <= 0) {
    return NULL;
  }

  result = sqlite3_prepare_v2(db, "SELECT id, rfid, name, gender, elo, image, play_count FROM players;", -1, &stmt, NULL);
  if (result != SQLITE_OK) {
    return NULL;
  }

  values = (Player *)malloc(sizeof(Player) * count);
  for (int i = 0; i < count; i++) {
    result = sqlite3_step(stmt);
    if (result != SQLITE_ROW) {
      free(values);
      return NULL;
    }

    values[i].id = sqlite3_column_int(stmt, 0);

    text = (char *)sqlite3_column_text(stmt, 1);
    values[i].rfid = text == NULL ? NULL : strdup(text);

    text = (char *)sqlite3_column_text(stmt, 2);
    values[i].name = text == NULL ? NULL : strdup(text);

    values[i].gender = sqlite3_column_int(stmt, 3);
    values[i].elo = sqlite3_column_int(stmt, 4);

    text = (char *)sqlite3_column_text(stmt, 5);
    values[i].image = text == NULL ? NULL : strdup(text);

    values[i].play_count = sqlite3_column_int(stmt, 6);
  }

  sqlite3_finalize(stmt);
  return values;
};
