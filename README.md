# VLSI設計自動化導論作業
如果你喜歡這個專案[![star it][star-img]][star-url] on GitHub.

## 輸入輸出格式

### 輸入(input.txt)
> CIRCUIT [Circuit Name] // 電路開始
> 
> INSTANCE [Instance Name] [Cell Type] // Cell Type為.lib裡定義之*Standard Cell Type*
> [Net Name(Weight, Direction1)] [Net Name2(weight2, Direction2)] … // Cell所接的Net
> …
> …
> ENDCIRCUIT // 電路結束

### 輸出(terminal)
![image][output-img]


## 輸入圖形
![image][input-img]


## 資料結構
class : graph, DFS, Dijkstra

以vector取代link-list實作Adjacency list，再將所有頂點以vector形式儲存為graph
![image][vertex_structure-img]

DFS, Dijkstra等graph algorithm所需的頂點資料以動態配置記憶體方式"平行"於graph的vertex vector
![image][graph_algorithm_structure-img]


## 功能
1. 以DFS遍歷所有Cell
2. 使用Dijkstra演算法求得所有Cell之最短路徑(起點為S)


## 環境
於Window開發，運行於Linux( *g++ 4.8.5 20150623* )
檔案編譯前記得以 dos2unix xxx.cpp 指令轉檔



<!-- MARKDOWN LINKS & IMAGES -->
[input-img]: https://github.com/catpupu/VLSI_finalproject/blob/master/picture/input_img.png
[output-img]: https://github.com/catpupu/VLSI_finalproject/blob/master/picture/output_img.png
[vertex_structure-img]: https://github.com/catpupu/VLSI_finalproject/blob/master/picture/Instance(vertex)_structure.jpg
[graph_algorithm_structure-img]: https://github.com/catpupu/VLSI_finalproject/blob/master/picture/graph_algorithm_structure.jpg

[star-img]: https://img.shields.io/github/stars/catpupu/VLSI_finalproject.svg?style=social
[star-url]: https://github.com/catpupu/VLSI_finalproject/stargazers
