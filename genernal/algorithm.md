### 数据范围 
- stationnum 100
- traveltime 10000
- stopovertime 10000
- saledate 90

## 算法
&emsp;&emsp;先写朴素算法，通过数据之后可以用树状数组 / zkw线段树 和 map / bpt 优化复杂度（不知道效果怎么样）。
&emsp;&emsp;class train 储存火车信息以及是否已发布，用一棵 B+ 树储存。
&emsp;&emsp;class ticket 储存火车票的对应火车编号（出发站和下一站不用存）和剩余张数。
&emsp;&emsp;class station 储存车站的车票信息，维护一个 vector\<ticket>t[63] 表示 每一天（最多 60 + 3 天）的从当前站点出发的火车票。

### SF (~1000000)
#### query_ticket
&emsp;&emsp;todo

#### buy_ticket
&emsp;&emsp;减少 -n 张在 -d 从 -f 出发，到达 -t 的 -i 火车的车票数
&emsp;&emsp;做法：先用 train 的 bpt 找到 -i 的火车从 -f 到 -t 所途径的所有站点。到每个站点里把计算出的第 -d 天的 -i 的车票都减少 -n 张。

### N (~10000)
#### release_train
&emsp;&emsp;在 -d 的每天给 -n 个车站加 -m 张车票。
&emsp;&emsp;做法：遍历所有的车站，计算出相应的 -d，在那个时间段里的每一天 i 给 t[i] 添加车票。
#### query_transfer
&emsp;&emsp;查询在 -d 从 -s 出发，到达 -t，恰好换乘一次的车次。
&emsp;&emsp;枚举中转的站点，变成查询在 -d 从 -s 到 -mid，再从 -mid 到 -t 的两个子问题。对 -s 到 -mid 进行 query_ticket 操作，再查询所有从 -mid 出发到 -t 的车票，把 -s 到 -mid 和 -mid 到 -t 的车票分别按 -d 排序，这样之后可以通过 O(n) 得到最优解。

