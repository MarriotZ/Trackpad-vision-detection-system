#ifndef G_DATA_H
#define G_DATA_H

#include"opencv2/opencv.hpp"
#include<QTimer>
#include<ytplc.h>
#include <ytplc_keyence_eip.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include"opencv2/opencv.hpp"

#ifndef __APPLE__
#  include "HalconCpp.h"
#  include "HDevThread.h"
#  if defined(__linux__) && !defined(__arm__) && !defined(NO_EXPORT_APP_MAIN)
#    include <X11/Xlib.h>
#  endif
#else
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#    include <HALCONCpp/HDevThread.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#    include <HALCONCppxl/HDevThread.h>
#  endif
#  include <stdio.h>
#  include <HALCON/HpThread.h>
#  include <CoreFoundation/CFRunLoop.h>
#endif

using namespace HalconCpp;
using namespace std;
using namespace cv;



extern  YtPlc_Keyence_Eip   *m_Eip;


extern volatile int WorkUnit3_GrapImage_Command;
extern volatile int WorkUnit3_GrapImage_Status ;
extern volatile bool WorkUnit3_GrapImage_Result ;

extern volatile int WorkUnit4_GrapImage_Command ;
extern volatile int WorkUnit4_GrapImage_Status ;
extern volatile bool WorkUnit4_GrapImage_Result ;

extern  volatile int WorkUnit5_GrapImage_Command ;
extern volatile int WorkUnit5_GrapImage_Status ;
extern volatile bool WorkUnit5_GrapImage_Result ;

extern volatile int WorkUnit6_GrapImage_Command ;
extern volatile int WorkUnit6_GrapImage_Status ;
extern volatile bool WorkUnit6_GrapImage_Result ;

extern volatile int WorkUnit7_GrapImage_Command ;
extern volatile int WorkUnit7_GrapImage_Status ;
extern volatile bool WorkUnit7_GrapImage_Result ;

extern volatile int WorkUnit8_GrapImage_Command ;
extern volatile int WorkUnit8_GrapImage_Status ;
extern volatile bool WorkUnit8_GrapImage_Result ;

extern volatile int WorkingPointOneNum    ;
extern volatile int EStopStatus           ;
extern volatile int DectectResult      ;

extern volatile int PLCScanBarcode    ;
extern volatile int ScanlineStatus    ;
extern volatile int PLCStatus         ;


extern  HObject Unit3_GrapImage;
extern  HTuple  hv_WindowHandle_3_1,hv_WindowHandle_3_2;







#endif // G_DATA_H
