//
// Created by JiamingShan on 2021/5/2.
//

#ifndef MAIN_CPP_MAPS_H
#define MAIN_CPP_MAPS_H

#include "vector.hpp"
#include "BPlusTree.hpp"
#include <functional>
#include "map.hpp"



struct HashString {
    unsigned long long operator()(const std::string &str) {
        int ans = 0;
        for (int i = 0; str[i] != '\0'; ++i) ans = ans * 19260817 + str[i];
        return ans;
    }
};


template<class Key, class Value, class Hash = HashString>
class DiskMap {
private:
    char file[50];
    BPlusTree bpt;
    std::fstream fio;

public:
    DiskMap(const char *_datafile) : bpt((std::string("bpt_") + _datafile).c_str()) {
        strcpy(file, _datafile);
        std::fstream fin(_datafile, std::ios::in | std::ios::binary);
        if (!fin.is_open()) {
            std::fstream fout(_datafile, std::ios::out | std::ios::binary);
            fout.close();
        }
        fin.close();
        fio.open(_datafile, std::ios::in | std::ios::out | std::ios::binary);
    };

    ~DiskMap() { fio.close(); }

    bool insert(const sjtu::pair<Key, Value> &pair) {
        fio.seekg(0, std::ios::end);
        int pos = fio.tellp();

        bool flag = bpt.insert(Hash()(pair.first), pos, 1);
        if (flag)
            fio.write(reinterpret_cast<const char *>(&(pair.second)), sizeof(pair.second));

        return flag;
    }

    bool erase(const Key &key) {
        return bpt.erase(Hash()(key));
    }

    sjtu::pair<int, bool> find(const Key &key) {
        int f = bpt.query(Hash()(key));
        return {f, ~f ? true : false};
    }

    Value getItem(int pos) {
        Value ans;
        fio.seekg(pos, std::ios::beg);
        fio.read(reinterpret_cast<char *>(&ans), sizeof(ans));
        return ans;
    }

    void setItem(int pos, const Value &value) {
        fio.seekg(pos, std::ios::beg);
        fio.write(reinterpret_cast<const char *>(&value), sizeof(value));
    }

    int get_size() const {
        return bpt.get_size();
    }

    bool empty() const {
        return bpt.get_size() == 0;
    }

    void clear() {
        fio.close();

        std::fstream fout(file, std::ios::out | std::ios::binary);
        fout.close();

        fio.open(file, std::ios::in | std::ios::out | std::ios::binary);

        bpt.clear();
    }
};

//?????????????????????????????????
template<class Key, class Value, class Hash = HashString, int MAXN = 25000>
//Hash????????????????????????????????????????????????????????????auto h = Hash<string>(), ??????????????????????????????????????????h(1),??????????????????size_t
class HashMap {
private:
    using ull = unsigned long long;
    static constexpr int MOD = (MAXN <= 10000) ? 100003 : (MAXN <= 100000) ? 1000003 : (MAXN <= 300000) ? 233347
                                                                                                        : 10000019;
    int tot = 0, size = 0;
    int *last;
    struct edge {
        Value val;
        ull too;
        int pre;
    } *e;
public:

    // MAXN ?????? insert ???????????????_mod ????????????????????????????????????
    HashMap() {
        last = new int[MOD];
        e = new edge[MAXN + 1];
    }

    ~HashMap() {
        delete last;
        delete e;
    }

    inline void add(int x, ull y, const Value &val) {
        e[++tot] = (edge) {val, y, last[x]};
        last[x] = tot;
        size++;
    }

    bool insert(const sjtu::pair<Key, Value> &pair) {
        ull hs = Hash()(pair.first);
        int hsmod = hs % MOD;
        for (int i = last[hsmod]; i; i = e[i].pre)
            if (e[i].too == hs)
                return 0;

        add(hsmod, hs, pair.second);
        return 1;
    }

    sjtu::pair<Value, bool> erase(const Key &key) {
        ull hs = Hash()(key);
        int hsmod = hs % MOD;
        for (int i = last[hsmod], *j = &last[hsmod]; i; j = &e[i].pre, i = e[i].pre)
            if (e[i].too == hs) {
                Value ans = e[i].val;
                *j = e[i].pre;
                size--;
                return {ans, 1};
            }

        return {Value(), 0};
    }

    bool empty() const {
        return size == 0;
    }

    int get_size() const {
        return size;
    }

    void clear() {
        tot = size = 0;
        memset(last, 0, sizeof(int) * MOD);
    }

    Value *find(Key key) {
        ull hs = Hash()(key);
        int hsmod = hs % MOD;
        for (int i = last[hsmod]; i; i = e[i].pre)
            if (e[i].too == hs)
                return &e[i].val;

        return nullptr;
    }
};


template<class T>
struct List {
    struct Node {
        T t;
        Node *n = nullptr;
        Node *p = nullptr;

        Node() {}

        Node(T t, Node *n, Node *p) : t(t), n(n), p(p) {}
    };

    Node *root = nullptr;
    Node *rear = nullptr;
    int num = 0;

    int size() const{ return num; }

    List() {
        rear = root = new Node();
    }

    List(const List &) = delete;

    List &operator=(const List &) = delete;

    virtual ~List() {
        while (root) {
            Node *nextroot = root->n;
            delete root;
            root = nextroot;
        }
    }

    void writeToFile(std::fstream &file, int address, int timesOfSpace = 1) {
        const int bitnum = sizeof(T) * size() * timesOfSpace;
        if (!bitnum) return;
        file.seekg(address);
        char *fixzero = new char[bitnum]{0};
        int i = -int(sizeof(T));
        auto fakeroot = root;
        while (fakeroot) {
            Node *nextroot = fakeroot->n;
            if (nextroot) memcpy(fixzero + (i += sizeof(T)), reinterpret_cast<char *>(&nextroot->t), sizeof(T));
            fakeroot = nextroot;
        }
        file.write(reinterpret_cast<const char *>(fixzero), bitnum);
        delete[] fixzero;
    }


    friend List *mergeList(List *dist, List *src) {
        auto distlast = dist->root;
        while (distlast->n) distlast = distlast->n;//better ????????????O(100),????????????
        auto srcroot = src->root;
        distlast->n = srcroot->n;
        if (srcroot->n)srcroot->n->p = distlast;
        dist->num += src->num;
        delete srcroot;
        return dist;
    }


    struct Iterator {
        Node *ptr;

        Iterator() = default;

        Iterator(Node *ptr) : ptr(ptr) {}

        Iterator &operator++() {
            ptr = ptr->n;
            return *this;
        }

        Iterator operator++(int) {//I promise that while using ++ and * and ->, won't use erase and pushfront.
            Iterator ret = *this;
            ++*this;
            return ret;
        }

        T &operator*() const {
            return ptr->t;
        }

        T *operator->() const noexcept {
            return &**this;
        }

        bool operator==(const Iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator!=(const Iterator &rhs) const {
            return !(rhs == *this);
        }
    };

    void push_front(T t) {//push to the place where begin() is pointing to.
        root->n = new Node(t, root->n, root);
        if (root->n->n)root->n->n->p = root->n;
        ++num;
        if(num==1) rear = root->n;
    }
    void push_back(T t) {//push to the place where begin() is pointing to.
        rear = rear->n = new Node(t, rear->n, rear);
        ++num;
    }

    void erase(Iterator &iter) {//return Iterator to next element, where ++Iterator go to the same element.
        Node *tmp = iter.ptr;
        if(tmp==rear) rear = rear->p;
        tmp->p->n = tmp->n;
        if (tmp->n)
            tmp->n->p = tmp->p;
        Node *ret = tmp->p;
        delete tmp;
        iter.ptr = ret;
        --num;
    }

    Iterator begin() const {
        return root->n;
    }

    Iterator end() const {
        return nullptr;
    }

    void clear() {
        Node *tmp = root->n;
        while (tmp) {
            Node *nexttmp = tmp->n;
            delete tmp;
            tmp = nexttmp;
        }
        root->n = nullptr;
        rear = root;
        num = 0;
    }

    void print() const {
        for (auto it = begin(); it != end(); ++it) std::cout << *it << " ";
        std::cout << std::endl;
    }
};

template<class Key, class Value,class Hash = HashString>
class CacheMap {//???????????????????????????threshold??????????????????????????????bpt
public:
    std::string fileName;
    std::fstream file;
    static constexpr int THRESHOLD = 5;//memo 1 ???debug????????????????????????????????????

    CacheMap(std::string fileName) : fileName(fileName),
                                     outmapper((std::string("inout_") + fileName).c_str()) {
        file.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
        if(!file){
            std::ofstream fout(fileName, std::ios::out | std::ios::binary);
            fout.close();
            file.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
        }
    }

    ~CacheMap() {
        //better ???????????????????????????????????????login??????logout??????????????????????????????????????????????????????????????????????????????????????????????????????
        //...?????????????????????????????????
        for (auto it = keySetter.begin(); it != keySetter.end(); ++it) {
            Data *dataptr = mapper.find(*it);
            putToFileWithoutFear(dataptr);
            outmapper.setItem(outmapper.find(*it).first, *dataptr);
        }
        for (auto it = listSetter.begin(); it != listSetter.end(); ++it) delete (*it);
        file.close();
    }


    void insert(const sjtu::pair<Key, Value> &pair) {
        Data *dataptr = safeGetDataFromInnerMapper(pair.first);
        dataptr->listptr->push_front(pair.second);
        if (dataptr->listptr->size() > THRESHOLD)
            putToFileWithoutFear(dataptr);
    }


    typedef Value *Iterator;

    Iterator find(Key key, int n) {//?????????n?????????????????????
        List<Value> *valueList = find(key);
        if (n < 1 || n > valueList->size()) return nullptr;
        auto it = valueList->begin();
        while (--n) ++it;
        return &(it.ptr->t);
    }


    void setintAndMaxFromOuterMapNumWhenFirstMeetKey(Key key) {
        auto fd = outmapper.find(key);
        assert(fd.second);
        CoreData cd = outmapper.getItem(fd.first);
        //?????????assert auto ???????????????????????????
        List<Value> *listptr = new List<Value>();
        listSetter.push_front(listptr);
        keySetter.push_front(key);
        if (cd.address == -2) {
            file.seekg(0, std::ios::end);
            cd.address = file.tellp();
            constexpr char allzero[THRESHOLD * sizeof(Value)] = {0};
            file.write(allzero, THRESHOLD * sizeof(Value));
        }
        mapper.insert({key, Data(listptr, cd.address, cd.maxnum, cd.nownum)});
    };//caution login ???????????????get??????

    List<Value> *readListFromFile(std::fstream &file, int address, int &nownum) {
        if (nownum == 0) {
            return new List<Value>();
        }
        file.seekg(address);
        const int bitnum = sizeof(Value) * nownum;
        char *fixzero = new char[bitnum]{0};
        file.read(fixzero, bitnum);
        List<Value> *listptr = new List<Value>();
        for (int i = nownum - 1; ~i; --i) {
            Value t;
            memcpy(reinterpret_cast<char *>(&t), fixzero + i * sizeof(Value), sizeof(Value));
            listptr->push_front(t);
        }
        memset(fixzero, 0, bitnum);
        file.seekg(address);
        file.write(fixzero, bitnum);
        delete[] fixzero;
        nownum = 0;
        return listptr;
    }


    List<Value> *find(Key key) {//caution find ????????????????????????????????????????????????????????? ??????????????????????????????????????????
        //better ?????????????????????????????????find???????????? ????????????return??????????????????????????????????????????????????????
        Data * dataptr = safeGetDataFromInnerMapper(key);
        List<Value> *listptr = readListFromFile(file, dataptr->address, dataptr->nownum);
        mergeList(dataptr->listptr, listptr);
        return dataptr->listptr;
    };

    void clear() {
        for (auto it = listSetter.begin(); it != listSetter.end(); ++it)
            delete (*it);
        mapper.clear();
        outmapper.clear();
        file.close();
        file.open(fileName, std::ios::out);
        file.close();
        file.open(fileName, std::ios::out | std::ios::in | std::ios::binary);
    }

//todo ??????????????????
//better ???????????????????????? ???????????? ??????Queue?????????????????????
private:
    struct CoreData {
        int address = -2;
        int maxnum = THRESHOLD;
        int nownum = 0;//??????????????????????????????

        CoreData() {};

        CoreData(int address, int maxnum, int nownum) : address(address), maxnum(maxnum), nownum(nownum) {}
    };

    struct Data : CoreData {
        List<Value> *listptr = nullptr;

        Data() {}

        Data(List<Value> *listptr, int address, int maxnum, int nownum) : listptr(listptr),
                                                                          CoreData(address, maxnum, nownum) {}
    };

    HashMap<Key, Data, Hash> mapper;
    DiskMap<Key, CoreData, Hash> outmapper;
    List<List<Value> *> listSetter;
    List<Key> keySetter;


    Data *safeGetDataFromInnerMapper(Key key) {
        Data *dataptr = mapper.find(key);
        if (dataptr)
            return dataptr;
        auto iterpair = outmapper.find(key);
        if (!iterpair.second) {
            outmapper.insert({key, CoreData()});
        }
        setintAndMaxFromOuterMapNumWhenFirstMeetKey(key);
        dataptr = mapper.find(key);
        return dataptr;
    }

    void putToFileWithoutFear(Data *&dataptr) {
        List<Value> *&valueList = dataptr->listptr;
        mergeList(valueList, readListFromFile(file, dataptr->address, dataptr->nownum));
        dataptr->nownum = valueList->size();
        if (dataptr->maxnum >= valueList->size()) {//???????????????find?????????????????????????????????
            valueList->writeToFile(file, dataptr->address);
        } else {
            file.seekg(0, std::ios::end);
            dataptr->maxnum *= 2;
            dataptr->address = file.tellg();
            valueList->writeToFile(file, dataptr->address, 2);//fixme ??????????????????????????????
        }
        valueList->clear();
    }

};



//recommend to implement by a list
//singleton pattern



template<class T>
struct CacheList : List<T> {
    std::string fileName;
    std::fstream file;

    using Node = typename List<T>::Node;
    using Iterator = typename List<T>::Iterator;

    CacheList(std::string fileName) : fileName(fileName) {//better ????????????????????????100w???????????????????????????????????????????????????????????????
        file.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
        if(!file){
            std::ofstream fout(fileName, std::ios::out | std::ios::binary);
            fout.close();
            file.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
        }
        file.seekg(0, std::ios::end);
        file.tellg();
        const int bitnum = file.tellg();
        if (bitnum != 0) {
            file.seekg(0, std::ios::beg);
            char *fixzero = new char[bitnum]{0};
            file.read(fixzero, bitnum);
            int lastNotZero = bitnum - 1;

            for (int i = lastNotZero / sizeof(T); ~i; --i) {
                T t;
                memcpy(reinterpret_cast<char *>(&t), fixzero + i * sizeof(T), sizeof(T));
                this->push_front(t);
            }
            delete[] fixzero;

        }
        file.close();
    }

    virtual ~CacheList() override {
        file.open(fileName, std::ios::out);
        file.close();
        file.open(fileName, std::ios::in |std::ios::out | std::ios::binary);
        List<T>::writeToFile(file, 0);
        file.close();
    }
};



//memo ???????????????queue???????????????queue????????????????????????????????????????????????????????????????????????refund??????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????

//???????????????????????????????????????????????????

#endif //MAIN_CPP_MAPS_H