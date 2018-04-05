#include "astar.h"
#include "mainwindow.h"
#include "minimax.h"
#include <QDebug>
#include <QMessageBox>

extern int finalBoard[8][8];
extern CHESS mychess[2];//0记录AI棋子的信息，1记录玩家棋子的信息
extern singalOBS littleOBS[98];


AStarNode  map_maze[8][8];//棋盘中所有的点的信息
PAStarNode open_table[64];//open列表
PAStarNode close_table[64];//close列表
int open_count;//open列表中的节点数量
int close_count;//close表中的节点数量

void heap_insert(PAStarNode index)
{
    int now = open_count++;//插入节点的位置
    while(now > 0)
    {
        int f = (now - 1)/2;//父亲节点的编号
        if(open_table[f]->g + open_table[f]->h <= index->g + index->h)
            break;//如果父亲节点数据大于当前元素则退出

        open_table[now] = open_table[f];
        now = f;
    }
    open_table[now] = index;
}
void heap_insert_float(float *arry, float index, int num)
{
    int now = num++;
    while(now > 0)
    {
        int f = (now - 1)/2;
        if(arry[f] <= index)
            break;

        arry[now] = arry[f];
        now = f;
    }
    arry[now] = index;
}

void heap_adjust(int index)
{
    int curr = index;
    int myparent = (curr-1)/2;

    while (curr > 0)
    {
        if (open_table[curr]->g + open_table[curr]->h >= open_table[myparent]->g + open_table[myparent]->h)
        {
            break;
        }
        else
        {
            PAStarNode tmp = open_table[curr];
            open_table[curr] = open_table[myparent];
            open_table[curr] = tmp;

            curr = myparent;
            myparent = (curr-1)/2;
        }
    }
}

PAStarNode heap_pop()
{
    PAStarNode res = open_table[0];
    PAStarNode x = open_table[--open_count];//最末尾的节点
    int i = 0;//从根节点开始操作
    while(i * 2 + 1 < open_count)
    {
        int lchild = 2 * i + 1;
        int rchild = 2 * i + 2;
        if(rchild < open_count && open_table[rchild]->g + open_table[rchild]->h < open_table[lchild]->g + open_table[lchild]->h)
              lchild = rchild;

        if(open_table[lchild]->g + open_table[lchild]->h >= x->g + x->h)
            break;//如果x已经在lchild上层了，就可以停止了

        open_table[i] = open_table[lchild];
        i = lchild;
    }
    open_table[i] = x;
    return res;  //放回最小值
}
float heap_pop_float(float *arry, int num)
{
    int res = arry[0];
    int x = arry[--num];
    int i = 0;
    while(i * 2 + 1 < num)
    {
        int lchild = 2 * i + 1;
        int rchild = 2 * i + 2;
        if(rchild < num && arry[rchild] < arry[lchild])
            lchild = rchild;

        if(arry[lchild] >= x)
            break;

        arry[i] = arry[lchild];
        i = lchild;
    }
    arry[i] = x;
    return res;
}

void insert_to_open(int x, int y, PAStarNode curr_node, PAStarNode end_node)
{
    int i;
    if(!map_maze[x][y].is_in_closetable)//不在close表中
    {
        if(map_maze[x][y].is_in_opentable)//在open表中
        {
            //需要判断curr_node是否是比当前父节点更优的父节点
            if(map_maze[x][y].g > curr_node->g + 1)
            {
                map_maze[x][y].g = curr_node->g + 1;
                map_maze[x][y].parent = curr_node;

                for(i = 0; i < open_count; i++)
                {
                    if(open_table[i]->x == x && open_table[i]->y == y)
                        break;
                }
                heap_adjust(i);
            }
        }
        else                                //不在open表中
        {
            map_maze[x][y].g = curr_node->g + 1;
            map_maze[x][y].h = abs(end_node->x - x) + abs(end_node->y - y);
            map_maze[x][y].parent = curr_node;
            map_maze[x][y].is_in_opentable = 1;
            heap_insert(&(map_maze[x][y]));
        }
    }

}

void get_neighbors(PAStarNode curr_node, PAStarNode end_node)
{
    int x = curr_node->x;
    int y = curr_node->y;

    if ( ( x + 1 ) >= 0 && ( x + 1 ) < 7 && y >= 0 && y < 7 )
    {
        if(littleOBS[42 + y*6 + x].status == Blank)
            insert_to_open( x+1, y, curr_node, end_node);
    }

    if ( ( x - 1 ) >= 0 && ( x - 1 ) < 8 && y >= 0 && y < 7 )
    {
        if(littleOBS[42 + y*6 + x-1].status == Blank)
            insert_to_open( x-1, y, curr_node, end_node);
    }

    if ( x >= 0 && x < 7 && ( y + 1 ) >= 0 && ( y + 1 ) < 7 )
    {
        if(littleOBS[x*6 + y].status == Blank)
            insert_to_open( x, y+1, curr_node, end_node);
    }

    if ( x >= 0 && x < 7 && ( y - 1 ) >= 0 && ( y - 1 ) < 7 )
    {
        if(littleOBS[x*6 + y-1].status == Blank)
            insert_to_open( x, y-1, curr_node, end_node);
    }
}
void astar_init()
{
    for(int i=0; i<7; i++)
    {
        for(int j=0; j<7; j++)
        {
            map_maze[i][j].g = 0;
            map_maze[i][j].h = 0;
            map_maze[i][j].is_in_closetable = 0;
            map_maze[i][j].is_in_opentable = 0;
            map_maze[i][j].style = finalBoard[i][j];
            map_maze[i][j].x = i;
            map_maze[i][j].y = j;
            map_maze[i][j].parent = NULL;
        }
    }
    open_count = 0;
    close_count = 0;
}
int minDis(int startX,int startY,int endX,int endY)
{
    AStarNode *start_node;
    AStarNode *end_node;
    AStarNode *curr_node;
    int isfound;
//    float MaxDis;//归一化时用到的最大曼哈顿距离

    astar_init();
    end_node = &map_maze[endX][endY];
    start_node = &map_maze[startX][startY];
    open_table[open_count++] = start_node;
    start_node->is_in_opentable = 1;
    start_node->g = 0;
    start_node->h = abs(end_node->x - start_node->x) + abs(end_node->y - start_node->y);
    start_node->parent = NULL;
    isfound = 0;

    while(1)
    {
        curr_node = heap_pop();

        close_table[close_count++] = curr_node;
        curr_node->is_in_closetable = 1;

        if(curr_node->x == end_node->x && curr_node->y == end_node->y)
        {
            isfound = 1;
            break;
        }
        get_neighbors(curr_node, end_node);

        if(open_count == 0)
        {
            isfound = 0;
            break;
        }
    }
    if(isfound)
    {
        return end_node->g;
    }
    else
        return -1;
}

int score_for_AStar()
{
    int dis_score;
    int AI_dis = 999,Human_dis = 999,AI_SEC = 999,Human_SEC = 999;//所有可行路径长度的最小值
    int AI_num = 0,Human_num = 0;//所有可行路径的个数
    //求ai的分数
    for(int i=0; i<7; i++)
    {
        int nowScore;
        nowScore = minDis(mychess[0].x,mychess[0].y,6,i);
        if(nowScore >= 0)
        {

            if(nowScore < AI_dis)
            {
                AI_SEC = AI_dis;
                AI_dis = nowScore;
            }
            else if(nowScore < AI_SEC)
            {
                AI_SEC = nowScore;
            }

            AI_num ++;
        }
    }

    //求human的分数
    for(int i=0; i<7; i++)
    {
        if(finalBoard[0][i] == Obstacle)
        {
            continue;
        }

        int nowScore;
        nowScore = minDis(mychess[1].x,mychess[1].y,0,i);
        if(nowScore >= 0)
        {
            if(nowScore < Human_dis)
            {
                Human_SEC = Human_dis;
                Human_dis = nowScore;
            }
            else if(nowScore < Human_SEC)
            {
                Human_SEC = nowScore;
            }
            Human_num ++;
        }
    }

    dis_score = AI_dis - Human_dis + mychess[1].obsNum - mychess[0].obsNum;
    return (dis_score);
}
