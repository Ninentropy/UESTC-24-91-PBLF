# PBLF项目后端食用指南

# 一、LinkedList

> 这是一个用于存储字符串的一个单向链表，其余的跟普通链表没什么区别。
>
> 在本项目中，该链表作为MusicList的属性，用于存储.txt文件中的每一行的字符串（一行字符串为一个节点，逐行遍历）
>
> 后续该链表也会用于开发歌单管理的模块，管理.txt文件，实现增删改查

  

## 1.添加节点

```cpp
void append(std::string value);
```

## 2.插入节点

```cpp
void insert(std::string value, int index);
```

## 3.删除节点

```cpp
void remove(int index);  
```

##   4.打印链表

```cpp
void display() const;
```

##  5.清除链表

```cpp
void clear();
```

## 6.获取链表长度  

```cpp
int size() const;
```

## 7.查找指定下标的元素值 

```cpp
 std::string findAt(int index);
```

## 8.判断链表是否为空  

```cpp
bool isEmpty() const;
```



## 二、Note

**属性**：

* int pitch：音高（实际值为midi协议中相应音高所对应的int值）
* int volume：音量（实际值一直为0x7f）



## 三、NoteList

> 单个NoteList的存储的不是一个乐谱的所有音符，而是以一次和弦为单位，存储和弦内的所有音符，用于实现这些音符的统一播放。
>
> 除了存储的节点（Node）是音符节点外，其余大体与普通链表无异。

**属性**：

* double duration：音符的持续时间（单位毫秒，默认值500，为一个四分音符持续时间）。之所以把这个属性定义在链表里而不是音符里，是因为同一个和弦的音符的节拍也是相同的。

与LinkedList相同的函数就不再缀叙。

### 1.设置&获取持续时间

```cpp
double getDuration() const;        // 获取持续时间
void setDuration(double d);        // 设置持续时间
```

* 主要是实现对私有属性duration的一个赋值和调用。

### 2.将音符转换为缓冲数组

```cpp
std::vector<int> transformNoteBuffer();
```

> 这个函数主要是实现将NoteList里存储的音符及其相应的属性（音高，音量）一一读取出来，并按照Windows midi协议将音符转换成一个个整型数字（一个数字代表一个音符），最后存入动态数组并返回。
>
> 之所以叫缓冲数组是它提供了一个暂存音符的容器，用于后续遍历并输出到midi设备中。



## 四、工具函数

### 1.判断是否为纯数字

> 使用正则表达式判断是否为纯数字。
>
> 使用场景：由于音乐的节奏并不是一成不变的，所以在变奏的时候会单独出现一个纯数字行来重新定义接下来的音符的默认节奏。因此，为了判断这一行是否为节奏定义行（而不是音符行），就需要调用这个函数来判断。



## 五、MusicList（考虑改名）

> 这一串代码属于负责raw data（.txt文件）的解析阶段，所以以字符串和文件的操作为主，主要目的还是将文件内容解析到字符串链表里。

**属性**：

* LinkedLIst list：字符串链表，每一个节点存储的是.txt文件中的每一行字符串。

### 1.添加文本行

```cpp
void add(std::string s)
```

* 将读取到的每一行字符串逐个添加到字符串链表中（包括空行，因为空行要用于左右手谱的区分）

### 2.读取文件

```cpp
void readFile(std::string fileName="")
```

* 逐行读取.txt文件，并调用add函数存储。

### 3.清除链表

```cpp
void clear(){
```



## 六、MusicPlayer

**属性**：

* enum scale：实现不同音高的音符其所对应的midi协议的值的映射。
* HMIDIOUT handle：midi输出句柄
* bool ENDMUSIC=0：为1时停止播放
* int STOP：为2时（因为是双线程演奏）停止播放
* std::mutex mu：互斥锁，保护多线程共享资源
* int tick1,tick2：时间戳，计算当前线程运行经过的时间（ms）

### 1.对字符串行分类

```cpp
void classifyList(MusicList &m)
```

* 逐行解析从文件中读取的字符串链表，将每一行的数据分类为节奏行和音符行。

* 如果是节奏行，存储值duration中；如果是音符行，则将字符串发送至play函数进行进一步解析。

### 2.转化成音符

```cpp
Note parseNote(const std::string& noteStr)
```

* 将输入的处理后的字符串（仅带有音符信息）进行解析，转化为音符对象。

* 案例输入：2^
* 错误输入：
  * 2^_（带有节奏字符）
  * [4,1,]（带有多个音符）

转换对应法则：

| 字符  | 说明   | 使用示例                                               |
| ----- | ------ | ------------------------------------------------------ |
| `1~7` | 音符   | 如 `1 2 3 4 5` 表示播放 `do re mi fa sol` 五个音符     |
| `0`   | 休止符 | 略                                                     |
| `,`   | 低音   | 即音符向下附点，最多可以有三个低音号。如 `1,` `2,,` 等 |
| `^`   | 高音   | 类比低音                                               |
| `#`   | 升音   | 向上半音高。如 `4#`                                    |

### 3.将单行字符串转化为NoteMap

**NoteMap数据结构图示**：<img src="https://www.helloimg.com/i/2024/12/15/675e514747740.png" />

* 每一个NoteNode对应一个音符
* 每一个NoteList存储一个和弦上的所有音符（非和弦则只存一个音符）
* 每一个vector存储一行字符串上解析出来的所有NoteList
* 最终构成NoteMap

**主要步骤**：

* 将字符串按照空格拆分为一个个token

* 处理token尾部的节奏字符串从而为duration赋值，同时删去节奏字符串；节奏映射如下：

  | 字符 | 说明   | 使用示例                                     |
  | ---- | ------ | -------------------------------------------- |
  | `-`  | 延音   | 每个延音号将为音符额外多出一个四分音符的延时 |
  | `_`  | 分音   | 每个分音号将使音符的延时减半                 |
  | `.`  | 附点   | 每个附点号将使音符的延时增半（×1.5）         |
  | `[]` | 和弦   | 中括号内的音符将一次性演奏                   |
  | `|`  | 小节线 | 程序将忽略这个符号                           |
  | `*`  | 三分音 | 每个分音号将使音符的延时除以三               |
  | `%`  | 五分音 | 每个分音号将使音符的延时除以五               |
  | `&`  | 七分音 | 每个分音号将使音符的延时除以七               |

* token分为和弦和非和弦，两种方式处理，殊途同归

* 如果是和弦，将token转化为一个个Note并存入NoteList

* 如果不是和弦，直接将Note存入NoteList

* 最后将获得的一个个NoteList存入NoteMap中

### 4.播放完整的一行乐谱

```cpp
void playSingleLine(std::vector<NoteList> noteLists, bool isMain)
```

> 主要是对NoteMap存储的音符按照midi要求统一输出。
>
> 同时跟随进度条，确保左右手进度一致，防止各弹各的。

### 5.播放整首曲子

```cpp
void play(std::string s1,std::string s2="",double duration = 500)
```

> 定义两个线程充当左右手去播放。



## 七、BGM

**属性**：

* MusicPlayer player：音乐播放对象
* MusicList nowList：每个节点存储.txt的字符串的一行信息

### 1.设置音乐

```cpp
void setMusic(std::string name)
```

* 设置读取的文件（实际上并没有调用这个函数，因为main函数里面调用了构造函数进行设置）

### 2.单个线程演奏

```cpp
void play_thread()
```

* 演奏单个线程，也未被调用，可能用于调试

### 3.演奏函数

```cpp
void play()
```

* 创建一个BGM线程演奏音乐，使之不影响其它函数的运行

### 4.停止播放

```cpp
void stop()
```