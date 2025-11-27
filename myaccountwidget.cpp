#include "myaccountwidget.h"
#include <QVBoxLayout>

MyAccountWidget::MyAccountWidget(std::shared_ptr<JsonTcpClient> tcpClient, std::shared_ptr<MessageBuilder> msgBuilder,QWidget *parent) :
    QWidget(parent)
    , m_tcpClient(tcpClient)
    , m_msgBuidler(msgBuilder)
{

}


