#include "minimax.h"
#include "mainwindow.h"
#include "astar.h"
#include <QStack>
#include <QMessageBox>
#include <QDebug>
#include <QVector>

extern int finalBoard[8][8];
extern CHESS mychess[2];
extern mOBSTACLE myOBS[112];
extern singalOBS littleOBS[98];
extern STEP AIstep[2];//记录ai的操作
extern int step_num;//记录是一步操作还是两步操作
extern int AIdepth;

static QStack<CHESS> myStack;
static int dx[4] = {0,0,-1,1};
static int dy[4] = {-1,1,0,0};

bool matchFinish()
{
    for(int i=0; i<8; i++)
    {
        if(finalBoard[0][i] == Playerchess || finalBoard[7][i] == AIchess)
        {
            return true;
        }
    }
    return false;
}

int MTDF(int test, int depth)
{
    int score, beta, upperbound, lowerbound;
    score = test;
    upperbound = +9999;
    lowerbound = -9999;
    do
    {
        beta = (score == lowerbound ? score + 1 : score);
        score = findAiMove(depth, beta , beta - 1);
        (score < beta ? upperbound : lowerbound) = score;
    } while (lowerbound < upperbound);

    return score;
}

int boardValue()
{
    for(int i=0; i<8; i++)
    {
        if(finalBoard[0][i] == Playerchess)
        {
            return 100;
        }
        else if(finalBoard[7][i] == AIchess)
        {
            return -100;
        }
    }//判断是否有人已经获胜

    //以下是用A*算法计算的棋子当前位置到终点的最短距离//这里可以根据场面上已有的障碍数来选择使用第k短的距离或能够完成比赛的路径数//由于我们的比赛挡板数足够所以不考虑剩余挡板的影响
    int myValue = score_for_AStar();

    return myValue;
}
//alpha > beta
int findAiMove(int depth, int alpha, int beta)
{
    int result = alpha;

    if(matchFinish() || depth == 0)
    {
        return boardValue();
    }

    for(int i=1; i<=2 && result > beta; i++)
    {
        if(i==1)//表示选择移动棋子
        {
            for(int j=0; j < 4 && result > beta; j++)
            {
                if(mychess[0].x+dx[j]<0 || mychess[0].x+dx[j]>7 || mychess[0].y+dy[j]<0 || mychess[0].y+dy[j]>7)//这里不能用mychess
                    continue;

                if(!MainWindow::canMove(mychess[0].x,mychess[0].y,mychess[0].x+dx[j],mychess[0].y+dy[j]))
                    continue;

                if(finalBoard[mychess[0].x+dx[j]][mychess[0].y+dy[j]] == Playerchess) // 需要判断这个点是否是对方的棋子，如果是的话需要再走一步
                {
                    int newX = mychess[0].x + dx[j];
                    int newY = mychess[0].y + dy[j];
                    finalBoard[mychess[0].x][mychess[0].y] = Blank;
                    myStack.push(mychess[0]);
                    for(int k=0; k<4; k++)
                    {
                        if(newX+dx[j]<0 || newX+dx[j]>7 || newY+dy[j]<0 || newY+dy[j]>7 || newX+dx[j]==mychess[0].x)
                            continue;

                        if(!MainWindow::canMove(newX,newY,newX+dx[j],newY+dy[j]))
                            continue;

                        finalBoard[newX+dx[j]][newY+dy[j]] = AIchess;
                        mychess[0].x = newX + dx[j];
                        mychess[0].y = newY + dy[j];
                        int humanScore = findHumanMove(depth-1, result, beta);

                        if(humanScore < result)
                        {
                            result = humanScore;
                            if(depth == AIdepth)
                            {
                                step_num = 0;
                                AIstep[step_num].status = 2;
                                AIstep[step_num].x = mychess[0].x;
                                AIstep[step_num].y = mychess[0].y;
                                AIstep[step_num].kind = -1;
                                step_num++;
                            }
                        }
                        finalBoard[mychess[0].x][mychess[0].y] = Blank;
                    }
                    mychess[0] = myStack.pop();
                    finalBoard[mychess[0].x][mychess[0].y] = AIchess;
                }
                else
                {
                    finalBoard[mychess[0].x][mychess[0].y] = Blank;
                    finalBoard[mychess[0].x+dx[j]][mychess[0].y+dy[j]] = AIchess;
                    myStack.push(mychess[0]);
                    mychess[0].x += dx[j];
                    mychess[0].y += dy[j];

                    int humanScore = findHumanMove(depth-1, result, beta);

                    if(humanScore < result)
                    {
                        result = humanScore;
                        if(depth == AIdepth)
                        {
                            step_num = 0;

                            AIstep[step_num].status = 2;
                            AIstep[step_num].x = mychess[0].x;
                            AIstep[step_num].y = mychess[0].y;//这里和上边注释出协调处理
                            AIstep[step_num].kind = -1;
                            step_num ++;
                        }

                    }

                    finalBoard[mychess[0].x][mychess[0].y] = Blank;

                    mychess[0] = myStack.pop();
                    finalBoard[mychess[0].x][mychess[0].y] = AIchess;
                }

             }
        }
        else//选择设置障碍
        {
            for(int j=0; j<98 && result > beta; j++)
            {
                if(littleOBS[myOBS[j].obs1].status != Blank || littleOBS[myOBS[j].obs2].status != Blank)
                    continue;
                littleOBS[myOBS[j].obs1].status = Obstacle;
                littleOBS[myOBS[j].obs2].status = Obstacle;
                if(!MainWindow::canfinish(mychess[0].x,mychess[0].y,1) || !MainWindow::canfinish(mychess[1].x,mychess[1].y,2))//判断这两个障碍会不会将路堵住，如果会就撤回
                {
                    littleOBS[myOBS[j].obs1].status = Blank;
                    littleOBS[myOBS[j].obs2].status = Blank;
                    continue;
                }

                int humanScore = findHumanMove(depth-1, result, beta);
                if(humanScore < result)
                {
                    result = humanScore;
                    if(depth == AIdepth)
                    {
                        step_num = 0;

                        AIstep[step_num].status = 1;
                        AIstep[step_num].x = -1;
                        AIstep[step_num].y = -1;
                        AIstep[step_num].kind = myOBS[j].obs1;
                        step_num ++;

                        AIstep[step_num].status = 1;
                        AIstep[step_num].x = -1;
                        AIstep[step_num].y = -1;
                        AIstep[step_num].kind = myOBS[j].obs2;
                        step_num++;
                    }

                }

                littleOBS[myOBS[j].obs1].status = Blank;
                littleOBS[myOBS[j].obs2].status = Blank;
            }
        }
    }
    return result;
}

int findHumanMove(int depth, int alpha, int beta)
{
    int result = beta;

    if(matchFinish() || depth == 0)
    {
        return boardValue();
    }

    for(int i=1; i<=2 && result < alpha; i++)
    {
        if(i==1)//表示选择移动棋子
        {
            for(int j=0; j < 4 && result < alpha; j++)
            {
                if(mychess[1].x+dx[j]<0 || mychess[1].x+dx[j]>7 || mychess[1].y+dy[j]<0 || mychess[1].y+dy[j]>7)
                    continue;

                if(!MainWindow::canMove(mychess[1].x,mychess[1].y,mychess[1].x+dx[j],mychess[1].y+dy[j]))//这里是与canfinish相对应的，现在的设置是不允许与对手重叠
                    continue;

                if(finalBoard[mychess[1].x+dx[j]][mychess[1].y+dy[j]] == AIchess) // 需要判断这个点是否是对方的棋子，如果是的话需要再走一步
                {
                    int newX = mychess[1].x + dx[j];
                    int newY = mychess[1].y + dy[j];
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    myStack.push(mychess[1]);
                    for(int k=0; k<4; k++)
                    {
                        if(newX+dx[j]<0 || newX+dx[j]>7 || newY+dy[j]<0 || newY+dy[j]>7 || newX+dx[j]==mychess[1].x)
                            continue;

                        if(!MainWindow::canMove(newX,newY,newX+dx[j],newY+dy[j]))
                            continue;

                        finalBoard[newX+dx[j]][newY+dy[j]] = Playerchess;
                        mychess[1].x = newX + dx[j];
                        mychess[1].y = newY + dy[j];
                        float humanScore = findHumanMove(depth-1, alpha, result);

                        if(humanScore > result)
                        {
                            result = humanScore;
                        }
                        finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    }
                    mychess[1] = myStack.pop();
                    finalBoard[mychess[1].x][mychess[1].y] = Playerchess;
                }
                else
                {
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalBoard[mychess[1].x+dx[j]][mychess[1].y+dy[j]] = Playerchess;
                    myStack.push(mychess[1]);
                    mychess[1].x += dx[j];
                    mychess[1].y += dy[j];

                    int humanScore = findAiMove(depth-1, alpha, result);

                    if(humanScore > result)
                    {
                        result = humanScore;
                    }

                    finalBoard[mychess[1].x][mychess[1].y] = Blank;

                    mychess[1] = myStack.pop();
                    finalBoard[mychess[1].x][mychess[1].y] = Playerchess;
                }
             }
        }
        else//选择设置障碍
        {
            for(int j=0; j<98 && result < alpha; j++)
            {
                if(littleOBS[myOBS[j].obs1].status != Blank || littleOBS[myOBS[j].obs2].status != Blank)
                    continue;

                littleOBS[myOBS[j].obs1].status = Obstacle;
                littleOBS[myOBS[j].obs2].status = Obstacle;

                if(!MainWindow::canfinish(mychess[0].x,mychess[0].y,1) || !MainWindow::canfinish(mychess[1].x,mychess[1].y,2))//判断这两个障碍会不会将路堵住，如果会就撤回
                {
                    littleOBS[myOBS[j].obs1].status = Blank;
                    littleOBS[myOBS[j].obs2].status = Blank;
                    continue;
                }

                int humanScore = findAiMove(depth-1, alpha, result);
                if(humanScore > result)
                {
                    result = humanScore;
                }

                littleOBS[myOBS[j].obs1].status = Blank;
                littleOBS[myOBS[j].obs2].status = Blank;
            }
        }
    }
    return result;
}
