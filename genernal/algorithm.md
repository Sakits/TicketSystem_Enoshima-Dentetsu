### 数据范围 
- stationnum 100
- traveltime 10000
- stopovertime 10000
- saledate 90

## 算法
&emsp;&emsp;先写朴素算法，通过数据之后可以用线段树 和 map / bpt 优化复杂度（不知道效果怎么样）。
&emsp;&emsp;class train 储存火车每一个站点的名字，剩余票数（可用线段树优化），到达时间。
&emsp;&emsp;class station 里面有一个 bitset<10000>，每一位 0 / 1 表示某个火车是否经过这个车站。

### SF (~1000000)
#### query_ticket 
&emsp;&emsp;查询在 -d 从 -s 出发，到达 -t 的所有车次。
&emsp;&emsp;做法：将 -s 的 bitset 和 -t 的 bitset 取 and 操作，可以得到从 -s 到 -t 的所有火车。遍历这些火车，先检查这些火车到 -s 的时间是否覆盖了 -d，如果覆盖的话查询每个火车的 -s 和 -t 的位置（可以用 map / bpt 优化），再找到 -s 到 -t 之间的车票最小值（可用线段树优化），加入答案（该最小值即需要输出的最多可以买的车票数）。

#### buy_ticket
&emsp;&emsp;减少 -n 张在 -d 从 -f 出发，到达 -t 的 -i 火车的车票数。
&emsp;&emsp;做法：-i 区间减 -n。

### N (~10000)
#### release_train
&emsp;&emsp;在 -d 的每天给 -n 个车站加 -m 张车票。
&emsp;&emsp;做法：-i 区间加 -n。
#### query_transfer
&emsp;&emsp;查询在 -d 从 -s 出发，到达 -t，恰好换乘一次的车次。
&emsp;&emsp;做法：枚举中转的站点，变成查询在 -d 从 -s 到 -mid，再从 -mid 到 -t 的两个子问题。对 -s 到 -mid 进行 query_ticket 操作，再查询所有从 -mid 出发到 -t 的车票，把 -s 到 -mid 和 -mid 到 -t 的车票分别按 -d 排序，这样之后可以通过 O(n) 得到最优解。

