#include "headers.h"


 hash :: hash(int ip) {
    p = ip;
    hashTable= new list<Nodelist<record> >*[p];
    fHash = &(dataBase::cmpHash);
    for (int i = 0; i < p; i++)
        hashTable[i] = new list<Nodelist<record> >;
}
hash :: ~hash() {
    for (int i = 0; i < p; i++) {
        hashTable[i]->clear();
        delete hashTable[i];
    }
    delete[] hashTable;
    fHash = 0;
}
void hash :: print(FILE * out, int k) {
    int i = 0;
    int t = 0;
    while ((t < k) && (i < p)) {
        if (hashTable[i]->getHead()) {
            int j = 0;
            Nodelist<Nodelist<record> > * tmp = hashTable[i]->getHead();
            while ((j < k) && (tmp)) {
                if (j == 0)
                    fprintf(out, "key = %d\n", i);
                tmp->getData()->print(out);
                tmp = tmp->getNext();
                j++;
            }
            if ((j == k)&&(tmp))
                fprintf(out, "...\n");
            if (j > 0)
                fprintf(out, "--------------------\n");
            t++;
        }
        i++;
    }
    fprintf(out, "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}
int hash :: getP() {return p;}
void hash :: setfHash(int (*f)(record*)) {fHash = f;}
void hash :: insert(Nodelist<record> *newNode) {
    Nodelist<Nodelist<record> > * a = new Nodelist<Nodelist<record> >;
    a->setNode(newNode);
    hashTable[fHash(newNode->getData())]->addToHead(a);
}
void hash :: NodesToList(list<Nodelist<record> >* root, command* currentCommand, list<Nodelist<Nodelist<record> > >* listApp) {
    Nodelist<Nodelist<record> > *tmp = root->head;
    while (tmp) {
        if (command :: satisfy(tmp->getData()->getData(), currentCommand)) {
            Nodelist<Nodelist<Nodelist<record> > >* x = new Nodelist<Nodelist<Nodelist<record> > >;
            x->setNode(tmp);
            listApp->addToHead(x);
        }
        tmp = tmp->getNext();
    }
}

list<Nodelist<Nodelist<record> > >*  hash :: select(command * currentCommand) {
    list<Nodelist<Nodelist<record> > > *listApp = new list<Nodelist<Nodelist<record> > >;
    if ((currentCommand->getCgroup() != COND_NONE) && (currentCommand->getOper() != OR)) {
        int value = fHash(currentCommand->getData());
        if (currentCommand->getCgroup() == EQ)
            NodesToList(hashTable[value], currentCommand, listApp);
        if (currentCommand->getCgroup() == GT)
            for (int i = value + 1; i < p; i++)
                NodesToList(hashTable[i], currentCommand, listApp);
        if (currentCommand->getCgroup() == GE)
            for (int i = value; i < p; i++)
                NodesToList(hashTable[i], currentCommand, listApp);
        if (currentCommand->getCgroup() == LT)
            for (int i = value-1; i >= 0; i--)
                NodesToList(hashTable[i], currentCommand, listApp);
        if (currentCommand->getCgroup() == LE)
            for (int i = value; i >= 0; i--)
                NodesToList(hashTable[i], currentCommand, listApp);
        if (currentCommand->getCgroup() == NE)
            for (int i = 0; i < p; i++)
                if (i != fHash(currentCommand->getData()))
                    NodesToList(hashTable[i], currentCommand, listApp);
    } else
        for (int i = 0; i < p; i++)
            NodesToList(hashTable[i], currentCommand, listApp);
    if (listApp->head == 0) {
        delete listApp;
        return 0;
    }
    return listApp;
}
list<Nodelist<Nodelist<record> > >* hash :: deleteElements(command * currentcommand) {
    list<Nodelist<Nodelist<record> > >* listApp = select(currentcommand);
    if (listApp == 0)
        return 0;
    Nodelist<Nodelist<Nodelist<record> > >* tmp = listApp->getHead();
    while (tmp) {
        hashTable[fHash(tmp->getData()->getData()->getData())]->GetOutByLink(tmp->getData());
        tmp = tmp->getNext();
    }
    return listApp;
}
