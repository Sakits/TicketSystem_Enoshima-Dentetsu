#ifndef _BPlusTree_h
#define _BPlusTree_h

#include <iostream>
#include <cstring>
#include <cstdio>
#include <fstream>

using namespace std;

class BPlusTree
{
    static const int max_size = 40, block_size = max_size / 2;

    public:
        int file_read_cnt = 0, file_write_cnt = 0;

    private:
        char file[30];
        fstream fio;
        int prex = -1, prec, presize, prenxt;

        class Node
        {
            public:
                int nxtptr = -1, preptr = -1, size = 0;
                int isleaf = 1;
                int child[max_size + 1], mxpos[max_size + 1];
                char key[max_size + 1][50];

                Node() 
                {
                    memset(child, 0, sizeof(child));
                    memset(mxpos, 0, sizeof(mxpos));
                    memset(key, 0, sizeof(key));
                }

                Node &operator = (const Node &other)
                {
                    if (this == &other) return *this;

                    nxtptr = other.nxtptr;
                    preptr = other.preptr;
                    size = other.size;
                    isleaf = other.isleaf;

                    memcpy(child, other.child, sizeof(child));
                    memcpy(key, other.key, sizeof(key));
                    memcpy(mxpos, other.mxpos, sizeof(mxpos));

                    return *this;
                }
        };

        template<typename T>
        void file_read(const int pos, T &p)
        {
            fio.seekg(pos, ios :: beg);
            fio.read(reinterpret_cast<char *>(&p), sizeof(p));
        }

        template<typename T>
        void file_write(const int pos, T &p)
        {
            fio.seekg(pos, ios :: beg);
            fio.write(reinterpret_cast<char *>(&p), sizeof(p));
        }

        int get_file_end()
        {
            fio.seekg(0, ios :: end);
            int pos = fio.tellp();
            return pos;
        }

        int get_pos(const Node &now, const char* _key, int pos)
        {
            for (int i = 0; i < now.size; i++)
            {
                int flag = strcmp(now.key[i], _key);
                if (flag > 0 || (!flag && now.mxpos[i] >= pos)) return i;
            }
            return now.size;
        }

        void insertone(Node &now, const int pos, const int file_pos, const char* _key, int mxpos)
        {
            for (int i = now.size; i > pos; i--)
            {
                now.child[i] = now.child[i - 1];
                strcpy(now.key[i], now.key[i - 1]);
                now.mxpos[i] = now.mxpos[i - 1];
            }

            now.child[pos] = file_pos;
            now.mxpos[pos] = mxpos;
            strcpy(now.key[pos], _key);
            now.size++;
        }

    public:
        BPlusTree (const char* file_name, int cache_size = 0)
        {
            strcpy(file, file_name);

            fstream fin (file, ios :: in | ios :: binary);
            if (!fin.is_open())
            {
                fstream fout(file, ios :: out | ios :: binary);
                Node initnode;
                fout.write(reinterpret_cast<char *>(&initnode), sizeof(initnode));
                fout.close();
            }
            fio.open(file, ios :: in | ios :: out | ios :: binary);
        }

        ~BPlusTree() {fio.close();}

        void insert(const char* _key, int file_pos, int x = 0, Node* const faptr = nullptr)
        {
            Node now; file_read(x, now);
            int pos = get_pos(now, _key, file_pos);

            if (now.isleaf) 
                insertone(now, pos, file_pos, _key, file_pos);
            else 
            {
                pos -= (pos == now.size);
                insert(_key, file_pos, now.child[pos], &now);
            }

            if (faptr)
            {
                Node &fa = *faptr;
                for (int i = 0; i < fa.size; i++)
                if (fa.child[i] == x) 
                    strcpy(fa.key[i], now.key[now.size - 1]), fa.mxpos[i] = now.mxpos[now.size - 1];
            }

            if (now.size > max_size)
            {
                Node nxt; int nxt_pos = get_file_end();
                nxt.isleaf = now.isleaf;
                nxt.size = now.size - block_size;
                nxt.nxtptr = now.nxtptr;
                nxt.preptr = x;

                if (~nxt.nxtptr) file_write(nxt.nxtptr + sizeof(int), nxt_pos);
                
                for (int i = block_size; i < now.size; i++)
                {
                    nxt.child[i - block_size] = now.child[i];
                    strcpy(nxt.key[i - block_size], now.key[i]);
                    nxt.mxpos[i - block_size] = now.mxpos[i];
                }

                file_write(nxt_pos, nxt);

                now.size = block_size;
                now.nxtptr = nxt_pos;

                if (!faptr)
                {
                    Node root; int root_pos = get_file_end();
                    x = root_pos; file_write(nxt_pos + sizeof(int), x); 
                    root_pos = 0; 
                    root.isleaf = 0;
                    root.size = 2;
                    root.child[0] = x; root.child[1] = nxt_pos;
                    strcpy(root.key[0], now.key[now.size - 1]);
                    strcpy(root.key[1], nxt.key[nxt.size - 1]);
                    root.mxpos[0] = now.mxpos[now.size - 1];
                    root.mxpos[1] = nxt.mxpos[nxt.size - 1];

                    file_write(root_pos, root); 
                }
                else
                {
                    Node &fa = *faptr;

                    int pos = 0;
                    for (int i = 0; i < fa.size; i++)
                        if (fa.child[i] == x)
                        {
                            pos = i;
                            fa.child[i] = nxt_pos;
                            strcpy(fa.key[i], nxt.key[nxt.size - 1]);
                            fa.mxpos[i] = nxt.mxpos[nxt.size - 1];
                            break;
                        }

                    insertone(fa, pos, x, now.key[now.size - 1], now.mxpos[now.size - 1]);
                }
            }

            file_write(x, now);
        }

        bool erase(const char* _key, int file_pos = -1, int x = 0, Node* const faptr = nullptr)
        {
            Node now; file_read(x, now);
            
            if (now.isleaf)
            {
                int pos = -1;
                if (~file_pos)
                {
                    for (int i = 0; i < now.size; i++)
                    if (now.child[i] == file_pos)
                    {
                        pos = i;
                        break;
                    }
                }
                else
                {
                    for (int i = 0; i < now.size; i++)
                    {
                        if (!strcmp(now.key[i], _key))
                        {
                            pos = i;
                            break;
                        }
                    }
                }
            
                if (pos == -1) return 0;

                for (int i = pos; i < now.size - 1; i++)
                {
                    now.child[i] = now.child[i + 1];
                    strcpy(now.key[i], now.key[i + 1]);
                    now.mxpos[i] = now.mxpos[i + 1];
                }

                now.size--;
            }
            else
            {
                int pos = get_pos(now, _key, file_pos);
                if (!erase(_key, file_pos, now.child[pos], &now)) return 0;
            }


            int nxtptr = -1, preptr = -1;
            if (faptr)
            {
                int pos = 0;
                Node &fa = *faptr;
                for (int i = 0; i < fa.size; i++)
                if (fa.child[i] == x)
                {
                    pos = i;
                    break;
                }
                if (pos) preptr = fa.child[pos - 1];
                if (pos != fa.size - 1) nxtptr = fa.child[pos + 1];
            }

            if (now.size < block_size)
            {
                Node nxt, pre; 
                if (~nxtptr)
                {
                    file_read(now.nxtptr, nxt);
                    
                    if (nxt.size > block_size)
                    {
                        now.child[now.size] = nxt.child[0];
                        strcpy(now.key[now.size], nxt.key[0]);
                        now.mxpos[now.size] = nxt.mxpos[0];
                        now.size++;

                        for (int i = 0; i < nxt.size - 1; i++)
                        {
                            nxt.child[i] = nxt.child[i + 1];
                            strcpy(nxt.key[i], nxt.key[i + 1]);
                            nxt.mxpos[i] = nxt.mxpos[i + 1];
                        }
                        nxt.size--;

                        file_write(now.nxtptr, nxt);
                    }
                    else
                    {
                        for (int i = 0; i < nxt.size; i++)
                        {
                            now.child[now.size] = nxt.child[i];
                            strcpy(now.key[now.size], nxt.key[i]);
                            now.mxpos[now.size] = nxt.mxpos[i];
                            now.size++;
                        }

                        Node &fa = *faptr;
                        for (int i = 0; i < fa.size; i++)
                        {
                            if (fa.child[i] == now.nxtptr)
                            {
                                for (int j = i; j < fa.size - 1; j++)
                                {
                                    fa.child[j] = fa.child[j + 1];
                                    strcpy(fa.key[j], fa.key[j + 1]);
                                    fa.mxpos[j] = fa.mxpos[j + 1];
                                }
                                fa.size--;
                                break;
                            }
                        }

                        if (~nxt.nxtptr)
                        {
                            now.nxtptr = nxt.nxtptr;
                            file_write(nxt.nxtptr + sizeof(int), x);
                        }
                        else now.nxtptr = -1;
                    }
                }
                else if (~preptr)
                {
                    file_read(now.preptr, pre);
                    
                    if (pre.size > block_size)
                    {
                        for (int i = now.size; i; i--)
                        {
                            now.child[i] = now.child[i - 1];
                            strcpy(now.key[i], now.key[i - 1]);
                            now.mxpos[i] = now.mxpos[i - 1];
                        }
                        now.child[0] = pre.child[pre.size - 1];
                        strcpy(now.key[0], pre.key[pre.size - 1]);
                        now.mxpos[0] = pre.mxpos[pre.size - 1];
                        now.size++;
                        pre.size--;

                        file_write(now.preptr, pre);

                        Node &fa = *faptr;
                        for (int i = 0; i < fa.size; i++)
                        if (fa.child[i] == now.preptr)
                        {
                            strcpy(fa.key[i], pre.key[pre.size - 1]);
                            fa.mxpos[i] = pre.mxpos[pre.size - 1];
                            break;
                        }
                    }
                    else
                    {
                        for (int i = 0; i < now.size; i++)
                        {
                            now.child[i + pre.size] = now.child[i];
                            strcpy(now.key[i + pre.size], now.key[i]);
                            now.mxpos[i + pre.size] = now.mxpos[i];
                        }

                        for (int i = 0; i < pre.size; i++)
                        {
                            now.child[i] = pre.child[i];
                            strcpy(now.key[i], pre.key[i]);
                            now.mxpos[i] = pre.mxpos[i];
                        }
                        now.size += pre.size;

                        Node &fa = *faptr;
                        for (int i = 0; i < fa.size; i++)
                        {
                            if (fa.child[i] == now.preptr)
                            {
                                for (int j = i; j < fa.size - 1; j++)
                                {
                                    fa.child[j] = fa.child[j + 1];
                                    strcpy(fa.key[j], fa.key[j + 1]);
                                    fa.mxpos[j] = fa.mxpos[j + 1];
                                }
                                fa.size--;
                                break;
                            }
                        }

                        if (~pre.preptr)
                        {
                            now.preptr = pre.preptr;
                            file_write(pre.preptr, x);
                        }
                        else now.preptr = -1;
                    }
                }
            }

            if (faptr)
            {
                Node &fa = *faptr;
                if (fa.size > 1 || ~fa.preptr || ~fa.nxtptr)
                {
                    for (int i = 0; i < fa.size; i++)
                    if (fa.child[i] == x)
                    {
                        strcpy(fa.key[i], now.key[now.size - 1]);
                        fa.mxpos[i] = now.mxpos[now.size - 1];
                        break;
                    }
                }
                else
                    x = -1, fa = now;
            }
            if (~x) file_write(x, now);

            return 1;
        }

        int query(const char* _key, int x = 0)
        {
            Node now; file_read(x, now);

            if (now.isleaf)
            {
                for (int i = 0; i < now.size; i++)
                    if (!strcmp(now.key[i], _key)) 
                    {
                        prex = x; prec = i; presize = now.size; prenxt = now.nxtptr;
                        return now.child[i];
                    }
                return -1;
            }
            
            int pos = get_pos(now, _key, -1);
            return (pos == now.size) ? -1 : query(_key, now.child[pos]);
        }

        int get_next()
        {
            if (prex == -1) return -1;

            if (prec < presize - 1)
            {
                int ans;
                prec++;
                fio.seekg(prex + sizeof(int) * 4 + sizeof(int) * prec, ios :: beg);
                fio.read(reinterpret_cast<char *>(&ans), sizeof(ans));
                return ans;
            }
            else
            {
                if (prenxt == -1) return -1;
                Node now; fio.seekg(prenxt, ios :: beg);
                fio.read(reinterpret_cast<char *>(&now), sizeof(now));
                prex = prenxt; prec = 0; presize = now.size; prenxt = now.nxtptr; 
                return now.child[0];
            }
        }
};

#endif