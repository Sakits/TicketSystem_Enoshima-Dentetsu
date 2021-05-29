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
    std :: fstream fio;
    char file[50];
    int last, front, rear;

    struct edge
    {
        T node;
        int pos, pre, nxt, epre, enxt;
    };

    static const int maxn = 2000, mod = 10007;
    int tot;
    int elast[mod];
    edge e[maxn + 1];

    int find(int x)
    {
        int hs = x % mod;
        for (int i = elast[hs]; i; i = e[i].pre)
        {
            // printf("i:%d\n", i);
            if (x == e[i].pos)
                return i;
        }
        
        return 0;
    }

    int mpinsert(int x, int pos = 0)
    {
        int hs = x % mod;
        if (!pos)
            pos = ++tot;
        e[pos] = (edge){T(), x, elast[hs], 0, 0, 0};

        if (elast[hs])
            e[elast[hs]].nxt = pos;
        elast[hs] = pos;

        return pos;
    }

public:
    MemoryPool(const char* file_name) 
    {
        tot = 0;
        memset(elast, 0, sizeof(elast));
        last = -1;
        front = rear = 0;
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

    void cut(int x)
    {
        if (rear == x)
            rear = e[x].epre;

        // printf("cut: x:%d e[x].epre:%d\n", x, e[x].epre);

        if (e[x].epre)
            e[e[x].epre].enxt = e[x].enxt;
        if (e[x].enxt)
            e[e[x].enxt].epre = e[x].epre;

        e[x].epre = e[x].enxt = 0;
    }

    void release(int x)
    {
        // printf("e[x].pos:%d\n", e[x].pos);
        fio.seekg(e[x].pos, std :: ios :: beg);
        fio.write(reinterpret_cast<char *>(&e[x].node), sizeof(e[x].node));
    }

    ~MemoryPool() 
    {
        while (front)
        {
            release(front);
            front = e[front].enxt;
        }
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

    void move_to_front(int x)
    {
        if (front == x)
            return;

        e[x].enxt = front;
        if (front)
            e[front].epre = x;
        front = x;
    }
    
    // void check(const T &p)
    // {
    //     for (int i = 0; i < p.size; i++)
    //         printf("%llu ", p.key[i]);
    //     puts("");
    // }

    T& cache_read(const int pos, int dep)
    {
        if (pos == -1) exit(0);
        int x = find(pos);
        
        if (x)
        {
            if (x != front)
            {
                cut(x);
                move_to_front(x);
            }
            // printf("pos:%d\n", pos);
            // check(e[x].node);
            return e[x].node;
        }

        if (tot < maxn)
        {
            x = mpinsert(pos);
            if (tot == 1)
                rear = x;
            // printf("tot:%d x:%d rear:%d\n", tot, x, rear);
        }
        else
        {
            x = rear;
            // printf("%d %d front:%drear:%d e[rear].epre:%d\n", tot, maxn, front, rear, e[rear].epre);
            int tmp = rear;

            cut(x);
            release(x);

            if (e[x].pre)
                e[e[x].pre].nxt = e[x].nxt;
            if (e[x].nxt)
                e[e[x].nxt].pre = e[x].pre;
            else
                elast[e[x].pos % mod] = e[x].pre;

            x = mpinsert(pos, tmp);
        }

        move_to_front(x);

        fio.seekg(e[x].pos, std :: ios :: beg);
        fio.read(reinterpret_cast<char *>(&e[x].node), sizeof(e[x].node));
        // printf("pos:%d\n", pos);
        // check(e[x].node);
        return e[x].node;
    }

    T& read(const int pos, int dep)
    {
        return cache_read(pos, dep);
    }

    void read(const int pos, int &p, int dep)
    {
        fio.seekg(pos, std :: ios :: beg);
        fio.read(reinterpret_cast<char *>(&p), sizeof(p));
    }

    void write(const int pos, T &p, int dep)
    {
        cache_read(pos, dep) = p;
    }

    void write(const int pos, int p, int dep, int offset)
    {
        if (offset)
            cache_read(pos, dep).preptr = p;
        else
            cache_read(pos, dep).nxtptr = p;
    }

    template<typename TT>
    void file_read(const int pos, TT &p)
    {
        fio.seekg(pos, std :: ios :: beg);
        fio.read(reinterpret_cast<char *>(&p), sizeof(p));
    }

    template<typename TT>
    void file_write(const int pos, TT &p)
    {
        fio.seekg(pos, std :: ios :: beg);
        fio.write(reinterpret_cast<char *>(&p), sizeof(p));
    }

    void insert(T &p, int dep)
    {
        if (~last)
        {
            cache_read(last, dep) = p;
            fio.seekg(last + sizeof(T), std :: ios :: beg);
            fio.read(reinterpret_cast<char *>(&last), sizeof(last));
        }
        else
        {
            fio.seekg(0, std :: ios :: end);
            fio.write(reinterpret_cast<char *>(&p), sizeof(p));
            int tmp = -1;
            fio.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
        }
    }

    template<typename TT>
    void file_insert(TT &p)
    {
        if (~last)
        {
            fio.seekg(last, std :: ios :: beg);
            fio.write(reinterpret_cast<char *>(&p), sizeof(T));
            fio.tellg();
            fio.read(reinterpret_cast<char *>(&last), sizeof(last));
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
        tot = 0;
        memset(elast, 0, sizeof(elast));
        fio.close();
        fio.open(file, std :: ios :: trunc | std :: ios :: in | std :: ios :: out | std :: ios :: binary);
    }
};

#endif
