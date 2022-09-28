#include "mythread.h"

myThread::myThread(QObject *parent) : QThread(parent)
{
    ho_Image.GenEmptyObj();
//    qDebug()<<1;
}

myThread::~myThread(){
    //    qDebug()<<QString::number(3)+QString::number(type);
}

void myThread::run(){
    if(type==1){
        HObject ho_ShapeModel1,ho_GrayImage,ho_DetectRegion;
        HTuple  hv_ModelID1,hv_Score;
        HTuple  hv_MatchResultRow, hv_MatchResultColumn,hv_MatchResultAngle;
        QString path;
        HTuple pathFile;
        path = "./Matches/station0Match0";
        pathFile = path.toStdString().c_str();
        ReadShapeModel(pathFile, &hv_ModelID1);
        GetShapeModelContours(&ho_ShapeModel1, hv_ModelID1, 1);
        HTuple num_channels;
        CountChannels(ho_Image,&num_channels);
        if(1 == num_channels.I())
            ho_GrayImage = ho_Image;
        else
            Rgb1ToGray(ho_Image,&ho_GrayImage);

        FindShapeModel(ho_GrayImage, hv_ModelID1,  -0.39, 1.57,
                       0.3, 1, 0.5, "least_squares", 4, 0.2, &hv_MatchResultRow, &hv_MatchResultColumn, &hv_MatchResultAngle, &hv_Score);

        if(0 == hv_MatchResultRow.Length()) return;
        path = path +"m_tuple";
        HTuple hv_MatchOrg,hv_HomMat2D;
        HObject ho_RotateImage,ho_res;
        ho_res.GenEmptyObj();
        ReadTuple(path.toStdString().c_str(),&hv_MatchOrg);

        VectorAngleToRigid(hv_MatchResultRow, hv_MatchResultColumn, hv_MatchResultAngle, hv_MatchOrg[0], hv_MatchOrg[1], hv_MatchOrg[2], &hv_HomMat2D);

        AffineTransImage(ho_Image,&ho_RotateImage,hv_HomMat2D,"constant","false");

        ho_Image = ho_RotateImage.Clone();


        emit setMatchImg1(ho_Image);


        //ACF
        path = "./Detectes/station0_0Detect1";

        pathFile = path.toStdString().c_str();

        HTuple m_FileExists;

        QString FileDect =path +".hobj";
        //判断文件是否存在
        FileExists(FileDect.toStdString().c_str(),&m_FileExists);

        if(m_FileExists.I() == 0)
        {
            emit noFile("station0_0Detect1.hobj");
            return;
        }

        //WriteShapeModel(hv_ModelID, pathFile);
        ho_DetectRegion.GenEmptyObj();
        ReadRegion(&ho_DetectRegion,pathFile);
        HTuple hv_DetectNum;
        CountObj(ho_DetectRegion,&hv_DetectNum);
        if(hv_DetectNum.I() == 0) return;
        HObject ho_DetectRegionSelected,ho_DetectImage;
        for (int i=0;i<hv_DetectNum;i++) {

            SelectObj(ho_DetectRegion,&ho_DetectRegionSelected,HTuple(i+1));
            ReduceDomain(ho_Image,ho_DetectRegionSelected,&ho_DetectImage);

            HObject   ho_Rectangle;
            HObject   ho_Image1, ho_Image2, ho_Image3;
            HObject  ho_ImageResult1, ho_ImageResult2, ho_ImageResult3;
            //  HObject  ho_ImageReduced1;

            // Local control variables

            HTuple  hv_Area, hv_Row1, hv_Column1;

            Decompose3(ho_DetectImage, &ho_Image1, &ho_Image2, &ho_Image3);
            TransFromRgb(ho_Image1, ho_Image2, ho_Image3, &ho_ImageResult1, &ho_ImageResult2,&ho_ImageResult3, "hsi");

            Threshold(ho_ImageResult2,&ho_Rectangle,60, 220);
            AreaCenter(ho_Rectangle,&hv_Area, &hv_Row1, &hv_Column1);
            HTuple hv_readTuple;
            ReadTuple(pathFile,&hv_readTuple);

            if(hv_Area.I()<hv_readTuple[0])
            {
                if(ho_res.CountObj()==0)
                    ho_res=ho_DetectRegionSelected;
                else
                    Union2(ho_res,ho_DetectRegionSelected,&ho_res);
                piece.acf_ng=1;
                piece.workUnit3_1_ok=0;

            }
        }
        if(piece.acf_ng==1){
            QString date = QDateTime::currentDateTime().toString("yyyy_MM_dd");
            QString time = QDateTime::currentDateTime().toString("hh_mm_ss");
            QString runPth = QDir::currentPath();
            runPth+="/Image/"+date+QString::fromLocal8Bit("/ACF胶漏贴");
            QDir dir(runPth);
            if(!dir.exists()){
                dir.mkpath(runPth);
            }
            runPth+="/"+time;
            QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
            std::string name = code->fromUnicode(runPth).data();
            if(runPth.isEmpty())
                return;
            HTuple hv_name = name.c_str();
            WriteImage(ho_Image,"tiff",0,hv_name);
        }

        //TBF
        path = "./Detectes/station0_0Detect2";

        pathFile = path.toStdString().c_str();

        FileDect =path +".hobj";
        //判断文件是否存在
        FileExists(FileDect.toStdString().c_str(),&m_FileExists);

        if(m_FileExists.I() == 0)
        {
            emit noFile("station0_0Detect2.hobj");
            return;
        }

        //WriteShapeModel(hv_ModelID, pathFile);
        ho_DetectRegion.GenEmptyObj();
        ReadRegion(&ho_DetectRegion,pathFile);
        CountObj(ho_DetectRegion,&hv_DetectNum);
        if(hv_DetectNum.I() == 0) return;
        for (int i=0;i<hv_DetectNum;i++) {
            SelectObj(ho_DetectRegion,&ho_DetectRegionSelected,HTuple(i+1));
            ReduceDomain(ho_Image,ho_DetectRegionSelected,&ho_DetectImage);
            HObject   ho_Rectangle;
            //  HObject  ho_ImageReduced1;

            // Local control variables
            HTuple  hv_Area, hv_Row1, hv_Column1;

            HTuple hv_readTuple;
            ReadTuple(pathFile,&hv_readTuple);

            Threshold(ho_DetectImage,&ho_Rectangle,0, hv_readTuple[1]);

            AreaCenter(ho_Rectangle,&hv_Area, &hv_Row1, &hv_Column1);

            if(hv_Area.I()<hv_readTuple[0])
            {
                if(ho_res.CountObj()==0)
                    ho_res=ho_DetectRegionSelected;
                else
                    Union2(ho_res,ho_DetectRegionSelected,&ho_res);
                piece.tbf_ng=1;
                piece.workUnit3_1_ok=0;
            }
        }
        if(piece.tbf_ng==1){
            QString date = QDateTime::currentDateTime().toString("yyyy_MM_dd");
            QString time = QDateTime::currentDateTime().toString("hh_mm_ss");
            QString runPth = QDir::currentPath();
            runPth+="/Image/"+date+QString::fromLocal8Bit("/TBF胶漏贴");
            QDir dir(runPth);
            if(!dir.exists()){
                dir.mkpath(runPth);
            }
            runPth+="/"+time;
            QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
            std::string name = code->fromUnicode(runPth).data();
            if(runPth.isEmpty())
                return;
            HTuple hv_name = name.c_str();
            WriteImage(ho_Image,"tiff",0,hv_name);
        }

        //PET
        path = "./Detectes/station0_0Detect5";
        pathFile = path.toStdString().c_str();

        FileDect =path +".hobj";
        //判断文件是否存在
        FileExists(FileDect.toStdString().c_str(),&m_FileExists);


        if(m_FileExists.I() == 0)
        {
            emit noFile("station0_0Detect5.hobj");
            return;
        }

        //WriteShapeModel(hv_ModelID, pathFile);
        ho_DetectRegion.GenEmptyObj();
        ReadRegion(&ho_DetectRegion,pathFile);
        CountObj(ho_DetectRegion,&hv_DetectNum);
        if(hv_DetectNum.I() == 0) return;
        for (int i=0;i<hv_DetectNum;i++) {
            SelectObj(ho_DetectRegion,&ho_DetectRegionSelected,HTuple(i+1));
            ReduceDomain(ho_Image,ho_DetectRegionSelected,&ho_DetectImage);
            HObject   ho_Image1, ho_Image2, ho_Image3;
            HObject  ho_ImageResult1, ho_ImageResult2, ho_ImageResult3;
            HObject  ho_Region, ho_Connected;
            HObject  ho_SelectedRegions;

            // Local control variables
            HTuple  hv_Area, hv_Row1, hv_Column1;

            //将三通道图像转化为三图像
            Decompose3(ho_DetectImage, &ho_Image1, &ho_Image2, &ho_Image3);
            //将图像从RGB颜色空间转化为任意颜色空间  （hsi常用于工业方面）
            TransFromRgb(ho_Image1, ho_Image2, ho_Image3, &ho_ImageResult1, &ho_ImageResult2,
                         &ho_ImageResult3, "hsi");
            // DrawRectangle2(200000, &hv_Row, &hv_Column, &hv_Phi, &hv_Length1, &hv_Length2);
            // GenRectangle2(&ho_Rectangle, hv_Row, hv_Column, hv_Phi, hv_Length1, hv_Length2);

            //   ReduceDomain(ho_ImageResult2, ho_DetectRegionSelected, &ho_ImageReduced1);
            // 图像二值化
            Threshold(ho_ImageResult2, &ho_Region, 90, 255);
            // 分离ROI
            Connection(ho_Region, &ho_Connected);
            // 找到面积中心
            AreaCenter(ho_Connected, &hv_Area, &hv_Row1, &hv_Column1);
            // 选择面积大于500 的图形
            SelectShape(ho_Connected, &ho_SelectedRegions, "area", "and", 450, 99999);


            if(ho_SelectedRegions.CountObj()>0)
            {
                if(ho_res.CountObj()==0)
                    ho_res=ho_DetectRegionSelected;
                else
                    Union2(ho_res,ho_DetectRegionSelected,&ho_res);
                piece.pet_cv_ng=1;
                piece.workUnit3_1_ok=0;
            }
        }
        if(piece.pet_cv_ng==1){
            QString date = QDateTime::currentDateTime().toString("yyyy_MM_dd");
            QString time = QDateTime::currentDateTime().toString("hh_mm_ss");
            QString runPth = QDir::currentPath();
            runPth+="/Image/"+date+QString::fromLocal8Bit("/PET贴偏漏铜");
            QDir dir(runPth);
            if(!dir.exists()){
                dir.mkpath(runPth);
            }
            runPth+="/"+time;
            QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
            std::string name = code->fromUnicode(runPth).data();
            if(runPth.isEmpty())
                return;
            HTuple hv_name = name.c_str();
            WriteImage(ho_Image,"tiff",0,hv_name);
        }
        if(piece.workUnit3_1_ok==1){
            QString date = QDateTime::currentDateTime().toString("yyyy_MM_dd");
            QString time = QDateTime::currentDateTime().toString("hh_mm_ss");
            QString runPth = QDir::currentPath();
            runPth+="/Image/"+date+QString::fromLocal8Bit("/工站1环光_OK");
            QDir dir(runPth);
            if(!dir.exists()){
                dir.mkpath(runPth);
            }
            runPth+="/"+time;
            QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
            std::string name = code->fromUnicode(runPth).data();
            if(runPth.isEmpty())
                return;
            HTuple hv_name = name.c_str();
            WriteImage(ho_Image,"tiff",0,hv_name);
        }
        emit setRes1(ho_res,index,piece);
    }










    else if(type==2){
        HObject ho_ShapeModel1,ho_GrayImage,ho_DetectRegion;
        HTuple  hv_ModelID1,hv_Score;
        HTuple  hv_MatchResultRow, hv_MatchResultColumn,hv_MatchResultAngle;
        QString path;
        HTuple pathFile;
        path ="./Matches/station0Match1";
        pathFile = path.toStdString().c_str();

        ReadShapeModel(pathFile, &hv_ModelID1);
        GetShapeModelContours(&ho_ShapeModel1, hv_ModelID1, 1);
        HTuple num_channels;
        CountChannels(ho_Image,&num_channels);
        if(1 == num_channels.I())
            ho_GrayImage = ho_Image;
        else
            Rgb1ToGray(ho_Image,&ho_GrayImage);
        HObject ho_DerivGauss,ho_ImageScaleMax,ho_res;
        ho_res.GenEmptyObj();
        FindShapeModel(ho_GrayImage, hv_ModelID1,  -0.39, 1.57,
                       0.3, 1, 0.5, "least_squares", 4, 0.2, &hv_MatchResultRow, &hv_MatchResultColumn, &hv_MatchResultAngle, &hv_Score);
        if(0 == hv_MatchResultRow.Length()) return;
        path = path +"m_tuple";
        HTuple hv_MatchOrg,hv_HomMat2D;
        HObject ho_RotateImage;
        ReadTuple(path.toStdString().c_str(),&hv_MatchOrg);

        VectorAngleToRigid(hv_MatchResultRow, hv_MatchResultColumn, hv_MatchResultAngle, hv_MatchOrg[0], hv_MatchOrg[1], hv_MatchOrg[2], &hv_HomMat2D);

        AffineTransImage(ho_Image,&ho_RotateImage,hv_HomMat2D,"constant","false");

        ho_Image = ho_RotateImage.Clone();

        emit setMatchImg2(ho_Image);

        //STOP
        path = "./Detectes/station0_1Detect6";

        pathFile = path.toStdString().c_str();

        HTuple m_FileExists;

        QString FileDect =path +".hobj";
        //判断文件是否存在
        FileExists(FileDect.toStdString().c_str(),&m_FileExists);


        if(m_FileExists.I() == 0)
        {
            emit noFile("station0_1Detect6.hobj");
            return;
        }

        //WriteShapeModel(hv_ModelID, pathFile);
        ho_DetectRegion.GenEmptyObj();
        ReadRegion(&ho_DetectRegion,pathFile);
        HTuple hv_DetectNum;
        CountObj(ho_DetectRegion,&hv_DetectNum);
        if(hv_DetectNum.I() == 0) return;
        HObject ho_DetectRegionSelected,ho_DetectImage;
        for (int i=0;i<hv_DetectNum;i++) {
            SelectObj(ho_DetectRegion,&ho_DetectRegionSelected,HTuple(i+1));
            ReduceDomain(ho_Image,ho_DetectRegionSelected,&ho_DetectImage);

            // Local iconic variables
            HObject    ho_ImageReduced;
            HObject   ho_GrayImage,ho_ImageEmphasize, ho_ImageScaleMax;
            // 裁出stop贴区域
            ReduceDomain(ho_Image, ho_DetectRegionSelected, &ho_ImageReduced);
            // 裁出区域转灰度化
            Rgb1ToGray(ho_ImageReduced, &ho_GrayImage);
            Emphasize(ho_GrayImage, &ho_ImageEmphasize, 7, 7, 1);
            HObject ho_LogImage,ho_Region,ho_RegionClosing,ho_ConnectedRegions,ho_RegionFillUp,ho_SelectedStopRegions;
            HObject ho_DerivGauss,ho_ImageScaleMax2,ho_RegionOpening;
            HTuple hv_Mean,hv_Deviation;

            // 计算图像对数
            LogImage(ho_ImageEmphasize, &ho_LogImage, "e");
            // 在0到255的范围内扩展的最大灰度值。
            ScaleImageMax(ho_LogImage, &ho_ImageScaleMax);
            // 高斯函数导数卷积图像
            DerivateGauss(ho_ImageScaleMax, &ho_DerivGauss, 1, "gradient");
            ScaleImageMax(ho_DerivGauss, &ho_ImageScaleMax2);
            Intensity(ho_DetectRegionSelected, ho_ImageScaleMax2, &hv_Mean, &hv_Deviation);
            //
            Threshold(ho_ImageScaleMax2, &ho_Region, hv_Mean+10, 255);
            OpeningCircle(ho_Region, &ho_RegionOpening, 1.5);
            ClosingCircle(ho_RegionOpening, &ho_RegionClosing, 10.5);

           // Eccentricity(ho_RegionClosing, &hv_Anisometry, &hv_Bulkiness, &hv_StructureFactor);
            Connection(ho_RegionClosing, &ho_ConnectedRegions);
            FillUp(ho_ConnectedRegions, &ho_RegionFillUp);
            //AreaCenter(ho_RegionFillUp, &hv_Area1, &hv_Row3, &hv_Column3);
            SelectShape(ho_RegionFillUp, &ho_SelectedStopRegions, "area", "and", 65000, 999999);


            if(ho_SelectedStopRegions.CountObj()==0)
            {
                if(ho_res.CountObj()==0)
                    ho_res=ho_DetectRegionSelected;
                else
                    Union2(ho_res,ho_DetectRegionSelected,&ho_res);
                piece.stop_ng=1;
                piece.workUnit3_2_ok=0;
            }
        }
        if(piece.stop_ng==1){
            QString date = QDateTime::currentDateTime().toString("yyyy_MM_dd");
            QString time = QDateTime::currentDateTime().toString("hh_mm_ss");
            QString runPth = QDir::currentPath();
            runPth+="/Image/"+date+QString::fromLocal8Bit("/STOP贴漏贴");
            QDir dir(runPth);
            if(!dir.exists()){
                dir.mkpath(runPth);
            }
            runPth+="/"+time;
            QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
            std::string name = code->fromUnicode(runPth).data();
            if(runPth.isEmpty())
                return;
            HTuple hv_name = name.c_str();
            WriteImage(ho_Image,"tiff",0,hv_name);
        }

        //螺丝掉漆
        path = "./Detectes/station0_1Detect10";

        pathFile = path.toStdString().c_str();

        FileDect =path +".hobj";
        //判断文件是否存在
        FileExists(FileDect.toStdString().c_str(),&m_FileExists);

        if(m_FileExists.I() == 0)
        {
            emit noFile("station0_1Detect10.hobj");
            return;
        }

        //WriteShapeModel(hv_ModelID, pathFile);
        ho_DetectRegion.GenEmptyObj();
        ReadRegion(&ho_DetectRegion,pathFile);
        CountObj(ho_DetectRegion,&hv_DetectNum);
        if(hv_DetectNum.I() == 0) return;
        for (int i=0;i<hv_DetectNum;i++) {
            SelectObj(ho_DetectRegion,&ho_DetectRegionSelected,HTuple(i+1));
            ReduceDomain(ho_Image,ho_DetectRegionSelected,&ho_DetectImage);

            HObject  ho_ImageReduced,ho_SelectedRegions;
            HObject  ho_ImageMean, ho_GrayImage, ho_Region, ho_ConnectedRegions;

            // Local control variables
            HTuple  hv_Area,hv_Row1, hv_Column1;

            // 裁出ROI
            ReduceDomain(ho_Image, ho_DetectRegionSelected, &ho_ImageReduced);
            // 均值滤波
            MeanImage(ho_ImageReduced, &ho_ImageMean, 9, 9);
            // 转灰度值
            Rgb1ToGray(ho_ImageReduced, &ho_GrayImage);
            // 二值化
            Threshold(ho_GrayImage, &ho_Region, 150, 255);
            // 分离区域
            Connection(ho_Region, &ho_ConnectedRegions);
            // 获取面积中心
            AreaCenter(ho_ConnectedRegions, &hv_Area, &hv_Row1, &hv_Column1);
            // 根据面积筛选出想要的区域
            SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 130, 99999);
            // 显示处理
            if(ho_SelectedRegions.CountObj()> 0)
            {
                if(ho_res.CountObj()==0)
                    ho_res=ho_DetectRegionSelected;
                else
                    Union2(ho_res,ho_DetectRegionSelected,&ho_res);
                piece.screw_ng=1;
                piece.workUnit3_2_ok=0;
            }
        }
        if(piece.screw_ng==1){
            QString date = QDateTime::currentDateTime().toString("yyyy_MM_dd");
            QString time = QDateTime::currentDateTime().toString("hh_mm_ss");
            QString runPth = QDir::currentPath();
            runPth+="/Image/"+date+QString::fromLocal8Bit("/螺丝掉漆");
            QDir dir(runPth);
            if(!dir.exists()){
                dir.mkpath(runPth);
            }
            runPth+="/"+time;
            QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
            std::string name = code->fromUnicode(runPth).data();
            if(runPth.isEmpty())
                return;
            HTuple hv_name = name.c_str();
            WriteImage(ho_Image,"tiff",0,hv_name);
        }

        //大连接器关闭
        path = "./Detectes/station0_1Detect7";

        pathFile = path.toStdString().c_str();

        FileDect =path +".hobj";
        //判断文件是否存在
        FileExists(FileDect.toStdString().c_str(),&m_FileExists);


        if(m_FileExists.I() == 0)
        {
            emit noFile("station0_1Detect7.hobj");
            return;
        }

        //WriteShapeModel(hv_ModelID, pathFile);
        ho_DetectRegion.GenEmptyObj();
        ReadRegion(&ho_DetectRegion,pathFile);
        CountObj(ho_DetectRegion,&hv_DetectNum);
        if(hv_DetectNum.I() == 0) return;
        for (int i=0;i<hv_DetectNum;i++) {
            SelectObj(ho_DetectRegion,&ho_DetectRegionSelected,HTuple(i+1));
            ReduceDomain(ho_Image,ho_DetectRegionSelected,&ho_DetectImage);

            HObject ho_GrayImage1, ho_ImageReduced1, ho_Region, ho_Connected,ho_SelectedRegions;

            // Local control variables
            //            HTuple  hv_Row, hv_Column, hv_Phi, hv_Length1;
            //            HTuple  hv_Length2, hv_Area, hv_Row1, hv_Column1;

//            HTuple hv_readTuple;
//            ReadTuple(pathFile,&hv_readTuple);
            // RGB -> Gray
            Rgb1ToGray(ho_Image, &ho_GrayImage1);
            // 裁出ROI
            ReduceDomain(ho_GrayImage1, ho_DetectRegionSelected, &ho_ImageReduced1);
            // 二值化
            Threshold(ho_ImageReduced1, &ho_Region, 180, 255);
            // 分离ROI
            Connection(ho_Region, &ho_Connected);
            // 从文件里读取一个元组选出面积大于
            SelectShape(ho_Connected, &ho_SelectedRegions, "contlength", "and", 70, 99999);

            if(ho_SelectedRegions.CountObj()>0)
            {
                if(ho_res.CountObj()==0)
                    ho_res=ho_DetectRegionSelected;
                else
                    Union2(ho_res,ho_DetectRegionSelected,&ho_res);
                piece.big_close_ng=1;
                piece.workUnit3_2_ok=0;
            }
        }
        if(piece.big_close_ng==1){
            QString date = QDateTime::currentDateTime().toString("yyyy_MM_dd");
            QString time = QDateTime::currentDateTime().toString("hh_mm_ss");
            QString runPth = QDir::currentPath();
            runPth+="/Image/"+date+QString::fromLocal8Bit("/大连接器关闭");
            QDir dir(runPth);
            if(!dir.exists()){
                dir.mkpath(runPth);
            }
            runPth+="/"+time;
            QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
            std::string name = code->fromUnicode(runPth).data();
            if(runPth.isEmpty())
                return;
            HTuple hv_name = name.c_str();
            WriteImage(ho_Image,"tiff",0,hv_name);
        }

        //大连接器破损
        path = "./Detectes/station0_1Detect8";

        pathFile = path.toStdString().c_str();

        FileDect =path +".hobj";
        //判断文件是否存在
        FileExists(FileDect.toStdString().c_str(),&m_FileExists);


        if(m_FileExists.I() == 0)
        {
            emit noFile("station0_1Detect8.hobj");
            return;
        }

        //WriteShapeModel(hv_ModelID, pathFile);
        ho_DetectRegion.GenEmptyObj();
        ReadRegion(&ho_DetectRegion,pathFile);
        CountObj(ho_DetectRegion,&hv_DetectNum);
        if(hv_DetectNum.I() == 0) return;
        for (int i=0;i<hv_DetectNum;i++) {
            SelectObj(ho_DetectRegion,&ho_DetectRegionSelected,HTuple(i+1));
            ReduceDomain(ho_Image,ho_DetectRegionSelected,&ho_DetectImage);

            //HObject   ho_Rectangle, ho_Rectangle1;
            HObject  ho_Image1, ho_Image2, ho_Image3,ho_ImageReduced;
            HObject  ho_Image11, ho_Image21, ho_Image31,ho_ImageSub, ho_Region2;
            HObject  ho_RegionOpening1, ho_ConnectedRegions, ho_SelectedRegions;
            // Local control variables
            HTuple  hv_Area, hv_Row4, hv_Column4;


            QString pathImage = path +"Image.tif";
            HObject ho_BackImage,ho_ImageDiff,ho_BackReduced,ho_BackGray;
            //读取图像
            ReadImage(&ho_BackImage, pathImage.toStdString().c_str());
            // 转化为三通道的三个图形
            Decompose3(ho_BackImage, &ho_Image1, &ho_Image2, &ho_Image3);
            Decompose3(ho_Image, &ho_Image11, &ho_Image21, &ho_Image31);
            // 裁出ROI
            ReduceDomain(ho_Image3,ho_DetectRegionSelected,&ho_BackReduced);
            ReduceDomain(ho_Image31,ho_DetectRegionSelected,&ho_ImageReduced);


            // 两图像做差分
            SubImage(ho_BackReduced, ho_ImageReduced, &ho_ImageSub, 1, 0);
            // 差分后图像做二值化
            Threshold(ho_ImageSub, &ho_Region2, 70, 255);
            // 开运算
            OpeningRectangle1(ho_Region2, &ho_RegionOpening1, 4, 4);
            //ClosingRectangle1(ho_Region2, &ho_RegionClosing1, 10, 10);
            // 分离ROI
            Connection(ho_RegionOpening1, &ho_ConnectedRegions);
            // 获取面积中心
            AreaCenter(ho_ConnectedRegions, &hv_Area, &hv_Row4, &hv_Column4);
            // 通过面积选择合适的区域
            SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 100, 99999);
            // 显示处理
            if(ho_SelectedRegions.CountObj()>0)
            {
                if(ho_res.CountObj()==0)
                    ho_res=ho_DetectRegionSelected;
                else
                    Union2(ho_res,ho_DetectRegionSelected,&ho_res);
                piece.big_damage_ng=1;
                piece.workUnit3_2_ok=0;
            }
        }
        if(piece.big_damage_ng==1){
            QString date = QDateTime::currentDateTime().toString("yyyy_MM_dd");
            QString time = QDateTime::currentDateTime().toString("hh_mm_ss");
            QString runPth = QDir::currentPath();
            runPth+="/Image/"+date+QString::fromLocal8Bit("/大连接器破损");
            QDir dir(runPth);
            if(!dir.exists()){
                dir.mkpath(runPth);
            }
            runPth+="/"+time;
            QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
            std::string name = code->fromUnicode(runPth).data();
            if(runPth.isEmpty())
                return;
            HTuple hv_name = name.c_str();
            WriteImage(ho_Image,"tiff",0,hv_name);
        }

        //小连接器打开
        path = "./Detectes/station0_1Detect16";

        pathFile = path.toStdString().c_str();

        FileDect =path +".hobj";
        //判断文件是否存在
        FileExists(FileDect.toStdString().c_str(),&m_FileExists);


        if(m_FileExists.I() == 0)
        {
            emit noFile("station0_1Detect16.hobj");
            return;
        }

        //WriteShapeModel(hv_ModelID, pathFile);
        ho_DetectRegion.GenEmptyObj();
        ReadRegion(&ho_DetectRegion,pathFile);
        CountObj(ho_DetectRegion,&hv_DetectNum);
        if(hv_DetectNum.I() == 0) return;
        for (int i=0;i<hv_DetectNum;i++) {
            SelectObj(ho_DetectRegion,&ho_DetectRegionSelected,HTuple(i+1));
            ReduceDomain(ho_Image,ho_DetectRegionSelected,&ho_DetectImage);

            // Local iconic variables
            HObject  ho_ImageReduced1,ho_GrayImage1;
            HObject  ho_Region, ho_Connected, ho_SelectedRegions;

            // Local control variables

            HTuple hv_readTuple;
            ReadTuple(pathFile,&hv_readTuple);
            //rgb 转换为 Gray
            Rgb1ToGray(ho_Image, &ho_GrayImage1);
            // 裁出ROI
            ReduceDomain(ho_GrayImage1, ho_DetectRegionSelected, &ho_ImageReduced1);
            // 二值化处理
            Threshold(ho_ImageReduced1, &ho_Region, 180, 255);
            // 分离区域
            Connection(ho_Region, &ho_Connected);
            //通过面积选择合适的区域
            //SelectShape(ho_Connected, &ho_SelectedRegions, "area", "and", hv_readTuple[0], 99999);
            SelectShape(ho_Connected, &ho_SelectedRegions, "contlength", "and", 80, 99999);
            //  AreaCenter(ho_Connected, &hv_Area, &hv_Row1, &hv_Column1);
            // 设置区域等高线长度
            //Contlength(ho_SelectedRegions, &hv_ContLength);
            // 获取最大值最小值  差值
//            TupleMax(hv_ContLength, &hv_Max);

//            TupleMin(hv_ContLength, &hv_Min);
//            TupleSub(hv_Max,hv_Min,&hv_diff);

            // 显示处理
            if(ho_SelectedRegions.CountObj()==0)
            {
                if(ho_res.CountObj()==0)
                    ho_res=ho_DetectRegionSelected;
                else
                    Union2(ho_res,ho_DetectRegionSelected,&ho_res);
                piece.small_open_ng=1;
                piece.workUnit3_2_ok=0;
            }
        }
        if(piece.small_open_ng==1){
            QString date = QDateTime::currentDateTime().toString("yyyy_MM_dd");
            QString time = QDateTime::currentDateTime().toString("hh_mm_ss");
            QString runPth = QDir::currentPath();
            runPth+="/Image/"+date+QString::fromLocal8Bit("/小连接器打开");
            QDir dir(runPth);
            if(!dir.exists()){
                dir.mkpath(runPth);
            }
            runPth+="/"+time;
            QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
            std::string name = code->fromUnicode(runPth).data();
            if(runPth.isEmpty())
                return;
            HTuple hv_name = name.c_str();
            WriteImage(ho_Image,"tiff",0,hv_name);
        }

        //小卡扣破损
        path = "./Detectes/station0_1Detect15";

        pathFile = path.toStdString().c_str();

        FileDect =path +".hobj";
        //判断文件是否存在
        FileExists(FileDect.toStdString().c_str(),&m_FileExists);


        if(m_FileExists.I() == 0)
        {
            emit noFile("station0_1Detect15.hobj");
            return;
        }

        //WriteShapeModel(hv_ModelID, pathFile);
        ho_DetectRegion.GenEmptyObj();
        ReadRegion(&ho_DetectRegion,pathFile);
        CountObj(ho_DetectRegion,&hv_DetectNum);
        if(hv_DetectNum.I() == 0) return;
        for (int i=0;i<hv_DetectNum;i++) {
            SelectObj(ho_DetectRegion,&ho_DetectRegionSelected,HTuple(i+1));
            ReduceDomain(ho_Image,ho_DetectRegionSelected,&ho_DetectImage);

            //HObject  ho_Rectangle, ho_Rectangle1;
            HObject  ho_Image1, ho_Image2, ho_Image3,ho_ImageReduced;
            HObject  ho_Image11, ho_Image21, ho_Image31;
            HObject  ho_ImageSub, ho_Region2;
            HObject  ho_ConnectedRegions, ho_SelectedRegions;
            HObject ho_BackImage,ho_BackReduced,ho_RegionOpening1;
            // Local control variables
            HTuple  hv_Area, hv_Row4, hv_Column4;

            QString pathImage = path +"Image.tif";
            ReadImage(&ho_BackImage, pathImage.toStdString().c_str());
            Decompose3(ho_BackImage, &ho_Image1, &ho_Image2, &ho_Image3);
            Decompose3(ho_Image, &ho_Image11, &ho_Image21, &ho_Image31);

            ReduceDomain(ho_Image3,ho_DetectRegionSelected,&ho_BackReduced);
            ReduceDomain(ho_Image31,ho_DetectRegionSelected,&ho_ImageReduced);


            SubImage(ho_BackReduced, ho_ImageReduced, &ho_ImageSub, 1, 0);
            Threshold(ho_ImageSub, &ho_Region2, 50, 255);
            OpeningRectangle1(ho_Region2, &ho_RegionOpening1, 5, 5);
//            Threshold(ho_ImageSub, &ho_Region2, 50, 255);
//            ClosingRectangle1(ho_Region2, &ho_RegionClosing1, 7, 7);
            Connection(ho_RegionOpening1, &ho_ConnectedRegions);
            AreaCenter(ho_ConnectedRegions, &hv_Area, &hv_Row4, &hv_Column4);
            SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 100, 99999);
            if(ho_SelectedRegions.CountObj()>0)
            {
                if(ho_res.CountObj()==0)
                    ho_res=ho_DetectRegionSelected;
                else
                    Union2(ho_res,ho_DetectRegionSelected,&ho_res);
                piece.small_damage_ng=1;
                piece.workUnit3_2_ok=0;
            }
        }
        if(piece.small_damage_ng==1){
            QString date = QDateTime::currentDateTime().toString("yyyy_MM_dd");
            QString time = QDateTime::currentDateTime().toString("hh_mm_ss");
            QString runPth = QDir::currentPath();
            runPth+="/Image/"+date+QString::fromLocal8Bit("/小连接器破损");
            QDir dir(runPth);
            if(!dir.exists()){
                dir.mkpath(runPth);
            }
            runPth+="/"+time;
            QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
            std::string name = code->fromUnicode(runPth).data();
            if(runPth.isEmpty())
                return;
            HTuple hv_name = name.c_str();
            WriteImage(ho_Image,"tiff",0,hv_name);
        }
        if(piece.workUnit3_2_ok==1){
            QString date = QDateTime::currentDateTime().toString("yyyy_MM_dd");
            QString time = QDateTime::currentDateTime().toString("hh_mm_ss");
            QString runPth = QDir::currentPath();
            runPth+="/Image/"+date+QString::fromLocal8Bit("/工站1同轴光_OK");
            QDir dir(runPth);
            if(!dir.exists()){
                dir.mkpath(runPth);
            }
            runPth+="/"+time;
            QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
            std::string name = code->fromUnicode(runPth).data();
            if(runPth.isEmpty())
                return;
            HTuple hv_name = name.c_str();
            WriteImage(ho_Image,"tiff",0,hv_name);
        }
        emit setRes2(ho_res,index,piece);
    }
    else if(type==3){
        HObject ho_ShapeModel1,ho_GrayImage,ho_DetectRegion,ho_res;
        ho_res.GenEmptyObj();;
        QString path;
        HTuple pathFile;
        path = "./Detectes/station2_0Detect11";

        pathFile = path.toStdString().c_str();

        HTuple m_FileExists;

        QString FileDect =path +".hobj";
        //判断文件是否存在
        FileExists(FileDect.toStdString().c_str(),&m_FileExists);

        if(m_FileExists.I() == 0)
        {
            emit noFile("station2_0Detect11.hobj");
            return;
        }

        //WriteShapeModel(hv_ModelID, pathFile);
        ho_DetectRegion.GenEmptyObj();
        ReadRegion(&ho_DetectRegion,pathFile);
        HTuple hv_DetectNum;
        CountObj(ho_DetectRegion,&hv_DetectNum);
        if(hv_DetectNum.I() == 0) return;
        HObject ho_DetectRegionSelected,ho_DetectImage;
        for (int i=0;i<hv_DetectNum;i++) {

            SelectObj(ho_DetectRegion,&ho_DetectRegionSelected,HTuple(i+1));
            ReduceDomain(ho_Image,ho_DetectRegionSelected,&ho_DetectImage);
            HObject  ho_Region, ho_RegionFillUp, ho_RegionDifference;
            HObject  ho_ConnectedRegions, ho_SelectedRegions, ho_RegionUnion;
            HObject  ho_RegionClosing, ho_ConnectedRegions1, ho_m_Region;
            HObject  ho_Circle, ho_RegionUnion1, ho_ImageReduced1, ho_Region1;
            HObject  ho_ConnectedRegions2, ho_SelectedRegions1;

            // Local control variables
            HTuple  hv_Area, hv_Row, hv_Column, hv_Index;



            Threshold(ho_DetectImage, &ho_Region, 150, 255);
            FillUp(ho_Region, &ho_RegionFillUp);
            Difference(ho_RegionFillUp, ho_Region, &ho_RegionDifference);
            Connection(ho_RegionDifference, &ho_ConnectedRegions);
            SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 500, 99999);

            Union1(ho_SelectedRegions, &ho_RegionUnion);
            ClosingCircle(ho_RegionUnion, &ho_RegionClosing, 50.5);
            Connection(ho_RegionClosing, &ho_ConnectedRegions1);
            AreaCenter(ho_ConnectedRegions1, &hv_Area, &hv_Row, &hv_Column);


            GenEmptyRegion(&ho_m_Region);
            {
                HTuple end_val22 = (hv_Row.TupleLength())-1;
                HTuple step_val22 = 1;
                for (hv_Index=0; hv_Index.Continue(end_val22, step_val22); hv_Index += step_val22)
                {

                    GenCircle(&ho_Circle, HTuple(hv_Row[hv_Index]), HTuple(hv_Column[hv_Index]),
                              40);
                    Union2(ho_m_Region, ho_Circle, &ho_RegionUnion1);
                    ho_m_Region = ho_RegionUnion1;

                }
            }


            ReduceDomain(ho_DetectImage, ho_RegionUnion1, &ho_ImageReduced1);

            Threshold(ho_ImageReduced1, &ho_Region1, 50, 255);
            Connection(ho_Region1, &ho_ConnectedRegions2);
            SelectShape(ho_ConnectedRegions2, &ho_SelectedRegions1, "area", "and", 3000, 9999999);
            if(ho_SelectedRegions1.CountObj()>0){
                if(ho_res.CountObj()==0)
                    ho_res=ho_SelectedRegions1;
                else
                    Union2(ho_res,ho_SelectedRegions1,&ho_res);
                piece.block_ng=1;
                piece.workUnit5_1_ok=0;
            }
        }

        if(piece.block_ng==1){
            QString date = QDateTime::currentDateTime().toString("yyyy_MM_dd");
            QString time = QDateTime::currentDateTime().toString("hh_mm_ss");
            QString runPth = QDir::currentPath();
            runPth+="/Image/"+date+QString::fromLocal8Bit("/螺丝孔堵塞");
            QDir dir(runPth);
            if(!dir.exists()){
                dir.mkpath(runPth);
            }
            runPth+="/"+time;
            QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
            std::string name = code->fromUnicode(runPth).data();
            if(runPth.isEmpty())
                return;
            HTuple hv_name = name.c_str();
            WriteImage(ho_Image,"tiff",0,hv_name);
        }
        if(piece.workUnit5_1_ok==1){
            QString date = QDateTime::currentDateTime().toString("yyyy_MM_dd");
            QString time = QDateTime::currentDateTime().toString("hh_mm_ss");
            QString runPth = QDir::currentPath();
            runPth+="/Image/"+date+QString::fromLocal8Bit("/螺丝孔_OK");
            QDir dir(runPth);
            if(!dir.exists()){
                dir.mkpath(runPth);
            }
            runPth+="/"+time;
            QTextCodec *code = QTextCodec::codecForName("GB2312");//解决中文路径问题
            std::string name = code->fromUnicode(runPth).data();
            if(runPth.isEmpty())
                return;
            HTuple hv_name = name.c_str();
            WriteImage(ho_Image,"tiff",0,hv_name);
        }

        emit setRes3(ho_res,index,piece);
    }

}

void myThread::getImg1(HObject getImg,int x){
    index=x;
    ho_Image=getImg;
    type=1;
    piece.acf_ng=0;
    piece.tbf_ng=0;
    piece.pet_cv_ng=0;
    piece.workUnit3_1_ok=1;
    start();
}

void myThread::getImg2(HObject getImg,int x){
    index=x;
    ho_Image=getImg;
    type=2;
    piece.stop_ng=0;
    piece.screw_ng=0;
    piece.big_close_ng=0;
    piece.big_damage_ng=0;
    piece.small_open_ng=0;
    piece.small_damage_ng=0;
    piece.workUnit3_2_ok=1;
    start();
}

void myThread::getImg3(HObject getImg, int x)
{
    index=x;
    ho_Image=getImg;
    type=3;
    piece.block_ng=0;
    piece.workUnit5_1_ok=1;
    start();
}




