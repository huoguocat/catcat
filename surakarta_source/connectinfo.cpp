#include "connectinfo.h"
#include "networksocket.h"

ConnectInfo::ConnectInfo(QObject *parent, const QString &userName, const QString &color, const QString &ip, const QString &port)
    : QObject(parent)
    , userName(userName)
    , color(color)
    , roomNum("1")
    , ip(ip)
    , port(10086)
    , vecSocket{nullptr, nullptr}
{
    bool ok = false;
    int nport = port.toInt(&ok);
    if(ok)
    {
        this->port = nport;
    }
}

QString ConnectInfo::GetUserName() const
{
    return this->userName;
}

QString ConnectInfo::GetColor() const
{
    return this->color;
}

QString ConnectInfo::GetIp() const
{
    return this->ip;
}

int ConnectInfo::GetPort() const
{
    return this->port;
}

QString ConnectInfo::GetRoomNum() const
{
    return this->roomNum;
}

void ConnectInfo::ConnectToServer(SOCKET_INDEX socketIndex)
{
    if(vecSocket[socketIndex] == nullptr)
    {
        vecSocket[socketIndex] = new NetworkSocket(new QTcpSocket(this), this);
        connect(this->vecSocket[socketIndex]->base(),
                &QTcpSocket::connected ,
                this,
                &ConnectInfo::Connected);

        connect(this->vecSocket[socketIndex]->base(),
                &QTcpSocket::errorOccurred ,
                this,
                &ConnectInfo::hasConnectError);

        connect(this->vecSocket[socketIndex], &NetworkSocket::receive,
                this, &ConnectInfo::receiveMessage);

    }

    vecSocket[socketIndex]->hello(this->ip, port);
    //this->vecSocket[HumenSocket_Index]->base()->waitForConnected(2000);

}

void ConnectInfo::DisconnectToServer(SOCKET_INDEX socketIndex)
{
    if(this->vecSocket[socketIndex] != nullptr)
    {
        this->vecSocket[socketIndex]->bye();
    }
}

void ConnectInfo::Connected()
{
    emit this->hasConnected();
}

void ConnectInfo::ConnectError(QAbstractSocket::SocketError error)
{
    emit this->hasConnectError();
}

void ConnectInfo::receiveMessage(NetworkData data)
{
    QObject *psend = this->sender();
    if(psend == this->vecSocket[HumenSocket_Index])
    {
        emit this->receive(HumenSocket_Index, data);
    }
    else if(psend == this->vecSocket[AISocket_Index])
    {
        emit this->receive(AISocket_Index, data);
    }
    else
    {
        emit this->receive(NoBadySocket, data);
    }

}

void ConnectInfo::SendMessage(SOCKET_INDEX socketIndex, const NetworkData& sendData)
{
    if(this->vecSocket[socketIndex] != nullptr && socketIndex != NoBadySocket)
    {
        this->vecSocket[socketIndex]->send(sendData);
    }
}
