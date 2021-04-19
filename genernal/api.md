## 构造
构造函数 

 - file_name : B+ 树储存位置的文件名
- cache_size : 给该 B+ 树分配的缓存大小（默认为 0）

> BPlusTree (const char* file_name, int cache_size = 0);
	
## 插入
B+ 树只用于储存节点对应信息在文件中的位置，所以 file_pos 为 int 类型。

插入键值为 _key，文件位置为 file_pos 的点。

unique = 1 （默认为 0）表示若树中已存在 _key 的键值的点则返回 0。

> bool insert(const char* _key, int file_pos, bool unique = 0);

删除键值为 _key，文件位置为 file_pos 的点（若不输入 file_pos 则需保证键值为 _key 的节点只有一个）。

找不到键值为 _key 的节点返回 0，删除成功返回 1。

> bool erase(const char* _key, int file_pos = -1);

## 查询
提供两个查询接口。

B+树中存在一个指针，它会指向上一次查询时返回的叶节点（用于 `get_next()` 操作）。

查询键值为 _key 的点在文件中的位置（如果有多个 key 则返回最小的文件位置），并把 B+ 树中的指针指向该节点。

> int query(const char* _key);
	
返回 B+ 树中当前指向的叶子节点的下一个叶子结点，并把当前的指针指向它。

叶子结点顺序按 _key  为第一关键字，按 file_pos 为关键字排序。

> int get_next();