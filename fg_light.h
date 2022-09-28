#ifndef FG_LIGHT_H
#define FG_LIGHT_H
#include<QSettings>
#include<QSerialPort>


class FG_light
{
public:
    FG_light();

    void open_cam(int workUnit,bool open_cam1,bool open_cam2,bool open_cam3,bool open_cam4);

    void close_cam(int workUnit);


    void open(QString comNum);

    void close(QString comNum);

private:
    QSettings *settings=new QSettings("./setting.ini", QSettings::IniFormat);


    QSerialPort *global_port;
};


#endif // FG_LIGHT_H
