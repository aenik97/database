#ifndef RBTREE
#define RBTREE



class rbTree {
private:
    nodeTree NIL;
    nodeTree* root;
    int (*cmp)(record*, record*);
    int hi;
    int lists;
    int vert;
    int same;
    friend class dataBase;
public:
    rbTree();
    ~rbTree();
    nodeTree* getRoot();
    void setCmp(int (*f)(record *, record*));
    void deleteTree(nodeTree *root);
    void setRoot(nodeTree * newRoot);
    void print(FILE *out, nodeTree *pRoot, int maxDeep = 5);
    void printTree(FILE *out, nodeTree *pRoot, int deep, int maxDeep);
    void printTreeNext(FILE *out, nodeTree *pRoot, int deep = 0, int maxDeep = 5);
    void rotateLeft(class nodeTree *x);
    void rotateRight(nodeTree *x);
    int insertNode(Nodelist<record>* data);
    nodeTree* find(nodeTree * root, command* currentCommand, list<nodeTree> * listApp);
    void solveGT(nodeTree * root, command* currentCommand, list<nodeTree> * listApp);
    void solveLT(nodeTree * root, command* currentCommand, list<nodeTree> * listApp);
    void solve(nodeTree * root, command* currentCommand, list<nodeTree> * listApp);
    int apply(FILE *out, command* currentCommand, list<record> *listdb, rbTree* t2);
    int deleteNode(nodeTree *z);
    list<nodeTree>* select(command* currentCommand);
    list<nodeTree>* deleteElements(command* currentcommand);
    void deleteFix(nodeTree *x);
    void insertFix(nodeTree *z);
    void swap(nodeTree *a, nodeTree *b);
    void fullCheck(int num);
    void myCheck(nodeTree *a);
    void rbCheck(nodeTree *a);
    void rbHCheck(nodeTree *a, int h);
};
#endif
