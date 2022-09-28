#include "ytplc.h"
#include <winsock.h>
#include <QDebug>

#pragma comment(lib,"ws2_32.lib")

YtPlc* g_plc = nullptr;

YtPlc::YtPlc()
{
    socket_ = 0;
}

YtPlc::~YtPlc()
{

}

void YtPlc::Startup()
{
    WSADATA  Ws;
    WSAStartup(MAKEWORD(2, 2), &Ws);
}

void YtPlc::Cleanup()
{
    WSACleanup();
}

bool YtPlc::Open(const char *addr, const int port, const int timeout_ms)
{
    if (socket_ > 0)
        return true;

    if (INVALID_SOCKET == (socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)))
    {
        return false;
    }

    // set block timeout
    setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout_ms, sizeof(int));
    setsockopt(socket_, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout_ms, sizeof(int));

    linger linger_set;
    linger_set.l_onoff = 0, linger_set.l_linger = 0;
    setsockopt(socket_, SOL_SOCKET, SO_LINGER, (const char*)&linger_set, sizeof(linger));

    sockaddr_in socker_addr;
    memset(&socker_addr, 0, sizeof(socker_addr));

    socker_addr.sin_family = AF_INET;
    socker_addr.sin_addr.s_addr = inet_addr(addr);
    socker_addr.sin_port = htons(port);

    if (SOCKET_ERROR == connect(socket_, (sockaddr*)&socker_addr, sizeof(socker_addr)))
    {
        socket_ = (int)INVALID_SOCKET;
        return false;
    }
    return true;
}

void YtPlc::Close()
{
    if (INVALID_SOCKET != socket_)
        closesocket(socket_);

    socket_ = (int)INVALID_SOCKET;
}

bool YtPlc::IsOpen()
{
    return (socket_ != INVALID_SOCKET);
}

bool YtPlc::Transfer(const char *send_data, const int send_size, char *recv_data, int &recv_size)
{
    // <out> parameter clear
    recv_size = 0;

    if (INVALID_SOCKET == socket_)
        return false;

    const int kBlockSize = 512;
    char data[kBlockSize]{ 0 };

    // <in> parameter check
    if (NULL == send_data || 0 == send_size)
        return false;

    // if iSendSize error, return
    if (send_size != send(socket_, send_data, send_size, 0))
        return false;

    int iCurRecvSize = 0;
    while (iCurRecvSize >= 0)
    {
        // receive block size each time
        iCurRecvSize = recv(socket_, data, kBlockSize, 0);

        if (iCurRecvSize <= 0)
            break;

        for (int i = 0; i != iCurRecvSize; ++i)
        {
            recv_data[recv_size + i] = data[i];
        }

        recv_size += iCurRecvSize;

        if (iCurRecvSize != kBlockSize)
            break;
    }
    return (recv_size > 0);
}
