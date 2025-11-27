#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>

class JsonTcpClient : public QObject
{
    Q_OBJECT
public:
    typedef enum {
        BOOK,
        VIDEO
    } RECEIVE_TYPE;
public:
    explicit JsonTcpClient(QObject *parent = nullptr);
    ~JsonTcpClient();

    // 连接到服务器
    bool connectToServer(const QString &host, quint16 port);
    // 断开连接
    void disconnectFromServer();
    // 发送JSON消息
    bool sendMessage(const QJsonObject &message);
    // 检查连接状态
    bool isConnected() const;
    // 获取服务器信息
    QString getServerInfo() const;

    // 便捷方法：发送特定类型的消息
    bool sendLogin(const QString &username, const QString &password = "");
    bool sendChatMessage(const QString &content, const QString &sender = "");
    bool sendPing();
    bool requestTime();

signals:
    void connected();
    void disconnected();
    //void messageReceived(const QJsonObject &message);
    void connectionError(const QString &errorString);
    void statusChanged(const QString &status);

    // 业务逻辑信号
    void bookMessageReceived(QJsonObject obj);
    void videoMessageReceived(QJsonObject obj);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onSocketError(QAbstractSocket::SocketError error);
    void onReconnectTimeout();

private:
    void dispatchMessage(QJsonObject obj);

private:
    QTcpSocket *m_socket;
    QTimer *m_reconnectTimer;
    QString m_host;
    quint16 m_port;
    bool m_autoReconnect;
    int m_reconnectInterval;

    QByteArray m_buffer;

    void initSocket();
    void parseJsonMessages();
};

#endif // TCPSOCKET_H
