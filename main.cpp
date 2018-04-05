#include "mainwindow.h"
#include <QApplication>
#include <QVector>
#include "minimax.h"
#include <QDebug>


//全局变量
int myBoard[8][8];//裁判任意摆放处的棋盘状态
int aimBoard[10];//经过计算出的最佳变换后的八皇后棋盘状态
int finally[8];//最后确定其中一个origin排列
int finalBoard[7][7];//决赛用棋盘，用来表示每个格当前的状态，0表示这个格里没有东西，1表示当前格子里是ai的棋子，2表示当前格子里是玩家的棋子
QVector<QPoint> myPoint;//按顺序记录myBoard中的棋子坐标
CHESS mychess[2];//0记录AI棋子的信息，1记录玩家棋子的信息
singalOBS littleOBS[84];
mOBSTACLE myOBS[72];
STEP AIstep[2];//记录ai的操作
int step_num = 0;//记录是一步操作还是两步操作
int AIdepth;

int main(int argc, char *argv[])
{
    int count = 0;
    QApplication a(argc, argv);
    //初始化障碍数组

    for(int i=0; i<6; i++)
    {
        for(int j=0; j<6; j++)
        {
            myOBS[count].obs1 = j * 6 + i;
            myOBS[count].obs2 = (j+1) * 6 + i;
            count++;
        }
    }
    for(int i=0; i<6; i++)
    {
        for(int j=0; j<6; j++)
        {
            myOBS[count].obs1 = 42 + j*6 + i;
            myOBS[count].obs2 = 42 + (j+1) * 6 + i;
            count++;
        }
    }

    MainWindow w;
    w.board_init();
    w.show();

    return a.exec();
}
