#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <string>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QQueue>
#include "astar.h"
#include "minimax.h"
#include "queue_8.h"

using namespace std;

extern int myBoard[8][8];//裁判任意摆放处的棋盘状态
extern int aimBoard[10];//经过计算出的最佳变换后的八皇后棋盘状态
extern int finally[8];//最后确定其中一个origin排列
extern int finalBoard[7][7];//决赛用棋盘，用来表示每个格当前的状态，0表示这个格里没有东西，1表示当前格子里是ai的棋子，2表示当前格子里是玩家的棋子，3表示当前格子里是障碍
extern QVector<QPoint> myPoint;//按顺序记录myBoard中的棋子坐标
extern float mindis;//记录八个棋子需要移动的最小距离和
extern int maxNum;//记录不需要移动的棋子的个数
extern CHESS mychess[2];
extern STEP AIstep[2];//记录ai的操作
extern int step_num;//记录是一步操作还是两步操作
extern singalOBS littleOBS[84];
extern mOBSTACLE myOBS[72];
extern int AIdepth;
extern SMALL mySmall[6][6];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->PortBox->addItem(tr("com3"));
    ui->PortBox->addItem(tr("com4"));
    ui->PortBox->addItem(tr("com5"));
    ui->PortBox->addItem(tr("com6"));
    ui->PortBox->addItem(tr("com7"));
    ui->PortBox->addItem(tr("com9"));
    ui->PortBox->addItem(tr("com10"));
    ui->PortBox->addItem(tr("com11"));
    ui->PortBox->addItem(tr("com15"));
    ui->PortBox->addItem(tr("com17"));
    ui->PortBox->addItem(tr("com18"));
    ui->PortBox->addItem(tr("com19"));
    ui->PortBox->addItem(tr("com20"));
    ui->first_last->addItem(tr("AI先手"));
    ui->first_last->addItem(tr("玩家先手"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::myaction()
{
    QStringList wordlist;
    QStringList littleword;
    QStringList childlist;
    mess = serialport->readAll();
    QString str(mess);
    if(!str.endsWith("\n"))
    {
        msg+=str;
    }
    else
    {
        msg+=str;
        wordlist=msg.split("\r\n");
        msg.clear();
        for(QString buf : wordlist)
        {
            if(buf!=NULL)
            {
                if(buf.startsWith('x'))
                {
                    littleword=buf.split(' ');
                    for(QString word:littleword)
                    {
                        childlist = word.split(':');
                        if(childlist[0] == "x")
                        {
                            ui->x_label->setText(childlist[1]);
                        }
                        else if(childlist[0] == "y")
                        {
                            ui->y_label->setText(childlist[1]);
                        }
                        else
                        {
                            ui->a_label->setText(childlist[1]);
                        }
                    }
                }
                else
                {
                   ui->textEdit->append(buf);
                }
            }
        }
        mess.clear();
     }
}

void MainWindow::board_init()
{
    memset(myBoard,0,sizeof(myBoard));
    memset(aimBoard,0,sizeof(aimBoard));
    memset(finalBoard,0,sizeof(finalBoard));

    myButton[0][0] = ui->btn11;
    myButton[0][1] = ui->btn12;
    myButton[0][2] = ui->btn13;
    myButton[0][3] = ui->btn14;
    myButton[0][4] = ui->btn15;
    myButton[0][5] = ui->btn16;
    myButton[0][6] = ui->btn17;
    myButton[0][7] = ui->btn18;

    myButton[1][0] = ui->btn21;
    myButton[1][1] = ui->btn22;
    myButton[1][2] = ui->btn23;
    myButton[1][3] = ui->btn24;
    myButton[1][4] = ui->btn25;
    myButton[1][5] = ui->btn26;
    myButton[1][6] = ui->btn27;
    myButton[1][7] = ui->btn28;

    myButton[2][0] = ui->btn31;
    myButton[2][1] = ui->btn32;
    myButton[2][2] = ui->btn33;
    myButton[2][3] = ui->btn34;
    myButton[2][4] = ui->btn35;
    myButton[2][5] = ui->btn36;
    myButton[2][6] = ui->btn37;
    myButton[2][7] = ui->btn38;

    myButton[3][0] = ui->btn41;
    myButton[3][1] = ui->btn42;
    myButton[3][2] = ui->btn43;
    myButton[3][3] = ui->btn44;
    myButton[3][4] = ui->btn45;
    myButton[3][5] = ui->btn46;
    myButton[3][6] = ui->btn47;
    myButton[3][7] = ui->btn48;

    myButton[4][0] = ui->btn51;
    myButton[4][1] = ui->btn52;
    myButton[4][2] = ui->btn53;
    myButton[4][3] = ui->btn54;
    myButton[4][4] = ui->btn55;
    myButton[4][5] = ui->btn56;
    myButton[4][6] = ui->btn57;
    myButton[4][7] = ui->btn58;

    myButton[5][0] = ui->btn61;
    myButton[5][1] = ui->btn62;
    myButton[5][2] = ui->btn63;
    myButton[5][3] = ui->btn64;
    myButton[5][4] = ui->btn65;
    myButton[5][5] = ui->btn66;
    myButton[5][6] = ui->btn67;
    myButton[5][7] = ui->btn68;

    myButton[6][0] = ui->btn71;
    myButton[6][1] = ui->btn72;
    myButton[6][2] = ui->btn73;
    myButton[6][3] = ui->btn74;
    myButton[6][4] = ui->btn75;
    myButton[6][5] = ui->btn76;
    myButton[6][6] = ui->btn77;
    myButton[6][7] = ui->btn78;

    myButton[7][0] = ui->btn81;
    myButton[7][1] = ui->btn82;
    myButton[7][2] = ui->btn83;
    myButton[7][3] = ui->btn84;
    myButton[7][4] = ui->btn85;
    myButton[7][5] = ui->btn86;
    myButton[7][6] = ui->btn87;
    myButton[7][7] = ui->btn88;

    finalButton[0][0] = ui->btn11_2;
    finalButton[0][1] = ui->btn12_2;
    finalButton[0][2] = ui->btn13_2;
    finalButton[0][3] = ui->btn14_2;
    finalButton[0][4] = ui->btn15_2;
    finalButton[0][5] = ui->btn16_2;
    finalButton[0][6] = ui->btn17_2;

    finalButton[1][0] = ui->btn21_2;
    finalButton[1][1] = ui->btn22_2;
    finalButton[1][2] = ui->btn23_2;
    finalButton[1][3] = ui->btn24_2;
    finalButton[1][4] = ui->btn25_2;
    finalButton[1][5] = ui->btn26_2;
    finalButton[1][6] = ui->btn27_2;

    finalButton[2][0] = ui->btn31_2;
    finalButton[2][1] = ui->btn32_2;
    finalButton[2][2] = ui->btn33_2;
    finalButton[2][3] = ui->btn34_2;
    finalButton[2][4] = ui->btn35_2;
    finalButton[2][5] = ui->btn36_2;
    finalButton[2][6] = ui->btn37_2;

    finalButton[3][0] = ui->btn41_2;
    finalButton[3][1] = ui->btn42_2;
    finalButton[3][2] = ui->btn43_2;
    finalButton[3][3] = ui->btn44_2;
    finalButton[3][4] = ui->btn45_2;
    finalButton[3][5] = ui->btn46_2;
    finalButton[3][6] = ui->btn47_2;

    finalButton[4][0] = ui->btn51_2;
    finalButton[4][1] = ui->btn52_2;
    finalButton[4][2] = ui->btn53_2;
    finalButton[4][3] = ui->btn54_2;
    finalButton[4][4] = ui->btn55_2;
    finalButton[4][5] = ui->btn56_2;
    finalButton[4][6] = ui->btn57_2;

    finalButton[5][0] = ui->btn61_2;
    finalButton[5][1] = ui->btn62_2;
    finalButton[5][2] = ui->btn63_2;
    finalButton[5][3] = ui->btn64_2;
    finalButton[5][4] = ui->btn65_2;
    finalButton[5][5] = ui->btn66_2;
    finalButton[5][6] = ui->btn67_2;

    finalButton[6][0] = ui->btn71_2;
    finalButton[6][1] = ui->btn72_2;
    finalButton[6][2] = ui->btn73_2;
    finalButton[6][3] = ui->btn74_2;
    finalButton[6][4] = ui->btn75_2;
    finalButton[6][5] = ui->btn76_2;
    finalButton[6][6] = ui->btn77_2;


//障碍的初始化
    littleOBS[0].x = 0 , littleOBS[0].y = 0 , littleOBS[0].kind = 0, littleOBS[0].btn = ui->obs110;
    littleOBS[1].x = 0 , littleOBS[1].y = 1 , littleOBS[1].kind = 0, littleOBS[1].btn = ui->obs120;
    littleOBS[2].x = 0 , littleOBS[2].y = 2 , littleOBS[2].kind = 0, littleOBS[2].btn = ui->obs130;
    littleOBS[3].x = 0 , littleOBS[3].y = 3 , littleOBS[3].kind = 0, littleOBS[3].btn = ui->obs140;
    littleOBS[4].x = 0 , littleOBS[4].y = 4 , littleOBS[4].kind = 0, littleOBS[4].btn = ui->obs150;
    littleOBS[5].x = 0 , littleOBS[5].y = 5 , littleOBS[5].kind = 0, littleOBS[5].btn = ui->obs160;

    littleOBS[6].x = 1 , littleOBS[6].y = 0 , littleOBS[6].kind = 0, littleOBS[6].btn = ui->obs210;
    littleOBS[7].x = 1 , littleOBS[7].y = 1 , littleOBS[7].kind = 0, littleOBS[7].btn = ui->obs220;
    littleOBS[8].x = 1 , littleOBS[8].y = 2 , littleOBS[8].kind = 0, littleOBS[8].btn = ui->obs230;
    littleOBS[9].x = 1 , littleOBS[9].y = 3 , littleOBS[9].kind = 0, littleOBS[9].btn = ui->obs240;
    littleOBS[10].x = 1 , littleOBS[10].y = 4 , littleOBS[10].kind = 0, littleOBS[10].btn = ui->obs250;
    littleOBS[11].x = 1 , littleOBS[11].y = 5 , littleOBS[11].kind = 0, littleOBS[11].btn = ui->obs260;

    littleOBS[12].x = 2 , littleOBS[12].y = 0 , littleOBS[12].kind = 0, littleOBS[12].btn = ui->obs310;
    littleOBS[13].x = 2 , littleOBS[13].y = 1 , littleOBS[13].kind = 0, littleOBS[13].btn = ui->obs320;
    littleOBS[14].x = 2 , littleOBS[14].y = 2 , littleOBS[14].kind = 0, littleOBS[14].btn = ui->obs330;
    littleOBS[15].x = 2 , littleOBS[15].y = 3 , littleOBS[15].kind = 0, littleOBS[15].btn = ui->obs340;
    littleOBS[16].x = 2 , littleOBS[16].y = 4 , littleOBS[16].kind = 0, littleOBS[16].btn = ui->obs350;
    littleOBS[17].x = 2 , littleOBS[17].y = 5 , littleOBS[17].kind = 0, littleOBS[17].btn = ui->obs360;

    littleOBS[18].x = 3 , littleOBS[18].y = 0 , littleOBS[18].kind = 0, littleOBS[18].btn = ui->obs410;
    littleOBS[19].x = 3 , littleOBS[19].y = 1 , littleOBS[19].kind = 0, littleOBS[19].btn = ui->obs420;
    littleOBS[20].x = 3 , littleOBS[20].y = 2 , littleOBS[20].kind = 0, littleOBS[20].btn = ui->obs430;
    littleOBS[21].x = 3 , littleOBS[21].y = 3 , littleOBS[21].kind = 0, littleOBS[21].btn = ui->obs440;
    littleOBS[22].x = 3 , littleOBS[22].y = 4 , littleOBS[22].kind = 0, littleOBS[22].btn = ui->obs450;
    littleOBS[23].x = 3 , littleOBS[23].y = 5 , littleOBS[23].kind = 0, littleOBS[23].btn = ui->obs460;

    littleOBS[24].x = 4 , littleOBS[24].y = 0 , littleOBS[24].kind = 0, littleOBS[24].btn = ui->obs510;
    littleOBS[25].x = 4 , littleOBS[25].y = 1 , littleOBS[25].kind = 0, littleOBS[25].btn = ui->obs520;
    littleOBS[26].x = 4 , littleOBS[26].y = 2 , littleOBS[26].kind = 0, littleOBS[26].btn = ui->obs530;
    littleOBS[27].x = 4 , littleOBS[27].y = 3 , littleOBS[27].kind = 0, littleOBS[27].btn = ui->obs540;
    littleOBS[28].x = 4 , littleOBS[28].y = 4 , littleOBS[28].kind = 0, littleOBS[28].btn = ui->obs550;
    littleOBS[29].x = 4 , littleOBS[29].y = 5 , littleOBS[29].kind = 0, littleOBS[29].btn = ui->obs560;

    littleOBS[30].x = 5 , littleOBS[30].y = 0 , littleOBS[30].kind = 0, littleOBS[30].btn = ui->obs610;
    littleOBS[31].x = 5 , littleOBS[31].y = 1 , littleOBS[31].kind = 0, littleOBS[31].btn = ui->obs620;
    littleOBS[32].x = 5 , littleOBS[32].y = 2 , littleOBS[32].kind = 0, littleOBS[32].btn = ui->obs630;
    littleOBS[33].x = 5 , littleOBS[33].y = 3 , littleOBS[33].kind = 0, littleOBS[33].btn = ui->obs640;
    littleOBS[34].x = 5 , littleOBS[34].y = 4 , littleOBS[34].kind = 0, littleOBS[34].btn = ui->obs650;
    littleOBS[35].x = 5 , littleOBS[35].y = 5 , littleOBS[35].kind = 0, littleOBS[35].btn = ui->obs660;

    littleOBS[36].x = 6 , littleOBS[36].y = 0 , littleOBS[36].kind = 0, littleOBS[36].btn = ui->obs710;
    littleOBS[37].x = 6 , littleOBS[37].y = 1 , littleOBS[37].kind = 0, littleOBS[37].btn = ui->obs720;
    littleOBS[38].x = 6 , littleOBS[38].y = 2 , littleOBS[38].kind = 0, littleOBS[38].btn = ui->obs730;
    littleOBS[39].x = 6 , littleOBS[39].y = 3 , littleOBS[39].kind = 0, littleOBS[39].btn = ui->obs740;
    littleOBS[40].x = 6 , littleOBS[40].y = 4 , littleOBS[40].kind = 0, littleOBS[40].btn = ui->obs750;
    littleOBS[41].x = 6 , littleOBS[41].y = 5 , littleOBS[41].kind = 0, littleOBS[41].btn = ui->obs760;
/**************************************************************************************************************/
    littleOBS[42].x = 0 , littleOBS[42].y = 0 , littleOBS[42].kind = 1, littleOBS[42].btn = ui->obs111;
    littleOBS[43].x = 0 , littleOBS[43].y = 1 , littleOBS[43].kind = 1, littleOBS[43].btn = ui->obs121;
    littleOBS[44].x = 0 , littleOBS[44].y = 2 , littleOBS[44].kind = 1, littleOBS[44].btn = ui->obs131;
    littleOBS[45].x = 0 , littleOBS[45].y = 3 , littleOBS[45].kind = 1, littleOBS[45].btn = ui->obs141;
    littleOBS[46].x = 0 , littleOBS[46].y = 4 , littleOBS[46].kind = 1, littleOBS[46].btn = ui->obs151;
    littleOBS[47].x = 0 , littleOBS[47].y = 5 , littleOBS[47].kind = 1, littleOBS[47].btn = ui->obs161;

    littleOBS[48].x = 1 , littleOBS[48].y = 0 , littleOBS[48].kind = 1, littleOBS[48].btn = ui->obs211;
    littleOBS[49].x = 1 , littleOBS[49].y = 1 , littleOBS[49].kind = 1, littleOBS[49].btn = ui->obs221;
    littleOBS[50].x = 1 , littleOBS[50].y = 2 , littleOBS[50].kind = 1, littleOBS[50].btn = ui->obs231;
    littleOBS[51].x = 1 , littleOBS[51].y = 3 , littleOBS[51].kind = 1, littleOBS[51].btn = ui->obs241;
    littleOBS[52].x = 1 , littleOBS[52].y = 4 , littleOBS[52].kind = 1, littleOBS[52].btn = ui->obs251;
    littleOBS[53].x = 1 , littleOBS[53].y = 5 , littleOBS[53].kind = 1, littleOBS[53].btn = ui->obs261;

    littleOBS[54].x = 2 , littleOBS[54].y = 0 , littleOBS[54].kind = 1, littleOBS[54].btn = ui->obs311;
    littleOBS[55].x = 2 , littleOBS[55].y = 1 , littleOBS[55].kind = 1, littleOBS[55].btn = ui->obs321;
    littleOBS[56].x = 2 , littleOBS[56].y = 2 , littleOBS[56].kind = 1, littleOBS[56].btn = ui->obs331;
    littleOBS[57].x = 2 , littleOBS[57].y = 3 , littleOBS[57].kind = 1, littleOBS[57].btn = ui->obs341;
    littleOBS[58].x = 2 , littleOBS[58].y = 4 , littleOBS[58].kind = 1, littleOBS[58].btn = ui->obs351;
    littleOBS[59].x = 2 , littleOBS[59].y = 5 , littleOBS[59].kind = 1, littleOBS[59].btn = ui->obs361;

    littleOBS[60].x = 3 , littleOBS[60].y = 0 , littleOBS[60].kind = 1, littleOBS[60].btn = ui->obs411;
    littleOBS[61].x = 3 , littleOBS[61].y = 1 , littleOBS[61].kind = 1, littleOBS[61].btn = ui->obs421;
    littleOBS[62].x = 3 , littleOBS[62].y = 2 , littleOBS[62].kind = 1, littleOBS[62].btn = ui->obs431;
    littleOBS[63].x = 3 , littleOBS[63].y = 3 , littleOBS[63].kind = 1, littleOBS[63].btn = ui->obs441;
    littleOBS[64].x = 3 , littleOBS[64].y = 4 , littleOBS[64].kind = 1, littleOBS[64].btn = ui->obs451;
    littleOBS[65].x = 3 , littleOBS[65].y = 5 , littleOBS[65].kind = 1, littleOBS[65].btn = ui->obs461;

    littleOBS[66].x = 4 , littleOBS[66].y = 0 , littleOBS[66].kind = 1, littleOBS[66].btn = ui->obs511;
    littleOBS[67].x = 4 , littleOBS[67].y = 1 , littleOBS[67].kind = 1, littleOBS[67].btn = ui->obs521;
    littleOBS[68].x = 4 , littleOBS[68].y = 2 , littleOBS[68].kind = 1, littleOBS[68].btn = ui->obs531;
    littleOBS[69].x = 4 , littleOBS[69].y = 3 , littleOBS[69].kind = 1, littleOBS[69].btn = ui->obs541;
    littleOBS[70].x = 4 , littleOBS[70].y = 4 , littleOBS[70].kind = 1, littleOBS[70].btn = ui->obs551;
    littleOBS[71].x = 4 , littleOBS[71].y = 5 , littleOBS[71].kind = 1, littleOBS[71].btn = ui->obs561;

    littleOBS[72].x = 5 , littleOBS[72].y = 0 , littleOBS[72].kind = 1, littleOBS[72].btn = ui->obs611;
    littleOBS[73].x = 5 , littleOBS[73].y = 1 , littleOBS[73].kind = 1, littleOBS[73].btn = ui->obs621;
    littleOBS[74].x = 5 , littleOBS[74].y = 2 , littleOBS[74].kind = 1, littleOBS[74].btn = ui->obs631;
    littleOBS[75].x = 5 , littleOBS[75].y = 3 , littleOBS[75].kind = 1, littleOBS[75].btn = ui->obs641;
    littleOBS[76].x = 5 , littleOBS[76].y = 4 , littleOBS[76].kind = 1, littleOBS[76].btn = ui->obs651;
    littleOBS[77].x = 5 , littleOBS[77].y = 5 , littleOBS[77].kind = 1, littleOBS[77].btn = ui->obs661;

    littleOBS[78].x = 6 , littleOBS[78].y = 0 , littleOBS[78].kind = 1, littleOBS[78].btn = ui->obs711;
    littleOBS[79].x = 6 , littleOBS[79].y = 1 , littleOBS[79].kind = 1, littleOBS[79].btn = ui->obs721;
    littleOBS[80].x = 6 , littleOBS[80].y = 2 , littleOBS[80].kind = 1, littleOBS[80].btn = ui->obs731;
    littleOBS[81].x = 6 , littleOBS[81].y = 3 , littleOBS[81].kind = 1, littleOBS[81].btn = ui->obs741;
    littleOBS[82].x = 6 , littleOBS[82].y = 4 , littleOBS[82].kind = 1, littleOBS[82].btn = ui->obs751;
    littleOBS[83].x = 6 , littleOBS[83].y = 5 , littleOBS[83].kind = 1, littleOBS[83].btn = ui->obs761;
//小障碍的初始化
    mySmall[0][0].btn = ui->small0 , mySmall[0][1].btn = ui->small1 , mySmall[0][2].btn = ui->small2 , mySmall[0][3].btn = ui->small3 , mySmall[0][4].btn = ui->small4 , mySmall[0][5].btn = ui->small5;
    mySmall[1][0].btn = ui->small6 , mySmall[1][1].btn = ui->small7 , mySmall[1][2].btn = ui->small8 , mySmall[1][3].btn = ui->small9 , mySmall[1][4].btn = ui->small10 , mySmall[1][5].btn = ui->small11;
    mySmall[2][0].btn = ui->small12 , mySmall[2][1].btn = ui->small13 , mySmall[2][2].btn = ui->small14 , mySmall[2][3].btn = ui->small15 , mySmall[2][4].btn = ui->small16 , mySmall[2][5].btn = ui->small17;
    mySmall[3][0].btn = ui->small18 , mySmall[3][1].btn = ui->small19 , mySmall[3][2].btn = ui->small20 , mySmall[3][3].btn = ui->small21 , mySmall[3][4].btn = ui->small22 , mySmall[3][5].btn = ui->small23;
    mySmall[4][0].btn = ui->small24 , mySmall[4][1].btn = ui->small25 , mySmall[4][2].btn = ui->small26 , mySmall[4][3].btn = ui->small27 , mySmall[4][4].btn = ui->small28 , mySmall[4][5].btn = ui->small29;
    mySmall[5][0].btn = ui->small30 , mySmall[5][1].btn = ui->small31 , mySmall[5][2].btn = ui->small32 , mySmall[5][3].btn = ui->small33 , mySmall[5][4].btn = ui->small34 , mySmall[5][5].btn = ui->small35;

    for(int i=0; i<84; i++)
    {
        littleOBS[i].status = Blank;
    }

    for(int i=0; i<6; i++)
    {
        for(int j=0; j<6; j++)
        {
            mySmall[i][j].x = i;
            mySmall[i][j].y = j;
            mySmall[i][j].status = Blank;
        }
    }


    ui->explain->setText("当前动作：");
    ui->ai_obs->setText("AI障碍:X10");
    ui->human_obs->setText("玩家障碍:X10");
    myStatus = 0;

    mystep.clear();

    finalBoard[0][3] = AIchess;
    finalBoard[6][3] = Playerchess;
    ui->obstacle->setEnabled(false);
    ui->move->setEnabled(false);
    ui->sure->setEnabled(false);
    ui->regret->setEnabled(false);
    finalButton[0][3]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
    finalButton[6][3]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
    mychess[0].x = 0;
    mychess[0].y = 3;
    mychess[1].x = 6;
    mychess[1].y = 3;
    mychess[0].obsNum = 10;
    mychess[1].obsNum = 10;
}

void MainWindow::on_btn11_clicked()
{
    if(myBoard[0][0] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn11->setStyleSheet("QPushButton#btn11{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[0][0] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn11->setStyleSheet("");
        myBoard[0][0] = 0;
        chessNum --;
    }

}

void MainWindow::on_btn12_clicked()
{
    if(myBoard[0][1] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn12->setStyleSheet("QPushButton#btn12{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[0][1] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn12->setStyleSheet("");
        myBoard[0][1] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn13_clicked()
{
    if(myBoard[0][2] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn13->setStyleSheet("QPushButton#btn13{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[0][2] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn13->setStyleSheet("");
        myBoard[0][2] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn14_clicked()
{
    if(myBoard[0][3] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn14->setStyleSheet("QPushButton#btn14{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[0][3] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn14->setStyleSheet("");
        myBoard[0][3] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn15_clicked()
{
    if(myBoard[0][4] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn15->setStyleSheet("QPushButton#btn15{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[0][4] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn15->setStyleSheet("");
        myBoard[0][4] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn16_clicked()
{
    if(myBoard[0][5] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn16->setStyleSheet("QPushButton#btn16{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[0][5] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn16->setStyleSheet("");
        myBoard[0][5] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn17_clicked()
{
    if(myBoard[0][6] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn17->setStyleSheet("QPushButton#btn17{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[0][6] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn17->setStyleSheet("");
        myBoard[0][6] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn18_clicked()
{
    if(myBoard[0][7] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn18->setStyleSheet("QPushButton#btn18{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[0][7] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn18->setStyleSheet("");
        myBoard[0][7] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn21_clicked()
{
    if(myBoard[1][0] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn21->setStyleSheet("QPushButton#btn21{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[1][0] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn21->setStyleSheet("");
        myBoard[1][0] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn22_clicked()
{
    if(myBoard[1][1] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn22->setStyleSheet("QPushButton#btn22{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[1][1] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn22->setStyleSheet("");
        myBoard[1][1] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn23_clicked()
{
    if(myBoard[1][2] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn23->setStyleSheet("QPushButton#btn23{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[1][2] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn23->setStyleSheet("");
        myBoard[1][2] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn24_clicked()
{
    if(myBoard[1][3] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn24->setStyleSheet("QPushButton#btn24{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[1][3] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn24->setStyleSheet("");
        myBoard[1][3] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn25_clicked()
{
    if(myBoard[1][4] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn25->setStyleSheet("QPushButton#btn25{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[1][4] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn25->setStyleSheet("");
        myBoard[1][4] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn26_clicked()
{
    if(myBoard[1][5] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn26->setStyleSheet("QPushButton#btn26{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[1][5] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn26->setStyleSheet("");
        myBoard[1][5] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn27_clicked()
{
    if(myBoard[1][6] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn27->setStyleSheet("QPushButton#btn27{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[1][6] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn27->setStyleSheet("");
        myBoard[1][6] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn28_clicked()
{
    if(myBoard[1][7] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn28->setStyleSheet("QPushButton#btn28{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[1][7] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn28->setStyleSheet("");
        myBoard[1][7] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn31_clicked()
{
    if(myBoard[2][0] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn31->setStyleSheet("QPushButton#btn31{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[2][0] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn31->setStyleSheet("");
        myBoard[2][0] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn32_clicked()
{
    if(myBoard[2][1] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn32->setStyleSheet("QPushButton#btn32{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[2][1] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn32->setStyleSheet("");
        myBoard[2][1] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn33_clicked()
{
    if(myBoard[2][2] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn33->setStyleSheet("QPushButton#btn33{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[2][2] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn33->setStyleSheet("");
        myBoard[2][2] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn34_clicked()
{
    if(myBoard[2][3] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn34->setStyleSheet("QPushButton#btn34{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[2][3] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn34->setStyleSheet("");
        myBoard[2][3] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn35_clicked()
{
    if(myBoard[2][4] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn35->setStyleSheet("QPushButton#btn35{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[2][4] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn35->setStyleSheet("");
        myBoard[2][4] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn36_clicked()
{
    if(myBoard[2][5] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn36->setStyleSheet("QPushButton#btn36{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[2][5] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn36->setStyleSheet("");
        myBoard[2][5] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn37_clicked()
{
    if(myBoard[2][6] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn37->setStyleSheet("QPushButton#btn37{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[2][6] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn37->setStyleSheet("");
        myBoard[2][6] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn38_clicked()
{
    if(myBoard[2][7] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn38->setStyleSheet("QPushButton#btn38{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[2][7] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn38->setStyleSheet("");
        myBoard[2][7] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn41_clicked()
{
    if(myBoard[3][0] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn41->setStyleSheet("QPushButton#btn41{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[3][0] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn41->setStyleSheet("");
        myBoard[3][0] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn42_clicked()
{
    if(myBoard[3][1] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn42->setStyleSheet("QPushButton#btn42{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[3][1] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn42->setStyleSheet("");
        myBoard[3][1] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn43_clicked()
{
    if(myBoard[3][2] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn43->setStyleSheet("QPushButton#btn43{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[3][2] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn43->setStyleSheet("");
        myBoard[3][2] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn44_clicked()
{
    if(myBoard[3][3] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn44->setStyleSheet("QPushButton#btn44{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[3][3] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn44->setStyleSheet("");
        myBoard[3][3] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn45_clicked()
{
    if(myBoard[3][4] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn45->setStyleSheet("QPushButton#btn45{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[3][4] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn45->setStyleSheet("");
        myBoard[3][4] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn46_clicked()
{
    if(myBoard[3][5] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn46->setStyleSheet("QPushButton#btn46{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[3][5] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn46->setStyleSheet("");
        myBoard[3][5] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn47_clicked()
{
    if(myBoard[3][6] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn47->setStyleSheet("QPushButton#btn47{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[3][6] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn47->setStyleSheet("");
        myBoard[3][6] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn48_clicked()
{
    if(myBoard[3][7] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn48->setStyleSheet("QPushButton#btn48{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[3][7] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn48->setStyleSheet("");
        myBoard[3][7] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn51_clicked()
{
    if(myBoard[4][0] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn51->setStyleSheet("QPushButton#btn51{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[4][0] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn51->setStyleSheet("");
        myBoard[4][0] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn52_clicked()
{
    if(myBoard[4][1] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn52->setStyleSheet("QPushButton#btn52{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[4][1] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn52->setStyleSheet("");
        myBoard[4][1] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn53_clicked()
{
    if(myBoard[4][2] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn53->setStyleSheet("QPushButton#btn53{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[4][2] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn53->setStyleSheet("");
        myBoard[4][2] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn54_clicked()
{
    if(myBoard[4][3] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn54->setStyleSheet("QPushButton#btn54{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[4][3] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn54->setStyleSheet("");
        myBoard[4][3] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn55_clicked()
{
    if(myBoard[4][4] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn55->setStyleSheet("QPushButton#btn55{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[4][4] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn55->setStyleSheet("");
        myBoard[4][4] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn56_clicked()
{
    if(myBoard[4][5] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn56->setStyleSheet("QPushButton#btn56{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[4][5] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn56->setStyleSheet("");
        myBoard[4][5] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn57_clicked()
{
    if(myBoard[4][6] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn57->setStyleSheet("QPushButton#btn57{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[4][6] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn57->setStyleSheet("");
        myBoard[4][6] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn58_clicked()
{
    if(myBoard[4][7] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn58->setStyleSheet("QPushButton#btn58{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[4][7] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn58->setStyleSheet("");
        myBoard[4][7] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn61_clicked()
{
    if(myBoard[5][0] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn61->setStyleSheet("QPushButton#btn61{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[5][0] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn61->setStyleSheet("");
        myBoard[5][0] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn62_clicked()
{
    if(myBoard[5][1] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn62->setStyleSheet("QPushButton#btn62{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[5][1] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn62->setStyleSheet("");
        myBoard[5][1] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn63_clicked()
{
    if(myBoard[5][2] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn63->setStyleSheet("QPushButton#btn63{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[5][2] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn63->setStyleSheet("");
        myBoard[5][2] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn64_clicked()
{
    if(myBoard[5][3] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn64->setStyleSheet("QPushButton#btn64{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[5][3] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn64->setStyleSheet("");
        myBoard[5][3] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn65_clicked()
{
    if(myBoard[5][4] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn65->setStyleSheet("QPushButton#btn65{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[5][4] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn65->setStyleSheet("");
        myBoard[5][4] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn66_clicked()
{
    if(myBoard[5][5] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn66->setStyleSheet("QPushButton#btn66{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[5][5] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn66->setStyleSheet("");
        myBoard[5][5] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn67_clicked()
{
    if(myBoard[5][6] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn67->setStyleSheet("QPushButton#btn67{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[5][6] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn67->setStyleSheet("");
        myBoard[5][6] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn68_clicked()
{
    if(myBoard[5][7] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn68->setStyleSheet("QPushButton#btn68{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[5][7] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn68->setStyleSheet("");
        myBoard[5][7] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn71_clicked()
{
    if(myBoard[6][0] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn71->setStyleSheet("QPushButton#btn71{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[6][0] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn71->setStyleSheet("");
        myBoard[6][0] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn72_clicked()
{
    if(myBoard[6][1] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn72->setStyleSheet("QPushButton#btn72{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[6][1] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn72->setStyleSheet("");
        myBoard[6][1] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn73_clicked()
{
    if(myBoard[6][2] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn73->setStyleSheet("QPushButton#btn73{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[6][2] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn73->setStyleSheet("");
        myBoard[6][2] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn74_clicked()
{
    if(myBoard[6][3] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn74->setStyleSheet("QPushButton#btn74{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[6][3] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn74->setStyleSheet("");
        myBoard[6][3] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn75_clicked()
{
    if(myBoard[6][4] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn75->setStyleSheet("QPushButton#btn75{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[6][4] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn75->setStyleSheet("");
        myBoard[6][4] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn76_clicked()
{
    if(myBoard[6][5] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn76->setStyleSheet("QPushButton#btn76{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[6][5] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn76->setStyleSheet("");
        myBoard[6][5] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn77_clicked()
{
    if(myBoard[6][6] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn77->setStyleSheet("QPushButton#btn77{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[6][6] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn77->setStyleSheet("");
        myBoard[6][6] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn78_clicked()
{
    if(myBoard[6][7] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn78->setStyleSheet("QPushButton#btn78{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[6][7] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn78->setStyleSheet("");
        myBoard[6][7] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn81_clicked()
{
    if(myBoard[7][0] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn81->setStyleSheet("QPushButton#btn81{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[7][0] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn81->setStyleSheet("");
        myBoard[7][0] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn82_clicked()
{
    if(myBoard[7][1] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn82->setStyleSheet("QPushButton#btn82{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[7][1] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn82->setStyleSheet("");
        myBoard[7][1] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn83_clicked()
{
    if(myBoard[7][2] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn83->setStyleSheet("QPushButton#btn83{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[7][2] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn83->setStyleSheet("");
        myBoard[7][2] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn84_clicked()
{
    if(myBoard[7][3] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn84->setStyleSheet("QPushButton#btn84{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[7][3] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn84->setStyleSheet("");
        myBoard[7][3] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn85_clicked()
{
    if(myBoard[7][4] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn85->setStyleSheet("QPushButton#btn85{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[7][4] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn85->setStyleSheet("");
        myBoard[7][4] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn86_clicked()
{
    if(myBoard[7][5] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn86->setStyleSheet("QPushButton#btn86{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[7][5] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn86->setStyleSheet("");
        myBoard[7][5] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn87_clicked()
{
    if(myBoard[7][6] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn87->setStyleSheet("QPushButton#btn87{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[7][6] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn87->setStyleSheet("");
        myBoard[7][6] = 0;
        chessNum --;
    }
}

void MainWindow::on_btn88_clicked()
{
    if(myBoard[7][7] == 0)
    {
        if(chessNum == 8)
        {
            QMessageBox::warning(this,"warning","已经放了8个棋子了！！！");
            return;
        }
        ui->btn88->setStyleSheet("QPushButton#btn88{background-color: black ;border: 1px solid black; border-radius:5px;}");
        myBoard[7][7] = 1;
        chessNum ++;
    }
    else
    {
        ui->btn88->setStyleSheet("");
        myBoard[7][7] = 0;
        chessNum --;
    }
}

void MainWindow::on_connect_clicked()
{
    if(ui->connect->text()==tr("建立连接"))
    {
        serialport = new QSerialPort();
        if(ui->PortBox->currentText().length() == 4)
            serialport->setPortName(ui->PortBox->currentText());
        else
            serialport->setPortName("\\\\.\\"+ui->PortBox->currentText());
        if(serialport->open(QIODevice::ReadWrite))
        {
            serialport->setBaudRate(115200);
            serialport->setDataBits(QSerialPort::Data8);
            serialport->setStopBits(QSerialPort::OneStop);
            serialport->setParity(QSerialPort::NoParity);
            serialport->setFlowControl(QSerialPort::NoFlowControl);
            ui->connect->setText(tr("断开连接"));
            connect(serialport,SIGNAL(readyRead()),this,SLOT(myaction()));
        }
        else
            QMessageBox::warning(this,"完了","全完了",QMessageBox::Retry);
    }
    else
    {
        serialport->clear();
        serialport->close();
        serialport->deleteLater();
        ui->connect->setText(tr("建立连接"));
    }
}
void MainWindow::on_pushButton_3_clicked()
{
    if(chessNum < 8)
    {
        QMessageBox::warning(this,"warning","裁判还没有摆够8个棋子");
        return;
    }

    mindis = 9999;
    maxNum = -1;

    myPoint.clear();
    for(int i = 0; i <= 7; i++)
    {
        for(int j = 0; j <= 7; j++)
        {
            if(myBoard[i][j] == 1)
            {
                QPoint newPoint;
                newPoint.setX(i+1);
                newPoint.setY(j+1);
                myPoint.append(newPoint);
            }
        }
    }

    Queen8(1);
    for(int i = 1; i <= 8; i++)
        qDebug()<<QString::number(finally[i-1])+","+QString::number(aimBoard[i]);

    for(int i = 1; i <= 8; i++)
    {
        if(myBoard[i-1][aimBoard[i]-1] == 1)
        {
            myButton[i-1][aimBoard[i]-1]->setStyleSheet(tr("QPushButton#btn%1%2{background-color: red ;border: 3px solid black; border-radius:5px;}").arg(QString::number(i),QString::number(aimBoard[i])));
        }
        else
        {
            myButton[i-1][aimBoard[i]-1]->setStyleSheet(tr("QPushButton#btn%1%2{background-color: blue ;border: 1px solid black; border-radius:5px;}").arg(QString::number(i),QString::number(aimBoard[i])));
        }
    }

}

void MainWindow::on_pushButton_4_clicked()
{
    memset(myBoard,0,sizeof(myBoard));
    chessNum = 0;
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
            myButton[i][j]->setStyleSheet("");

    }
}

void MainWindow::on_obstacle_clicked()
{
    myStatus = 1;
    ui->obstacle->setStyleSheet("QPushButton#obstacle{background-color: #cce4f7 ;border: 1px solid #005499; border-radius:3px;}");
    ui->move->setStyleSheet("");
    ui->explain->setText("当前动作：设置两格障碍");

    for(int i=0;i<84;i++)
    {
        if(littleOBS[i].status == Blank && littleOBS[i].kind == 0)
        {
            littleOBS[i].btn->setStyleSheet("QPushButton{border : none;background-color : white;}"
                                            "QPushButton:hover{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏hover.png);}");
        }
        else if(littleOBS[i].status == Blank && littleOBS[i].kind == 1)
        {
            littleOBS[i].btn->setStyleSheet("QPushButton{border : none;background-color : white;}"
                                            "QPushButton:hover{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏hover.png);}");
        }

    }
    for(int i=0; i<7; i++)
    {
        for(int j=0; j<7; j++)
        {
            if(finalBoard[i][j] == Blank)
                finalButton[i][j]->setStyleSheet("");
        }
    }
}

void MainWindow::on_move_clicked()
{
    myStatus = 2;
    ui->move->setStyleSheet("QPushButton#move{background-color: #cce4f7 ;border: 1px solid #005499; border-radius:3px;}");
    ui->obstacle->setStyleSheet("");
    ui->explain->setText("当前动作：移动一格");
    for(int i=0; i<7; i++)
    {
        for(int j=0;j<7;j++)
        {
            if(finalBoard[i][j] == Blank)
                finalButton[i][j]->setStyleSheet("QPushButton:hover{border: 1px solid #adadad;background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子hover.png);}");
        }
    }
    for(int i=0; i<84 ;i++)
    {
        if(littleOBS[i].status == Blank)
        {
            littleOBS[i].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");
        }
    }
}


void MainWindow::on_gamestart_clicked()
{
    if(ui->gamestart->text() == "重新开始")
    {
        ui->obstacle->setStyleSheet("");
        ui->move->setStyleSheet("");
        myStatus = 0;
        ui->explain->setText("当前动作：");
        memset(finalBoard,0,sizeof(finalBoard));
        for(int i=0; i<84; i++)
        {
            littleOBS[i].status = Blank;
            littleOBS[i].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");
        }
        for(int i=0; i<6; i++)
        {
            for(int j=0; j<6; j++)
            {
                mySmall[i][j].status = Blank;
                mySmall[i][j].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");
            }
        }

        mystep.clear();

        finalBoard[0][3] = AIchess;
        finalBoard[6][3] = Playerchess;
        ui->gamestart->setText("游戏开始");
        ui->obstacle->setEnabled(false);
        ui->move->setEnabled(false);
        ui->sure->setEnabled(false);
        ui->regret->setEnabled(false);
        finalButton[0][3]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
        finalButton[6][3]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
        mychess[0].x = 0;
        mychess[0].y = 3;
        mychess[1].x = 6;
        mychess[1].y = 3;
        mychess[0].obsNum = 10;
        mychess[1].obsNum = 10;
        ui->ai_obs->setText("AI障碍:X10");
        ui->human_obs->setText("玩家障碍:X10");
        for(int i=0; i<7; i++)
        {
            for(int j=0; j<7; j++)
            {
                if(finalBoard[i][j] == 0)
                    finalButton[i][j]->setStyleSheet("");
            }
        }
    }
    else if(ui->gamestart->text() == "游戏开始")
    {
        whoFirst = ui->first_last->currentIndex();
        ui->gamestart->setText("重新开始");
        ui->obstacle->setEnabled(true);
        ui->move->setEnabled(true);
        ui->sure->setEnabled(true);
        ui->regret->setEnabled(true);
        if(whoFirst == 0)
        {
            AIdepth = 4;
            int score = findAiMove(AIdepth,999,-999);
            qDebug()<<score;

            if(step_num == 1)
            {
                if(AIstep[0].status == 1)
                {
                    QMessageBox::warning(this,"waning","AI故障：每回合需放置两个障碍或者移动一格！！！");
                    return;
                }
                else
                {
                    STEP aiStep;
                    aiStep.x = mychess[0].x;
                    aiStep.y = mychess[0].y;
                    ai_pre_step.append(aiStep);
                    finalButton[mychess[0].x][mychess[0].y]->setStyleSheet("");
                    finalBoard[mychess[0].x][mychess[0].y] = Blank;
                    mychess[0].x = AIstep[0].x;
                    mychess[0].y = AIstep[0].y;
                    finalBoard[mychess[0].x][mychess[0].y] = AIchess;
                    finalButton[mychess[0].x][mychess[0].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    if(mychess[0].x == 6)
                        QMessageBox::information(this,"菜逼","AI获胜啦!!!");
                    step_num = 0;
                }
            }
            else if(step_num == 2)
            {
                if(littleOBS[AIstep[0].kind].status != Blank || littleOBS[AIstep[1].kind].status != Blank)
                {
                    QMessageBox::warning(this,"warning","AI故障：向不能放置障碍的地方放置了障碍");
                    return;
                }
                else
                {
                    littleOBS[AIstep[0].kind].status = Obstacle;
                    littleOBS[AIstep[1].kind].status = Obstacle;
                    mychess[0].obsNum --;
                    if(AIstep[0].kind < 42)
                        littleOBS[AIstep[0].kind].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
                    else
                        littleOBS[AIstep[0].kind].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");

                    if(AIstep[1].kind < 42)
                        littleOBS[AIstep[1].kind].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
                    else
                        littleOBS[AIstep[1].kind].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
                    STEP aiStep1,aiStep2;
                    aiStep1.kind = AIstep[0].kind;
                    aiStep2.kind = AIstep[1].kind;
                    ai_pre_step.append(aiStep1);
                    ai_pre_step.append(aiStep2);
                    step_num = 0;
                }
            }
            else
            {
                QMessageBox::warning(this,"warning","AI故障：您还没有行动！！！");
                qDebug()<<"stepnum is:"<<step_num;
                return;
            }
        }
    }
}

bool MainWindow::canMove(int x0, int y0, int x1, int y1)
{
    if(x0-x1 == 1 && y0 == y1)
    {
        if(littleOBS[42+y0*6+x1].status == Blank)
            return true;
        else
            return false;
    }
    else if(x1-x0 == 1 && y0 == y1)
    {
        if(littleOBS[42+y0*6+x0].status == Blank)
            return true;
        else
            return false;
    }
    else if(y1-y0 == 1 && x0 == x1)
    {
        if(littleOBS[x0*6+y0].status == Blank)
            return true;
        else
            return false;
    }
    else if(y0-y1 == 1 && x0 == x1)
    {
        if(littleOBS[x0*6+y1].status == Blank)
            return true;
        else
            return false;
    }
    else
        return false;
}

void MainWindow::on_btn11_2_clicked()
{
    int btnX = 0;int btnY = 0;

    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                QMessageBox::information(this,"恭喜","恭喜你获胜！！！");
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn12_2_clicked()
{
    int btnX = 0;int btnY = 1;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                QMessageBox::information(this,"恭喜","恭喜你获胜！！！");
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn13_2_clicked()
{
    int btnX = 0;int btnY = 2;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                QMessageBox::information(this,"恭喜","恭喜你获胜！！！");
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn14_2_clicked()
{
    int btnX = 0;int btnY = 3;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                QMessageBox::information(this,"恭喜","恭喜你获胜！！！");
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn15_2_clicked()
{
    int btnX = 0;int btnY = 4;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                QMessageBox::information(this,"恭喜","恭喜你获胜！！！");
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn16_2_clicked()
{
    int btnX = 0;int btnY = 5;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                QMessageBox::information(this,"恭喜","恭喜你获胜！！！");
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn17_2_clicked()
{
    int btnX = 0;int btnY = 6;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                QMessageBox::information(this,"恭喜","恭喜你获胜！！！");
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn71_2_clicked()
{
    int btnX = 6;int btnY = 0;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn72_2_clicked()
{
    int btnX = 6;int btnY = 1;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn73_2_clicked()
{
    int btnX = 6;int btnY = 2;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn74_2_clicked()
{
    int btnX = 6;int btnY = 3;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn75_2_clicked()
{
    int btnX = 6;int btnY = 4;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn76_2_clicked()
{
    int btnX = 6;int btnY = 5;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn77_2_clicked()
{
    int btnX = 6;int btnY = 6;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn61_2_clicked()
{
    int btnX = 5;int btnY = 0;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn62_2_clicked()
{
    int btnX = 5;int btnY = 1;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn63_2_clicked()
{
    int btnX = 5;int btnY = 2;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn64_2_clicked()
{
    int btnX = 5;int btnY = 3;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn65_2_clicked()
{
    int btnX = 5;int btnY = 4;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn66_2_clicked()
{
    int btnX = 5;int btnY = 5;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn67_2_clicked()
{
    int btnX = 5;int btnY = 6;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}
void MainWindow::on_btn51_2_clicked()
{
    int btnX = 4;int btnY = 0;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn52_2_clicked()
{
    int btnX = 4;int btnY = 1;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn53_2_clicked()
{
    int btnX = 4;int btnY = 2;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn54_2_clicked()
{
    int btnX = 4;int btnY = 3;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn55_2_clicked()
{
    int btnX = 4;int btnY = 4;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn56_2_clicked()
{
    int btnX = 4;int btnY = 5;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn57_2_clicked()
{
    int btnX = 4;int btnY = 6;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn41_2_clicked()
{
    int btnX = 3;int btnY = 0;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn42_2_clicked()
{
    int btnX = 3;int btnY = 1;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn43_2_clicked()
{
    int btnX = 3;int btnY = 2;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn44_2_clicked()
{
    int btnX = 3;int btnY = 3;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn45_2_clicked()
{
    int btnX = 3;int btnY = 4;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn46_2_clicked()
{
    int btnX = 3;int btnY = 5;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn47_2_clicked()
{
    int btnX = 3;int btnY = 6;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}
void MainWindow::on_btn31_2_clicked()
{
    int btnX = 2;int btnY = 0;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn32_2_clicked()
{
    int btnX = 2;int btnY = 1;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn33_2_clicked()
{
    int btnX = 2;int btnY = 2;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn34_2_clicked()
{
    int btnX = 2;int btnY = 3;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn35_2_clicked()
{
    int btnX = 2;int btnY = 4;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn36_2_clicked()
{
    int btnX = 2;int btnY = 5;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn37_2_clicked()
{
    int btnX = 2;int btnY = 6;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}
void MainWindow::on_btn21_2_clicked()
{
    int btnX = 1;int btnY = 0;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn22_2_clicked()
{
    int btnX = 1;int btnY = 1;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn23_2_clicked()
{
    int btnX = 1;int btnY = 2;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn24_2_clicked()
{
    int btnX = 1;int btnY = 3;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn25_2_clicked()
{
    int btnX = 1;int btnY = 4;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}

void MainWindow::on_btn26_2_clicked()
{
    int btnX = 1;int btnY = 5;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}
void MainWindow::on_btn27_2_clicked()
{
    int btnX = 1;int btnY = 6;
    if(myStatus == 2)
    {
        if(!mystep.empty())
        {
            QMessageBox::warning(this,"warning","您已经做过别的操作，不可再移动棋子！！！");
            return;
        }
        if(finalBoard[btnX][btnY] == Blank)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(finalBoard[mychess[1].x][mychess[1].y] == Playerchess)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                }
                else
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
                    finalBoard[mychess[1].x][mychess[1].y] = AIchess;
                }
                finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
                finalBoard[btnX][btnY] = Playerchess;
                STEP newstep;
                newstep.status = 2;
                newstep.x = btnX;
                newstep.y = btnY;
                newstep.kind = -1;
                mystep.append(newstep);
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else if(finalBoard[btnX][btnY] == AIchess)
        {
            if(this->canMove(btnX,btnY,mychess[1].x,mychess[1].y))
            {
                if(QMessageBox::information(this,"提醒","您当前的动作与对方的棋子重合，确定要继续此动作并再向四周移动一步么？",QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
                {
                    finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
                    finalBoard[mychess[1].x][mychess[1].y] = Blank;
                    finalButton[btnX][btnY]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家压ai.png);border:1px solid #ababab;}");
                    mychess[1].x = btnX;
                    mychess[1].y = btnY;
                    ui->obstacle->setEnabled(false);
                }
                else
                {
                    return;
                }
                return;
            }
            else
            {
                QMessageBox::warning(this,"warning","您只能在棋子周围一格内移动！！！");
                return;
            }
        }
        else
        {
            QMessageBox::warning(this,"warning","您不能将棋子移动到这里");
            return;
        }
    }
}
bool MainWindow::canfinish(int x, int y, int kind)//kind为1表示这是ai的棋子，kind为2表示这是玩家的棋子
{
    QQueue<QPoint> test;
    int w[7][7];
    int dx[4] = {1,-1,0,0};
    int dy[4] = {0,0,1,-1};

    memset(w,0,sizeof(w));
    w[x][y] = 1;
    QPoint firstPoint;
    firstPoint.setX(x);
    firstPoint.setY(y);
    test.enqueue(firstPoint);

    while(!test.empty())
    {
        QPoint nowPoint = test.dequeue();
        int nowX = nowPoint.x();
        int nowY = nowPoint.y();
        if((kind == 1 && nowX == 6) || (kind == 2 && nowX == 0))
            return true;

        for(int i = 0; i <= 3; i++)
        {
            if(nowX+dx[i]<0 || nowX+dx[i]>6 || nowY+dy[i]<0 || nowY+dy[i]>6)
                continue;

            if(w[nowX+dx[i]][nowY+dy[i]]==0 && canMove(nowX,nowY,nowX+dx[i],nowY+dy[i]))
            {
                w[nowX+dx[i]][nowY+dy[i]] = 1;
                QPoint newPoint;
                newPoint.setX(nowX+dx[i]);
                newPoint.setY(nowY+dy[i]);
                test.enqueue(newPoint);
            }
        }
    }
    return false;
}

void MainWindow::on_sure_clicked()
{
    player_pre_step.clear();
    ai_pre_step.clear();
    if(mystep.count() == 1)
    {
        if(mystep[0].status == 1)
        {
            QMessageBox::warning(this,"waning","每回合需放置两个障碍或者移动一格！！！");
            return;
        }
        else
        {
            STEP playerStep;
            playerStep.x = mychess[1].x;
            playerStep.y = mychess[1].y;
            player_pre_step.append(playerStep);
            mychess[1].x = mystep[0].x;
            mychess[1].y = mystep[0].y;
        }
    }
    else if(mystep.count() == 2)
    {
        if(!canfinish(mychess[0].x,mychess[0].y,1) || !canfinish(mychess[1].x,mychess[1].y,2))
        {
            QMessageBox::warning(this,"warning","您不可以用障碍将路封住！！！");

            littleOBS[mystep[0].kind].status = Blank;
            littleOBS[mystep[0].kind].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");

            littleOBS[mystep[1].kind].status = Blank;
            littleOBS[mystep[1].kind].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");

            int minindex = mystep[0].kind < mystep[1].kind ? mystep[0].kind : mystep[1].kind;
            int smallX = littleOBS[minindex].x;
            int smallY = littleOBS[minindex].y;
            if(minindex < 42)
            {
                mySmall[smallX][smallY].status = Blank;
                mySmall[smallX][smallY].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");
            }
            else if(minindex > 41)
            {
                mySmall[smallY][smallX].status = Blank;
                mySmall[smallY][smallX].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");
            }
            mystep.clear();
            return;
        }
        else
        {
            STEP playerStep1,playerStep2;
            playerStep1.kind = mystep[0].kind;
            playerStep2.kind = mystep[1].kind;
            player_pre_step.append(playerStep1);
            player_pre_step.append(playerStep2);
            mychess[1].obsNum --;
            ui->human_obs->setText(tr("玩家障碍:X%1").arg(mychess[1].obsNum));
        }
    }
    else
    {
        QMessageBox::warning(this,"warning","您还没有行动！！！");
        return;
    }
    mystep.clear();
    myStatus = 0;
    ui->obstacle->setEnabled(true);
    ui->obstacle->setEnabled(true);
    ui->obstacle->setStyleSheet("");
    ui->move->setStyleSheet("");
    for(int i=0;i<7;i++)
    {
        for(int j=0;j<7;j++)
        {
            if(finalBoard[i][j] == Blank)
                finalButton[i][j]->setStyleSheet("");
        }
    }
    int obsNum = 0;
    for(int i=0; i<84; i++)
    {
        if(littleOBS[i].status == Blank)
            littleOBS[i].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");
        else
            obsNum ++;
    }
    //执行ai的算法
    AIdepth = 4;

    int score = findAiMove(AIdepth,999,-999);
    qDebug()<<score;

    if(step_num == 1)
    {
        if(AIstep[0].status == 1)
        {
            QMessageBox::warning(this,"waning","AI故障：每回合需放置两个障碍或者移动一格！！！");
            return;
        }
        else
        {
            STEP aiStep;
            aiStep.x = mychess[0].x;
            aiStep.y = mychess[0].y;
            ai_pre_step.append(aiStep);
            finalButton[mychess[0].x][mychess[0].y]->setStyleSheet("");
            finalBoard[mychess[0].x][mychess[0].y] = Blank;
            mychess[0].x = AIstep[0].x;
            mychess[0].y = AIstep[0].y;
            finalBoard[mychess[0].x][mychess[0].y] = AIchess;
            finalButton[mychess[0].x][mychess[0].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
            if(mychess[0].x == 6)
                QMessageBox::information(this,"菜逼","AI获胜啦!!!");
            step_num = 0;
        }
    }
    else if(step_num == 2)
    {
        if(littleOBS[AIstep[0].kind].status != Blank || littleOBS[AIstep[1].kind].status != Blank)
        {
            QMessageBox::warning(this,"warning","AI故障：向不能放置障碍的地方放置了障碍");
            return;
        }
        else
        {
            littleOBS[AIstep[0].kind].status = Obstacle;
            littleOBS[AIstep[1].kind].status = Obstacle;
            mychess[0].obsNum --;
            ui->ai_obs->setText(tr("AI障碍:X%1").arg(mychess[0].obsNum));

            if(AIstep[0].kind < 42)
            {
                littleOBS[AIstep[0].kind].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
                littleOBS[AIstep[1].kind].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");

                int minindex = AIstep[0].kind < AIstep[1].kind ? AIstep[0].kind : AIstep[1].kind;
                int smallX = littleOBS[minindex].x;
                int smallY = littleOBS[minindex].y;
                mySmall[smallX][smallY].status = Obstacle;
                mySmall[smallX][smallY].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);}");
            }
            else
            {
                littleOBS[AIstep[0].kind].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
                littleOBS[AIstep[1].kind].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");

                int minindex = AIstep[0].kind < AIstep[1].kind ? AIstep[0].kind : AIstep[1].kind;
                int smallX = littleOBS[minindex].x;
                int smallY = littleOBS[minindex].y;
                mySmall[smallY][smallX].status = Obstacle;
                mySmall[smallY][smallX].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);}");
            }
            STEP aiStep1,aiStep2;
            aiStep1.kind = AIstep[0].kind;
            aiStep2.kind = AIstep[1].kind;
            ai_pre_step.append(aiStep1);
            ai_pre_step.append(aiStep2);
            step_num = 0;
        }
    }
    else
    {
        QMessageBox::warning(this,"warning","AI故障：您还没有行动！！！");
        qDebug()<<"stepnum is:"<<step_num;
        return;
    }
}

void MainWindow::on_regret_clicked()
{
    if(mystep.empty())
    {
        QMessageBox::warning(this,"warning","您还没有行动");
        return;
    }

    int i = mystep.count()-1;

    if(mystep[i].status == 1)
    {
        if(mystep.count() == 1)
        {
            littleOBS[mystep[i].kind].status = Blank;
            littleOBS[mystep[i].kind].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");
        }
        else if(mystep.count() == 2)
        {
            littleOBS[mystep[i].kind].status = Blank;
            littleOBS[mystep[i].kind].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");

            int minindex = mystep[0].kind < mystep[1].kind ? mystep[0].kind : mystep[1].kind;
            int smallX = littleOBS[minindex].x;
            int smallY = littleOBS[minindex].y;
            if(minindex < 42)
            {
                mySmall[smallX][smallY].status = Blank;
                mySmall[smallX][smallY].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");
            }
            else if(minindex > 41)
            {
                mySmall[smallY][smallX].status = Blank;
                mySmall[smallY][smallX].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");
            }
        }
    }
    else
    {
        finalBoard[mystep[i].x][mystep[i].y] = Blank;
        finalButton[mystep[i].x][mystep[i].y]->setStyleSheet("");

        finalBoard[mychess[1].x][mychess[1].y] = Playerchess;
        finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("QPushButton{background-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
    }
    mystep.removeLast();
}

bool MainWindow::canSet(int index1, int index2)
{
    if(littleOBS[index1].kind != littleOBS[index2].kind)
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
                mySmall[smallX][smallY].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
                return true;
            }
            else if(minindex > 41 && mySmall[smallY][smallX].status == Blank)
            {
                mySmall[smallY][smallX].status = Obstacle;
                mySmall[smallY][smallX].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
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



void MainWindow::on_obs110_clicked()
{
    int obsNum = 0;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs120_clicked()
{
    int obsNum = 1;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs130_clicked()
{
    int obsNum = 2;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs140_clicked()
{
    int obsNum = 3;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs150_clicked()
{
    int obsNum = 4;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs160_clicked()
{
    int obsNum = 5;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs210_clicked()
{
    int obsNum = 6;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs220_clicked()
{
    int obsNum = 7;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs230_clicked()
{
    int obsNum = 8;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs240_clicked()
{
    int obsNum = 9;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs250_clicked()
{
    int obsNum = 10;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs260_clicked()
{
    int obsNum = 11;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs310_clicked()
{
    int obsNum = 12;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs320_clicked()
{
    int obsNum = 13;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs330_clicked()
{
    int obsNum = 14;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs340_clicked()
{
    int obsNum = 15;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs350_clicked()
{
    int obsNum = 16;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs360_clicked()
{
    int obsNum = 17;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs410_clicked()
{
    int obsNum = 18;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs420_clicked()
{
    int obsNum = 19;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs430_clicked()
{
    int obsNum = 20;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs440_clicked()
{
    int obsNum = 21;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs450_clicked()
{
    int obsNum = 22;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs460_clicked()
{
    int obsNum = 23;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs510_clicked()
{
    int obsNum = 24;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs520_clicked()
{
    int obsNum = 25;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs530_clicked()
{
    int obsNum = 26;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs540_clicked()
{
    int obsNum = 27;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs550_clicked()
{
    int obsNum = 28;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs560_clicked()
{
    int obsNum = 29;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}
void MainWindow::on_obs610_clicked()
{
    int obsNum = 30;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs620_clicked()
{
    int obsNum = 31;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs630_clicked()
{
    int obsNum = 32;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs640_clicked()
{
    int obsNum = 33;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs650_clicked()
{
    int obsNum = 34;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs660_clicked()
{
    int obsNum = 35;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}
void MainWindow::on_obs710_clicked()
{
    int obsNum = 36;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}
void MainWindow::on_obs720_clicked()
{
    int obsNum = 37;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs730_clicked()
{
    int obsNum = 38;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs740_clicked()
{
    int obsNum = 39;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs750_clicked()
{
    int obsNum = 40;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs760_clicked()
{
    int obsNum = 41;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs111_clicked()
{
    int obsNum = 42;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs121_clicked()
{
    int obsNum = 43;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs131_clicked()
{
    int obsNum = 44;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs141_clicked()
{
    int obsNum = 45;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs151_clicked()
{
    int obsNum = 46;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs161_clicked()
{
    int obsNum = 47;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}
void MainWindow::on_obs211_clicked()
{
    int obsNum = 48;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs221_clicked()
{
    int obsNum = 49;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs231_clicked()
{
    int obsNum = 50;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs241_clicked()
{
    int obsNum = 51;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs251_clicked()
{
    int obsNum = 52;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs261_clicked()
{
    int obsNum = 53;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs311_clicked()
{
    int obsNum = 54;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs321_clicked()
{
    int obsNum = 55;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs331_clicked()
{
    int obsNum = 56;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs341_clicked()
{
    int obsNum = 57;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs351_clicked()
{
    int obsNum = 58;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs361_clicked()
{
    int obsNum = 59;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs411_clicked()
{
    int obsNum = 60;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs421_clicked()
{
    int obsNum = 61;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs431_clicked()
{
    int obsNum = 62;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs441_clicked()
{
    int obsNum = 63;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs451_clicked()
{
    int obsNum = 64;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs461_clicked()
{
    int obsNum = 65;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs511_clicked()
{
    int obsNum = 66;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs521_clicked()
{
    int obsNum = 67;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs531_clicked()
{
    int obsNum = 68;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs541_clicked()
{
    int obsNum = 69;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs551_clicked()
{
    int obsNum = 70;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs561_clicked()
{
    int obsNum = 71;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs611_clicked()
{
    int obsNum = 72;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs621_clicked()
{
    int obsNum = 73;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs631_clicked()
{
    int obsNum = 74;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs641_clicked()
{
    int obsNum = 75;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs651_clicked()
{
    int obsNum = 76;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs661_clicked()
{
    int obsNum = 77;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs711_clicked()
{
    int obsNum = 78;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs721_clicked()
{
    int obsNum = 79;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs731_clicked()
{
    int obsNum = 80;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs741_clicked()
{
    int obsNum = 81;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs751_clicked()
{
    int obsNum = 82;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_obs761_clicked()
{
    int obsNum = 83;
    if(myStatus == 1)//现在的动作是要放置障碍
    {
        if(mystep.count() >= 2)
        {
            QMessageBox::warning(this,"warning","您已经放置了两个障碍，别再放了！！！");
            return;
        }
        else if(mystep.count() == 1)
        {
            if(mystep[0].status == 2)
            {
                QMessageBox::warning(this,"warning","您已经移动过棋子了，不能再放置障碍！！！");
                return;
            }
            else if(mystep[0].status == 1)
            {
                if(!canSet(mystep[0].kind,obsNum))
                {
                    QMessageBox::warning(this,"warning","每次只能放置相邻的障碍！！！");
                    return;
                }
            }
        }

        if(littleOBS[obsNum].status == Blank)
        {
            if(littleOBS[obsNum].kind == 0)
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/竖栅栏.png);border:1px solid #ababab;}");
            else
                littleOBS[obsNum].btn->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/横栅栏.png);border:1px solid #ababab;}");
            littleOBS[obsNum].status = Obstacle;
            STEP newstep;
            newstep.status = 1;
            newstep.x = -1;
            newstep.y = -1;
            newstep.kind = obsNum;
            mystep.append(newstep);
        }
        else
            QMessageBox::warning(this,"warning","这里不能摆放障碍");
    }
}

void MainWindow::on_big_regret_clicked()
{
    if(player_pre_step.count() == 1)//上一步玩家选择了移动棋子
    {
        finalBoard[mychess[1].x][mychess[1].y] = Blank;
        finalBoard[player_pre_step[0].x][player_pre_step[0].y] = Playerchess;
        finalButton[mychess[1].x][mychess[1].y]->setStyleSheet("");
        finalButton[player_pre_step[0].x][player_pre_step[0].y]->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/玩家棋子.png);border:1px solid #ababab;}");
        mychess[1].x = player_pre_step[0].x;
        mychess[1].y = player_pre_step[0].y;
    }
    else if(player_pre_step.count() == 2)//玩家选择了放置障碍
    {
        littleOBS[player_pre_step[0].kind].status = Blank;
        littleOBS[player_pre_step[1].kind].status = Blank;
        littleOBS[player_pre_step[0].kind].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");
        littleOBS[player_pre_step[1].kind].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");

        int minindex = player_pre_step[0].kind < player_pre_step[1].kind ? player_pre_step[0].kind : player_pre_step[1].kind;
        int smallX = littleOBS[minindex].x;
        int smallY = littleOBS[minindex].y;
        if(minindex < 42)
        {
            mySmall[smallX][smallY].status = Blank;
            mySmall[smallX][smallY].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");
        }
        else if(minindex > 41)
        {
            mySmall[smallY][smallX].status = Blank;
            mySmall[smallY][smallX].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");
        }
        mychess[1].obsNum ++;
        ui->human_obs->setText(tr("玩家障碍:X%1").arg(mychess[1].obsNum));
    }
    else
        qDebug()<<"error:"<<player_pre_step.count();

    if(ai_pre_step.count() == 1)//上一步ai选择了移动棋子
    {
        finalBoard[mychess[0].x][mychess[0].y] = Blank;
        finalBoard[ai_pre_step[0].x][ai_pre_step[0].y] = AIchess;
        finalButton[mychess[0].x][mychess[0].y]->setStyleSheet("");
        finalButton[ai_pre_step[0].x][ai_pre_step[0].y]->setStyleSheet("QPushButton{border-image: url(:/build-smartCar-Desktop_Qt_5_8_0_MinGW_32bit-Debug/ai棋子.png);border:1px solid #ababab;}");
        mychess[0].x = ai_pre_step[0].x;
        mychess[0].y = ai_pre_step[0].y;
    }
    else if(ai_pre_step.count() == 2)//玩家选择了放置障碍
    {
        littleOBS[ai_pre_step[0].kind].status = Blank;
        littleOBS[ai_pre_step[1].kind].status = Blank;
        littleOBS[ai_pre_step[0].kind].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");
        littleOBS[ai_pre_step[1].kind].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");

        int minindex = ai_pre_step[0].kind < ai_pre_step[1].kind ? ai_pre_step[0].kind :ai_pre_step[1].kind;
        int smallX = littleOBS[minindex].x;
        int smallY = littleOBS[minindex].y;
        if(minindex < 42)
        {
            mySmall[smallX][smallY].status = Blank;
            mySmall[smallX][smallY].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");
        }
        else if(minindex > 41)
        {
            mySmall[smallY][smallX].status = Blank;
            mySmall[smallY][smallX].btn->setStyleSheet("QPushButton{border : none;background-color : white;}");
        }
        mychess[0].obsNum ++;
        ui->ai_obs->setText(tr("AI障碍:X%1").arg(mychess[0].obsNum));
    }
    else
        qDebug()<<"error:"<<ai_pre_step.count();
}
