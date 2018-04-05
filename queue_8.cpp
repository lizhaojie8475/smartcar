#include "queue_8.h"
#include "mainwindow.h"
#include <math.h>
#include <algorithm>


extern int myBoard[8][8];
extern int aimBoard[8][8];
extern int finally[8];
extern QVector<QPoint> myPoint;

using namespace std;

int a[10];//表示一个可行解
bool b[10];//表示这一列是否有棋子
bool c[20];//表示这条左对角线是否有棋子
bool d[20];//表示这条有对角线是否有棋子

float mindis = 99999;//记录八个棋子需要移动的最小距离和
int maxNum = -1;//记录不需要移动的棋子的个数
int origin[8];//生成八皇后与裁判摆放棋子建对应关系的全排列

float myDistance(QPoint point, int x, int y)
{
    int x1,y1,x2,y2;
    float dis;
    x1 = point.x();
    y1 = point.y();
    x2 = x;
    y2 = y;
    dis = sqrtf(pow(x1-x2,2)+pow(y1-y2,2));
    return dis;
}

void Queen8(int x)
{
    if(x > 8)
    {
        int staticNum = 0;//不需要移动的棋子个数
        QVector<int> myStatic;//记录不需要移动的棋子是第几个
        for(int i = 1; i <= 8; i++)
        {
            if(myBoard[i-1][a[i]-1] == 1)
            {
                staticNum ++;
                for(int j = 0; j < 8; j++)
                {
                    if(myPoint[j].x() == i && myPoint[j].y() == a[i])
                    {
                        myStatic.append(j);
                        break;
                    }

                }
                break;
            }
        }
        if(staticNum > maxNum)
        {
            maxNum = staticNum;
        }
        else
        {
            return;//不需要棋子的数量最多是最优先考虑的
        }


        for(int i = 0; i < 8 ; i++)
        {
            origin[i] = i+1;
        }

        do
        {
            for(int i = 0; i < staticNum; i++)
            {
                if(origin[myStatic[i]] != myPoint[myStatic[i]].x())
                {
                    next_permutation(origin, origin+8);
                    break;
                }
            }

            float nowdis = 0;
            for(int i = 0; i < 8; i++)
            {
                nowdis += myDistance(myPoint[i],origin[i],a[origin[i]]);
            }
            if(nowdis < mindis)
            {
                mindis = nowdis;
                memcpy(aimBoard,a,sizeof(aimBoard));
                memcpy(finally,origin,sizeof(finally));
            }

         } while (next_permutation(origin, origin+8));

        return;
    }
    for(int i = 1;i <= 8;i++)if(!(b[i] || c[x+i] || d[x-i+8]))
    {
        b[i] = c[x+i] = d[x-i+8] = true;
        a[x] = i;
        Queen8(x+1);
        b[i] = c[x+i] = d[x-i+8] = false;
    }
}
