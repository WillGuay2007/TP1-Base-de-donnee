#pragma once

#include "entrypoint.h"
#include "sqlite3.h"
#include "knob.h"

#include <time.h>
#include <stdlib.h>

#define LOG_SQLITE3_ERROR(db) knob_log(KNOB_ERROR, "%s:%d: SQLITE3 ERROR: %s\n", __FILE__, __LINE__, sqlite3_errmsg(db))

//Structures pour m'aider a la simplicité

typedef struct {
    int id;
    char nom[100];
    int vie_max;
    int vie_actuelle;
    int force;
} Ennemi;

//Lieux
void AfficherLesLieux(sqlite3* db);
void AfficherLieu(sqlite3* db, int ID);
int TrouverLieuIDParNom(sqlite3* db, const char* Nom_Lieu);
void AfficherObjetsLieux(sqlite3* db, int ID);
void AfficherEnnemisLieux(sqlite3* db, int ID);
int ObtenirQuantiteObjetsLieu(sqlite3* db, int id_lieu);
int LieuTrouverObjetAleatoire(sqlite3* db, int id_lieu);
int ObtenirQuantiteEnnemisLieu(sqlite3* db, int id_lieu);
int LieuTrouverEnnemiAleatoire(sqlite3* db, int id_lieu);

//Joueur
int CreerJoueur(sqlite3* db, char* Nom, int Vie, int Force);
void AfficherJoueurs(sqlite3* db);
int ObtenirPositionJoueur(sqlite3* db, int id_joueur);
void AfficherPositionJoueur(sqlite3* db, int ID);
int GetPlayerCount(sqlite3* db);
int PromptPlayerChoice(sqlite3* db);
void AfficherJoueurParID(sqlite3* db, int ID);
int DeplacerJoueur(sqlite3* db, int id_joueur, int id_lieu_destination);
int ObtenirForceJoueur(sqlite3* db, int id_joueur);
int ObtenirVieJoueur(sqlite3* db, int id_joueur);

//Objet
int RamasserObjet(sqlite3* db, int id_joueur, int id_objet);
int ObtenirQuantiteObjetInventaire(sqlite3* db, int id_joueur, int id_objet);
void AfficherInfoObjet(sqlite3* db, int id_objet);
float ObtenirForceMultiplierObjet(sqlite3* db, int id_objet);
void AfficherObjetsDeHealInventaire(sqlite3* db, int id_joueur);
void AfficherObjetsAttaqueInventaire(sqlite3* db, int id_joueur);
int ObtenirQuantiteObjHealInventaire(sqlite3* db, int id_joueur);
int ObtenirQuantiteObjAttaqueInventaire(sqlite3* db, int id_joueur);
int OffrirChoixHealItem(sqlite3* db, int id_joueur);
int OffrirChoixAttackItem(sqlite3* db, int id_joueur);
int ObtenirCountChoixsHeal(sqlite3* db, int id_joueur);
int ObtenirCountChoixsAttaque(sqlite3* db, int id_joueur);
int ObtenirHealObjet(sqlite3* db, int id_objet);

//Inventaire
void AfficherInventaire(sqlite3* db, int id_joueur);
void SupprimerObjetInventaire(sqlite3* db, int id_joueur, int id_objet);

//Ennemis
void AfficherInfoEnnemi(sqlite3* db, int id_ennemi);

//Deplacement joueur
void Menu_Deplacement(sqlite3* db, int id_joueur);
void Avancer(sqlite3* db, int id_joueur);
void ChangerDeLieu(sqlite3* db, int id_joueur);

//Systeme de combat
int HealPlayer(sqlite3* db, int MaxHealth, int* PlayerHealth, int id_objet)
int CalculerDegats(int force);
int InitialiserCombat(sqlite3* db, int id_joueur, int id_ennemi);
int AttaquerEnnemi(Ennemi* ennemi, int forceJoueur);