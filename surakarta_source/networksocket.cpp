#include "networksocket.h"
#include <QtCore>

// NetworkSocket 类的构造函数，初始化 socket 并连接 readyRead 信号
NetworkSocket::NetworkSocket(QTcpSocket* socket, QObject* parent)
    : QObject(parent)
    , socket(socket) {
    // 将 socket 的父对象设置为这个 NetworkSocket 实例
    this->socket->setParent(this);
    // 连接 socket 的 readyRead 信号到这个实例的 receiveData 槽
    connect(this->base(), &QAbstractSocket::readyRead, this, &NetworkSocket::receiveData);
}

// 返回底层的 QTcpSocket 实例
QTcpSocket* NetworkSocket::base() const {
    return this->socket;
}

// 将 socket 连接到指定的主机和端口
void NetworkSocket::hello(const QString& host, quint16 port) {
    // 中止任何现有的连接
    this->socket->abort();
    // 连接到指定的主机和端口
    this->socket->connectToHost(host, port);
}

// 将 socket 从主机断开连接
void NetworkSocket::bye() {
    // 从主机断开连接
    this->socket->disconnectFromHost();
}

// 槽函数，用于处理从 socket 接收到的数据
void NetworkSocket::receiveData() {
    // 读取 socket 中所有可用的数据
    auto whole_block = this->socket->readAll();
    // 根据换行符将数据分割成多个块
    auto blocks = whole_block.split('\n');
    // 处理每个数据块
    for (const auto& block : blocks) {
        try {
            // 尝试从数据块构造一个 NetworkData 对象
            NetworkData recv(block);
            // 输出接收到的数据
            qDebug() << "Client: "
                     << static_cast<int>(recv.op)
                     << ' ' << recv.data1 << ' ' << recv.data2 << ' ' << recv.data3 << Qt::endl;
            // 发出接收信号并传递接收到的数据
            emit receive(recv);
        }
        catch (const InvalidMessage& e) {
            // 如果解析错误且消息不为空，发出解析错误信号
            if (!e.messageReceived().isEmpty()) {
                emit parseError(e);
            }
        }
    }
}

// 通过 socket 发送提供的 NetworkData 对象
void NetworkSocket::send(NetworkData data) {
    // 将编码后的数据写入 socket
    this->socket->write(data.encode());
    // 刷新 socket，确保所有数据被发送
    this->socket->flush();
}
