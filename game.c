#include "game.h"

static int
setup_table(db)
  sqlite3 *db;
{
  sqlite3_stmt *stmt;
  int count, result;

  /* Check for existance of table */
  result = sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM sqlite_master WHERE type = 'table' AND name = 'games';", -1, &stmt, NULL);
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

  result = sqlite3_prepare_v2(db, "CREATE TABLE games (id INTEGER PRIMARY KEY, player0_id INTEGER, player1_id INTEGER, start TEXT, end TEXT, duration INTEGER, player0_score INTEGER, player1_score INTEGER, score_delta INTEGER, winner_id INTEGER);", -1, &stmt, NULL);
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
games_count(db)
  sqlite3 *db;
{
  sqlite3_stmt *stmt;
  int count, result;

  result = setup_table(db);
  if (result != SQLITE_OK) {
    return -1;
  }

  result = sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM games;", -1, &stmt, NULL);
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

Game *
games_find(db)
  sqlite3 *db;
{
  sqlite3_stmt *stmt;
  int count, result;
  char *text;
  Game *values;

  count = games_count(db);
  if (count <= 0) {
    return NULL;
  }

  result = sqlite3_prepare_v2(db, "SELECT id, player0_id, player1_id, start, end, duration, player0_score, player1_score, score_delta, winner_id) FROM games;", -1, &stmt, NULL);
  if (result != SQLITE_OK) {
    return NULL;
  }

  values = (Game *)malloc(sizeof(Game) * count);
  for (int i = 0; i < count; i++) {
    result = sqlite3_step(stmt);
    if (result != SQLITE_ROW) {
      free(values);
      return NULL;
    }

    values[i].id = sqlite3_column_int(stmt, 0);
    values[i].player0_id = sqlite3_column_int(stmt, 1);
    values[i].player1_id = sqlite3_column_int(stmt, 2);

    text = (char *)sqlite3_column_text(stmt, 3);
    values[i].start = text == NULL ? NULL : strdup(text);

    text = (char *)sqlite3_column_text(stmt, 4);
    values[i].end = text == NULL ? NULL : strdup(text);

    values[i].player0_score = sqlite3_column_int(stmt, 5);
    values[i].player1_score = sqlite3_column_int(stmt, 6);
    values[i].score_delta = sqlite3_column_int(stmt, 7);
    values[i].winner_id = sqlite3_column_int(stmt, 8);
  }

  sqlite3_finalize(stmt);
  return values;
};

void
games_free(games, count)
  Game *games;
  int count;
{
  if (games == NULL) {
    return;
  }

  for (int i = 0; i < count; i++) {
    if (games[i].start != NULL) {
      free(games[i].start);
    }

    if (games[i].end != NULL) {
      free(games[i].end);
    }
  }
  free(games);
}
