#include "entrypoint.h"
#include "sqlite3.h"
#include "declarations.h"
#include "knob.h"

#include <time.h>
#include <stdlib.h>

void see_state_fruits(sqlite3* db,char* state_name){
    sqlite3_stmt* stmt = NULL;
    char content[] =
    "SELECT fruit,price\n"
    "FROM FruitsForSale\n"
    "WHERE etat=?;";
    int ret = sqlite3_prepare_v2(db,content,-1,&stmt,NULL);

    if(ret != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }

    if(sqlite3_bind_text(stmt,1,state_name,-1,SQLITE_STATIC) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }

    for(int ret = sqlite3_step(stmt);ret != SQLITE_DONE;ret = sqlite3_step(stmt)){
        if(ret != SQLITE_ROW){
            LOG_SQLITE3_ERROR(db);
        }
        int column = 0;
        const unsigned char* fruit = sqlite3_column_text(stmt,column++);
        float price = sqlite3_column_double(stmt,column);
        knob_log(KNOB_INFO,"Etat %s a le fruit %s avec le prix %.2f$",state_name,fruit,price);
    }

    sqlite3_finalize(stmt);
}
const char* fruits[] = {
    "Banana",
    "Orange",
    "Grape",
    "Pear",
    "Kumqat"
};
const char* states[] = {
    "CA",
    "SC",
    "NC",
    "FL",
    "HA"
};

void add_random_fruit(sqlite3* db){
    if (sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
        return;
    }
    sqlite3_stmt* stmt = NULL;
    char content[] =
    "INSERT INTO FruitsForSale(fruit,etat,price)\n"
    "VALUES\n"
    "(?,?,?);";
    int ret = sqlite3_prepare_v2(db,content,-1,&stmt,NULL);
    int fruit = rand() %5;
    int state = rand() %5;
    double price = ((rand()+1) % 100) /100 + rand() % 2;
    if(sqlite3_bind_text(stmt,1,fruits[fruit],-1,SQLITE_STATIC) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    if(sqlite3_bind_text(stmt,2,states[state],-1,SQLITE_STATIC) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    if(sqlite3_bind_double(stmt,3,price) != SQLITE_OK){
        LOG_SQLITE3_ERROR(db);
    }
    ret = sqlite3_step(stmt);
    if (sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
    }
}

void afficher_tous_lieux(sqlite3* db) {
    sqlite3_stmt* stmt = NULL;
    const char* query = "SELECT ID, Nom, Description FROM Lieux;";
    
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        LOG_SQLITE3_ERROR(db);
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
        sqlite3_finalize(stmt);
        return -1;
    }

    if (sqlite3_bind_text(stmt, 1, Nom_Lieu, -1, SQLITE_STATIC) != SQLITE_OK) {
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
