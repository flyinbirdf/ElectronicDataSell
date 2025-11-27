#ifndef VIDEOSELLWIDGET_H
#define VIDEOSELLWIDGET_H

#include <QWidget>
#include "tcpsocket.h"
#include "messagebuilder.h"
#include <memory.h>

class VideoSellWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VideoSellWidget(std::shared_ptr<JsonTcpClient> tcpClient, std::shared_ptr<MessageBuilder> msgBuilder, QWidget *parent = nullptr);

signals:

public slots:

private:
    std::shared_ptr<JsonTcpClient> m_tcpClient;
    std::shared_ptr<MessageBuilder> m_messageBuilder;
};

#endif // VIDEOSELLWIDGET_H
