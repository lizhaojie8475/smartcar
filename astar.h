#ifndef ASTAR_H
#define ASTAR_H

typedef struct AStarNode
{
    int x;//坐标
    int y;
    int g;//起点到该点的实际距离
    int h;//该点到终点的预估距离
    int style;//该点的节点类型：起点，终点，障碍
    struct AStarNode * parent;//父节点
    int is_in_closetable;//是否在close表中
    int is_in_opentable;//是否在open表中

}AStarNode, *PAStarNode;

void heap_insert(PAStarNode index);
void heap_insert_float(int *arry, int index, int num);
void heap_adjust(int index);
PAStarNode heap_pop();
int heap_pop_float(int *arry, int num);

void insert_to_open(int x, int y, PAStarNode curr_node, PAStarNode end_node);//将节点插入开启队列
void get_neighbors(PAStarNode curr_node, PAStarNode end_node);//对邻居节点的处理
void astar_init();
int minDis(int startX,int startY,int endX,int endY);//返回给定起点和终点间的最短距离，若无法到达终点，则返回-1
int score_for_AStar();






#endif // ASTAR_H
