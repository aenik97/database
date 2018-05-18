#ifndef CLASSCOMMAND
#define CLASSCOMMAND


enum cmd_type { CMD_NONE, QUIT, SELECT, INSERT, DELETE };
enum cond_type {COND_NONE, EQ, NE, LT, GT, LE, GE, LIKE };
enum op_type {OP_NONE, AND, OR };


class command {
private:
    cmd_type type;
    cond_type c_name;
    cond_type c_phone;
    cond_type c_group;
    op_type oper;
    record * data;
    friend class rbTree;
    friend class database;
public:
    static int opDetect(char **a, int i);
    static int typeDetect(char **a);
    static int condDetect(char ** a, int i);
    static int isNumber(char *a);
    cond_type getCname();
    cond_type getCphone();
    cond_type getCgroup();
    op_type getOper();
    cmd_type getType();
    record * getData();
    void setData(record * newData);
    void setCname(cond_type);
    void setCphone(cond_type);
    void setCgroup(cond_type);
    void setOper(op_type);
    void setType(cmd_type);
    command(record * idata = 0);
    ~command();
    void printCommand(FILE *out);
    int readCommand(FILE *in);
    int init(char *line);
    int fieldDetect(char **a, int i);
    static cond_type inverse(cond_type cond);
    static bool satisfyName(record *a, command* currentCommand);
    static bool satisfyGroup(record *a, command* currentCommand);
    static bool satisfyPhone(record *a, command* currentCommand);
    static bool satisfy(record *a, command* currentCommand);
};
#endif // CLASSCOMMAND
