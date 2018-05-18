#ifndef TEMLATECLASSLIST_H
#define TEMLATECLASSLIST_H

#include <stdio.h>
template <typename nodeStruct> class list;

template <class nodeStruct> class Nodelist {
private:
    Nodelist<nodeStruct> *next, *prev;
    nodeStruct * data;
    friend class list<nodeStruct>;
    friend class rbTree;
public:
    Nodelist(nodeStruct * inode = 0, Nodelist<nodeStruct>* inext = 0, Nodelist<nodeStruct> *iprev = 0);
    Nodelist<nodeStruct>* getNext();
    Nodelist<nodeStruct>* getPrev();
    ~Nodelist();
    nodeStruct * getData();
    void setNext(Nodelist<nodeStruct> * a);
    void setPrev(Nodelist<nodeStruct> * a);
    void setNode(nodeStruct * a);
    int readNode(FILE * in);
    void print(FILE * out);
    int init(nodeStruct* data, Nodelist<nodeStruct> *next,Nodelist<nodeStruct> *prev);
};

template <class nodeStruct> class list {
private:
    Nodelist <nodeStruct> *head;
    Nodelist <nodeStruct> *tail;
    int len;
    friend class hash;
public:
    list();
    ~list();
    void printList(FILE *out, int maxNode = 10);
    void deleteHead();
    Nodelist<nodeStruct>* getHead();
    Nodelist<nodeStruct>* getTail();
    int getLen();
    void addToTail(Nodelist<nodeStruct> * a);
    void addToHead(Nodelist<nodeStruct> * a);
    //void Check(Node<nodeStruct> * tmp, command* com);
    int readList(FILE * in);
    void deleteByLink(Nodelist<nodeStruct> * link);
    void GetOutByLink(Nodelist<nodeStruct> * link);
    void clear();
    list<Nodelist<nodeStruct> >* select(command* );
};

template <class T> Nodelist<T> :: Nodelist(T * inode, Nodelist<T>* inext, Nodelist<T> *iprev) {
    next = inext;
    data = inode;
    prev = iprev;
}

template <class T> Nodelist<T> :: ~Nodelist() {next = 0; data = 0; prev = 0;}
template <class T> Nodelist<T> * Nodelist<T> :: getNext() {return next;}
template <class T> Nodelist<T> * Nodelist<T> :: getPrev() {return prev;}
template <class T> T * Nodelist<T> :: getData() {return data;}
template <class T> void Nodelist<T> :: setNext(Nodelist<T> * a) { next = a;}
template <class T> void Nodelist<T> :: setPrev(Nodelist<T> * a) { prev = a;}
template <class T> void  Nodelist<T> :: setNode(T * a) { data = a;}

template <class T> Nodelist<T>* list <T> :: getHead() {return head;}
template <class T> Nodelist<T>* list <T> :: getTail() {return tail;}
template <class T> int list<T>:: getLen() {return len;}
template <class T> int Nodelist<T> :: init(T * idata, Nodelist<T> *inext, Nodelist<T> *iprev) {
    this->~Nodelist<T>();
    data = idata;
    next = inext;
    prev = iprev;
    return 0;//NO_PROBLEMS;
}
template <class T> int Nodelist<T> :: readNode(FILE * in) {
    if (data == 0)
        data = new T;
    int ret = data->readRecord(in);
    if (ret != NO_PROBLEMS)
        delete data;
    return ret;
}
template <class T> void Nodelist<T> :: print(FILE * out) {
    data->print(out);
}
template <class T> list<T>:: list() {head = 0; tail = 0; len = 0;} 
template <class T> list<T>:: ~list() {
    while (head) {
        if (head->getData())
            delete head->getData();
        this->deleteHead();
    }
    len = 0;
    tail = 0;
}

template <class T> void list<T> :: clear() {
    while (head)
        this->deleteHead();
    len = 0;
    tail = 0;
}
template <class T> void list<T> :: deleteHead() {
    deleteByLink(head);
}
template <class T> void list<T> :: addToHead(Nodelist<T> * a) {
    if (a) {
        len++;
        a->next = head;
        a->prev = 0;
        if (head)
            head->prev = a;
        else
            tail = a;
        head = a;
    }
}
template <class T> void list<T> :: addToTail(Nodelist<T> * a) {
    if (a) {
        len++;
        a->next = 0;
        a->prev = tail;
        if (tail == 0) {
            tail = a;
            head = a;
            return;
        }
        tail->next = a;
        tail = a;
    }
}

template <class T> void list<T> :: printList(FILE *out, int maxNode) {
    Nodelist<T> *tmp = head;
    int i = 0;
    while ((tmp) && (i < maxNode)) {
        tmp->getData()->print(out);
        tmp = tmp->getNext();
        i++;
    }
    if (tmp)
        fprintf(out, "...\n");

}

template <class T> int list<T> :: readList(FILE * in) {
    Nodelist<T> *a = new Nodelist<T>;
    int ret;
    while ((ret = a->readNode(in)) == 0) {//NO_PROBLEMS
        addToTail(a);
        a = new Nodelist<T>;
    }
    if (ret == -3)//ERROR
        delete a;
    return ret;
}
template <class T> void list<T> :: deleteByLink(Nodelist<T> * link) {
    GetOutByLink(link);
    if (link)
        delete link;
}
template <class T> void list<T> :: GetOutByLink(Nodelist<T> * link) {
    if (link) {
        len--;
        if (link == head) {
            head = head->next;
            if (head)
                head->setPrev(0);
            link->setNext(0);
            link->setPrev(0);
        }
        else {
            if (link->next)
                link->next->prev = link->prev;
            if (link->prev)
                link->prev->next = link->next;
            link->setNext(0);
            link->setPrev(0);
        }
    }
}


#endif // TEMLATECLASSLIST_H
