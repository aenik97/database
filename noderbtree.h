#ifndef NODERBTREE
#define NODERBTREE



enum nodeColor{ RED, BLACK};

class nodeTree {
private:
    nodeTree *left, *right, *parent, *next;
    Nodelist<record> *data;
    nodeColor color;
    friend class rbTree;
public:
    Nodelist<record> * getData();
    nodeTree* getLeft();
    nodeTree* getRight();
    nodeTree* getParent();
    nodeTree* getNext();
    void setLeft(nodeTree * newLeft);
    void setRight(nodeTree * newRight);
    void setParent(nodeTree * newParent);
    void setColor(nodeColor newColor);
    void setNext(nodeTree *newNext);
    void print(FILE* out);
    nodeTree(nodeTree * ileft = 0,nodeTree * iright = 0, nodeTree * iparent = 0, nodeTree *inext = 0, Nodelist<record> *idata = 0, nodeColor icolor = nodeColor(0));
    ~nodeTree();
    int init(nodeTree* ileft, nodeTree* iright, nodeTree* iparent, nodeTree *inext,  Nodelist<record>* idata, nodeColor icolor);
    static void NodesToList(nodeTree* root, command* currentCommand, list<nodeTree>* listApp);
};
#endif
