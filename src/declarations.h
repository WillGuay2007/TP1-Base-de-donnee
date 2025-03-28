#pragma once

#include "entrypoint.h"
#include "sqlite3.h"
#include "knob.h"

#include <time.h>
#include <stdlib.h>

#define LOG_SQLITE3_ERROR(db) knob_log(KNOB_ERROR, "%s:%d: SQLITE3 ERROR: %s\n", __FILE__, __LINE__, sqlite3_errmsg(db))

//LIEUX
void AfficherLesLieux(sqlite3* db);
void AfficherLieu(sqlite3* db, int ID);
int TrouverLieuIDParNom(sqlite3* db, const char* Nom_Lieu);
void AfficherObjetsLieux(sqlite3* db, int ID);
void AfficherEnnemisLieux(sqlite3* db, int ID);

//Joueur
int CreerJoueur(sqlite3* db, char* Nom, int Vie, int Force);
void AfficherJoueurs(sqlite3* db);
int ObtenirPositionJoueur(sqlite3* db, int id_joueur);
void AfficherPositionJoueur(sqlite3* db, int ID);
int GetPlayerCount(sqlite3* db);
int PromptPlayerChoice(sqlite3* db);
void AfficherJoueurParID(sqlite3* db, int ID);