#ifndef MYACCOUNTWIDGET_H
#define MYACCOUNTWIDGET_H

#include <QWidget>
#include <QVector>
#include <QToolBar>
#include <QGridLayout>
#include <tcpsocket.h>
#include <messagebuilder.h>

class MyAccountWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccountWidget(std::shared_ptr<JsonTcpClient> tcpClient, std::shared_ptr<MessageBuilder> msgBuilder, QWidget *parent = nullptr);


signals:

public slots:

private:
    std::shared_ptr<JsonTcpClient> m_tcpClient;
    std::shared_ptr<MessageBuilder> m_msgBuidler;
};

#endif // MYACCOUNTWIDGET_H


