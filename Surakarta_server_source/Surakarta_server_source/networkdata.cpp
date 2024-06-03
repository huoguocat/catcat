#include "networkdata.h"

// 构造函数,接受操作码、三个字符串数据作为参数,并将其赋值给对应的成员变量
NetworkData::NetworkData(OPCODE op, QString data1, QString data2, QString data3)
    : op(op)
    , data1(data1)
    , data2(data2)
    , data3(data3) {}

// 构造函数,接受一个QByteArray类型的消息作为参数
// 解析这个消息,并将其中的操作码和三个字符串数据赋值给对应的成员变量
NetworkData::NetworkData(QByteArray message) {
    // 如果消息为空,抛出InvalidMessage异常
    if (message.isEmpty())
        throw InvalidMessage(message);

    // 使用QJsonDocument解析消息,得到一个JSON对象
    QJsonParseError jserr;
    QJsonDocument doc = QJsonDocument::fromJson(message, &jserr);
    if (doc.isObject()) {
        QJsonObject recv_json = doc.object();
        // 检查JSON对象是否包含"op"、"data1"、"data2"和"data3"四个键
        if (recv_json.contains("op") && recv_json.contains("data1") && recv_json.contains("data2") && recv_json.contains("data3")) {
            // 将值赋给对应的成员变量
            this->op = static_cast<OPCODE>(recv_json.value("op").toInt());
            this->data1 = recv_json.value("data1").toString();
            this->data2 = recv_json.value("data2").toString();
            this->data3 = recv_json.value("data3").toString();
        }
        else
            // 如果JSON对象不包含所需的键,抛出InvalidMessage异常
            throw InvalidMessage(message);
    }
    else
        // 如果无法正确解析JSON,抛出InvalidMessage异常
        throw InvalidMessage(message);
}

// 将成员变量的值编码为JSON格式的QByteArray
QByteArray NetworkData::encode() {
    QJsonObject send_json;
    send_json.insert("op", static_cast<int>(op));
    send_json.insert("data1", data1);
    send_json.insert("data2", data2);
    send_json.insert("data3", data3);
    QJsonDocument doc;
    doc.setObject(send_json);
    QByteArray block = doc.toJson(QJsonDocument::Compact);
    block.append('\n');
    return block;
}

// InvalidMessage异常的实现
// 返回引发异常的消息
QByteArray InvalidMessage::messageReceived() const {
    return this->message;
}

// InvalidMessage异常的构造函数,接受一个QByteArray类型的消息作为参数
InvalidMessage::InvalidMessage(QByteArray message)
    : message(message) {}
