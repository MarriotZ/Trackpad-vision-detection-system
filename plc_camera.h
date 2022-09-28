#ifndef PLC_CAMERA_H
#define PLC_CAMERA_H

#include <QThread>
#include <QObject>
#include<QDebug>
#include<ytplc.h>
#include <ytplc_keyence_eip.h>
#include<QCoreApplication>
#include<QTimer>
#include"G_Data.h"







//typedef enum
//{

//    EStopClose =0,
//    EStopOpen =1

//}TYP_ESTOP_STATUS;


//typedef enum
//{
//    DetectResultOK =1,
//    DetectResultNG =2
//}TYP_DETECTRESULT;



//typedef enum
//{
//    PLCReseting =0,
//    StartScanBarcode =1,
//    CompleteScanBarcode =2,
//    FailedgetBarcode  =3,
//    MESSetNG =4,
//}TYP_PLCBARCODESTATUS;


//typedef enum
//{
//    FirstCompleteScanline =10,
//    SecondCompleteScanline =20
//}TYP_SCANLINESTATUS;

//typedef enum
//{
//    PLCInit =0,
//    PLCAutoRun =1,
//    PLCMannulMode =2
//}TYP_PLCSTATUS;

class PLC_camera : public QThread
{
    Q_OBJECT
public:
    explicit PLC_camera(QObject *parent = nullptr);
    ~PLC_camera();

    void run();

    ////add by David

public:

//    int WorkUnit3_GrapImage_Command;
//    int WorkUnit3_GrapImage_Status ;

//    int WorkUnit4_GrapImage_Command ;
//    int WorkUnit4_GrapImage_Status ;

//    int WorkUnit5_GrapImage_Command ;
//    int WorkUnit5_GrapImage_Status ;

//    int WorkUnit6_GrapImage_Command ;
//    int WorkUnit6_GrapImage_Status ;

//    int WorkUnit7_GrapImage_Command ;
//    int WorkUnit7_GrapImage_Status ;

//    int WorkUnit8_GrapImage_Command ;
//    int WorkUnit8_GrapImage_Status ;

//    int WorkingPointOneNum    ;
//    int EStopStatus           ;
//    int DectectResult      ;

//    int PLCScanBarcode    ;
//    int ScanlineStatus    ;
//    int PLCStatus         ;

//    int workUnit3_light=0;

public:

    bool PLC_Connect_Open();
    bool PLC_Connect_Close();

    void runCycl();

private slots:
    void workUnit3_timeout();

signals:
    void station1_type(int);

    void station3_type(int);

    void scan();

private:
    YtPlc_Keyence_Eip   *m_Eip;
    int eip_port;
    QString PLCip;
    QTimer *timer_workUint3;
    QTimer *timer_workUint5;

};

#endif // PLC_CAMERA_H
