/**
 * File: graph_adjacency_matrix.c
 * Created Time: 2023-07-06
 * Author: NI-SW (947743645@qq.com)
 */

#include "../utils/common.h"

/* 基于邻接矩阵实现的无向图类结构 */
struct graphAdjMat
{
    int* vertices;
    unsigned int** adjMat;
    unsigned int size;
    unsigned int capacity;
   
    void (*addVertex)(struct graphAdjMat* t, int val);
    void (*addEdge)(struct graphAdjMat* t, int i, int j);
    void (*removeVertex)(struct graphAdjMat* t, unsigned int index);
    void (*removeEdge)(struct graphAdjMat* t, int i, int j);
    void (*print_graph)(struct graphAdjMat* t);
};

typedef struct graphAdjMat graphAdjMat;

/* 添加边 */
void addEdge(graphAdjMat* t, int i, int j)
{
    // 越界检查
    if(i >= t->size || j >= t->size || i < 0 || j < 0 || i == j)
    {
        printf("Out of range in %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    
    t->adjMat[i][j] = 1;
    t->adjMat[j][i] = 1;

}

/* 删除边 */
void removeEdge(graphAdjMat* t, int i, int j) 
{
    // 越界检查
    if(i >= t->size || j >= t->size || i < 0 || j < 0 || i == j)
    {
        printf("Out of range in %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    t->adjMat[i][j] = 0;
    t->adjMat[j][i] = 0;

}

/* 添加顶点 */
void addVertex(graphAdjMat* t, int val) 
{
    // 如果实际使用不大于预设空间，则直接初始化新空间
    if(t->size < t->capacity)
    {
        t->vertices[t->size] = val;

        // 邻接矩新列阵置0
        for(int i = 0; i < t->size; i++)
        {
            t->adjMat[i][t->size] = 0;
        }
        memset(t->adjMat[t->size], 0, sizeof(unsigned int) * (t->size + 1));
        t->size++;
        return;
    }
    
    // 扩容，申请新的顶点数组
    int* temp = (int*)malloc(sizeof(int) * (t->size * 2));
    memcpy(temp, t->vertices, sizeof(int) * t->size);
    temp[t->size] = val;

    // 释放原数组
    free(t->vertices);
    t->vertices = temp;
    
    // 扩容，申请新的二维数组
    unsigned int** temp_Mat = (unsigned int**)malloc(sizeof(unsigned int*) * t->size * 2);
    unsigned int* temp_Mat_line = (unsigned int*)malloc(sizeof(unsigned int) * (t->size * 2) * (t->size * 2));
    memset(temp_Mat_line, 0,  sizeof(unsigned int) * (t->size * 2) * (t->size * 2));
    for(int k = 0; k < t->size * 2; k++)
    {
        temp_Mat[k] = temp_Mat_line + k * (t->size * 2);
    }
    
    // 原数据复制到新数组
    for(int i = 0;i < t->size;i++)
    {
        memcpy(temp_Mat[i], t->adjMat[i], sizeof(unsigned int) * t->size);
    }

    // 新列置0
    for(int i = 0;i < t->size;i++)
    {
        temp_Mat[i][t->size] = 0;
    }
    memset(temp_Mat[t->size], 0, sizeof(unsigned int) * (t->size + 1));

    // 释放原数组
    free(t->adjMat[0]);
    free(t->adjMat);

    // 扩容后，指向新地址
    t->adjMat = temp_Mat;
    t->capacity = t->size * 2;
    t->size++;

}

/* 删除顶点 */
void removeVertex(graphAdjMat* t, unsigned int index)
{
    // 越界检查
    if(index >= t->size || index < 0)
    {
        printf("Out of range in %s:%d\n", __FILE__, __LINE__);
        exit(1);
    }
    
    // 清除删除的顶点
    for(int i = index; i < t->size - 1; i++)
    {
        t->vertices[i] = t->vertices[i + 1];
    }
    
    // 将被前移的最后一个顶点置0
    t->vertices[t->size - 1] = 0;

    // 清除邻接矩阵中删除的列
    for(int i = 0; i < t->size - 1; i++)
    { 
        if(i < index)
        {
            // 被删除列后的所有列前移
            for(int j = index; j < t->size - 1; j++)
            {
                t->adjMat[i][j] = t->adjMat[i][j + 1];            
            }
        }
        else
        {
            // 被删除行的下方所有行上移
            memcpy(t->adjMat[i],t->adjMat[i + 1], sizeof(unsigned int) * t->size);
            // 被删除列后的所有列前移
            for(int j = index; j < t->size; j++)
            {
                t->adjMat[i][j] = t->adjMat[i][j + 1];            
            }
        }
    }
    t->size--;
}

/* 打印顶点与邻接矩阵 */
void print_graph(graphAdjMat* t)
{
    if(t->size == 0)
    {
        printf("graph is empty\n");
        return;
    }
    printf("顶点列表 = [");
    for(int i = 0; i < t->size; i++)
    {
        if(i != t->size - 1)
        {
            printf("%d, ", t->vertices[i]);
        }
        else
        {
            printf("%d", t->vertices[i]);
        }
    }
    printf("]\n");
    printf("邻接矩阵 =\n[\n");
    for(int i = 0;i < t->size; i++)
    {
        printf("  [");
        for(int j = 0; j < t->size; j++)
        {
            if(j != t->size - 1)
            {
                printf("%u, ", t->adjMat[i][j]);
            }
            else
            {
                printf("%u", t->adjMat[i][j]);
            }
        }
        printf("],\n");

    }
    printf("]\n");
}

/* 构造函数 */
graphAdjMat* new_graphic(unsigned int number_vertices, int* vertices, unsigned int** adjMat)
{    
    // 函数指针
    graphAdjMat* newGraph = (graphAdjMat*)malloc(sizeof(graphAdjMat)); 
    newGraph->addVertex = addVertex;
    newGraph->addEdge = addEdge;
    newGraph->removeVertex = removeVertex;
    newGraph->removeEdge = removeEdge;
    newGraph->print_graph = print_graph;    

    // 申请内存
    newGraph->vertices = (int*)malloc(sizeof(int) * number_vertices * 2);
    newGraph->adjMat = (unsigned int**)malloc(sizeof(unsigned int*) * number_vertices * 2);
    unsigned int* temp = (unsigned int*)malloc(sizeof(unsigned int) * number_vertices * 2 * number_vertices * 2);
    newGraph->size = number_vertices;
    newGraph->capacity = number_vertices * 2;

    // 配置二维数组
    for(int i = 0; i < number_vertices * 2; i++)
    {
        newGraph->adjMat[i] =  temp + i * number_vertices * 2;
    }

    // 赋值
    memcpy(newGraph->vertices, vertices, sizeof(int) * number_vertices);
    for(int i = 0; i < number_vertices; i++)
    { 
        memcpy(newGraph->adjMat[i], adjMat[i], sizeof(unsigned int) * number_vertices);
    }

    return newGraph;

}

/* Driver Code */
int main() 
{

    /* 初始化无向图 */
    int vertices[5] = {1,3,2,5,4};
    unsigned int** edge = (unsigned int**)malloc(sizeof(unsigned int*) * 5);

    // 用于构建二维数组的一维指针
    unsigned int* temp = (unsigned int*)malloc(sizeof(unsigned int) * 25);
    memset(temp, 0,sizeof(unsigned int) * 25);
    for(int k = 0; k < 5; k++)
    {
        edge[k] = temp + k * 5;
    }
    edge[0][1] = edge[1][0] = 1;
    edge[0][3] = edge[3][0] = 1;
    edge[1][2] = edge[2][1] = 1;
    edge[2][3] = edge[3][2] = 1;
    edge[2][4] = edge[4][2] = 1;
    edge[3][4] = edge[4][3] = 1;

    graphAdjMat* graph = new_graphic(5,vertices,edge);
    free(edge);
    free(temp);
    printf("初始化后，图为:\n");
    graph->print_graph(graph);
    
    /* 添加边 */
    // 顶点 1, 2 的索引分别为 0, 2
    graph->addEdge(graph,0,2);
    printf("添加边 1-2 后图为\n");
    graph->print_graph(graph);

    /* 删除边 */
    // 顶点 1, 3 的索引分别为 0, 1
    graph->removeEdge(graph,0,1);
    printf("删除边 1-3 后，图为\n");
    graph->print_graph(graph);

    /* 添加顶点 */
    graph->addVertex(graph,6);
    printf("添加顶点 6 后，图为\n");
    graph->print_graph(graph);

    /* 删除顶点 */
    // 顶点 3 的索引为 1
    graph->removeVertex(graph,1);
    printf("删除顶点 3 后，图为\n");
    graph->print_graph(graph);

    return 0;
}
