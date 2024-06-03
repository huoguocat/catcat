#include "networkserver.h"

// 构造函数,初始化server和两个信号映射器
NetworkServer::NetworkServer(QObject* parent)
    : QTcpServer(parent)
    , disconnMapper(new QSignalMapper(this))
    , recvMapper(new QSignalMapper(this)) {
    // 连接newConnection信号到newconnection槽
    connect(this, &QTcpServer::newConnection, this, &NetworkServer::newconnection);
    // 连接disconnMapper的mappedObject信号到disconnect槽
    connect(this->disconnMapper, &QSignalMapper::mappedObject, this, &NetworkServer::disconnect);
    // 连接recvMapper的mappedObject信号到receiveData槽
    connect(this->recvMapper, &QSignalMapper::mappedObject, this, &NetworkServer::receiveData);
}

// 接收客户端发来的数据并处理
void NetworkServer::receiveData(QObject* obj) {
    // 将obj转换为QTcpSocket指针
    QTcpSocket* client = dynamic_cast<QTcpSocket*>(obj);
    Q_ASSERT(client != nullptr);
    // 读取客户端发来的所有数据
    QByteArray whole_block = client->readAll();
    // 将数据按换行符分割成多个块
    QList<QByteArray> blocks = whole_block.split('\n');
    // 遍历每个块
    for (const auto& block : blocks) {
        // 如果块为空,跳过
        if (block.isEmpty())
            continue;
        try {
            // 尝试解析块为NetworkData对象
            NetworkData data(block);
            // 发射receive信号,包含客户端socket和解析后的数据
            emit receive(client, data);
        }
        catch (const InvalidMessage& m) {
            // 如果解析失败,发射parseError信号,包含引发异常的消息
            emit parseError(m);
        }
    }
}

// 向客户端发送数据
void NetworkServer::send(QTcpSocket* client, NetworkData data) {
    // 编码数据为QByteArray并发送
    client->write(data.encode());
    client->flush();
}

// 处理客户端断开连接
void NetworkServer::disconnect(QObject* client) {
    // 从clients列表中删除断开的客户端
    for (auto it = clients.begin(); it != clients.end(); ++it) {
        if (*it == client) {
            emit leave(*it);
            clients.erase(it);
            break;
        }
    }
}

// 处理新连接
void NetworkServer::newconnection() {
    // 接受新连接
    QTcpSocket* client = this->nextPendingConnection();
    // 将新客户端加入clients列表
    clients.append(client);
    // 连接客户端的readyRead信号到recvMapper的映射
    connect(client, &QIODevice::readyRead, this->recvMapper, qOverload<>(&QSignalMapper::map));
    // 连接客户端的disconnected信号到disconnMapper的映射
    connect(client, &QAbstractSocket::disconnected, this->disconnMapper, qOverload<>(&QSignalMapper::map));
    // 设置recvMapper和disconnMapper的映射对象为client
    this->disconnMapper->setMapping(client, client);
    this->recvMapper->setMapping(client, client);
}
