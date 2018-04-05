#include "minimax.h"
#include "mainwindow.h"
#include "astar.h"
#include <QStack>
#include <QMessageBox>
#include <QDebug>
#include <QVector>
#include <QTime>

extern int finalBoard[7][7];
extern CHESS mychess[2];
extern mOBSTACLE myOBS[72];
extern singalOBS littleOBS[84];
extern STEP AIstep[2];//记录ai的操作
extern int step_num;//记录是一步操作还是两步操作
extern int AIdepth;
extern SMALL mySmall[6][6];

static QStack<CHESS> myStack;
static int dx_ai[4] = {1,0,0,-1};
static int dy_ai[4] = {0,1,-1,0};
static int dx_human[4] = {-1,0,0,1};
static int dy_human[4] = {0,-1,1,0};
static int obsSEQ[72] = {0,1,2,3,4,5,6,7,8,9,10,11,12,
                         13,14,15,16,17,18,19,20,21,22,23,24,
                         25,26,27,28,29,30,31,32,33,34,35,36,
                         37,38,39,40,41,42,43,44,45,46,47,48,
                         49,50,51,52,53,54,55,56,57,58,59,60,
                         61,62,63,64,65,66,67,68,69,70,71};

bool matchFinish()
{
    for(int i=0; i<7; i++)
    {
        if(finalBoard[0][i] == Playerchess || finalBoard[6][i] == AIchess)
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

bool auto_can_set(int index1, int index2)
{
    if(littleOBS[index1].kind != littleOBS[index2].kind)
        return false;
    if(littleOBS[index1].status == Obstacle || littleOBS[index2].status == Obstacle)
        return false;

    else
    {
        if((abs(littleOBS[index1].x - littleOBS[index2].x) ==1 && littleOBS[index1].y == littleOBS[index2].y) || (abs(littleOBS[index1].y - littleOBS[index2].y) ==1 && littleOBS[index1].x == littleOBS[index2].x))
        {
            int minindex = index1 < index2 ? index1 : index2;
            int smallX = littleOBS[minindex].x;
            int smallY = littleOBS[minindex].y;

            if(minindex < 42 && mySmall[smallX][smallY].status == Blank)
            {
                mySmall[smallX][smallY].status = Obstacle;
                return true;
            }
            else if(minindex > 41 && mySmall[smallY][smallX].status == Blank)
            {
                mySmall[smallY][smallX].status = Obstacle;
                return true;
            }
            else
                return false;
        }
        else
        {
            return false;
        }
    }
}
void auto_deset(int index1, int index2)
{
    littleOBS[index1].status = Blank;
    littleOBS[index2].status = Blank;

    int minindex = index1 < index2 ? index1 : index2;
    int smallX = littleOBS[minindex].x;
    int smallY = littleOBS[minindex].y;
    if(minindex < 42)
    {
        mySmall[smallX][smallY].status = Blank;
    }
    else if(minindex > 41)
    {
        mySmall[smallY][smallX].status = Blank;
    }
}
void LuanXu(int length)
{
    for(int i=0; i<length; i++)
    {
        QTime t;
        t = QTime::currentTime();
        qsrand(t.msec() + t.second()*1000);
        int r = i + qrand() % (length - i);
        int temp = obsSEQ[i];
        obsSEQ[i] = obsSEQ[r];
        obsSEQ[r] = temp;
    }
}
int boardValue()
{
    for(int i=0; i<7; i++)
    {
        if(finalBoard[0][i] == Playerchess)
        {
            return 100;
        }
        else if(finalBoard[6][i] == AIchess)
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
                if(mychess[0].x+dx_ai[j]<0 || mychess[0].x+dx_ai[j]>6 || mychess[0].y+dy_ai[j]<0 || mychess[0].y+dy_ai[j]>6)//这里不能用mychess
                    continue;

                if(!MainWindow::canMove(mychess[0].x,mychess[0].y,mychess[0].x+dx_ai[j],mychess[0].y+dy_ai[j]))
                    continue;

                if(finalBoard[mychess[0].x+dx_ai[j]][mychess[0].y+dy_ai[j]] == Playerchess) // 需要判断这个点是否是对方的棋子，如果是的话需要再走一步
                {
                    int newX = mychess[0].x + dx_ai[j];
                    int newY = mychess[0].y + dy_ai[j];
                    finalBoard[mychess[0].x][mychess[0].y] = Blank;
                    myStack.push(mychess[0]);
                    for(int k=0; k<4; k++)
                    {
                        if(newX+dx_ai[j]<0 || newX+dx_ai[j]>6 || newY+dy_ai[j]<0 || newY+dy_ai[j]>6 || newX+dx_ai[j]==mychess[0].x)
                            continue;

                        if(!MainWindow::canMove(newX,newY,newX+dx_ai[j],newY+dy_ai[j]))
                            continue;

                        finalBoard[newX+dx_ai[j]][newY+dy_ai[j]] = AIchess;
                        mychess[0].x = newX + dx_ai[j];
                        mychess[0].y = newY + dy_ai[j];
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
                    finalBoard[mychess[0].x+dx_ai[j]][mychess[0].y+dy_ai[j]] = AIchess;
                    myStack.push(mychess[0]);
                    mychess[0].x += dx_ai[j];
                    mychess[0].y += dy_ai[j];

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
            //LuanXu(72);
            for(int j=0; j<72 && result > beta; j++)
            {
                //int j = obsSEQ[j];
                if(!auto_can_set(myOBS[j].obs1,myOBS[j].obs2))
                    continue;

                littleOBS[myOBS[j].obs1].status = Obstacle;
                littleOBS[myOBS[j].obs2].status = Obstacle;
                if(!MainWindow::canfinish(mychess[0].x,mychess[0].y,1) || !MainWindow::canfinish(mychess[1].x,mychess[1].y,2))//判断这两个障碍会不会将路堵住，如果会就撤回
                {
                    auto_deset(myOBS[j].obs1,myOBS[j].obs2);
                    continue;
                }
                //能运行到这里，表示ai已经成功使用了一块障碍
                mychess[0].obsNum --;

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
               auto_deset(myOBS[j].obs1,myOBS[j].obs2);
               //deset之后ai恢复一块障碍
               mychess[0].obsNum ++;
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
                if(mychess[1].x+dx_human[j]<0 || mychess[1].x+dx_human[j]>6 || mychess[1].y+dy_human[j]<0 || mychess[1].y+dy_human[j]>6)
                    continue;

                if(!MainWindow::canMove(mychess[1].x,mychess[1].y,mychess[1].x+dx_human[j],mychess[1].y+dy_human[j]))//这里是与canfinish相对应的，现在的设置是不允许与对手重叠
                    continue;

                if(finalBoard[mychess[1].x+dx_human[j]][mychess[1].y+dy_human[j]] == AIchess) // 需要判断这个点是否是对方的棋子，如果是的话需要再走一步
                {
                    int newX = mychess[1].x + dx_human[j];
                    int newY = mychess[1].y + dy_human[j];
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    myStack.push(mychess[1]);
                    for(int k=0; k<4; k++)
                    {
                        if(newX+dx_human[j]<0 || newX+dx_human[j]>6 || newY+dy_human[j]<0 || newY+dy_human[j]>6 || newX+dx_human[j]==mychess[1].x)
                            continue;

                        if(!MainWindow::canMove(newX,newY,newX+dx_human[j],newY+dy_human[j]))
                            continue;

                        finalBoard[newX+dx_human[j]][newY+dy_human[j]] = Playerchess;
                        mychess[1].x = newX + dx_human[j];
                        mychess[1].y = newY + dy_human[j];
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
                    finalBoard[mychess[1].x+dx_human[j]][mychess[1].y+dy_human[j]] = Playerchess;
                    myStack.push(mychess[1]);
                    mychess[1].x += dx_human[j];
                    mychess[1].y += dy_human[j];

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
            //LuanXu(72);
            for(int j=0; j<72 && result < alpha; j++)
            {
                //int j = obsSEQ[j];
                if(!auto_can_set(myOBS[j].obs1,myOBS[j].obs2))
                    continue;

                littleOBS[myOBS[j].obs1].status = Obstacle;
                littleOBS[myOBS[j].obs2].status = Obstacle;

                if(!MainWindow::canfinish(mychess[0].x,mychess[0].y,1) || !MainWindow::canfinish(mychess[1].x,mychess[1].y,2))//判断这两个障碍会不会将路堵住，如果会就撤回
                {
                    auto_deset(myOBS[j].obs1,myOBS[j].obs2);
                    continue;
                }
                mychess[1].obsNum --;
                int humanScore = findAiMove(depth-1, alpha, result);
                if(humanScore > result)
                {
                    result = humanScore;
                }
                auto_deset(myOBS[j].obs1,myOBS[j].obs2);
                mychess[1].obsNum ++;
            }
        }
    }
    return result;
}
