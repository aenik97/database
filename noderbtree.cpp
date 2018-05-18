#include "headers.h"

Nodelist<record> * nodeTree :: getData() {return data;}
nodeTree* nodeTree :: getLeft() {return left;}
nodeTree* nodeTree :: getRight() {return right;}
nodeTree* nodeTree :: getParent() {return parent;}
nodeTree* nodeTree :: getNext() {return next;}
void nodeTree :: setLeft(nodeTree * newLeft) {left = newLeft;}
void nodeTree :: setRight(nodeTree * newRight) {right = newRight;}
void nodeTree :: setNext(nodeTree *newNext) {next = newNext;}
void nodeTree :: setParent(nodeTree * newParent) {
    nodeTree *tmp = this;
    while (tmp) {
        tmp->parent = newParent;
        tmp = tmp->next;
    }
}
void nodeTree :: setColor(nodeColor newColor) {
    nodeTree *tmp = this;
    while (tmp) {
        tmp->color = newColor;
        tmp = tmp->next;
    }
}
nodeTree :: nodeTree(nodeTree *ileft, nodeTree *iright, nodeTree *iparent, nodeTree *inext, Nodelist<record> *idata, nodeColor icolor) {
    left = ileft;
    right = iright;
    parent = iparent;
    color = icolor;
    next = inext;
    data = idata;
}
nodeTree :: ~nodeTree() {
    left = 0;
    right = 0;
    parent = 0;
    next = 0;
    data = 0;
    color = RED;
}
int nodeTree :: init(nodeTree* ileft, nodeTree* iright, nodeTree* iparent, nodeTree *inext,  Nodelist<record>* idata, nodeColor icolor) {
    this->~nodeTree();
    parent = iparent;
    left = ileft;
    right = iright;
    color = icolor;
    next = inext;
    data = idata;
    return 0;
}

void nodeTree :: NodesToList(nodeTree* root, command* currentCommand, list<nodeTree>* listApp) {
    nodeTree * tmp = root;
    while (tmp) {
        if (command::satisfy(tmp->getData()->getData(), currentCommand)) {
            Nodelist<nodeTree>* tmpApply = new Nodelist<nodeTree>;
            tmpApply->setNode(tmp);
            listApp->addToTail(tmpApply);
        }
        tmp = tmp->getNext();
    }
}
void nodeTree :: print(FILE* out) {
    this->getData()->print(out);
}
