#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include<QObject>
#include<QDebug>
#include <QWidget>
#include "HalconCpp.h"
#include "HDevThread.h"
#include<QFileDialog>
#include<QMainWindow>
#include<QDateTime>
#include<QTextCodec>
#include<QTimer>
#include"workpiece.h"

using namespace HalconCpp;

class myThread:public QThread
{
    Q_OBJECT
public:
    explicit myThread(QObject *parent = nullptr);
    ~myThread();
    void run();
signals:
    void setMatchImg1(HObject);
    void setRes1(HObject,int,workpiece);
    void setMatchImg2(HObject);
    void setRes2(HObject,int,workpiece);

    void setRes3(HObject,int,workpiece);
    void noFile(QString);
private slots:
    void getImg1(HObject,int);
    void getImg2(HObject,int);
    void getImg3(HObject,int);
private:
    HObject ho_Image;
    int type=0;
    int index;
    workpiece piece;
};

#endif // MYTHREAD_H
