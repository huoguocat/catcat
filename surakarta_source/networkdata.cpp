#include "networkdata.h"

// 构造函数，使用操作码和三个数据字符串初始化 NetworkData 对象
NetworkData::NetworkData(OPCODE op, QString data1, QString data2, QString data3)
    : op(op)
    , data1(data1)
    , data2(data2)
    , data3(data3) {}

// 构造函数，从 QByteArray 消息初始化 NetworkData 对象
NetworkData::NetworkData(QByteArray message) {
    // 如果消息为空，抛出 InvalidMessage 异常
    if (message.isEmpty())
        throw InvalidMessage(message);

    // 解析 JSON 文档
    QJsonParseError jserr;
    QJsonDocument doc = QJsonDocument::fromJson(message, &jserr);
    if (doc.isObject()) {
        QJsonObject recv_json = doc.object();
        // 检查 JSON 对象是否包含所需的键
        if (recv_json.contains("op") && recv_json.contains("data1") && recv_json.contains("data2") && recv_json.contains("data3")) {
            // 从 JSON 对象中提取值并赋值给相应的成员变量
            this->op = static_cast<OPCODE>(recv_json.value("op").toInt());
            this->data1 = recv_json.value("data1").toString();
            this->data2 = recv_json.value("data2").toString();
            this->data3 = recv_json.value("data3").toString();
        } else {
            // 如果缺少所需的键，抛出 InvalidMessage 异常
            throw InvalidMessage(message);
        }
    } else {
        // 如果解析失败，抛出 InvalidMessage 异常
        throw InvalidMessage(message);
    }
}

// 编码 NetworkData 对象为 QByteArray
QByteArray NetworkData::encode() {
    // 创建 JSON 对象并插入数据
    QJsonObject send_json;
    send_json.insert("op", static_cast<int>(op));
    send_json.insert("data1", data1);
    send_json.insert("data2", data2);
    send_json.insert("data3", data3);

    // 创建 JSON 文档并设置为 JSON 对象
    QJsonDocument doc;
    doc.setObject(send_json);

    // 将 JSON 文档转换为紧凑格式的 QByteArray，并添加换行符
    QByteArray block = doc.toJson(QJsonDocument::Compact);
    block.append('\n');
    return block;
}

// 返回 InvalidMessage 对象中接收到的消息
QByteArray InvalidMessage::messageReceived() const {
    return this->message;
}

// 构造函数，使用接收到的消息初始化 InvalidMessage 对象
InvalidMessage::InvalidMessage(QByteArray message)
    : message(message) {}
