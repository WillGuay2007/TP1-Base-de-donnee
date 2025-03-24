#pragma once

#include "entrypoint.h"
#include "sqlite3.h"
#include "knob.h"

#include <time.h>
#include <stdlib.h>

#define LOG_SQLITE3_ERROR(db) knob_log(KNOB_ERROR, "%s:%d: SQLITE3 ERROR: %s\n", __FILE__, __LINE__, sqlite3_errmsg(db))

void see_state_fruits(sqlite3* db,char* state_name);
void add_random_fruit(sqlite3* db);
void afficher_tous_lieux(sqlite3* db);
int TrouverLieuIDParNom(sqlite3* db, const char* Nom_Lieu);