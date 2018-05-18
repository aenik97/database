#include "headers.h"
void command :: printCommand(FILE *out) {
    switch (int(type)) {
    case 0: fprintf(out,"CMD_NONE  ");
        break;
    case 1: fprintf(out,"QUIT      ");
        break;
    case 2: fprintf(out,"SELECT    ");
        break;
    case 3: fprintf(out,"INSERT    ");
        break;
    case 4: fprintf(out,"DELETE    ");
        break;
    }
    for(int i = 0; i < 3; i++) {
        int j;
        if (i == 0) j = int(c_name);
        if (i == 1) j = int(c_phone);
        if (i == 2) j = int(c_group);
        switch (j) {
        case 0: fprintf(out,"NONE    ");
            break;
        case 1: fprintf(out,"EQ      ");
            break;
        case 2: fprintf(out,"NE      ");
            break;
        case 3: fprintf(out,"LT      ");
            break;
        case 4: fprintf(out,"GT      ");
            break;
        case 5: fprintf(out,"LE      ");
            break;
        case 6: fprintf(out,"GE      ");
            break;
        case 7: fprintf(out,"LIKE    ");
            break;
        }
    }
    switch (int(oper)) {
    case 0: fprintf(out,"OP_NONE  ");
        break;
    case 1: fprintf(out,"AND      ");
        break;
    case 2: fprintf(out,"OR       ");
        break;
    }
    fprintf(out,"%s   ", data->name);
    fprintf(out,"%d     ", data->phone);
    fprintf(out,"%d\n", data->group);
}

int command :: readCommand(FILE *in) {
    char *comm = new char[LEN];
    if (fgets(comm, LEN, in) == 0) {
        delete []comm;
        if (feof(in))
            return NOT_COMMAND;
        else
            return ERROR;
    }
    int len = 0;
    for(len = 0; (comm[len] != '\n')&&(comm[len] != '\0') && (comm[len] != ';');len++);
    comm[len] = '\0';
    return init(comm);
}

int command :: init(char *line) {
    this->~command();
    record * tmp = new record;
    this->data = tmp;
    int ret = NO_PROBLEMS;
    char **a = new char*[N];
    for (int i = 0; i < N; i++)
        a[i] = new char[M];
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            a[i][j] = '\0';
    int len = 0;
    for(len = 0; (line[len] != '\n')&&(line[len] != '\0') && (line[len] != ';');len++)
        if ((line[len] == ')') || (line[len] == '(') || (line[len] == ';') || (line[len] == ','))
            line[len] = ' ';
    line[len] = '\0';
    sscanf(line, "%s%s%s%s%s%s%s%s%s%s", a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
    //printf("%s %s %s %s %s %s %s %s %s %s\n", a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9]);
    if (typeDetect(a) != NOT_COMMAND)
        type = cmd_type(typeDetect(a));
    else
        ret = NOT_COMMAND;
    if (type == SELECT)
        ret = fieldDetect(a, 3);
    if (type == DELETE)
        ret = fieldDetect(a, 2);
    if (type == INSERT) {
        int t = 0;
        if ((t = isNumber(a[2])) < 0)
            ret = NOT_COMMAND;
        data->phone = t;
        t = 0;
        if ((t = isNumber(a[3])) < 0)
            ret = NOT_COMMAND;
        data->group = t;
        int len = strlen(a[1]);
        char * name2 = new char[len + 1];
        for(int j = 0; j < len; j++)
            name2[j] = a[1][j];
        name2[len] = '\0';
        data->name = name2;
    }
    for (int i = 0; i < N; i++)
        delete[] a[i];
    delete[] a;
    delete[] line;//delete when client
    this->printCommand(stdout);
    return ret;
}
int command :: fieldDetect(char **a, int i) {
    if (strcmp(a[i],"name") == 0) {
        if (condDetect(a, i + 1) >= 0)
            c_name = cond_type(condDetect(a, i + 1));
        else
            return NOT_COMMAND;
        char * name2 = new char[strlen(a[i+2]) + 1];
        for(int j = 0; a[i + 2][j] != '\0'; j++)
            name2[j] = a[i + 2][j];
        name2[strlen(a[i + 2])] = '\0';
        data->name = name2;
        if (opDetect(a, i + 3) > 0) {
            oper = op_type(opDetect(a, i + 3));
            if (fieldDetect(a, i + 4) < 0)
                return NOT_COMMAND;
        }
        if (opDetect(a, i + 3) < 0) {
            delete []name2;
            return NOT_COMMAND;
        }
    }
    if (strcmp(a[i],"phone") == 0) {
        if (condDetect(a, i + 1) >= 0)
            c_phone = cond_type(condDetect(a, i + 1));
        else
            return NOT_COMMAND;
        int t = 0;
        if ((t = isNumber(a[i+2])) < 0)
            return NOT_COMMAND;
        data->phone = t;
        if (opDetect(a, i + 3) > 0) {
            oper = op_type(opDetect(a, i + 3));
            if (fieldDetect(a, i + 4) < 0)
                return NOT_COMMAND;
        }
        if (opDetect(a, i + 3) < 0)
            return NOT_COMMAND;
    }
    if (strcmp(a[i],"group") == 0) {
        if (condDetect(a, i + 1) >= 0)
            c_group = cond_type(condDetect(a, i + 1));
        else
            return NOT_COMMAND;
        int t = 0;
        if ((t = isNumber(a[i+2])) < 0)
            return NOT_COMMAND;
        data->group = t;
        if (opDetect(a, i + 3) > 0) {
            oper = op_type(opDetect(a, i + 3));
            if (fieldDetect(a, i + 4) < 0)
                return NOT_COMMAND;
        }
        if (opDetect(a, i + 3) < 0)
            return NOT_COMMAND;
    }
    return NO_PROBLEMS;
}
int command :: opDetect(char **a, int i) {//op_type
    if (strcmp(a[i],"and")==0)
        return int(AND);
    if (strcmp(a[i],"or")==0)
        return int(OR);
    if (strcmp(a[i],"") ==0)
        return int(OP_NONE);
    return NOT_COMMAND;
}
int command :: typeDetect(char **a) {//cmd_type
    if (strcmp(a[0],"insert") == 0)
        return int(INSERT);
    if ((strcmp(a[0],"select") == 0) && (strcmp(a[1],"*") == 0) && ( strcmp(a[2], "where") == 0))
        return int(SELECT);
    if ((strcmp(a[0],"delete") == 0)&&(strcmp(a[1],"where") == 0))
        return int(DELETE);
    if (strcmp(a[0],"quit") == 0)
        return int(QUIT);
    return NOT_COMMAND;
}
int command :: condDetect(char ** a, int i) {//cond_type
    if (strcmp(a[i],"=") == 0)
        return int(EQ);
    if (strcmp(a[i],">") == 0)
        return int(GT);
    if (strcmp(a[i],"<") == 0)
        return int(LT);
    if (strcmp(a[i],">=") == 0)
        return int(GE);
    if (strcmp(a[i],"<=") == 0)
        return int(LE);
    if (strcmp(a[i],"<>") == 0)
        return NE;
    return NOT_COMMAND;
}
int command :: isNumber(char *a) {
    int t = 0;
    for(int j = 0; a[j] != '\0'; j++) {
        if ((a[j] < 48)||(a[j] > 57))
            return NOT_COMMAND;
        t = t * 10 +  (a[j] - 48);
    }
    return t;
}
cond_type command :: getCname() {return c_name;}
cond_type command :: getCphone() {return c_phone;}
cond_type command :: getCgroup() {return c_group;}
op_type command :: getOper() {return oper;}
cmd_type command :: getType() {return type;}
record * command :: getData() {return data;}
void command :: setData(record * newData) { data = newData;}
void command :: setCname(cond_type newCname) { c_name = newCname;}
void command :: setCphone(cond_type newCphone) {c_phone = newCphone;}
void command :: setCgroup(cond_type newCgroup) {c_group = newCgroup;}
void command :: setOper(op_type newoper) {oper = newoper;}
void command :: setType(cmd_type newType) {type =newType;}
command :: command(record * idata) {
    type = CMD_NONE;
    c_name = COND_NONE;
    c_phone = COND_NONE;
    c_group = COND_NONE;
    oper = OP_NONE;
    data = idata;
}
command :: ~command() {
    type = CMD_NONE;
    c_name = COND_NONE;
    c_phone = COND_NONE;
    c_group = COND_NONE;
    oper = OP_NONE;
    if (data)
        delete data;
    data = 0;
}

bool command :: satisfyName(record *a, command* currentCommand) {
    if (currentCommand->getCname() == COND_NONE)
        return false;
    int cmpNameRes = dataBase::cmpName(currentCommand->getData(), a);
    if ((currentCommand->getCname() == EQ)&&(cmpNameRes!= 0))
        return false;
    if ((currentCommand->getCname() == GT)&&(cmpNameRes >= 0))
        return false;
    if ((currentCommand->getCname() == LT)&&(cmpNameRes <= 0))
        return false;
    if ((currentCommand->getCname() == LE)&&(cmpNameRes < 0))
        return false;
    if ((currentCommand->getCname() == GE)&&(cmpNameRes > 0))
        return false;
    if ((currentCommand->getCname() == NE)&&(cmpNameRes == 0))
        return false;
    return true;
}
bool command :: satisfyGroup(record *a, command* currentCommand) {
    if (currentCommand->getCgroup() == COND_NONE)
        return false;
    int cmpGroupRes = dataBase::cmpGroup(a, currentCommand->getData());
    if ((currentCommand->getCgroup() == EQ)&&(cmpGroupRes!= 0))
        return false;
    if ((currentCommand->getCgroup() == GT)&&(cmpGroupRes >= 0))
        return false;
    if ((currentCommand->getCgroup() == LT)&&(cmpGroupRes <= 0))
        return false;
    if ((currentCommand->getCgroup() == LE)&&(cmpGroupRes < 0))
        return false;
    if ((currentCommand->getCgroup() == GE)&&(cmpGroupRes > 0))
        return false;
    if ((currentCommand->getCgroup() == NE)&&(cmpGroupRes == 0))
        return false;
    return true;
}
bool command :: satisfyPhone(record *a, command* currentCommand) {
    if (currentCommand->getCphone() == COND_NONE)
        return false;
    int cmpPhoneRes = dataBase::cmpPhone(a, currentCommand->getData());
    if ((currentCommand->getCphone() == EQ)&&(cmpPhoneRes!= 0))
        return false;
    if ((currentCommand->getCphone() == GT)&&(cmpPhoneRes >= 0))
        return false;
    if ((currentCommand->getCphone() == LT)&&(cmpPhoneRes <= 0))
        return false;
    if ((currentCommand->getCphone() == LE)&&(cmpPhoneRes < 0))
        return false;
    if ((currentCommand->getCphone() == GE)&&(cmpPhoneRes > 0))
        return false;
    if ((currentCommand->getCphone() == NE)&&(cmpPhoneRes == 0))
        return false;
    return true;
}
bool command :: satisfy(record *a, command* currentCommand) {
    bool satName = command::satisfyName(a, currentCommand);
    bool satPhone = command::satisfyPhone(a, currentCommand);
    bool satGroup = command::satisfyGroup(a, currentCommand);
    if ((currentCommand->getOper() == OP_NONE) || (currentCommand->getOper() == OR)) {
        if ((satName) || (satPhone) || (satGroup))
            return true;
        else
            return false;
    }
    if (currentCommand->getOper() == AND) {
        if (currentCommand->getCgroup() != COND_NONE) {
            if (currentCommand->getCname() != COND_NONE) {
                if ((satGroup) && (satName))
                    return true;
                else
                    return false;
            } else {
                if ((satGroup) && (satPhone))
                    return true;
                else
                    return false;
            }
        } else {
            if ((satName) && (satPhone))
                return true;
            else
                return false;
        }
    }
    printf("\n\nWTF!!\n\n");
    return false;
}

cond_type command :: inverse(cond_type cond) {
    if (cond == COND_NONE)
        return COND_NONE;
    if (cond == EQ)
        return NE;
    if (cond == GT)
        return LE;
    if (cond == LT)
        return GE;
    if (cond == LE)
        return GT;
    if (cond == GE)
        return LT;
    if (cond == NE)
        return EQ;
    return EQ;
}
