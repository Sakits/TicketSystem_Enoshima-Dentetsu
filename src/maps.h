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

// Hash 将 Key 映射为一个 unsigned long long
template<class Key, class Value, class Hash>
class OuterUniqueUnorderMap 
{
private:
    char file[30];
    BPlusTree bpt;
    std :: fstream fio;

public:
    OuterUniqueUnorderMap(const char* _datafile, const char* _bptfile) : bpt(_bptfile)
    {
        strcpy(file, _datafile);
        std :: fstream fin(_datafile, std :: ios :: in | std :: ios :: binary);
        if (!fin.is_open())
        {
            std :: fstream fout(_datafile, std :: ios :: out | std :: ios :: binary);
            fout.close();
        }
        fin.close();

        fio.open(_datafile, std :: ios :: in | std :: ios :: out | std :: ios :: binary);
    };

    ~OuterUniqueUnorderMap() {fio.close();}

    bool insert(std::pair<Key, Value> &pair) 
    {
        fio.seekg(0, std :: ios :: end);
        int pos = fio.tellp();

        bool flag = bpt.insert(Hash()(pair.first), pos, 1);
        if (flag)
            fio.write(reinterpret_cast<char*>(&(pair.second)), sizeof(pair.second));
        
        return flag;
    }

    bool erase(const Key &key) 
    {
        return bpt.erase(Hash()(key));
    }

    std::pair<int, bool> find(const Key &key) 
    {
        int f = bpt.query(Hash()(key));
        return {f, ~f ? 1 : 0};
    }

    Value getItem(int pos) 
    {
        Value ans;
        fio.seekg(pos, std :: ios :: beg);
        fio.read(reinterpret_cast<char*>(&ans), sizeof(ans));
        return ans;
    }

    void setItem(int pos, Value &value) 
    {
        fio.seekg(pos, std :: ios :: beg);
        fio.write(reinterpret_cast<char*>(&value), sizeof(value));
    }

    int get_size() const
    {
        return bpt.get_size();
    }

    bool empty() const 
    {
        return bpt.get_size() == 0;
    }

    void clear() 
    {
        fio.close();

        std :: fstream fout(file, std :: ios :: out | std :: ios :: binary);
        fout.close();

        fio.open(file, std :: ios :: in | std :: ios :: out | std :: ios :: binary);

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
struct InnerList{
    struct Node {
        T t;
        Node *n = nullptr;
        Node *p = nullptr;

        Node() {}

        Node(T t, Node *n, Node *p) : t(t), n(n), p(p) {}
    } ;
    Node *root = nullptr;
    int num = 0;

    int size(){return num;}

    InnerList() {
        root = new Node();
    }

    InnerList(const InnerList&) = delete;
    InnerList& operator=(const InnerList&) = delete;

    virtual ~InnerList() {
        while (root) {
            Node *nextroot = root->n;
            delete root;
            root = nextroot;
        }
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

    void clear(){
        Node * tmp = root->n;
        while (tmp) {
            Node *nexttmp = tmp->n;
            delete tmp;
            tmp = nexttmp;
        }
        root->n = nullptr;
        num = 0;
    }

    void print() const{
        for(auto it = begin(); it != end(); ++it) std::cout << *it << " ";
        std::cout << std::endl;
    }
};



template<class Key, class Value, class Hash>
class InnerOuterMultiUnorderMap {
public:
    InnerOuterMultiUnorderMap(){

    }
    ~InnerOuterMultiUnorderMap(){
        clear();
    }

    void insert(const std::pair<Key, Value> &pair) {
        InnerList<Value>* keylist = find(pair.first);
        keylist->push_front(pair.second);
    }

    typedef Value* Iterator;

    Iterator find(Key key, int n) {//找到第n新插入的东西。
        InnerList<Value>* keylist = find(key);
        if(n < 1 || n > keylist->size()) return nullptr;
        auto it = keylist->begin();
        while(--n) ++it;
        return &(it.ptr->t);
    }

    InnerList<Value>* find(Key key) {
        auto iter = mapper.find(key);
        if (iter == nullptr) {
            auto listptr = new InnerList<Value>();
            setter.push_front(listptr);
            mapper.insert({key,listptr});
            return *(mapper.find(key));
        }
        return *iter;
    };

    void clear() {
        for(auto it = setter.begin(); it != setter.end(); ++it) delete (*it);
    }

//todo 还没可持久化
//better 空间太大刷入外存 接口不变 这跟Queue的原理应当类似
private:
    InnerUniqueUnorderMap<Key, InnerList<Value>*, Hash> mapper;
    InnerList<InnerList<Value>*> setter;
};



//recommend to implement by a list
//singleton pattern


template<class T>
struct Queue : InnerList<T> {
    FileName fileName;

    using Node = typename InnerList<T>::Node;
    using Iterator = typename InnerList<T>::Iterator;

    Queue(FileName fileName) : fileName(fileName) {
        fcreate(fileName);
        std::ifstream file(fileName, std::ios::in | std::ios::binary);
        assert(file);
        T t;
        Node *constructor = InnerList<T>::root;
        while (file) {
            fread(file, t);
            if(file)
            constructor = constructor->n = new Node(t, nullptr, constructor);
        }
        file.close();
    }

    virtual ~Queue() override {
        fclear(fileName);
        std::ofstream file(fileName, std::ios::out | std::ios::binary);
        assert(file);
        while (InnerList<T>::root) {
            Node *nextroot = InnerList<T>::root->n;
            if (nextroot) fwrite(file, nextroot->t);
            InnerList<T>::root= nextroot;
        }
        file.close();//fixme 开了文件就别关了，退出再关。
    }
    //better 加功能：空间太大刷入外存。接口不变。
};


//memo 关于写法：queue整万划分，queue基本在内存中进行，直到内存达万划入一个整万块。在refund时，一个个把整万块拿出来在栈空间检查，然后如果某个整万块退成了，把那个块在内存里改掉后在外存里全部覆写一遍。

//memo 关于userorder的写法： 内存中维护一个{loginuser，the beginning of userorder-list} 的unordered_map,然后所有userorder-list 存入一个文件，如果某个user内存中有过长的order，就把它刷到文件末尾，并附加刷一个指向上一个块的位置。
// 优化1.如果某次query_order发现碎片块过多了，就把它们全读入内存整合，然后在文件末尾写入一个整合后的连续链表。优化2. 维护某个外存整块100bit-200bit，200bit-300bit被用了多少字节。如果某一时刻发现其用了0字节，就知道它可用了。以此完成垃圾回收。
//caution 因为毕竟是F操作，如果时耗足够小这两个优化是可以一起不用做的。如果队列够短，也是不用做整万划分的。
//更正：写成vector

//bug 数据结构有bug，先把这个写好吧。
#endif //MAIN_CPP_MAPS_H
