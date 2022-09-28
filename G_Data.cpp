#include "G_Data.h"



  YtPlc_Keyence_Eip   *m_Eip =NULL;


volatile int WorkUnit3_GrapImage_Command =0;
volatile int WorkUnit3_GrapImage_Status =0;
volatile bool WorkUnit3_GrapImage_Result  =false;

volatile int WorkUnit4_GrapImage_Command =0;
volatile int WorkUnit4_GrapImage_Status =0;
volatile bool WorkUnit4_GrapImage_Result  =false;

volatile int WorkUnit5_GrapImage_Command =0;
volatile int WorkUnit5_GrapImage_Status =0;
volatile bool WorkUnit5_GrapImage_Result  =false;

volatile int WorkUnit6_GrapImage_Command =0;
volatile int WorkUnit6_GrapImage_Status =0;
volatile bool WorkUnit6_GrapImage_Result  =false;

volatile int WorkUnit7_GrapImage_Command =0;
volatile int WorkUnit7_GrapImage_Status =0;
volatile bool WorkUnit7_GrapImage_Result  =false;

volatile int WorkUnit8_GrapImage_Command =0;
volatile int WorkUnit8_GrapImage_Status =0;
volatile bool WorkUnit8_GrapImage_Result  =false;

volatile int WorkingPointOneNum   =0 ;
volatile int EStopStatus         =0  ;
volatile int DectectResult    =0  ;

volatile int PLCScanBarcode    =0;
volatile int ScanlineStatus    =0;
volatile int PLCStatus        =0 ;


 HObject Unit3_GrapImage;
 HTuple  hv_WindowHandle_3_1,hv_WindowHandle_3_2;


