#ifndef VIDEOSELLWIDGET_H
#define VIDEOSELLWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollArea>
#include <QComboBox>
#include "tcpsocket.h"
#include "messagebuilder.h"
#include <memory.h>
#include "videoinfo.h"
#include "searchbar.h"

class VideoSellWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VideoSellWidget(std::shared_ptr<JsonTcpClient> tcpClient, std::shared_ptr<MessageBuilder> msgBuilder, QWidget *parent = nullptr);

signals:
    void showVideoDetail(QString fileName);

public slots:
    void onBuyVideo(QJsonObject obj);
    void onCurrentIndexChanged(int index);
    void refreshPage(QJsonObject obj);

private:
    void setupUI();
    // 后期再生成样式
    void setupStyle() {};
    void setupConnection();

private:
    int m_page;
    std::shared_ptr<JsonTcpClient> m_tcpClient;
    std::shared_ptr<MessageBuilder> m_messageBuilder;
    QVector<VideoInfo*> m_videoInfoVector;
    QVBoxLayout *m_mainLayout;
    QWidget *m_contentWidget;
    SearchBar *m_searchBar;
    QScrollArea *m_content;
    QComboBox *m_showTypeComb;
    VideoInfo::VideoType m_showType;
    QPushButton *m_prevBtn;
    QPushButton *m_nextBtn;
    QLabel *m_infoLabel;
    // 防止按键按下太快，如果有服务器请求没有返回，暂时不能再按
    bool m_needWait;

private:
    const int COLUMN = 8;
    const int ROW = 7;
    const int PAGE_SIZE = 56;
};

#endif // VIDEOSELLWIDGET_H
