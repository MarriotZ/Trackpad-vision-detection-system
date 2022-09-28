#include "plc_camera.h"

PLC_camera::PLC_camera(QObject *parent) : QThread(parent)
{
    //PLC
    m_Eip = new YtPlc_Keyence_Eip();
    PLCip = "192.168.0.10";
    eip_port = 8501;
}

PLC_camera::~PLC_camera(){
    requestInterruption();
    PLC_Connect_Close();
    quit();
    wait();
}

void PLC_camera::run(){
    PLC_Connect_Open();
    runCycl();
}



bool PLC_camera::PLC_Connect_Open()
{
    timer_workUint3=new QTimer();
    timer_workUint3->setSingleShot(true);
    timer_workUint5=new QTimer();
    timer_workUint5->setSingleShot(true);
    connect(timer_workUint3,SIGNAL(timeout()),this,SLOT(workUnit3_timeout()));
    WorkUnit3_GrapImage_Command =0;
    WorkUnit3_GrapImage_Status  =0;

    WorkUnit4_GrapImage_Command =0;
    WorkUnit4_GrapImage_Status  =0;

    WorkUnit5_GrapImage_Command =0;
    WorkUnit5_GrapImage_Status  =0;

    WorkUnit6_GrapImage_Command =0;
    WorkUnit6_GrapImage_Status  =0;

    WorkUnit7_GrapImage_Command =0;
    WorkUnit7_GrapImage_Status  =0;

    WorkUnit8_GrapImage_Command =0;
    WorkUnit8_GrapImage_Status  =0;

    PLCip = "192.168.0.10";
    eip_port = 8501;

    YtPlc::Startup();
    return m_Eip->Open(PLCip.toStdString().c_str(), eip_port);
}

bool PLC_camera::PLC_Connect_Close()
{
    m_Eip->Close();

    YtPlc::Cleanup();


    return true;
}

void PLC_camera::runCycl()
{
    //相机
    int data1[12]{0};
    //扫码枪
    int size = 0;
    int writedata[2]{0};
    while(!isInterruptionRequested()){
        m_Eip->PLCRead((int)e_plc_code_dm_keyence_eip, adr_WorkUnit3_GrapImage_Command, 12, data1, size);
        WorkUnit3_GrapImage_Command =data1[0];
        WorkUnit4_GrapImage_Command =data1[2];
        WorkUnit5_GrapImage_Command =data1[4];
        WorkUnit6_GrapImage_Command =data1[6];
        WorkUnit7_GrapImage_Command =data1[8];
        WorkUnit8_GrapImage_Command =data1[10];

//        if(WorkUnit3_GrapImage_Command ==0)
//        {
//            timer_workUint3->stop();
//            WorkUnit3_GrapImage_Status  =0;
//            writedata[0] = WorkUnit3_GrapImage_Status;
//            m_Eip->PLCWrite((int)e_plc_code_dm_keyence_eip, writedata, 2, adr_WorkUnit3_GrapImage_Status);
//        }
//        else if(WorkUnit3_GrapImage_Command > WorkUnit3_GrapImage_Status )
//        {
//            timer_workUint3->stop();
//            if(WorkUnit3_GrapImage_Command==10){
//                //切光
//                emit station1_type(1);
//                timer_workUint3->start(3);
//            }
//            else if(WorkUnit3_GrapImage_Command==20){
//                //切光
//                emit station1_type(2);
//                timer_workUint3->start(3);
//            }
//            WorkUnit3_GrapImage_Status  =WorkUnit3_GrapImage_Command;
//            writedata[0] = WorkUnit3_GrapImage_Status;
// //            if (!workUnit3_light) {
// //                m_Eip->PLCWrite((int)e_plc_code_dm_keyence_eip, writedata, 2, adr_WorkUnit3_GrapImage_Status);
// //                workUnit3_light=0;
// //            }

//        }
//        else if(WorkUnit3_GrapImage_Command == WorkUnit3_GrapImage_Status && WorkUnit3_GrapImage_Command > 0){
// //            if (!workUnit3_light) {
// //                m_Eip->PLCWrite((int)e_plc_code_dm_keyence_eip, writedata, 2, adr_WorkUnit3_GrapImage_Status);
// //                workUnit3_light=0;
// //            }
//        }


//        if(WorkUnit4_GrapImage_Command ==0)
//        {
//            WorkUnit4_GrapImage_Status  =0;
//            writedata[0] = WorkUnit4_GrapImage_Status;
//            m_Eip->PLCWrite((int)e_plc_code_dm_keyence_eip, writedata, 2, adr_WorkUnit4_GrapImage_Status);
//        }
//        else if(WorkUnit4_GrapImage_Command > WorkUnit4_GrapImage_Status){
//            WorkUnit4_GrapImage_Status  =WorkUnit4_GrapImage_Command;
//            writedata[0] = WorkUnit4_GrapImage_Status;
//            m_Eip->PLCWrite((int)e_plc_code_dm_keyence_eip, writedata, 2, adr_WorkUnit4_GrapImage_Status);
//        }



//        if(WorkUnit5_GrapImage_Command ==0)
//        {
//            timer_workUint5->stop();
//            WorkUnit5_GrapImage_Status  =0;
//            writedata[0] = WorkUnit5_GrapImage_Status;
//            m_Eip->PLCWrite((int)e_plc_code_dm_keyence_eip, writedata, 2, adr_WorkUnit5_GrapImage_Status);
//        }
//        else if(WorkUnit5_GrapImage_Command > WorkUnit5_GrapImage_Status){
//            timer_workUint5->stop();
//            if(WorkUnit5_GrapImage_Command==10){
//                //切光
//                emit station3_type(1);
//                timer_workUint5->start(3);
//            }
//            WorkUnit5_GrapImage_Status  =WorkUnit5_GrapImage_Command;
//            writedata[0] = WorkUnit5_GrapImage_Status;
//            m_Eip->PLCWrite((int)e_plc_code_dm_keyence_eip, writedata, 2, adr_WorkUnit5_GrapImage_Status);
//        }

//        if(WorkUnit6_GrapImage_Command ==0)
//        {
//            WorkUnit6_GrapImage_Status  =0;
//            writedata[0] = WorkUnit6_GrapImage_Status;
//            m_Eip->PLCWrite((int)e_plc_code_dm_keyence_eip, writedata, 2, adr_WorkUnit6_GrapImage_Status);
//        }
//        else if(WorkUnit6_GrapImage_Command > WorkUnit6_GrapImage_Status){
//            WorkUnit6_GrapImage_Status  =WorkUnit6_GrapImage_Command;
//            writedata[0] = WorkUnit6_GrapImage_Status;
//            m_Eip->PLCWrite((int)e_plc_code_dm_keyence_eip, writedata, 2, adr_WorkUnit6_GrapImage_Status);
//        }

//        if(WorkUnit7_GrapImage_Command ==0)
//        {
//            WorkUnit7_GrapImage_Status  =0;
//            writedata[0] = WorkUnit7_GrapImage_Status;
//            m_Eip->PLCWrite((int)e_plc_code_dm_keyence_eip, writedata, 2, adr_WorkUnit7_GrapImage_Status);
//        }
//        else if(WorkUnit7_GrapImage_Command > WorkUnit7_GrapImage_Status){
//            WorkUnit7_GrapImage_Status  =WorkUnit7_GrapImage_Command;
//            writedata[0] = WorkUnit7_GrapImage_Status;
//            m_Eip->PLCWrite((int)e_plc_code_dm_keyence_eip, writedata, 2, adr_WorkUnit7_GrapImage_Status);
//        }


//        if(WorkUnit8_GrapImage_Command ==0)
//        {
//            WorkUnit8_GrapImage_Status  =0;
//            writedata[0] = WorkUnit8_GrapImage_Status;
//            m_Eip->PLCWrite((int)e_plc_code_dm_keyence_eip, writedata, 2, adr_WorkUnit8_GrapImage_Status);
//        }
//        else if(WorkUnit8_GrapImage_Command > WorkUnit8_GrapImage_Status )
//        {

//             WorkUnit8_GrapImage_Status  =WorkUnit8_GrapImage_Command;
//             writedata[0] = WorkUnit8_GrapImage_Status;
//             m_Eip->PLCWrite((int)e_plc_code_dm_keyence_eip, writedata, 2, adr_WorkUnit8_GrapImage_Status);

//        }

        ////////////////////////////////////////////////

        EStopStatus = m_Eip->getEStopStatus();

        WorkingPointOneNum = m_Eip->getWorkingPointOne_Number();

        PLCScanBarcode =  m_Eip->getPLCBarcodeStatus();
        if(PLCScanBarcode==1){
            emit scan();
        }

        ScanlineStatus =  m_Eip->getScanlineStatus();

        PLCStatus =  m_Eip->getPLCStatus();

        msleep(50);
        QCoreApplication::processEvents();

    }

}

void PLC_camera::workUnit3_timeout()
{
    m_Eip->setGrapImageStatus(3,WorkUnit3_GrapImage_Command+2);
}
