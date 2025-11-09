#include "tcpsocket.h"
#include <QSettings>

TcpSocket::TcpSocket(QObject *parent) : QObject(parent)
  , m_socket(new QTcpSocket(this))

{
    QSettings settings("config.ini");
    settings.beginGroup("SERVER");
    QString ip = settings.value("IP").toString();
    quint16 port = settings.value("PORT").toString().toUShort();
    settings.endGroup();

    connectToServer(QHostAddress(ip), port);

    connect(m_socket, &QTcpSocket::readyRead, this, &TcpSocket::readData);
}

bool TcpSocket::connectToServer(QHostAddress ip, quint16 port)
{
    m_socket->bind(QHostAddress::LocalHost, port);

    m_socket->connectToHost(ip, port);

    return true;
}

void TcpSocket::sendSearchText(QString text)
{
    QJsonObject obj;
    obj["search"] = text;
    // 使用QJsonDocument将QJsonObject转换为JSON格式的QByteArray
    QJsonDocument jsonDoc(obj);
    QByteArray jsonBytes = jsonDoc.toJson(); // 转换为UTF-8编码的字节数组

    m_socket->write(jsonBytes);

}

void TcpSocket::readData()
{
    QByteArray data = m_socket->readAll();

    QJsonParseError JsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &JsonError);

    if(JsonError.error == QJsonParseError::NoError)
    {
        if(jsonDoc.isObject())
        {
            QJsonObject jsonObject = jsonDoc.object();
        }
        else
        {
            qDebug()<<"JSON document is not an object";
        }
    }
    else
    {
        qDebug()<< "JSON parsing error: " << JsonError.errorString();
    }
}
