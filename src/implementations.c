#include "entrypoint.h"
#include "sqlite3.h"
#include "declarations.h"
#include "knob.h"

#include <time.h>
#include <stdlib.h>

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
        return -1;
    }  
    sqlite3_bind_int(stmt, 1, id_lieu_destination);
    if (sqlite3_step(stmt) != SQLITE_ROW)  {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return -1;
    } // Ca verifie si c'est un lieu valide, si ca l'est pas, ca quitte la fonction
    stmt = NULL;
    query = "UPDATE Joueurs SET Position_ID = ? WHERE ID = ?;";
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return -1;
    }  
    sqlite3_bind_int(stmt, 1, id_lieu_destination);
    sqlite3_bind_int(stmt, 2, id_joueur);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        LOG_SQLITE3_ERROR(db);
        sqlite3_finalize(stmt);
        return -1;
    }

    sqlite3_finalize(stmt);
    printf("Le joueur #%d a ete deplace au lieux #%d avec succes\n", id_joueur, id_lieu_destination);
    return 1;
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
        printf("Cet objet n'est pas disponible ici ou n'existe pas.\n");
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
    
    printf("Vous avez ramasse l'objet avec succes!\n");
    return 1;
}