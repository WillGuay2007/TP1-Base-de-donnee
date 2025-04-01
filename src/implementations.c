#include "entrypoint.h"
#include "sqlite3.h"
#include "declarations.h"
#include "knob.h"

#include <time.h>
#include <stdlib.h>
#include <string.h>

//Lieux
void AfficherLesLieux(sqlite3* db) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT ID, Nom, Description FROM Lieux;";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return;
    }
    
    printf("\n=== LISTE DES LIEUX DISPONIBLES ===\n");
    int lieux_trouves = 0;
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* nom = (const char*)sqlite3_column_text(stmt, 1);
        const char* description = (const char*)sqlite3_column_text(stmt, 2);
        
        printf("Lieu #%d : %s\n", id, nom);
        printf("  %s\n\n", description);
        lieux_trouves = 1;
    }
    
    if (!lieux_trouves) {
        printf("Aucun lieu trouvé dans la base de données.\n");
    }
    
    sqlite3_finalize(stmt);
}
int TrouverLieuIDParNom(sqlite3* db, const char* Nom_Lieu) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT ID FROM Lieux WHERE Nom = ?;";
    int ID = -1;

    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return -1;
    }

    if (sqlite3_bind_text(stmt, 1, Nom_Lieu, -1, SQLITE_STATIC) != SQLITE_OK) {
         LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return -1;
    }

     if (sqlite3_step(stmt) == SQLITE_ROW) {
        ID = sqlite3_column_int(stmt, 0);
        printf("Lieu trouve avec ID: %d\n", ID);
    } else {
        printf("Aucun lieu trouve avec le nom: %s\n", Nom_Lieu);
    }
    
    sqlite3_finalize(stmt);

    return ID;

}
void AfficherLieu(sqlite3* db, int ID) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT Nom, Description FROM Lieux WHERE ID = ?";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_bind_int(stmt, 1, ID);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("----------\nLieux: %s\nDescription: %s\n----------\n", sqlite3_column_text(stmt, 0), sqlite3_column_text(stmt, 1));
    } else {
        printf("Aucun lieux trouvé.\n");
    }
    sqlite3_finalize(stmt);
}
void AfficherObjetsLieux(sqlite3* db, int ID) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT Nom, Description FROM Objets WHERE ID_lieu = ?";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return;
    }  

    if (sqlite3_bind_int(stmt, 1, ID) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        printf("----------\nObjet: %s\nDescription: %s\n----------\n", sqlite3_column_text(stmt, 0), sqlite3_column_text(stmt, 1));
    }
    
    sqlite3_finalize(stmt);
}
void AfficherEnnemisLieux(sqlite3* db, int ID) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT Nom, Vie, Force FROM Ennemis WHERE ID_lieu = ?";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return;
    }  

    if (sqlite3_bind_int(stmt, 1, ID) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        printf("----------\nNom: %s\nVie: %d\nForce: %d\n----------\n", sqlite3_column_text(stmt, 0), sqlite3_column_int(stmt, 1), sqlite3_column_int(stmt, 2));
    }
    
    sqlite3_finalize(stmt);
}
int ObtenirQuantiteObjetsLieu(sqlite3* db, int id_lieu) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT COUNT(*) FROM Objets WHERE ID_lieu= ?";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }  

    sqlite3_bind_int(stmt, 1, id_lieu);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }

    int quantite = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    return quantite;

}
int LieuTrouverObjetAleatoire(sqlite3* db, int id_lieu) {
    int ObjectCount = ObtenirQuantiteObjetsLieu(db, id_lieu);
    if (ObjectCount < 1) return 0;
    int Offset = rand() % ObjectCount; // Pour choisir une rangee aleatoire

    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT ID FROM Objets WHERE ID_lieu = ? LIMIT 1 OFFSET ?";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }  

    sqlite3_bind_int(stmt, 1, id_lieu);
    sqlite3_bind_int(stmt, 2, Offset);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }
    
    int ChoixItemID = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    return ChoixItemID;
    
}
int ObtenirQuantiteEnnemisLieu(sqlite3* db, int id_lieu) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT COUNT(*) FROM Ennemis WHERE ID_lieu = ?";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }  

    sqlite3_bind_int(stmt, 1, id_lieu);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }

    int quantite = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    return quantite;
}
int LieuTrouverEnnemiAleatoire(sqlite3* db, int id_lieu) {
    int EnnemiCount = ObtenirQuantiteEnnemisLieu(db, id_lieu);
    if (EnnemiCount < 1) return 0;
    int Offset = rand() % EnnemiCount; // Pour choisir une rangee aleatoire

    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT ID FROM Ennemis WHERE ID_lieu = ? LIMIT 1 OFFSET ?";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }  

    sqlite3_bind_int(stmt, 1, id_lieu);
    sqlite3_bind_int(stmt, 2, Offset);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }
    
    int ChoixEnnemiID = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    return ChoixEnnemiID;
}

//Joueur
int CreerJoueur(sqlite3* db, char* Nom, int Vie, int Force) {
    if (sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return -1;
    }
    
    sqlite3_stmt* stmt = NULL;
    const char* query = "INSERT INTO Joueurs(ID, Nom, Vie, Force, Position_ID) VALUES ((SELECT COALESCE(MAX(ID), 0) + 1 FROM Joueurs), ?, ?, ?, 1)";

    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return -1;
    }

    if (sqlite3_bind_text(stmt, 1, Nom, -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 2, Vie) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 3, Force) != SQLITE_OK) {
        
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return -1;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        return -1;
    }

    int nouveau_joueur_id = sqlite3_last_insert_rowid(db);

    sqlite3_finalize(stmt);

    if (sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return -1;
    }

    printf("Nouveau joueur cree avec l'ID : %d\n", nouveau_joueur_id);

    return nouveau_joueur_id;
}
void AfficherJoueurs(sqlite3* db) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT ID, Nom, Vie, Force, Position_ID FROM Joueurs;";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return;
    }  

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int JoueurID = sqlite3_column_int(stmt, 0);
        const unsigned char* Nom = (const unsigned char*)sqlite3_column_text(stmt, 1);
        int Vie = sqlite3_column_int(stmt, 2);
        int Force = sqlite3_column_int(stmt, 3);
        int PositionID = sqlite3_column_int(stmt, 4);
        printf("Joueur #%d\n----------\nNom: %s\nVie: %d\nForce: %d\nPosition_ID: %d\n----------\n", JoueurID, Nom, Vie, Force, PositionID);
    }

    sqlite3_finalize(stmt);
    
}
int ObtenirPositionJoueur(sqlite3* db, int id_joueur) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT Position_ID FROM Joueurs WHERE ID = ?;";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return -1;
    }  

    sqlite3_bind_int(stmt, 1, id_joueur);

    sqlite3_step(stmt);
    int Position = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return Position;
}
void AfficherPositionJoueur(sqlite3* db, int ID) {
    printf("Le joueur #%d est a la position:\n");
    AfficherLieu(db, ObtenirPositionJoueur(db, ID));
}
int GetPlayerCount(sqlite3* db) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT COUNT(ID) AS PlayerCount FROM Joueurs;";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return -1;
    }

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return -1;
    }

    int PlrCount = sqlite3_column_int(stmt, 0);
    
    sqlite3_finalize(stmt);

    return PlrCount;
}
int PromptPlayerChoice(sqlite3* db) {
    printf("Voulez vous choisir un joueur existant ou bien en creer un?\n1 = choisir\n2 = creer\n");
    int Choix = -1;
    scanf("%d", &Choix);
    if (Choix < 2) {
        if (GetPlayerCount(db) < 1) {
            printf("Aucun joueur existant. Un nouveau joueur sera cree avec les stats de base.\n");
            return CreerJoueur(db, "DefaultPlayer", 100, 25);
        }
        printf("Vous avec choisi de prendre un personnage deja existant.\nVeuillez entrer un ID valide.\n");
        int id = -1;
        AfficherJoueurs(db);
        printf("Vos choixs sont juste en haut...\n");
        while (id < 0 || id > GetPlayerCount(db)) {
            scanf("%d", &id);
        }
        printf("\033[0;32mVos informations:\033[0;0m\n");
        AfficherJoueurParID(db, id);
        return id;
    } else {
        printf("Vous avez choisi de creer un joueur.\nVeuillez entrer les stats\n");
        char nom[100];
        int vie;
        int force;
        printf("Veuillez entrer votre nouveau username\n");
        scanf("%s", nom);
        printf("Veuillez entrer votre vie\n");
        scanf("%d", &vie);
        printf("Veuillez entrer votre force\n");
        scanf("%d", &force);
        printf("Votre personnage est pret a l'initialisation...\n");
        int id = CreerJoueur(db, nom, vie, force);
        return id;
    }
}
void AfficherJoueurParID(sqlite3* db, int ID) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT ID, Nom, Vie, Force, Position_ID FROM Joueurs WHERE ID = ?;";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_bind_int(stmt, 1, ID);
    sqlite3_step(stmt);
    printf("Joueur:\n----------\nID = %d\nNom: %s\nVie: %d\nForce: %d\nPosition: %d\n", sqlite3_column_int(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_int(stmt, 2), sqlite3_column_int(stmt, 3), sqlite3_column_int(stmt, 4));
}
int DeplacerJoueur(sqlite3* db, int id_joueur, int id_lieu_destination) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT * FROM Lieux WHERE ID = ?;";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }  
    sqlite3_bind_int(stmt, 1, id_lieu_destination);
    if (sqlite3_step(stmt) != SQLITE_ROW)  {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    } // Ca verifie si c'est un lieu valide, si ca l'est pas, ca quitte la fonction
    stmt = NULL;
    query = "UPDATE Joueurs SET Position_ID = ? WHERE ID = ?;";
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }  
    sqlite3_bind_int(stmt, 1, id_lieu_destination);
    sqlite3_bind_int(stmt, 2, id_joueur);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    printf("Le joueur #%d a ete deplace au lieux #%d avec succes\n", id_joueur, id_lieu_destination);
    return 1;
}
int ObtenirForceJoueur(sqlite3* db, int id_joueur) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT Force FROM Joueurs WHERE ID = ?";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }  

    sqlite3_bind_int(stmt, 1, id_joueur);
    sqlite3_step(stmt);

    int Force = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return Force;
}
int ObtenirVieJoueur(sqlite3* db, int id_joueur) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT Vie FROM Joueurs WHERE ID = ?";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }  

    sqlite3_bind_int(stmt, 1, id_joueur);
    sqlite3_step(stmt);

    int Vie = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return Vie;
}

//Objet
int RamasserObjet(sqlite3* db, int id_joueur, int id_objet) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT o.ID FROM Objets o JOIN Joueurs j ON o.ID_lieu = j.Position_ID WHERE o.ID = ? AND j.ID = ?";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, id_objet);
    sqlite3_bind_int(stmt, 2, id_joueur);
    
    if (sqlite3_step(stmt) != SQLITE_ROW) {
        printf("\033[0;31mCet objet n'est pas disponible ici ou n'existe pas.\033[0m\n");
        sqlite3_finalize(stmt);
        return 0;
    }
    
    sqlite3_finalize(stmt);
    
    if (sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return 0;
    }
    
    stmt = NULL;
    const char* insert_query = "INSERT INTO Inventaire(ID, ID_joueur, ID_objet) VALUES ((SELECT COALESCE(MAX(ID), 0) + 1 FROM Inventaire), ?, ?)";
    
    if (sqlite3_prepare_v2(db, insert_query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        sqlite3_finalize(stmt);
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, id_joueur);
    sqlite3_bind_int(stmt, 2, id_objet);
    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        sqlite3_finalize(stmt);
        return 0;
    }
    
    sqlite3_finalize(stmt);
    
    if (sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return 0;
    }
    
    printf("\033[0;32mVous avez ramasse l'objet avec id: %d avec succes!\033[0m\n", id_objet);
    return 1;
}
int ObtenirQuantiteObjetInventaire(sqlite3* db, int id_joueur, int id_objet) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT COUNT(ID_objet) AS Quantite FROM Inventaire WHERE ID_objet = ? AND ID_joueur = ? GROUP BY ID_objet";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    } 

    sqlite3_bind_int(stmt, 1, id_objet);
    sqlite3_bind_int(stmt, 2, id_joueur);

    sqlite3_step(stmt);

    return sqlite3_column_int(stmt, 0);

}
void AfficherInfoObjet(sqlite3* db, int id_objet) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT Nom, Description, ForAttack, ForHeal, AttackOrHealPercentage FROM Objets WHERE ID = ?";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return;
    } 

    sqlite3_bind_int(stmt, 1, id_objet);

    sqlite3_step(stmt);
    unsigned const char* Nom = sqlite3_column_text(stmt, 0);
    unsigned const char* Description = sqlite3_column_text(stmt, 1);
    int CanAttack = sqlite3_column_int(stmt, 2);
    int CanHeal = sqlite3_column_int(stmt, 3);
    int Pourcentage = sqlite3_column_int(stmt, 4);

    printf("\033[1;4;32mInformations de l'objet avec l'ID %d:\033[0m\n----------\nNom: %s\nDescription: %s\nPeut attaquer = %d\nPeut heal = %d\nPourcentage d'attaque ou heal = %d\n----------\n", id_objet, Nom, Description, CanAttack, CanHeal, Pourcentage);
    sqlite3_finalize(stmt);
    return;
}
float ObtenirForceMultiplierObjet(sqlite3* db, int id_objet) {
    float ForceOrHealMultiplier = 1.0f;
    sqlite3_stmt* stmt = NULL;

    const char* query = "SELECT AttackOrHealPercentage FROM Objets WHERE ID = ?";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    } 

    sqlite3_bind_int(stmt, 1, id_objet);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        float Percentage = (float)sqlite3_column_double(stmt, 0) / 100;
        ForceOrHealMultiplier = 1.0 + Percentage;
    }
    sqlite3_finalize(stmt);

    return ForceOrHealMultiplier;
}
void AfficherObjetsDeHealInventaire(sqlite3* db, int id_joueur) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT o.ID, COUNT(*) AS Quantite FROM Inventaire i JOIN Objets o ON o.ID = i.ID_objet WHERE i.ID_joueur = ? AND o.ForHeal = 1 GROUP BY o.ID;";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return;
    }  

    sqlite3_bind_int(stmt, 1, id_joueur);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        AfficherInfoObjet(db, sqlite3_column_int(stmt, 0));
        printf("\033[1;32mQuantite de l'objet du haut = %d\n\033[0m", sqlite3_column_int(stmt, 1));
    }

    sqlite3_finalize(stmt);
}
void AfficherObjetsAttaqueInventaire(sqlite3* db, int id_joueur) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT o.ID, COUNT(*) AS Quantite FROM Inventaire i JOIN Objets o ON o.ID = i.ID_objet WHERE i.ID_joueur = ? AND o.ForAttack = 1 GROUP BY o.ID;";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return;
    }  

    sqlite3_bind_int(stmt, 1, id_joueur);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        AfficherInfoObjet(db, sqlite3_column_int(stmt, 0));
        printf("\033[1;32mQuantite de l'objet du haut = %d\n\033[0m", sqlite3_column_int(stmt, 1));
    }

    sqlite3_finalize(stmt);
}
int ObtenirQuantiteObjHealInventaire(sqlite3* db, int id_joueur) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT COUNT(*) AS Quantite FROM Inventaire i JOIN Objets o ON o.ID = i.ID_objet WHERE i.ID_joueur = ? AND o.ForHeal = 1;";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }  

    sqlite3_bind_int(stmt, 1, id_joueur);

    sqlite3_step(stmt);
    int Quantite = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return Quantite;
}
int ObtenirQuantiteObjAttaqueInventaire(sqlite3* db, int id_joueur) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT COUNT(*) AS Quantite FROM Inventaire i JOIN Objets o ON o.ID = i.ID_objet WHERE i.ID_joueur = ? AND o.ForAttack = 1;";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }  

    sqlite3_bind_int(stmt, 1, id_joueur);

    sqlite3_step(stmt);
    int Quantite = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return Quantite;
}
int OffrirChoixHealItem(sqlite3* db, int id_joueur) {

    int ChosenItem = 0;
    int ChoixOffset = 0;

    if (ObtenirCountChoixsHeal(db, id_joueur) <= 0) return 0;

    while (ChosenItem != 1)
    {
        printf("\033[0;33mVeuillez entrer un nombre entre 1 et le nombre d'items disponibles pour faire votre choix.\033[0m\n");
        scanf("%d", &ChoixOffset);
        if (ChoixOffset > ObtenirCountChoixsHeal(db, id_joueur) || ChoixOffset <= 0) {
            printf("Nombre invalide\n");
        } else {
            printf("Item choisi\n");
            ChosenItem = 1;
        }
    }

    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT ID_objet FROM Inventaire i JOIN Objets o ON o.ID = i.ID_objet WHERE i.ID_joueur = ? AND o.ForHeal = 1 GROUP BY i.ID_objet LIMIT 1 OFFSET ?;";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }  

    sqlite3_bind_int(stmt, 1, id_joueur);
    sqlite3_bind_int(stmt, 2, ChoixOffset - 1);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }
    int ChosenItemID = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return ChosenItemID;
}
int OffrirChoixAttackItem(sqlite3* db, int id_joueur) {
    int ChosenItem = 0;
    int ChoixOffset = 0;

    if (ObtenirCountChoixsAttaque(db, id_joueur) <= 0) return 0;

    while (ChosenItem != 1)
    {
        printf("\033[0;33mVeuillez entrer un nombre entre 1 et le nombre d'items disponibles pour faire votre choix.\033[0m\n");
        scanf("%d", &ChoixOffset);
        if (ChoixOffset > ObtenirCountChoixsAttaque(db, id_joueur) || ChoixOffset <= 0) {
            printf("Nombre invalide\n");
        } else {
            printf("Item choisi\n");
            ChosenItem = 1;
        }
    }

    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT ID_objet FROM Inventaire i JOIN Objets o ON o.ID = i.ID_objet WHERE i.ID_joueur = ? AND o.ForAttack = 1 GROUP BY i.ID_objet LIMIT 1 OFFSET ?;";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }  

    sqlite3_bind_int(stmt, 1, id_joueur);
    sqlite3_bind_int(stmt, 2, ChoixOffset - 1);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }
    int ChosenItemID = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    return ChosenItemID;
}
int ObtenirCountChoixsHeal(sqlite3* db, int id_joueur) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT COUNT(*) FROM (SELECT ID_objet FROM Inventaire i JOIN Objets o ON o.ID = i.ID_objet WHERE i.ID_joueur = ? AND o.ForHeal = 1 GROUP BY i.ID_objet);";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }  

    sqlite3_bind_int(stmt, 1, id_joueur);

    sqlite3_step(stmt);

    int HealChoices = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

}
int ObtenirCountChoixsAttaque(sqlite3* db, int id_joueur) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT COUNT(*) FROM (SELECT ID_objet FROM Inventaire i JOIN Objets o ON o.ID = i.ID_objet WHERE i.ID_joueur = ? AND o.ForAttack = 1 GROUP BY i.ID_objet);";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return 0;
    }  

    sqlite3_bind_int(stmt, 1, id_joueur);

    sqlite3_step(stmt);

    int AttackChoices = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    return AttackChoices;
}

//Inventaire
void AfficherInventaire(sqlite3* db, int id_joueur) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT o.ID, o.Nom, COUNT(o.ID) AS Quantite FROM Inventaire i JOIN Objets o ON o.ID = i.ID_objet WHERE i.ID_joueur = ? GROUP BY o.id";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_bind_int(stmt, 1, id_joueur);

    printf("\033[0;32mInventaire de joueur #%d:\033[0m\n----------\n", id_joueur);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int ID = sqlite3_column_int(stmt, 0);
        const unsigned char* Nom = sqlite3_column_text(stmt, 1);
        int Count = sqlite3_column_int(stmt, 2);
        printf("ID: %d\nNom: %s\nQuantite: %d\n----------\n", ID, Nom, Count);
    }
    
    sqlite3_finalize(stmt);
    return;
}
void SupprimerObjetInventaire(sqlite3* db, int id_joueur, int id_objet) {

    if (ObtenirQuantiteObjetInventaire(db, id_joueur, id_objet) < 1) return;

    sqlite3_stmt* stmt = NULL;
    const char* query = "DELETE FROM Inventaire WHERE ID = (SELECT ID FROM Inventaire WHERE ID_objet = ? AND ID_joueur = ? LIMIT 1)";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return;
    }  

    sqlite3_bind_int(stmt, 1, id_objet);
    sqlite3_bind_int(stmt, 2, id_joueur);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return;
    }

    sqlite3_finalize(stmt);

    printf("\033[0;31mItem avec id %d supprime de l'inventaire.\033[0m\n", id_objet);

}

//Ennemis
void AfficherInfoEnnemi(sqlite3* db, int id_ennemi) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT Nom, Vie, Force FROM Ennemis WHERE ID = ?";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return;
    } 

    sqlite3_bind_int(stmt, 1, id_ennemi);

    sqlite3_step(stmt);
    unsigned const char* Nom = sqlite3_column_text(stmt, 0);
    int Vie = sqlite3_column_int(stmt, 1);
    int Force = sqlite3_column_int(stmt, 2);

    printf("Informations de l'ennemi avec l'ID %d:\n----------\nNom: %s\nVie: %d\nForce: %d\n----------\n", id_ennemi, Nom, Vie, Force);
    sqlite3_finalize(stmt);
    return;
}

//Deplacement joueur
void Menu_Deplacement(sqlite3* db, int id_joueur) {
    int choix;
    printf("\033[1;33mVoulez vous avancer ou bien changer de lieu?\nAvancer = 1\nChanger de lieu = 2\033[0m\n");
    scanf("%d", &choix);
    if (choix < 2) {
        Avancer(db, id_joueur);
    } else {
        ChangerDeLieu(db, id_joueur);
    }

    Menu_Deplacement(db, id_joueur);
}
void Avancer(sqlite3* db, int id_joueur) {
    //Pour pas que ca soit trop complex j'ai fait un systeme de deplacement quand meme simple.
    //Le joueur peut avancer. il va soit tomber sur rien, un objet random ou bien un ennemi random. ca continue a l'infini.
    int RandomEvent = rand() % 3 + 1; // 1 = Rien, 2 = Ennemi, 3 = Objet
    if (RandomEvent == 1) {
        printf("Vous ne trouvez rien.\n");
    } else if (RandomEvent == 2) {
        printf("\033[4;31mVous etes tombe sur un ennemi:\033[0m\n");
        int EnnemiID = LieuTrouverEnnemiAleatoire(db, ObtenirPositionJoueur(db, id_joueur));
        AfficherInfoEnnemi(db, EnnemiID);
        printf("Voulez vous fuir ou bien le combattre?\n1: Fuire\n2: Combattre\n");
        int choix = 0;
        scanf("%d", &choix);
        if (choix < 2) {
            printf("\033[0;31mVous avez choisi de fuire...\n\033[0m");
        } else {
            printf("\033[0;32mVous vous appretez a combattre l'ennemi!\n\033[0m");
            InitialiserCombat(db, id_joueur, EnnemiID);
        }

    } else {
        printf("\033[4;32mVous avez trouve un objet:\033[0m\n");
        int ObjetID = LieuTrouverObjetAleatoire(db, ObtenirPositionJoueur(db, id_joueur));
        AfficherInfoObjet(db, ObjetID);
        printf("Voulez vous le ramasser ou bien le laisser ici?\n1 = Ramasser\n2 = Laisser ici\n");
        int choix = 0;
        scanf("%d", &choix);
        if (choix < 2) {
            RamasserObjet(db, id_joueur, ObjetID);
        } else {
            printf("\033[0;31mVous avez choisi d'ignorer l'objet.\033[0m\n");
        }
    }
}
void ChangerDeLieu(sqlite3* db, int id_joueur) {
    printf("Vous avez choisi de changer de lieu. Veuillez entrer votre choix:\n");
    int Choix = 0;
    scanf("%d", &Choix);
    if (DeplacerJoueur(db, id_joueur, Choix) == 1) AfficherLieu(db, Choix);
}

//Systeme de combat
int CalculerDegats(int force) {
    int degats_base = force;
    
    int variation = (rand() % 41) - 20; //Chiffre entre -20 et 20 (ca va etre le pourcentage)

    float variation_exacte = (degats_base * variation) / 100.0f;
    int degats_finaux = degats_base + (int)variation_exacte; // Mon systeme fait en sorte que la valeur maximale de variation est la force de base + 20% de celle ci, donc 120%. (80% est le minimum)
    
    if (degats_finaux < 1) degats_finaux = 1; // Si le degat final est negatif, ca va le remettre a 1, le minimum
    
    return degats_finaux;
}
int InitialiserCombat(sqlite3* db, int id_joueur, int id_ennemi) {
    Ennemi ennemi = {0};
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT Nom, Vie, Force FROM Ennemis WHERE ID = ?"; // Obtenir les informations de l'ennemi
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return -1;
    } 

    sqlite3_bind_int(stmt, 1, id_ennemi);

    sqlite3_step(stmt);
    const unsigned char* NomEnnemi = sqlite3_column_text(stmt, 0);
    int VieEnnemi = sqlite3_column_int(stmt, 1);
    int ForceEnnemi = sqlite3_column_int(stmt, 2);

    //Inserer les valeurs de l'ennemi dans la structure temporaire
    ennemi.vie_max = VieEnnemi;
    ennemi.force = ForceEnnemi;
    ennemi.vie_actuelle = VieEnnemi;
    ennemi.id = id_ennemi;
    strcpy(ennemi.nom, (const char*)NomEnnemi);

    sqlite3_finalize(stmt);

    int ForceJoueur = ObtenirForceJoueur(db, id_joueur);
    int VieMaxJoueur = ObtenirVieJoueur(db, id_joueur);
    int VieActuelleJoueur = VieMaxJoueur;

    //Boucle de combat
    while (VieActuelleJoueur > 0)
    {
        
        int EnnemiSkipTurn = 0;

        //JOUEUR
        printf("\033[4;1;33mSTATS DU COMBAT:\033[0m\n----------\nVIE DU JOUEUR:\n----------\n%d\n----------\nVIE DE L'ENNEMI:\n----------\n%d\n----------\n", VieActuelleJoueur, ennemi.vie_actuelle);
        printf("\nVeuillez choisir votre option:\nFuire = 1\nAttaquer = 2\nAttaquer avec objet = 3\nHeal (ennemi n'attaquera pas) = 4\n");
        int choix = 0;
        scanf("%d", &choix);
        if (choix < 2) {
            printf("\033[0;31mVous avez choisi de fuire.\n");
            int ChanceDeSucces = rand() % 4; // 0 - 1 - 2 - 3 (1 chance sur 4 de fuire)
            if (ChanceDeSucces == 0) {
                return 0;
            } else {
                printf("Malheureusement, vous n'avez pas reussit a fuire...\n");
            }
        } else if (choix == 2) {
            AttaquerEnnemi(&ennemi, ForceJoueur);
        } else if (choix == 3) {
            if (ObtenirQuantiteObjAttaqueInventaire(db, id_joueur) < 1) {
                printf("Vous n'avez aucun objet d'attaque. Vous allez attaquer regulierement.\n");
                AttaquerEnnemi(&ennemi, ForceJoueur);
            } else {
                printf("\033[0;32mVous vous appretez a attaquer l'ennemi avec un objet.\nVoici vos choixs:\n\033[0m");
                AfficherObjetsAttaqueInventaire(db, id_joueur);
                int ChosenItem = OffrirChoixAttackItem(db, id_joueur);
                printf("\033[32mVous vous appretez a attaquer l'ennemi avec cet objet:\033[0m\n");
                AfficherInfoObjet(db, ChosenItem);
                AttaquerEnnemi(&ennemi, (float)ForceJoueur * ObtenirForceMultiplierObjet(db, ChosenItem));
                SupprimerObjetInventaire(db, id_joueur, ChosenItem);
            }
        } else {
            if (ObtenirQuantiteObjHealInventaire(db, id_joueur) < 1) {
                printf("Vous avez aucun objet de heal. L'ennemi peut toujours vous attaquer. Vous allez faire une attaque reguliere.\n");
                int DegatsInfligés = AttaquerEnnemi(&ennemi, ForceJoueur);
            } else {
                EnnemiSkipTurn = 1;
                printf("Vous avez choisi de vous heal.\nVoici la liste de vos objets qui peuvent heal:\n");
                AfficherObjetsDeHealInventaire(db, id_joueur);
                int ChosenItem = OffrirChoixHealItem(db, id_joueur);
                printf("Item choisi:\n");
                AfficherInfoObjet(db, ChosenItem);
            }
        }

        if (ennemi.vie_actuelle <= 0) {
            printf("\033[4;32mVous avez gagne le combat!\033[0m\n");
            return 1; // Retourner 1 si le joueur gagne, sinon 2
        }

        //ENNEMI
        if (EnnemiSkipTurn == 0) {
            int Degats = CalculerDegats(ennemi.force);
            VieActuelleJoueur -= Degats;
            printf("\033[0;31m%s vous a inflige %d de degats!\n\033[0m", ennemi.nom, Degats);
        } else {
            printf("\033[0;32mVous avez heal. L'ennemi ne peut pas vous attaquer pour ce tour.\n\033[0m");
        }

    }
    printf("\033[0;31mVous avez perdu le combat...\n\033[0m");
    return 2; // Si le joueur perd
}
int AttaquerEnnemi(Ennemi* ennemi, int forceJoueur) {
    int Degats = CalculerDegats(forceJoueur);
    ennemi->vie_actuelle -= Degats;
    printf("\033[1;32mVous avez inflige %d de degats a %s!\n\033[0m", Degats, ennemi->nom);
    return Degats;
}