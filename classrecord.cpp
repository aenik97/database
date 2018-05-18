
#include "headers.h"

int record :: getPhone() {return phone;}
int record :: getGroup() {return group;}
char * record :: getName() {return name;}
void record :: print(FILE *out) {fprintf(out,"%s %d %d \n",name, phone, group);}
void record :: setPhone(int nPhone) {phone = nPhone;}
void record :: setGroup(int nGroup) {group = nGroup;}

record :: record(char *iname, int iphone, int igroup) {
    name = iname;
    phone = iphone;
    group = igroup;
}
record :: ~record() {
    if (name) delete []name;
    name = 0;
    phone = 0;
    group = 0;
}
int record :: init(char *newName, int phone, int group) {
    this->~record();
    this->phone = phone;
    this->group = group;
    if (newName) {
        name = new char[strlen(newName) + 1];
        if (name)
            strcpy(name, newName);
        else {
            name = 0;
            return ERROR;
        }
    }
    else
        name = 0;
    return NO_PROBLEMS;
}
int record :: readRecord(FILE * in) {
    char name[LEN];
    for (int i = 0; i < LEN; i++) {
        name[i] = 0;
    }
    char rec[LEN + 2];
    int phone = 0;
    int group = 0;
    if (fgets(rec, LEN, in) == 0) {
        return EMPTY;//ERROR
    }
    int i = 0;
    int t = 1;
    int namei = 0;
    while ((rec[i] != 0) && (rec[i] != '\n')) {
        while (rec[i] == ' ') {
            if ((i > 0) && (rec[i - 1] != ' '))
                t++;
            i++;
        }

        if (t > 3)
            return ERROR;
        if (t == 1) {
            if (((toupper(rec[i]) >= 65) && (toupper(rec[i]) <= 90)) || ((rec[i] >= 48) && (rec[i] <= 57))) {
                name[namei] = rec[i];
                namei++;
            } else
                return ERROR;
        } else
            if ((rec[i] >= 48) && (rec[i] <= 57)) {
                if (t == 2) {
                    phone *= 10;
                    phone += rec[i] - 48;
                }
                if (t == 3) {
                    group *= 10;
                    group += rec[i] - 48;
                }
            }
        i++;
    }
    this->init(name, phone, group);
    return NO_PROBLEMS;
}
