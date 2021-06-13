# 火车票管理系统设计文档

组队成员：唐嘉铭，单佳铭

## 整体架构

#### 文件结构

- main.cpp

  从主函数进入，由function_chooser进入各个函数，完成相应任务。

- UserTrainOrder.h

  提供了main.cpp中各个函数所需要的类和方法，主要有User，Train,Order，及附属的一系列组织它们的数据结构的实例

- DataStructures.h

  提供了UserTrainOrder.h中所需要的数据结构

- logger.h

  提供了日志打印，对标，时间测试的能力。在关掉日志功能时，它只提供简单的输出与错误处理。
  
- vector.hpp

  STL大作业中的vector。（by 单佳铭）
  
- map.hpp

  STL大作业中的map。（by 唐嘉铭）

- utility.hpp

  STL大作业中的pair.

- exception.hpp

  STL大作业中的错误处理.

#### 层级关系

- main.cpp
  - logger.h
  - UserTrainOrder.h
    - DataStructures.h

- vector 和map和pair在以上各处都有使用
  - exception.hpp（为了兼容性引入，实际上不会用到）

#### DataStructures.h

定义了五个数据结构：

DiskMap：对BPlusTree的一个封装，模拟了map的服务，不支持重复值。插入某一Value时，会将Value写入数据文件，并将其写入的地址存入BPlueTree。
其setItem时需要提供的一个int，是写入地址。常被命名为xxPtr。

HashMap：用拉链法实现了哈希表。单键值。

CacheMap：实现了多键值外存版map，通过缓存使得其插入几乎不需要文件读写，内存超出Threshold后刷至外存。同时保证外存中数据连续，使得如QueryOrder等操作可以一次读出所有Order。实现方法类似vector，针对某个key值对应的value如果写满了，就再在文件末尾开两倍的空间，并且将原来的一倍的数据复制过去。在内存中，使用HashMap与List结合来进行多键值存储。使用DiskMap存储各key的元数据，如其对应的Value们现在在数据文件中的起始地址是多少，有多少个Value在文件中，现在不扩容的话最多能写多少Value。只在构造时用DiskMap加载数据，之后这些数据都在内存中，析构时写回DiskMap即可。

List：普普通通的链表。

CacheList：一个会在构造时从文件中读取数据，析构时把数据写回文件的链表。

#### UserTrainOrder.h

定义的**类**有：

cStringType<int N>：可赋值的char[N]；

HourMinute: 时间类型，几分几秒

MonthDate: 时间类型，几月几号

FullDate：时间类型，几月几号几分几秒

User：User的除username以外的全部信息（因为使用时是DiskMap<username, User>的形式。）

Train：关于某个TrainID的车的全部信息，包括其在93天内各站票数分别是多少。

ZipedTrain：将票数从4字节压为了3字节，压缩了20%左右。写了从Train到ZipedTrain的构造函数和从ZipedTrain到Train的转换函数，从而使存储方式由Train变为ZipedTrain时代码不用改动。只用于存储，使用时便会隐式转换为Train类型。

Order：和某个订单有关的一切信息。

定义的**对象**有：

HashMap<Username, Privilege> loginUsers

DiskMap<Username, User> existUsers

DiskMap<TrainID, ZipedTrain> existTrains

CacheMap<StationName, TrainPtr> stmap //stmap是StationName_TrainPtr_Map的缩写。TrainPtr是int，是existTrains对应的列车存储地址。

MidRetType findMidStation(StationName fromStation, StationName toStation)

CacheMap<Username, Order> userOrders

CacheList<Order> waitQueue

#### main.cpp

从main进入，循环执行function_chooser。若catch到抛出的ErrorOccur异常，返回-1。

function_chooser：接收字符串，抽出相应的信息，进行完所有需要的字符处理以后进入相应的函数。

**亮点**：使用``表驱动法``大幅缩短了代码长度，减少了代码出错概率，增加了代码的可维护性。
平凡地实现了各函数。
```
namespace user {

    void login(Username, Password);

    void logout(Username);

    void add_user(Username, Username, Password, Name, MailAddr, Privilege);

    void query_profile(Username, Username);

    void modify_profile(Username, Username, Password, Name, MailAddr, Privilege);

}

namespace train {

    void add_train(TrainID, StationNum, SeatNum, StationNames, Prices, HourMinute, TravelTimes, StopoverTimes, SaleDates, Type);

    void release_train(TrainID);

    void query_train(TrainID, MonthDate);

    void delete_train(TrainID);

    void query_ticket(StationName, StationName, MonthDate, TwoChoice);

    void query_transfer(StationName, StationName, MonthDate, TwoChoice);

    void buy_ticket(Username, TrainID, MonthDate, TicketNum, StationName, StationName, TwoChoice);

    void query_order(Username);

    void refund_ticket(Username, OrderNumth);
}

namespace sys {

    void noReturnClean();

    void clean();

    void exit();
}

```

#### logger.h

主要实现了三大功能，可以根据使用需要开关：

**亮点**：使用装饰性设计，非常小巧灵活。

1. 日志记录

   记录输入输出字符串，用Error函数记录出错原因。

2. 自动对标

   该程序输出时均用Return函数，该函数输出指定内容，并从标答中拉取一行答案比对两者是否相同。

3. 时间统计

   在任意地方插入ResetClock或LocalClock，即可装饰性地查看某段或某函数的时间消耗和被运行了多少次。详见于数据结构群里发的time_tracer.h

## 组员分工

唐嘉铭：BPlusTree的编写，DataStructure.h中DiskMap与HashMap的编写，算法框架设计。

单佳铭：其余逻辑部分编写，算法框架设计。

