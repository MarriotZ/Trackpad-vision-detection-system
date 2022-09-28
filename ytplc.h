/**************************************************************************

Author: CaoJian caojian005@aliyun.com

Date:   2019-07-10

Description: YtPLC is a base class for PLC control.
    This class should be singleton, but for inheritance reasons we use a global variable instead.

**************************************************************************/
#ifndef YTPLC_H
#define YTPLC_H


#include <stdio.h>
#include <QString>

#define YTPLC_DEFAULT_TIMEOUT 3000

#define _IN
#define _OUT

class YtPlc
{
public:
    explicit YtPlc();
    ~YtPlc(void);

    static void Startup();
    static void Cleanup();

    //创建socket套接字，并建立连接，iTimeout超时时间单位毫秒
    bool Open(const char* addr, const int port, const int timeout_ms = YTPLC_DEFAULT_TIMEOUT);

    //关闭socket套接字
    void Close();

    //获取socket套接字情况
    bool IsOpen();

    //socket发送与接收
    bool Transfer(const char* send_data, const int send_size, _OUT char* recv_data, _OUT int& recv_size);

    //PLC写入指令，包含了报文拼接，socket发送，接收，结果确认
    virtual bool PLCWrite(const int plc_code, const int* write_data, const int len, const int start_addr) = 0;

    //PLC去取指令，包含了报文拼接，socket发送，接收，结果解析
    virtual bool PLCRead(const int plc_code, const int start_addr, const int len, int *recv_data, int &recv_size) = 0;

    //初始化参数
    virtual void InitParam(){}

    //获取RecipeNo、通道
    virtual bool GetRecipe(const int read_addr, int &recipe)
    {
        recipe = 0;
        Q_UNUSED(read_addr);
        return true;
    }

    //获取ID
    virtual bool GetID(const int read_addr, const int len, QString &id_string)
    {
        id_string.clear();
        Q_UNUSED(read_addr);
        Q_UNUSED(len);
        return true;
    }

    //写入结果
    virtual bool WriteResult(const int write_addr, const QString result)
    {
        Q_UNUSED(write_addr);
        Q_UNUSED(result);
        return true;
    }

protected:
    int socket_;

    char write_buff_[1024]{ 0 };
    char read_buff_[1024]{ 0 };
};

extern YtPlc* g_plc;
#endif // YTPLC_H
