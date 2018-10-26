#include "widget.h"
#include "ui_widget.h"
#include "Windows.h"
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QRegExp>
#include <QRegExpValidator>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    myTimer = new QTimer(this);
    //刷新按钮 信号槽
    connect(ui->beginButton,&QPushButton::clicked,this,&Widget::beginRefresh);
    //停止按钮 信号槽
    connect(ui->stopButton,&QPushButton::clicked,this,&Widget::stopRefresh);
}

void Widget::beginRefresh(){
    QString winTitle = ui->htmlWinTitle->text();
    qDebug() << winTitle;
    char* ch = winTitle.toUtf8().data();
    //我打开的这个网页：https://blog.csdn.net/qq_29542611
    //刷新网页窗口的标题：【无欲则刚】30岁回农村 - CSDN博客 - 2345加速浏览器 9.5
    qDebug() << ch;
    //这里必须转换！不转换会报错: cannot convert 'char*' to 'LPCWSTR {aka const wchar_t*}' for argument '2' to 'HWND__* FindWindowW(LPCWSTR, LPCWSTR)'
    LPCWSTR str = (LPCWSTR)ch;
    //需要Windows.h头文件
    HWND win = ::FindWindow(NULL, str);

    if(win != NULL){
        qDebug() << "找到窗口："+winTitle;
    }else{
        qDebug() << "没有找到窗口!!!";
        return;
    }


    if(myTimer->timerId()!=-1){
        //定时器在运行，先关掉
        myTimer->stop();
    }

    QString intervalStr = ui->lineEdit->text();
    QRegExp reg("[1-9]");
    if( reg.exactMatch(intervalStr)){
        int interval = intervalStr.toInt();
        //每interval秒刷新一次网页
        qDebug() << "间隔时间"<< interval;
        myTimer->start(interval*1000);
    }else{
        qDebug() << "时间间隔请输入1-9的数字";
        return;
    }

    refresh(win);
    //设置定时器
    connect(myTimer,&QTimer::timeout,[=](){
        qDebug() << "执行定时器";
        refresh(win);
    });

}
void Widget::stopRefresh(){
   myTimer->stop();
}

void Widget::refresh(HWND win){
    //将指定的窗口设置到最前面并处于激活状态，不然当没有在最前面是不会刷新的。
    SetForegroundWindow(win);
    PostMessage(win, WM_KEYDOWN, VK_F5,1);
    Sleep(50);
    PostMessage(win, WM_KEYUP, VK_F5,1);
}

Widget::~Widget()
{
    delete ui;
}
