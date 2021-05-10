//
// Created by JiamingShan on 2021/5/2.
//

#ifndef MAIN_CPP_MAPS_H
#define MAIN_CPP_MAPS_H

//stub
#include <unordered_map>
#include "vector.hpp"
#include "filemanip.h"
#include "BPlusTree.hpp"
#include <set>
#include <functional>
#include <ostream>

// Hash 将 Key 映射为一个 unsigned long long
template<class Key, class Value, class Hash>
class OuterUniqueUnorderMap {
private:
    char file[30];
    BPlusTree bpt;
    std::fstream fio;

public:
    OuterUniqueUnorderMap(const char *_datafile) : bpt((std::string("bpt_")+_datafile).c_str()) {
        strcpy(file, _datafile);
        std::fstream fin(_datafile, std::ios::in | std::ios::binary);
        if (!fin.is_open()) {
            std::fstream fout(_datafile, std::ios::out | std::ios::binary);
            fout.close();
        }
        fin.close();

        fio.open(_datafile, std::ios::in | std::ios::out | std::ios::binary);
    };

    ~OuterUniqueUnorderMap() { fio.close(); }

    bool insert(const std::pair<Key, Value> &pair) {
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

    std::pair<int, bool> find(const Key &key) {
        int f = bpt.query(Hash()(key));
        return {f, ~f ? 1 : 0};
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


//stub 唐嘉铭 todo
template<class Key, class Value, class Hash>
//Hash是一个模板类名，它实例化后的一个对象例为auto h = Hash<string>(), 这个对象重载了括号，比如可以h(1),然后返回一个size_t
class InnerUniqueUnorderMap {
public:

    bool insert(const std::pair<Key, Value> &pair) {
        return mapper.insert(pair).second;
    }

    std::pair<Value, bool> erase(const Key &key) {
        auto iter = mapper.find(key);
        if (iter == mapper.end()) return {Value(), 0};
        Value v = iter->second;
        mapper.erase(iter);
        return {v, 1};
    }

    bool empty() const {
        return mapper.empty();
    }

    void clear() {
        mapper.clear();
    }

    Value *find(Key key) {
        auto f = mapper.find(key);
        if (f == mapper.end()) return nullptr;
        return &(f->second);
    }

private:
    std::unordered_map<Key, Value, Hash> mapper;
};


template<class T>
struct InnerList {
    struct Node {
        T t;
        Node *n = nullptr;
        Node *p = nullptr;

        Node() {}

        Node(T t, Node *n, Node *p) : t(t), n(n), p(p) {}
    };

    Node *root = nullptr;
    int num = 0;

    int size() { return num; }

    InnerList() {
        root = new Node();
    }

    InnerList(const InnerList &) = delete;

    InnerList &operator=(const InnerList &) = delete;

    virtual ~InnerList() {
        while (root) {
            Node *nextroot = root->n;
            delete root;
            root = nextroot;
        }
    }

    void writeToFile(std::fstream &file, Address address, int timesOfSpace = 1) {
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


    friend InnerList *mergeList(InnerList *dist, InnerList *src) {
        auto distlast = dist->root;
        while (distlast->n) distlast = distlast->n;//better 随便写个O(100),估计没事
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
    }

    void erase(Iterator &iter) {//return Iterator to next element, where ++Iterator go to the same element.
        Node *tmp = iter.ptr;
        tmp->p->n = tmp->n;
        if (tmp->n)
            tmp->n->p = tmp->p;
        Node *ret = tmp->p;//bug
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
        num = 0;
    }

    void print() const {
        for (auto it = begin(); it != end(); ++it) std::cout << *it << " ";
        std::cout << std::endl;
    }
};


template<class Key, class Value, class Hash>
class InnerOuterMultiUnorderMap {
public:
    FileName fileName;
    std::fstream file;
    std::function<void(Key, Address, int)> setData;//存在固有的文件读写方面的问题
    static constexpr int THRESHOLD = 100;//memo 1 是debug用的数据，到时候再改回来

    InnerOuterMultiUnorderMap(FileName fileName, std::function<void(Key, Address, int)> setData) : fileName(fileName),
                                                                                                   setData(setData) {
        fcreate(fileName);
        file.open(fileName, std::ios::in | std::ios::out | std::ios::binary);
    }

    ~InnerOuterMultiUnorderMap() {
        //better 已偷懒：在一次文件打开后，login的人logout以后也不会再刷回去，只有结束的时候才刷。这会导致小幅的多余内存浪费。
        //...对内部所有人进行刷数据
        for (auto it = keySetter.begin(); it != keySetter.end(); ++it) {
            Data *data = mapper.find(*it);//待改
            setData(*it, data->address, data->maxnum);
        }
        for (auto it = listSetter.begin(); it != listSetter.end(); ++it) delete (*it);
        file.close();
    }


    void insert(const std::pair<Key, Value> &pair) {
        Data *dataptr = mapper.find(pair.first);
        InnerList<Value> *valueList = dataptr->listptr;
        valueList->push_front(pair.second);
        if (valueList->size() > THRESHOLD) {
            mergeList(valueList, readFromFile(file, dataptr->address, dataptr->maxnum));
            if (dataptr->maxnum > valueList->size()) {//刷数据也在find里刷了。之后注意一下。
                valueList->writeToFile(file, dataptr->address);
            } else {
                file.seekg(0, std::ios::end);
                dataptr->maxnum *= 2;
                dataptr->address = file.tellg();
                valueList->writeToFile(file, dataptr->address, 2);//fixme 没把文件的东西拿出来
            }
            valueList->clear();
        }
    }

    typedef Value *Iterator;

    Iterator find(Key key, int n) {//找到第n新插入的东西。
        InnerList<Value> *valueList = find(key);
        if (n < 1 || n > valueList->size()) return nullptr;
        auto it = valueList->begin();
        while (--n) ++it;
        return &(it.ptr->t);
    }

    void getAddressAndMaxNum(Key key, Address address, int maxnum) {
        InnerList<Value> *listptr = new InnerList<Value>();
        listSetter.push_front(listptr);
        keySetter.push_front(key);
        if (address == -2) {
//            if(file.eofbit)std::exit(0);
            file.seekg(0, std::ios::end);
            assert(file.badbit);
            address = file.tellp();
            constexpr char allzero[THRESHOLD * sizeof(Value)] = {0};
            file.write(allzero, THRESHOLD * sizeof(Value));
            maxnum = THRESHOLD;
        }
        mapper.insert({key, Data(listptr, address, maxnum)});
    };//caution login 的时候注意get一下



    InnerList<Value> *readFromFile(std::fstream &file, Address address, int maxnum) {
        if (maxnum == 0) {
            return new InnerList<Value>();//估计用不到，输入不会是0吧？
        }
        file.seekg(address);

        const int bitnum = sizeof(Value) * maxnum;
        char *fixzero = new char[bitnum]{0};
        file.read(fixzero, bitnum);

        int lastNotZero = bitnum - 1;
        while (!fixzero[lastNotZero]) {
            if (lastNotZero == 0) return new InnerList<Value>();
            --lastNotZero;
        }
        //读出后要set0吗？
        InnerList<Value> *listptr = new InnerList<Value>();
        for (int i = lastNotZero / sizeof(Value); ~i; --i) {
            Value t;
            memcpy(reinterpret_cast<char *>(&t), fixzero + i * sizeof(Value), sizeof(Value));
            listptr->push_front(t);
        }
        memset(fixzero, 0, bitnum);
        file.write(fixzero, bitnum);
        delete[] fixzero;
        return listptr;
    }

    InnerList<Value> *find(Key key) {//caution find 出来的链表会即使在内存中栈空间清除吗？ 不然写了这个也白白地没有用。
        auto iter = mapper.find(key);
        InnerList<Value> *listptr = readFromFile(file, iter->address, iter->maxnum);
        mergeList(iter->listptr, listptr);
        return iter->listptr;
    };

    void clear() {
        for (auto it = listSetter.begin(); it != listSetter.end(); ++it)
            delete (*it);//todo 还要全setAddressAndMAxNum(Key key){
        mapper.clear();
        file.close();
        file.open(fileName, std::ios::trunc);
        file.close();
        file.open(fileName, std::ios::out | std::ios::in | std::ios::binary);
    }

//todo 还没可持久化
//better 空间太大刷入外存 接口不变 这跟Queue的原理应当类似
private:
    struct Data {
        InnerList<Value> *listptr = nullptr;
        Address address = 0;
        int maxnum = 0;

        Data(InnerList<Value> *listptr, Address address, int maxnum) : listptr(listptr), address(address),
                                                                       maxnum(maxnum) {}
    };

    InnerUniqueUnorderMap<Key, Data, Hash> mapper;
    InnerList<InnerList<Value> *> listSetter;
    InnerList<Key> keySetter;
};



//recommend to implement by a list
//singleton pattern


template<class T>
struct Queue : InnerList<T> {
    FileName fileName;
    std::fstream file;

    using Node = typename InnerList<T>::Node;
    using Iterator = typename InnerList<T>::Iterator;

    Queue(FileName fileName) : fileName(fileName) {//better 构建队列需不需要100w次文件读写？不过这显然不是瓶颈，但是可玩。
        fcreate(fileName);

        file.open(fileName, std::ios::in | std::ios::binary);
        assert(file);
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

    virtual ~Queue() override {
        file.open(fileName, std::ios::trunc);
        InnerList<T>::writeToFile(file, 0);
        file.close();
    }
    //maybe 加功能：空间太大刷入外存。接口不变。
};


struct HashString {
    unsigned long long operator()(const std::string &str) {
        int ans = 0;
        for (int i = 0; str[i] != '\0'; ++i) ans = ans * 19260817 + str[i];
        return ans;
    }
};

//memo 关于写法：queue整万划分，queue基本在内存中进行，直到内存达万划入一个整万块。在refund时，一个个把整万块拿出来在栈空间检查，然后如果某个整万块退成了，把那个块在内存里改掉后在外存里全部覆写一遍。

//memo 关于userorder的写法： 内存中维护一个{loginuser，the beginning of userorder-list} 的unordered_map,然后所有userorder-list 存入一个文件，如果某个user内存中有过长的order，就把它刷到文件末尾，并附加刷一个指向上一个块的位置。
// 优化1.如果某次query_order发现碎片块过多了，就把它们全读入内存整合，然后在文件末尾写入一个整合后的连续链表。优化2. 维护某个外存整块100bit-200bit，200bit-300bit被用了多少字节。如果某一时刻发现其用了0字节，就知道它可用了。以此完成垃圾回收。
//caution 因为毕竟是F操作，如果时耗足够小这两个优化是可以一起不用做的。如果队列够短，也是不用做整万划分的。
//更正：写成vector

//bug 数据结构有bug，先把这个写好吧。
#endif //MAIN_CPP_MAPS_H