#include "fg_light.h"

FG_light::FG_light()
{
    global_port=new QSerialPort();
}

void FG_light::open_cam(int workUnit, bool open_cam1, bool open_cam2, bool open_cam3, bool open_cam4)
{
    QString cam1,cam2,cam3,cam4;
    QString res;
    if(open_cam1)
        cam1="T";
    else
        cam1="F";
    if(open_cam2)
        cam2="T";
    else
        cam2="F";
    if(open_cam3)
        cam3="T";
    else
        cam3="F";
    if(open_cam4)
        cam4="TC#";
    else
        cam4="FC#";
    switch (workUnit) {
    case 3:
    {
        res="S"+settings->value("fg21").toString()+cam1+settings->value("fg22").toString()+cam2
                      +settings->value("fg23").toString()+cam3+settings->value("fg24").toString()+cam4;
    }
        break;
    case 4:
    {
        res="S"+settings->value("fg31").toString()+cam1+settings->value("fg32").toString()+cam2
              +settings->value("fg33").toString()+cam3+settings->value("fg34").toString()+cam4;

    }
        break;
    case 5:
    {
        res="S"+settings->value("fg11").toString()+cam1+settings->value("fg12").toString()+cam2
              +settings->value("fg13").toString()+cam3+settings->value("fg14").toString()+cam4;
    }
        break;
    case 6:
    {
        res="S"+settings->value("fg41").toString()+cam1+settings->value("fg42").toString()+cam2
              +settings->value("fg43").toString()+cam3+settings->value("fg44").toString()+cam4;
    }
        break;
    default:
        res="";
        break;
    }
    QByteArray byte = res.toUtf8();
    global_port->write(byte);
    return ;
}

void FG_light::close_cam(int workUnit)
{
    QString res;
    switch (workUnit) {
    case 3:
    {
        res="S"+settings->value("fg21").toString()+"F"+settings->value("fg22").toString()+"F"
              +settings->value("fg23").toString()+"F"+settings->value("fg24").toString()+"F";
    }
    break;
    case 4:
    {
        res="S"+settings->value("fg31").toString()+"F"+settings->value("fg32").toString()+"F"
              +settings->value("fg33").toString()+"F"+settings->value("fg34").toString()+"F";

    }
    break;
    case 5:
    {
        res="S"+settings->value("fg11").toString()+"F"+settings->value("fg12").toString()+"F"
              +settings->value("fg13").toString()+"F"+settings->value("fg14").toString()+"F";
    }
    break;
    case 6:
    {
        res="S"+settings->value("fg41").toString()+"F"+settings->value("fg42").toString()+"F"
              +settings->value("fg43").toString()+"F"+settings->value("fg44").toString()+"F";
    }
    break;
    default:
        res="";
        break;
    }
    QByteArray byte = res.toUtf8();
    global_port->write(byte);
    return ;
}

void FG_light::open(QString comNum)
{
    global_port->setParity(QSerialPort::NoParity);
    global_port->setDataBits(QSerialPort::Data8);
    global_port->setStopBits(QSerialPort::OneStop);
    global_port->setBaudRate(19200);
    global_port->setPortName(comNum);
    global_port->open(QIODevice::ReadWrite);
}

void FG_light::close(QString comNum)
{
    global_port->close();
}
