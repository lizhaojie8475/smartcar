#ifndef MINIMAX_H
#define MINIMAX_H

#include <QPushButton>

typedef struct
{
    int x;//注意，当kind为0时，x对应的是行，当kind为1时，x对应的是列
    int y;
    int kind;//该障碍是横着摆还是竖着摆的，0表示竖着摆，1表示横着摆
    int status;//0表示现在没有障碍，3表示现在有障碍
    QPushButton *btn;
}singalOBS;


typedef struct
{
    int obs1;
    int obs2;
}mOBSTACLE;



int findAiMove(int depth, int alpha, int beta);//找出ai当前的最优操作
int findHumanMove(int depth, int alpha, int beta);//找出玩家的最优操作
bool matchFinish();//判断比赛是否完成
int boardValue();//AI会想尽办法使玩家分数降低，玩家想办法提高分数
int MTDF(int test, int depth);

#endif // MINIMAX_H
