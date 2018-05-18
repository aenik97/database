#include "headers.h"
dataBase :: dataBase(int p) {
    dblist = new list<record>;
    tName = new rbTree;
    tPhone = new rbTree;
    hashTable = new hash(p);
}
dataBase :: ~dataBase() {
    delete tName;
    delete tPhone;
    delete hashTable;
    delete dblist;
}

int dataBase :: build(FILE * in) {
    tName->setCmp(&cmpName);
    tPhone->setCmp(&cmpPhone);
    hashTable->setfHash(&cmpHash);
    Nodelist<record> * a = new Nodelist<record>;
    int ret;
    while ((ret = a->readNode(in)) == NO_PROBLEMS) {
        if (tPhone->insertNode(a) != ERROR) {
            dblist->addToTail(a);
            tName->insertNode(a);
            hashTable->insert(a);
            a = new Nodelist<record>;
        }
    }
    delete a;
    if (ret == ERROR) {
        if (!feof(in)) {
            printf("File with database is incorrect\n");
            return ERROR;
        }
    }
    return NO_PROBLEMS;
}
void dataBase :: print(FILE * out) {
    fprintf(out, "list\n");
    dblist->printList(out);
    fprintf(out, "tName\n");
    tName->print(out, tName->getRoot(), 3);
    fprintf(out, "tPhone\n");
    tPhone->print(out, tPhone->getRoot(), 3);
    fprintf(out, "hashTable\n");
    hashTable->print(out, 4);
}
void dataBase :: myCheck(int i) {
    printf("tName\n");
    tName->fullCheck(i);
    printf("tPhone\n");
    tPhone->fullCheck(i);
}

int dataBase :: apply(FILE *resOut, command *currentCommand) {
    int ret = 0;
    currentCommand->printCommand(resOut);
    if (currentCommand->getType() == QUIT) {
        fprintf(resOut, "Goodbye!\n");
        return QUIT;
    }
    if (currentCommand->getType() == INSERT) {
        record *x = new record;
        x->init(currentCommand->getData()->getName(), currentCommand->getData()->getPhone(), currentCommand->getData()->getGroup());
        Nodelist<record> * y = new Nodelist<record>;
        y->init(x,0,0);
        if (tPhone->insertNode(y) != ERROR) {
            tName->insertNode(y);
            dblist->addToHead(y);
            hashTable->insert(y);
            fprintf(resOut, "%s %d %d was inserted\n", x->getName(), x->getPhone(), x->getGroup());
        } else {
            fprintf(resOut, "%s %d %d was NOT inserted\n", x->getName(), x->getPhone(), x->getGroup());
            delete y;
            delete x;
        }
    }
    if (currentCommand->getType() == DELETE) {
        list<nodeTree> *listApptName = tName->deleteElements(currentCommand);
        list<nodeTree> *listApptPhone = tPhone->deleteElements(currentCommand);
        list<Nodelist<Nodelist<record> > >*listAppHashTable = hashTable->deleteElements(currentCommand);
        if (listApptName == 0) {
            fprintf(resOut, "0 elements was deleted\n");
            return 0;
        }
        int len = listApptName->getLen();
        delete listApptName;
        delete listApptPhone;
        Nodelist<Nodelist<Nodelist<record> > > * tmp = listAppHashTable->getHead();
        while (tmp) {
            delete tmp->getData()->getData()->getData();
            dblist->deleteByLink(tmp->getData()->getData());
            tmp = tmp->getNext();
        }
        delete listAppHashTable;
        fprintf(resOut, "%d elements was deleted\n", len);
    }
    if (currentCommand->getType() == SELECT) {
        list<nodeTree> * tNameSelect = 0;
        list<nodeTree> * tPhoneSelect = 0;
        list<Nodelist<Nodelist<record> > > * HashTableSelect = 0;
        int len = 0;
        if ((currentCommand->getOper() != OR) || (currentCommand->getCname() != LIKE) || (currentCommand->getCgroup() != LIKE) || (currentCommand->getCphone() != LIKE)) {
            if (currentCommand->getCgroup() != COND_NONE) {
                HashTableSelect = hashTable->select(currentCommand);
                if (HashTableSelect) {
                    len = HashTableSelect->getLen();
                    HashTableSelect->printList(resOut, 5);
                }
            }
            else
                if (currentCommand->getCphone() != COND_NONE) {
                    tPhoneSelect = tPhone->select(currentCommand);
                    if (tPhoneSelect) {
                        len = tPhoneSelect->getLen();
                        tPhoneSelect->printList(resOut, 5);
                        tPhoneSelect->clear();
                        delete tPhoneSelect;
                    }
                }
                else {
                    tNameSelect = tName->select(currentCommand);
                    if (tNameSelect) {
                        len = tNameSelect->getLen();
                        tNameSelect->printList(resOut, 5);
                        tNameSelect->clear();
                        delete tNameSelect;
                    }
                }
            fprintf(resOut, "%d elements was selected\n", len);
        } else {
            Nodelist<record> *tmp = dblist->getHead();
            int i = 0;
            while (tmp) {
                if (command :: satisfy(tmp->getData(), currentCommand)) {
                    if (i < 5)
                        tmp->getData()->print(resOut);
                    if (i == 5)
                        fprintf(resOut,"...\n");
                    i++;
                }
                tmp=tmp->getNext();
            }
            fprintf(resOut,"%d elements was selected\n", i);
        }
    }
    fprintf(resOut, "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    return ret;
}




int dataBase :: ExecuteCommandsFromFile(FILE * inCommand, FILE * resOut) {
    int i = 1, ret = 0;
    command *currentCommand = new command;
    while ((ret = (currentCommand->readCommand(inCommand))) != NOT_COMMAND) {
        printf("#%d\n", i);
        fprintf(resOut, "#%d\n", i);
        if ((ret = this->apply(resOut, currentCommand)) == QUIT)
            break;
        i++;
    }
    delete currentCommand;
    return ret;
}
int dataBase :: stdcmp(record* a, record * b) {return a->getGroup() - b->getGroup();}
int dataBase :: cmpPhone(record * a, record * b) {return a->getPhone() - b->getPhone();}
int dataBase :: cmpGroup(record * a, record * b) {return a->getGroup() - b->getGroup();}
int dataBase :: cmpName(record *a, record *b) {return strcmp(a->getName(), b->getName());}
int dataBase :: cmpHash(record * a) {return a->getGroup()%100;}
