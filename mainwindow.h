#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QByteArray>
#include <string>
#include <QVector>
#include <QPushButton>
#include <QPoint>

#define Blank       0
#define AIchess     1
#define Playerchess 2
#define Obstacle    3
#define Ending      4

class QSerialPort;
/**************以下是结构体的定义****************/
typedef struct
{
    int x;//横坐标
    int y;//纵坐标
    int kind;//如果是障碍的话，就记录是第几个障碍
    int status;//1表示这是一步障碍，2表示这是一步棋子
}STEP;//用来记录玩家每一步的操作

typedef struct
{
    int x;//横坐标
    int y;//纵坐标
    int distance;//找出该棋子目前完成比赛所需的最步数
}CHESS;//用来记录当前两个棋子的信息,0是ai，1是玩家

/**************以上是结构体的定义****************/


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void board_init();//对预赛和决赛的棋盘初始化，按钮的配置

/*************以下是预赛用到的函数*****************/


/*************以下是决赛用到的函数*****************/

    static bool canfinish(int x,int y, int kind);//x,y表示棋子当前的坐标，kind为1表示是ai的棋子，为2表示是玩家的棋子
    static bool canMove(int x0,int y0,int x1,int y1);//判断移动棋子的时候，是否可以从当前点移动到目标点
    bool canSet(int index1, int index2);//判断放置障碍的时候，是否可以放置当前障碍

private slots:
    void myaction();

    void on_btn11_clicked();

    void on_btn12_clicked();

    void on_btn13_clicked();

    void on_btn14_clicked();

    void on_btn15_clicked();

    void on_btn16_clicked();

    void on_btn17_clicked();

    void on_btn18_clicked();

    void on_btn21_clicked();

    void on_btn22_clicked();

    void on_btn23_clicked();

    void on_btn24_clicked();

    void on_btn25_clicked();

    void on_btn26_clicked();

    void on_btn27_clicked();

    void on_btn28_clicked();

    void on_btn31_clicked();

    void on_btn32_clicked();

    void on_btn33_clicked();

    void on_btn34_clicked();

    void on_btn35_clicked();

    void on_btn36_clicked();

    void on_btn37_clicked();

    void on_btn38_clicked();

    void on_btn41_clicked();

    void on_btn42_clicked();

    void on_btn43_clicked();

    void on_btn44_clicked();

    void on_btn45_clicked();

    void on_btn46_clicked();

    void on_btn47_clicked();

    void on_btn48_clicked();

    void on_btn51_clicked();

    void on_btn52_clicked();

    void on_btn53_clicked();

    void on_btn54_clicked();

    void on_btn55_clicked();

    void on_btn56_clicked();

    void on_btn57_clicked();

    void on_btn58_clicked();

    void on_btn61_clicked();

    void on_btn62_clicked();

    void on_btn63_clicked();

    void on_btn64_clicked();

    void on_btn65_clicked();

    void on_btn66_clicked();

    void on_btn67_clicked();

    void on_btn68_clicked();

    void on_btn71_clicked();

    void on_btn72_clicked();

    void on_btn73_clicked();

    void on_btn74_clicked();

    void on_btn75_clicked();

    void on_btn76_clicked();

    void on_btn77_clicked();

    void on_btn78_clicked();

    void on_btn81_clicked();

    void on_btn82_clicked();

    void on_btn83_clicked();

    void on_btn84_clicked();

    void on_btn85_clicked();

    void on_btn86_clicked();

    void on_btn87_clicked();

    void on_btn88_clicked();

    void on_connect_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_obstacle_clicked();

    void on_move_clicked();


    void on_gamestart_clicked();

    void on_btn11_2_clicked();

    void on_btn12_2_clicked();

    void on_btn13_2_clicked();

    void on_btn14_2_clicked();

    void on_btn15_2_clicked();

    void on_btn16_2_clicked();

    void on_btn17_2_clicked();

    void on_btn18_2_clicked();

    void on_btn81_2_clicked();

    void on_btn82_2_clicked();

    void on_btn83_2_clicked();

    void on_btn84_2_clicked();

    void on_btn85_2_clicked();

    void on_btn86_2_clicked();

    void on_btn87_2_clicked();

    void on_btn88_2_clicked();

    void on_btn71_2_clicked();

    void on_btn72_2_clicked();

    void on_btn73_2_clicked();

    void on_btn74_2_clicked();

    void on_btn75_2_clicked();

    void on_btn76_2_clicked();

    void on_btn77_2_clicked();

    void on_btn78_2_clicked();

    void on_btn61_2_clicked();

    void on_btn62_2_clicked();

    void on_btn63_2_clicked();

    void on_btn64_2_clicked();

    void on_btn65_2_clicked();

    void on_btn66_2_clicked();

    void on_btn67_2_clicked();

    void on_btn68_2_clicked();

    void on_btn51_2_clicked();

    void on_btn52_2_clicked();

    void on_btn53_2_clicked();

    void on_btn54_2_clicked();

    void on_btn55_2_clicked();

    void on_btn56_2_clicked();

    void on_btn57_2_clicked();

    void on_btn58_2_clicked();

    void on_btn41_2_clicked();

    void on_btn42_2_clicked();

    void on_btn43_2_clicked();

    void on_btn44_2_clicked();

    void on_btn45_2_clicked();

    void on_btn46_2_clicked();

    void on_btn47_2_clicked();

    void on_btn48_2_clicked();

    void on_btn31_2_clicked();

    void on_btn32_2_clicked();

    void on_btn33_2_clicked();

    void on_btn34_2_clicked();

    void on_btn35_2_clicked();

    void on_btn36_2_clicked();

    void on_btn37_2_clicked();

    void on_btn38_2_clicked();

    void on_btn21_2_clicked();

    void on_btn22_2_clicked();

    void on_btn23_2_clicked();

    void on_btn24_2_clicked();

    void on_btn25_2_clicked();

    void on_btn26_2_clicked();

    void on_btn27_2_clicked();

    void on_btn28_2_clicked();

    void on_sure_clicked();

    void on_regret_clicked();

    void on_obs110_clicked();

    void on_obs120_clicked();

    void on_obs130_clicked();

    void on_obs140_clicked();

    void on_obs150_clicked();

    void on_obs160_clicked();

    void on_obs170_clicked();

    void on_obs210_clicked();

    void on_obs220_clicked();

    void on_obs230_clicked();

    void on_obs240_clicked();

    void on_obs250_clicked();

    void on_obs260_clicked();

    void on_obs270_clicked();

    void on_obs310_clicked();

    void on_obs320_clicked();

    void on_obs330_clicked();

    void on_obs340_clicked();

    void on_obs350_clicked();

    void on_obs360_clicked();

    void on_obs370_clicked();

    void on_obs410_clicked();

    void on_obs420_clicked();

    void on_obs430_clicked();

    void on_obs440_clicked();

    void on_obs450_clicked();

    void on_obs460_clicked();

    void on_obs470_clicked();

    void on_obs510_clicked();

    void on_obs520_clicked();

    void on_obs530_clicked();

    void on_obs540_clicked();

    void on_obs550_clicked();

    void on_obs560_clicked();

    void on_obs570_clicked();

    void on_obs610_clicked();

    void on_obs620_clicked();

    void on_obs630_clicked();

    void on_obs640_clicked();

    void on_obs650_clicked();

    void on_obs660_clicked();

    void on_obs670_clicked();

    void on_obs710_clicked();

    void on_obs720_clicked();

    void on_obs730_clicked();

    void on_obs740_clicked();

    void on_obs750_clicked();

    void on_obs760_clicked();

    void on_obs770_clicked();

    void on_obs810_clicked();

    void on_obs820_clicked();

    void on_obs830_clicked();

    void on_obs840_clicked();

    void on_obs850_clicked();

    void on_obs860_clicked();

    void on_obs870_clicked();

    void on_obs111_clicked();

    void on_obs121_clicked();

    void on_obs131_clicked();

    void on_obs141_clicked();

    void on_obs151_clicked();

    void on_obs161_clicked();

    void on_obs171_clicked();

    void on_obs211_clicked();

    void on_obs221_clicked();

    void on_obs231_clicked();

    void on_obs241_clicked();

    void on_obs251_clicked();

    void on_obs261_clicked();

    void on_obs271_clicked();

    void on_obs311_clicked();

    void on_obs321_clicked();

    void on_obs331_clicked();

    void on_obs341_clicked();

    void on_obs351_clicked();

    void on_obs361_clicked();

    void on_obs371_clicked();

    void on_obs411_clicked();

    void on_obs421_clicked();

    void on_obs431_clicked();

    void on_obs441_clicked();

    void on_obs451_clicked();

    void on_obs461_clicked();

    void on_obs471_clicked();

    void on_obs511_clicked();

    void on_obs521_clicked();

    void on_obs531_clicked();

    void on_obs541_clicked();

    void on_obs551_clicked();

    void on_obs561_clicked();

    void on_obs571_clicked();

    void on_obs611_clicked();

    void on_obs621_clicked();

    void on_obs631_clicked();

    void on_obs641_clicked();

    void on_obs651_clicked();

    void on_obs661_clicked();

    void on_obs671_clicked();

    void on_obs711_clicked();

    void on_obs721_clicked();

    void on_obs731_clicked();

    void on_obs741_clicked();

    void on_obs751_clicked();

    void on_obs761_clicked();

    void on_obs771_clicked();

    void on_obs811_clicked();

    void on_obs821_clicked();

    void on_obs831_clicked();

    void on_obs841_clicked();

    void on_obs851_clicked();

    void on_obs861_clicked();

    void on_obs871_clicked();

    void on_big_regret_clicked();

private:
    Ui::MainWindow *ui;
/*******************以下是设置串口用到变量*********************/
    QSerialPort *serialport;
    QByteArray mess;
    QString msg;

/*******************以下是预赛用到的变量*********************/

    QPushButton *myButton[8][8];//预赛用的棋盘按钮
    int chessNum = 0;//裁判摆放棋子时的计数，不得超过八个

/*******************以下是决赛用到的变量*********************/
    QPushButton *finalButton[8][8];//决赛用的棋盘按钮
    int myStatus = 0;//表示当前的操作是要放障碍还是移动，0表示什么也没做，1表示准备放下一个障碍，2表示准备移动一步
    int whoFirst = 0;//0表示ai先手，1表示玩家先手
    QVector<STEP> mystep;//记录本回合的操作
    QVector<STEP> ai_pre_step;//记录上回合的操作，用来赖皮
    QVector<STEP> player_pre_step;//记录上回合的操作，用来赖皮
    int prescore = 0;//上一次的MTDF值

};

#endif // MAINWINDOW_H
