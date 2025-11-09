#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QHostAddress>

class TcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = nullptr);
    bool connectToServer(QHostAddress ip, quint16 port);
    void sendSearchText(QString text);
signals:

public slots:
    void readData();
private:
    QTcpSocket *m_socket;
    QHostAddress m_serverAddress;
    quint16 m_serverPort;
};

#endif // TCPSOCKET_H
