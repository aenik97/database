#ifndef RECORD_H
#define RECORD_H



class record {
private:
    char *name;
    int phone;
    int group;
    friend class command;
    friend class nodeTree;
    friend class rbTree;
    friend class database;
public:
    int getPhone();
    int getGroup();
    char *getName();
    void print(FILE *out);
    void setPhone(int nPhone);
    void setGroup(int nGroup);
    record(char *iname = 0, int iphone = 0, int igroup = 0);
    ~record();
    int init(char *newName, int phone, int group);
    int readRecord(FILE * in);
};
#endif // RECORD_H
