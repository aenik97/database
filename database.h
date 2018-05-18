#ifndef DATABASE_H
#define DATABASE_H



class dataBase {
private:
    list<record> * dblist;
    rbTree * tName;
    rbTree * tPhone;
    hash * hashTable;
public:
    dataBase(int p);
    ~dataBase();
    int build(FILE * in);
    void print(FILE * out);
    int apply(FILE *resOut, command *currentCommand);
    void myCheck(int i);
    static int stdcmp(record* a, record * b);
    static int cmpPhone(record * a, record * b);
    static int cmpGroup(record * a, record * b);
    static int cmpName(record *a, record *b);
    static int cmpHash(record * a);
    int ExecuteCommandsFromFile(FILE * in, FILE * resOut);
};

#endif // DATABASE_H
