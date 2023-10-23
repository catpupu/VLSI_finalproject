# VLSI設計自動化導論作業

### 輸入格式

> CIRCUIT [Circuit Name] // 電路開始
> 
> INSTANCE [Instance Name] [Cell Type] // Cell Type為.lib裡定義之*Standard Cell Type*
> [Net Name(Weight, Direction1)] [Net Name2(weight2, Direction2)] … // Cell所接的Net
> …
> …
> ENDCIRCUIT // 電路結束


### 資料結構

以vector取代link-list實作Adjacency list
```c++=
# include <vector>

struct adjacent {
    string net_name ;
    string direction ;
    int weight ;
} ;

struct Instance {
    string instance_name ;
    vector <adjacent> out ;
} ;

class Graph {
    vector <Instance> vertices ;
    ...
}
```

### 功能
1. 以DFS遍歷所有Cell
2. 使用Dijkstra演算法求得所有Cell之最短路徑(起點為S)


### 環境
於Window開發，運行於Linux( *g++ 4.8.5 20150623* )
編譯前記得以 dos2unix 指令轉檔

