#include "entrypoint.h"
#include "sqlite3.h"
#include "knob.h"
#include "declarations.h"

#include <time.h>
#include <stdlib.h>

#define LOG_SQLITE3_ERROR(db) knob_log(KNOB_ERROR, "%s:%d: SQLITE3 ERROR: %s\n", __FILE__, __LINE__, sqlite3_errmsg(db))

void raylib_start(void){

    //Initialisation de la database et création d'un seed.
    srand(time(NULL));
    sqlite3* db = NULL;
    sqlite3_open("./aventure_quete.db",&db);

    //Début du code.
    int ID = PromptPlayerChoice(db);
    DeplacerJoueur(db, ID, 2);
    AfficherLieu(db, ObtenirPositionJoueur(db, ID));
    RamasserObjet(db, ID, 4);

    return;
}