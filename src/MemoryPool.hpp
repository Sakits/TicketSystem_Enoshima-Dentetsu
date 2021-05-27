#ifndef _MemoryPool_h
#define _MemoryPool_h

#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>

template<typename T>
class MemoryPool
{
private:
    char file[50];
    int last;
    std :: fstream fio;

public:
    MemoryPool(const char* file_name) : last(-1)
    {
        strcpy(file, file_name);
        
        std :: fstream fin (file, std :: ios :: in | std :: ios :: binary);

        if (!fin.is_open())
        {
            std :: fstream fout(file, std :: ios :: out | std :: ios :: binary);
            fout.write(reinterpret_cast<char *>(&last), sizeof(last));
            fout.close();
        }
        else
        {
            fin.read(reinterpret_cast<char *>(&last), sizeof(last));
            fin.close();
        }
        
        fio.open(file, std :: ios :: in | std :: ios :: out | std :: ios :: binary);
    }

    ~MemoryPool() 
    {
        fio.seekg(0, std :: ios :: beg);
        fio.write(reinterpret_cast<char *>(&last), sizeof(last));
        fio.close();
    }

    int get_file_end()
    {
        fio.seekg(0, std :: ios :: end);
        int pos = fio.tellp();
        return pos;
    }

    int get_nxt_pos()
    {
        return ~last ? last : get_file_end();
    }

    template<typename TT>
    void read(const int pos, TT &p)
    {
        fio.seekg(pos, std :: ios :: beg);
        fio.read(reinterpret_cast<char *>(&p), sizeof(p));
    }

    template<typename TT>
    void write(const int pos, TT &p)
    {
        fio.seekg(pos, std :: ios :: beg);
        fio.write(reinterpret_cast<char *>(&p), sizeof(p));
    }

    template<typename TT>
    void insert(TT &p)
    {
        if (~last)
        {
            // printf("start: last:%d\n", last);
            fio.seekg(last, std :: ios :: beg);
            fio.write(reinterpret_cast<char *>(&p), sizeof(T));
            fio.tellg();
            fio.read(reinterpret_cast<char *>(&last), sizeof(last));
            // printf("las:%d\n", last);
        }
        else
        {
            fio.seekg(0, std :: ios :: end);
            fio.write(reinterpret_cast<char *>(&p), sizeof(T));
            int tmp = -1;
            fio.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
        }
    }

    void erase(int pos)
    {
        fio.seekg(pos + sizeof(T), std :: ios :: beg);
        fio.write(reinterpret_cast<char *>(&last), sizeof(last));
        last = pos;
    }

    void clear()
    {
        fio.close();
        fio.open(file, std :: ios :: trunc | std :: ios :: in | std :: ios :: out | std :: ios :: binary);
    }
};

#endif
