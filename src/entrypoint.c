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
    //afficher_tous_lieux(db);
    CreerJoueur(db, "William", 100, 25);
    CreerJoueur(db, "SpookyChadi", 100, 25);
    CreerJoueur(db, "FreakyGabby", 100, 25);
    return;
}