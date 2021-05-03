//
// Created by JiamingShan on 2021/5/2.
//

#ifndef MAIN_CPP_MAPS_H
#define MAIN_CPP_MAPS_H


template<class Key, class Value, class Hash>
class OuterUniqueUnorderMap {
public:

    bool insert(const std::pair<Key, Value> &pair) {
        return mapper.insert(pair).second;
    }

    bool erase(const Key &key) {
        return mapper.erase(key);
    }


//    struct Iterator {
//    };

    typedef typename std::unordered_map<Key, Value, Hash>::iterator Iterator;

    std::pair<Iterator, bool> find(Key key) {
        auto f = mapper.find(key);
        if(f==mapper.end()) return {mapper.end(),0};
        return {f,true};
    }

    Value getItem(Iterator iter) {
        return iter->second;
    }

    void setItem(Iterator iter, Value value) {
        iter->second = value;
    }

    bool empty() const {
        return mapper.empty();
    }

    void clear() {
        mapper.clear();
    }
private:
    std::unordered_map<Key, Value, Hash> mapper;
};


template<class Key, class Value, class Hash>
//Hash是一个模板类名，它实例化后的一个对象例为auto h = Hash<string>(), 这个对象重载了括号，比如可以h(1),然后返回一个size_t
class InnerUniqueUnorderMap {
public:

    bool insert(const std::pair<Key, Value> &pair) {
        return mapper.insert(pair).second;
    }

    bool erase(const Key &key) {
        return mapper.erase(key);
    }

    bool empty() const {
        return mapper.empty();
    }

    void clear() {
        mapper.clear();
    }

    Value *find(Key key) {
        auto f = mapper.find(key);
        if(f==mapper.end()) return nullptr;
        return &(f->second);
    }

private:
    std::unordered_map<Key, Value, Hash> mapper;
};


//recommend to implement by a list
//singleton pattern
template<class T>
struct Queue {
    struct Iterator{
        Iterator& operator++(){}
        Iterator operator++(int) {//I promise that while using ++ and * and ->, won't use erase and pushfront.
            Iterator ret = *this;
            ++*this;
            return ret;
        }
        T& operator*(){

        }
        T* operator->() const noexcept {
            return &**this;
        }
    };
    void push_front(T t){//push to the place where begin() is pointing to.

    }
    Iterator erase(Iterator){//return Iterator to next element, where ++Iterator go to the same element.

    }

    Iterator begin(){//pointing to most recent push_front element

    }
    Iterator end(){

    }
    void readFromFile(){
    }
    void writeToFile(){}
};

#endif //MAIN_CPP_MAPS_H
