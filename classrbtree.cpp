#include "headers.h"
nodeTree* rbTree :: getRoot() {return root;}
void rbTree :: setCmp(int (*f)(record *, record*)) { cmp = f;}
void rbTree :: deleteTree(nodeTree *root) {
    if ((root != &NIL) && (root)) {
        deleteTree(root->left);
        deleteTree(root->right);
        nodeTree * tmp = root;;
        while (root) {
            tmp = root;
            root = root->getNext();
            delete tmp;
        }
    }
}
void rbTree :: setRoot(nodeTree * newRoot) {root = newRoot;}
void rbTree :: print(FILE *out, nodeTree *pRoot, int maxDeep) {
    if ((root) && (root != &NIL))
        fprintf(out, "| %s %d %d|\n", root->data->data->name, root->data->data->phone, root->data->data->group);
    printTree(out, pRoot, 0, maxDeep);
    printTreeNext(out, pRoot, 0, maxDeep);
    fprintf(out, "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}
void rbTree :: printTreeNext(FILE *out, nodeTree *pRoot, int deep, int maxDeep) {
    if ((pRoot) && (pRoot != &NIL) && (deep < maxDeep)) {
        nodeTree * tmp = pRoot->next;
        if (tmp)
            fprintf(out, "\n%s %d %d : ", pRoot->data->data->name, pRoot->data->data->phone, pRoot->data->data->group);
        int j = 0;
        while ((tmp) && (j < 5)) {
            fprintf(out, "%s %d %d, ", tmp->data->data->name, tmp->data->data->phone, tmp->data->data->group);
            tmp = tmp->next;
            j++;
        }
        if (tmp)
            printf("...");
        printTreeNext(out, pRoot->left, deep + 1);
        printTreeNext(out, pRoot->right, deep + 1);
    }
}

void rbTree :: printTree(FILE *out, nodeTree *pRoot, int deep, int maxDeep) {
    if ((pRoot != &NIL) && (pRoot) && (deep < maxDeep)) {
        printTree(out, pRoot->left, deep + 1, maxDeep);
        if ((pRoot->left != &NIL) && (deep <= maxDeep)) {
            for (int i = 0; i < deep*3 + 3; i++)
                fprintf(out ," ");
            fprintf(out, "/ \n");
        }
        for (int i = 0; i < deep * 3; i++)
            fprintf(out ," ");
        if (pRoot->color == RED)
            fprintf(out ,"%s %d %d R\n", pRoot->data->data->name, pRoot->data->data->phone, pRoot->data->data->group);
        if (pRoot->color == BLACK)
            fprintf(out, "%s %d %d B\n", pRoot->data->data->name, pRoot->data->data->phone, pRoot->data->data->group);
        if ((pRoot->right != &NIL) && (deep + 1 <= maxDeep)) {
            for (int i = 0; i < deep*3 + 3; i++)
                fprintf(out ," ");
            fprintf(out, "\\ \n");
        }
        printTree(out, pRoot->right, deep + 1, maxDeep);
    }
}
void rbTree :: fullCheck(int num) {
    printf("Check %d start\n", num);
    hi = 0;
    myCheck(root);
    rbCheck(root);
    rbHCheck(root, 0);
    printf("Check %d end\n \n \n", num);
}

void rbTree :: myCheck(nodeTree *a) {
    if (a == &NIL)
        return;
    if (a == 0)
        printf("WTF a = 0\n");
    if (a->parent == 0)
        printf("WTF parent = 0\n");
    if (a->left == 0)
        printf("WTF left = 0\n");
    if (a->right == 0)
        printf("WTF right = 0\n");
    if ((a->left != &NIL) && (a->left->parent != a)) {
        a->data->print(stdout);
        printf("WTF left child\n");
    }
    if ((a->right != &NIL) && (a->right->parent != a)) {
        a->data->print(stdout);
        printf("WTF right child\n");
    }
    if ((a->parent != &NIL) && (cmp(a->data->data,a->parent->data->data) > 0) && (a->parent->right != a)) {
        a->data->print(stdout);
        printf("WTF parent\n");
    }
    if ((a->parent != &NIL) && (cmp(a->data->data,a->parent->data->data) < 0) && (a->parent->left != a)) {
        a->data->print(stdout);
        printf("WTF parent\n");
    }
    if (((a->parent != &NIL)&&(cmp(a->data->data,a->parent->data->data) == 0)) || ((a->left != &NIL)&&(cmp(a->data->data,a->left->data->data) == 0)) || ((a->right != &NIL)&&(cmp(a->data->data,a->right->data->data) == 0))) {
        a->data->print(stdout);
        printf("WTF parent\n");
    }
    myCheck(a->left);
    myCheck(a->right);
}
void rbTree :: rbCheck(nodeTree *a) {
    if (a == &NIL)
        return;
    if (a == 0)
        printf("WTFrb 0\n");
    if (root->color == RED)
        printf("WTFrb root-red\n");
    if ((a->color == RED)&&((a->left->color == RED)||(a->right->color == RED)))
        printf("WTFrb red->red\n");
    rbCheck(a->left);
    rbCheck(a->right);
}
void rbTree :: rbHCheck(nodeTree *a, int h = 0) {
    if (a == 0) {
        printf("WTFrb MEGA\n");
        return;
    }
    if (a == &NIL) {
        if (hi == 0)
            hi = h;
        if (h != hi) {
            printf("h = %d hi = %d WTFrb H\n", h, hi);
        }
        return;
    }
    int t = 0;
    if (a->color == BLACK)
        t = 1;
    rbHCheck(a->left, h + t);
    rbHCheck(a->right, h + t);
}
void rbTree :: rotateLeft (class nodeTree *x) {
    nodeTree *y = x->right;
    x->setRight(y->left);
    if (y->left != &NIL)
        y->left->setParent(x);
    y->setParent(x->parent);
    if (x->parent == &NIL)
        this->setRoot(y);
    else {
        if (x == x->parent->left)
            x->parent->setLeft(y);
        else
            x->parent->setRight(y);
    }
    y->setLeft(x);
    x->setParent(y);
}
void rbTree :: rotateRight(nodeTree *x) {
    nodeTree *y = x->left;
    x->setLeft(y->right);
    if (y->right != &NIL)
        y->right->setParent(x);
    y->setParent(x->parent);
    if (x->parent == &NIL)
        this->setRoot(y);
    else {
        if (x == x->parent->right)
            x->parent->setRight(y);
        else
            x->parent->setLeft(y);
    }
    y->setRight(x);
    x->setParent(y);
}

int rbTree :: insertNode(Nodelist<record>* newNode) {
    nodeTree *x = root;
    nodeTree *y = &NIL;
    nodeTree *z = new nodeTree;
    z->setLeft(&NIL);
    z->setRight(&NIL);
    z->setParent(&NIL);
    z->data = newNode;
    z->setColor(RED);
    while (x != &NIL) {
        y = x;
        if (cmp(x->data->data, z->data->data) == 0) {
            nodeTree * tmp = x;
            while (tmp) {
                if ((dataBase::cmpPhone(tmp->data->data, newNode->data) == 0) && (dataBase::cmpName(tmp->data->data, newNode->data) == 0) && (dataBase::cmpGroup(tmp->data->data, newNode->data) == 0)) {
                    delete z;
                    same++;
                    return ERROR;
                }
                tmp = tmp->getNext();
            }
            z->next = x->next;
            x->setNext(z);
            z->setParent(x->parent);
            z->setLeft(x->left);
            z->setRight(x->right);
            z->setColor(x->color);
            lists++;
            vert++;
            return NO_PROBLEMS;
        }
        if (cmp(x->data->data, z->data->data) > 0)
            x = x->left;
        else
            x = x->right;
    }
    z->setParent(y);
    if (y == &NIL)
        setRoot(z);
    else {
        if (cmp (y->data->data, z->data->data) > 0)
            y->setLeft(z);
        else
            y->setRight(z);
    }
    z->setLeft(&NIL);
    z->setRight(&NIL);
    z->setColor(RED);
    insertFix(z);
    vert++;
    if (z != 0)
        return 1;
    else
        return ERROR;
}
void rbTree :: insertFix(nodeTree *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            nodeTree* y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->setColor(BLACK);
                y->setColor(BLACK);
                z->parent->parent->setColor(RED);
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotateLeft(z);
                }
                z->parent->setColor(BLACK);
                z->parent->parent->setColor(RED);
                rotateRight(z->parent->parent);
            }
        } else {
            nodeTree* y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->setColor(BLACK);
                y->setColor(BLACK);
                z->parent->parent->setColor(RED);
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotateRight(z);
                }
                z->parent->setColor(BLACK);
                z->parent->parent->setColor(RED);
                rotateLeft(z->parent->parent);
            }
        }
    }
    root->setColor(BLACK);
}

int rbTree :: deleteNode(nodeTree *z) {
    NIL.left=&NIL;
    NIL.right=&NIL;
    NIL.parent=&NIL;
    NIL.next = 0;
    NIL.color = BLACK;
    if ((z == 0) || (z == &NIL))
        return NO_PROBLEMS;
    if ((z->parent->right != z) && (z->parent->left != z)) {
        nodeTree *tmp = 0;
        if (cmp(z->data->data, z->parent->data->data) > 0)
            tmp = z->parent->right;
        else
            tmp = z->parent->left;
        while (tmp->next != z)
            tmp = tmp->next;
        tmp->next = z->next;
        //delete z;
        return NO_PROBLEMS;
    }
    if (z->next) {
        z->next->setLeft(z->left);
        z->left->setParent(z->next);
        z->next->setRight(z->right);
        z->right->setParent(z->next);
        z->next->setColor(z->color);
        z->next->setParent(z->parent);
        if (z == root)
            setRoot(z->next);
        else {
            if(z->parent->left == z)
                z->parent->setLeft(z->next);
            else
                z->parent->setRight(z->next);
        }
        z->left = 0;
        z->right = 0;
        z->parent = 0;
        z->next = 0;
        //delete z;
        return NO_PROBLEMS;
    }
    nodeTree *x, *y = z;
    nodeColor yColor = y->color;
    if (z->left == &NIL) {
        x = z->right;
        swap(z, z->right);
    } else {
        if (z->right == &NIL) {
            x = z->left;
            swap(z, z->left);
        } else {
            nodeTree *tmp = z->right;
            while (tmp->left != &NIL)
                tmp = tmp->left;
            y = tmp;
            yColor = y->color;
            x = y->right;
            if (y->parent == z)
                x->setParent(y);
            else {
                swap(y, y->right);
                y->setRight(z->right);
                y->right->setParent(y);
            }
            swap(z, y);
            y->setLeft(z->left);
            y->left->setParent(y);
            y->setColor(z->color);
        }
    }
    if (yColor == BLACK)
        deleteFix(x);
    //delete z;
    NIL.left=&NIL;
    NIL.right=&NIL;
    NIL.parent=&NIL;
    NIL.next = 0;
    NIL.color = BLACK;
    return NO_PROBLEMS;
}
void rbTree :: swap(nodeTree *a, nodeTree *b) {
    if (a->parent == &NIL)
        setRoot(b);
    else {
        if (a == a->parent->left)
            a->parent->setLeft(b);
        else
            a->parent->setRight(b);
    }
    b->setParent(a->parent);
}
void rbTree :: deleteFix(nodeTree * x) {
    while ((x != root) && (x->color == BLACK)) {
        if (x == x->parent->left) {
            nodeTree *w = x->parent->right;
            if (w->color == RED) {
                w->setColor(BLACK);
                x->parent->setColor(RED);
                rotateLeft (x->parent);
                w = x->parent->right;
            }
            if ((w->left->color == BLACK) && (w->right->color == BLACK)) {
                w->setColor(RED);
                x = x->parent;
            }
            else {
                if (w->right->color == BLACK) {
                    w->left->setColor(BLACK);
                    w->setColor(RED);
                    rotateRight (w);
                    w = x->parent->right;
                }
                w->setColor(x->parent->color);
                x->parent->setColor(BLACK);
                w->right->setColor(BLACK);
                rotateLeft (x->parent);
                x = root;
            }
        }
        else {
            nodeTree *w = x->parent->left;
            if (w->color == RED) {
                w->setColor(BLACK);
                x->parent->setColor(RED);
                rotateRight (x->parent);
                w = x->parent->left;
            }
            if ((w->right->color == BLACK) && (w->left->color == BLACK)) {
                w->setColor(RED);
                x = x->parent;
            }
            else {
                if (w->left->color == BLACK) {
                    w->right->setColor(BLACK);
                    w->setColor(RED);
                    rotateLeft (w);
                    w = x->parent->left;
                }
                w->setColor(x->parent->color);
                x->parent->setColor(BLACK);
                w->left->setColor(BLACK);
                rotateRight (x->parent);
                x = root;
            }
        }
    }
    x->setColor(BLACK);
}
nodeTree * rbTree :: find(nodeTree * root, command* currentCommand, list<nodeTree> * listApp) {
    if ((root != &NIL) && (root)) {
        if (cmp(root->data->data, currentCommand->data) < 0)
            return find(root->right, currentCommand, listApp);
        else
            if (cmp(root->data->data, currentCommand->data) > 0)
                return find(root->left, currentCommand, listApp);
            else {
                nodeTree::NodesToList(root, currentCommand, listApp);
                return root;
            }
    }
    return 0;
}
void rbTree :: solveGT(nodeTree * root, command* currentCommand, list<nodeTree> * listApp) {
    if ((root != &NIL) && (root)) {
        if (command::satisfy(root->data->data, currentCommand))
            nodeTree::NodesToList(root, currentCommand, listApp);
        if (cmp(root->data->data, currentCommand->data) > 0) {
            solveGT(root->left, currentCommand, listApp);
            solveGT(root->right, currentCommand, listApp);
        }
        if (cmp(root->data->data, currentCommand->data) < 0)
            solveGT(root->right, currentCommand, listApp);
    }
}
void rbTree :: solveLT(nodeTree * root, command* currentCommand, list<nodeTree> * listApp) {
    if ((root != &NIL) && (root)) {
        if (command :: satisfy(root->data->data, currentCommand))
            nodeTree::NodesToList(root, currentCommand, listApp);
        if (cmp(root->data->data, currentCommand->data) < 0) {
            solveLT(root->left, currentCommand, listApp);
            solveLT(root->right, currentCommand, listApp);
        }
        if (cmp(root->data->data, currentCommand->data) > 0)
            solveLT(root->left, currentCommand, listApp);
    }
}
void rbTree :: solve(nodeTree * root, command* currentCommand, list<nodeTree> * listApp) {
    if ((root != &NIL) && (root)) {
        nodeTree::NodesToList(root, currentCommand, listApp);
        solve(root->left, currentCommand, listApp);
        solve(root->right, currentCommand, listApp);
    }
}
list<nodeTree> *rbTree::select(command* currentCommand) {
    list<nodeTree> * listApp = new list<nodeTree>;
    cond_type cond = currentCommand->getCphone();
    if (cmp == &(dataBase::cmpName))
        cond = currentCommand->getCname();

    if ((cond != COND_NONE) && (cond != NE) && (cond != LIKE) && (currentCommand->getOper() != OR)) {
        if (cond == EQ)
            find(root, currentCommand, listApp);
        if ((cond == GT) || (cond == GE))
            solveGT(root, currentCommand, listApp);
        if ((cond == LT) || (cond == LE))
            solveLT(root, currentCommand, listApp);
    } else
        solve(root, currentCommand, listApp);

    if (listApp->getHead()== 0) {
        delete listApp;
        return 0;
    }
    return listApp;
}

list<nodeTree>* rbTree :: deleteElements(command* currentcommand) {
    list<nodeTree>* listApp = select(currentcommand);
    if (listApp == 0)
        return 0;
    Nodelist<nodeTree> * tmp = listApp->getHead();

    while (tmp) {
        this->deleteNode(tmp->getData());
        tmp=tmp->getNext();
    }
    return listApp;
}

rbTree :: rbTree() {
    NIL.left = &NIL;
    NIL.right = &NIL;
    NIL.parent = &NIL;
    NIL.next = 0;
    NIL.data = 0;
    NIL.color = BLACK;
    root = &NIL;
    cmp = & (dataBase::stdcmp);
    hi = 0;
    lists = 0;
    vert = 0;
    same = 0;
}
rbTree :: ~rbTree() {
    deleteTree(root);
    cmp = 0;
    //delete &NIL;
    hi = 0;
    lists = 0;
    vert = 0;
    same = 0;
}
