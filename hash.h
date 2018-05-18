#ifndef HASH_H
#define HASH_H



class hash {
private:
    int p;
    list<Nodelist<record> > **hashTable;
    int (*fHash)(record *);
public:
    hash(int ip);
    ~hash();
    int getP();
    void setfHash(int (*f)(record*));
    void insert(Nodelist<record> *);
    void print(FILE * out, int k);
    static void NodesToList(list<Nodelist<record> >* root, command* currentCommand, list<Nodelist<Nodelist<record> > >* listApp);
    list<Nodelist<Nodelist<record> > >* select(command * currentCommand);
    list<Nodelist<Nodelist<record> > >* deleteElements(command * currentcommand);
};

#endif // HASH_H
