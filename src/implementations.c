#include "entrypoint.h"
#include "sqlite3.h"
#include "declarations.h"
#include "knob.h"

#include <time.h>
#include <stdlib.h>

//LIEUX
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

//JOUEUR
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