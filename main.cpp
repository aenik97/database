#include "headers.h"

using namespace std;
int main() {
    printf("START\n");
    FILE *inDB;
    if (!(inDB = fopen("db_100.txt", "r"))) {
        printf("Cannot open DB\n");
        return 0;
    }
    FILE *inCommand;
    if (!(inCommand = fopen("commands0.txt", "r"))) {
        printf("Cannot open Commands\n");
        fclose(inDB);
        return 0;
    }
    FILE *resOut;
    if (!(resOut = fopen("res.txt", "w"))) {
        printf("Cannot open res.txt\n");
        fclose(inDB);
        fclose(inCommand);
        return 0;
    }

    dataBase* db = new dataBase(100);
    printf("READ AND CONSTRUCTION OF STRUCTURES \n");
    if (db->build(inDB) == -3) {
        fclose(inDB);
        fclose(inCommand);
        fclose(resOut);
        delete db;
        printf("File with database is incorrect\n");
        printf("\nUNSUCCESS\n");
        return 0;
    }
    fclose(inDB);
    db->print(stdout);
    printf("PRINT STRUCTURES TO RES.TXT\n");
    db->print(resOut);


    printf("WORK\n");
    if (db->ExecuteCommandsFromFile(inCommand, resOut) == 1) {
        db->print(resOut);
        printf("\nSUCCESS\n");
    }
    else
        printf("\nUNSUCCESS\n");
    fclose(inCommand);
    fclose(resOut);
    delete db;
    return 0;
}
