#include "tcpsocket.h"
#include <QHostAddress>
#include <QDebug>
#include <memory.h>

JsonTcpClient::JsonTcpClient(QObject *parent)
    : QObject(parent)
    , m_socket(nullptr)
    , m_reconnectTimer(new QTimer(this))
    , m_host("")
    , m_port(0)
    , m_autoReconnect(true)
    , m_reconnectInterval(3000)
{
    initSocket();

    m_reconnectTimer->setSingleShot(true);
    connect(m_reconnectTimer, &QTimer::timeout, this, &JsonTcpClient::onReconnectTimeout);
}

JsonTcpClient::~JsonTcpClient()
{
    disconnectFromServer();
    if (m_socket) {
        m_socket->deleteLater();
    }
}

void JsonTcpClient::initSocket()
{
    if (m_socket) {
        m_socket->deleteLater();
    }

    m_socket = new QTcpSocket(this);

    connect(m_socket, &QTcpSocket::connected, this, &JsonTcpClient::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &JsonTcpClient::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &JsonTcpClient::onReadyRead);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &JsonTcpClient::onSocketError);
}

bool JsonTcpClient::connectToServer(const QString &host, quint16 port)
{
    if (isConnected()) {
        qWarning() << "Already connected to server";
        return true;
    }

    m_host = host;
    m_port = port;

    emit statusChanged(QString("Connecting to %1:%2...").arg(host).arg(port));
    qInfo() << "Connecting to JSON server" << host << ":" << port;

    m_socket->connectToHost(host, port);

    if (m_socket->waitForConnected(5000)) {
        return true;
    } else {
        QString errorStr = m_socket->errorString();
        qWarning() << "Connection failed:" << errorStr;
        emit connectionError(errorStr);

        if (m_autoReconnect) {
            m_reconnectTimer->start(m_reconnectInterval);
        }
        return false;
    }
}

void JsonTcpClient::disconnectFromServer()
{
    m_reconnectTimer->stop();

    if (m_socket && m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->disconnectFromHost();
        if (m_socket->state() != QAbstractSocket::UnconnectedState) {
            m_socket->waitForDisconnected(1000);
        }
    }

    emit statusChanged("Disconnected");
}

bool JsonTcpClient::sendMessage(const QJsonObject &message)
{
    if (!isConnected()) {
        qWarning() << "Cannot send message: not connected to server";
        return false;
    }

    QJsonDocument doc(message);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

    // 添加消息长度前缀
    QByteArray packet;
    quint32 dataSize = jsonData.size();
    packet.append(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
    packet.append(jsonData);

    qint64 bytesWritten = m_socket->write(packet);
    if (bytesWritten == -1) {
        qWarning() << "Failed to send JSON message:" << m_socket->errorString();
        emit connectionError(m_socket->errorString());
        return false;
    }

    m_socket->flush();
    qDebug() << "Sent JSON message to server, size:" << bytesWritten << "bytes";
    return true;
}

bool JsonTcpClient::sendLogin(const QString &username, const QString &password)
{
    QJsonObject loginMsg;
    loginMsg["type"] = "login";
    loginMsg["timestamp"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    QJsonObject data;
    data["username"] = username;
    if (!password.isEmpty()) {
        data["password"] = password;
    }
    loginMsg["data"] = data;

    return sendMessage(loginMsg);
}

bool JsonTcpClient::sendChatMessage(const QString &content, const QString &sender)
{
    QJsonObject chatMsg;
    chatMsg["type"] = "chat";
    chatMsg["timestamp"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    QJsonObject data;
    data["content"] = content;
    data["sender"] = sender.isEmpty() ? "Client" : sender;
    chatMsg["data"] = data;

    return sendMessage(chatMsg);
}

bool JsonTcpClient::sendPing()
{
    QJsonObject pingMsg;
    pingMsg["type"] = "ping";
    pingMsg["timestamp"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    return sendMessage(pingMsg);
}

bool JsonTcpClient::requestTime()
{
    QJsonObject timeMsg;
    timeMsg["type"] = "get_time";
    timeMsg["timestamp"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    return sendMessage(timeMsg);
}

bool JsonTcpClient::isConnected() const
{
    return m_socket && m_socket->state() == QAbstractSocket::ConnectedState;
}

QString JsonTcpClient::getServerInfo() const
{
    if (!isConnected()) {
        return "Not connected";
    }
    return QString("%1:%2").arg(m_socket->peerAddress().toString()).arg(m_socket->peerPort());
}

void JsonTcpClient::onConnected()
{
    m_reconnectTimer->stop();

    QString serverInfo = getServerInfo();
    qInfo() << "Connected to JSON server:" << serverInfo;
    emit statusChanged(QString("Connected to %1").arg(serverInfo));
    emit connected();
}

void JsonTcpClient::onDisconnected()
{
    qInfo() << "Disconnected from JSON server";
    emit statusChanged("Disconnected");
    emit disconnected();

    if (m_autoReconnect && !m_host.isEmpty() && m_port > 0) {
        qInfo() << "Auto reconnect in" << m_reconnectInterval << "ms";
        m_reconnectTimer->start(m_reconnectInterval);
    }
}

void JsonTcpClient::onReadyRead()
{
    m_buffer.append(m_socket->readAll());
    parseJsonMessages();
}

void JsonTcpClient::parseJsonMessages()
{
    while (m_buffer.size() >= 4) {
        // 读取消息长度
        quint32 messageSize;
        memcpy(&messageSize, m_buffer.constData(), sizeof(quint32));

        // 检查是否收到完整消息
        if (m_buffer.size() < sizeof(quint32) + messageSize) {
            break;
        }

        // 提取JSON数据
        QByteArray jsonData = m_buffer.mid(sizeof(quint32), messageSize);

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qWarning() << "JSON parse error from server:" << parseError.errorString();
        } else if (doc.isObject()) {
            QJsonObject jsonObj = doc.object();
            qDebug() << "Received JSON message from server:" << QString::fromUtf8(jsonData);
            dispatchMessage(jsonObj);
        }

        // 移除已处理的数据
        m_buffer = m_buffer.mid(sizeof(quint32) + messageSize);
    }
}

void JsonTcpClient::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error)
    QString errorStr = m_socket->errorString();
    qWarning() << "Socket error:" << errorStr;
    emit connectionError(errorStr);
    emit statusChanged(QString("Error: %1").arg(errorStr));
}

void JsonTcpClient::onReconnectTimeout()
{
    if (!isConnected() && !m_host.isEmpty() && m_port > 0) {
        qInfo() << "Attempting to reconnect to" << m_host << ":" << m_port;
        emit statusChanged("Reconnecting...");
        connectToServer(m_host, m_port);
    }
}

void JsonTcpClient::dispatchMessage(QJsonObject obj)
{
    if(obj.contains("responseType"))
    {
        qDebug()<<"收到的消息没有responseType字段，无法处理";
        return;
    }

    int msg_type = obj["responseType"].toInt();
    RECEIVE_TYPE type =  (RECEIVE_TYPE)(msg_type);

    switch(type) {
    case RECEIVE_TYPE::BOOK: {
        emit bookMessageReceived(obj);
        break;
    }
    case RECEIVE_TYPE::VIDEO: {
        emit videoMessageReceived(obj);
        break;
    }
    default: {
        qDebug()<<QString("收到未知消息，消息类型为：%1").arg(type);
        break;
    }
    }
}
