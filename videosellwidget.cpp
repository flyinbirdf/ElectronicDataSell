#include "videosellwidget.h"
#include <QDebug>

VideoSellWidget::VideoSellWidget(std::shared_ptr<JsonTcpClient> tcpClient, std::shared_ptr<MessageBuilder> msgBuilder, QWidget *parent) 
    : QWidget(parent)
    , m_page(0)
    , m_tcpClient(tcpClient)
    , m_messageBuilder(msgBuilder)
    , m_contentWidget(new QWidget(this))
    , m_searchBar(new SearchBar(SearchBar::SEARCH_TYPE::VIDEO, tcpClient, msgBuilder, this))
    , m_content(new QScrollArea(this))
    , m_showTypeComb(new QComboBox(this))
    , m_prevBtn(new QPushButton(tr("Previous"), this))
    , m_nextBtn(new QPushButton(tr("Next"), this))
    , m_infoLabel(new QLabel(QString(tr("Current Page: %1")).arg(m_page), this))
    , m_needWait(false)
{
    // 界面设置
    setupUI();
    // 连接设置
    setupConnection();
    // 设置样式
    setupStyle();
    // 刚启动，获取推荐页面
    //QJsonObject msg = m_messageBuilder->reqestFavoritePageBookInfo(0);
    //m_tcpClient->sendMessage(msg);
    //m_needWait = true;
}

void VideoSellWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    // 显示类型设置
    QStringList searchTypes = { QString(tr("ABSTRACT")), QString(tr("DETAIL")) };
    m_showTypeComb->addItems(searchTypes);
    m_showTypeComb->setCurrentIndex(1);
    m_showType = VideoInfo::VideoType::DETAIL_MODE;
    onCurrentIndexChanged(1);
    // 搜索栏添加
    QHBoxLayout *searchLayout = new QHBoxLayout(this);
    searchLayout->addStretch();
    searchLayout->addWidget(m_showTypeComb);
    searchLayout->addWidget(m_searchBar);
    searchLayout->addStretch();
    m_mainLayout->addLayout(searchLayout);
    // 显示内容设置
    QGridLayout *contentLayout = new QGridLayout(m_contentWidget);
    for(int i = 0; i < ROW; i++)
    {
        for(int j = 0; j < COLUMN; j++)
        {
            VideoInfo* video = new VideoInfo(VideoInfo::VideoType::DETAIL_MODE, m_contentWidget);
            m_videoInfoVector.push_back(video);
            video->setInfo(QString(":/img/picdemo.jpg"), QString("LOADING"), "This is demo for Book Name", 0, QString(""));
            contentLayout->addWidget(video, i, j);
        }
    }

    contentLayout->setSpacing(30);
    contentLayout->setContentsMargins(20,20,20,20);
    m_contentWidget->setLayout(contentLayout);
    m_content->setWidget(m_contentWidget);
    m_mainLayout->addWidget(m_content);

    // 页面切换按钮和显示
    QHBoxLayout *lowerLayout = new QHBoxLayout(this);
    lowerLayout->addStretch();
    lowerLayout->addWidget(m_prevBtn);
    lowerLayout->addWidget(m_infoLabel);
    lowerLayout->addWidget(m_nextBtn);
    lowerLayout->addStretch();
    m_mainLayout->addLayout(lowerLayout);

    this->setLayout(m_mainLayout);
}

void VideoSellWidget::setupConnection()
{
    // 每个视频需要有显示细节和购买调用接口
    for(auto *item : m_videoInfoVector)
    {
        connect(item, &VideoInfo::showDetail, this, &VideoSellWidget::showVideoDetail);
        connect(item, &VideoInfo::buyVideo, this, &VideoSellWidget::onBuyVideo);
    }
    connect(m_showTypeComb, &QComboBox::currentIndexChanged, this, &VideoSellWidget::onCurrentIndexChanged);
    // 收到服务器返回的消息
    connect(m_tcpClient.get(),  &JsonTcpClient::videoMessageReceived, this, &VideoSellWidget::refreshPage);
    // 显示下一页
    connect(m_prevBtn, &QPushButton::clicked, this, [=](bool) {
        if(m_page == 0) {
            m_infoLabel->setText(QString(tr("Current Page: %1, No Previous Page")).arg(m_page));
            return;
        }
        auto msg = m_messageBuilder->reqestPageBookInfo(m_page-1, m_searchBar->text());
        m_tcpClient->sendMessage(msg);
    });
    // 显示上一页
    connect(m_nextBtn, &QPushButton::clicked, this, [=](bool) {
        auto msg = m_messageBuilder->reqestPageBookInfo(m_page+1, m_searchBar->text());
        m_tcpClient->sendMessage(msg);
    });

}

void VideoSellWidget::setupStyle()
{
    setStyleSheet(
        "QWidget {"
        "    background-color: #E3F2FD;"
        "}"
        ""
        "QScrollArea {"
        "    background-color: #E3F2FD;"
        "    border: none;"
        "}"
        ""
        "QComboBox {"
        "    border: 2px solid #BBDEFB;"
        "    border-radius: 8px;"
        "    padding: 8px 15px;"
        "    font-size: 14px;"
        "    background-color: white;"
        "    color: #333333;"
        "    min-width: 120px;"
        "}"
        "QComboBox:focus {"
        "    border: 2px solid #2196F3;"
        "    background-color: #F5FAFF;"
        "}"
        "QComboBox::drop-down {"
        "    border: none;"
        "    background-color: #2196F3;"
        "    border-radius: 0 8px 8px 0;"
        "    width: 30px;"
        "}"
        "QComboBox::down-arrow {"
        "    image: none;"
        "    border-left: 5px solid transparent;"
        "    border-right: 5px solid transparent;"
        "    border-top: 6px solid white;"
        "    width: 0;"
        "    height: 0;"
        "}"
        "QComboBox QAbstractItemView {"
        "    border: 2px solid #BBDEFB;"
        "    border-radius: 8px;"
        "    selection-background-color: #2196F3;"
        "    selection-color: white;"
        "    background-color: white;"
        "}"
        ""
        "QPushButton {"
        "    border: none;"
        "    border-radius: 8px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    color: white;"
        "    background-color: #2196F3;"
        "    padding: 8px 20px;"
        "    min-width: 80px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1565C0;"
        "}"
        ""
        "QLabel {"
        "    font-size: 14px;"
        "    color: #1976D2;"
        "    font-weight: bold;"
        "}"
    );
}

void VideoSellWidget::refreshPage(QJsonObject obj)
{
    int page = obj["page"].toInt();

    QJsonArray items = obj["items"].toArray();

    if(PAGE_SIZE != items.size())
    {
        qDebug()<<QString("收到：%1，当前页大小：%2。").arg(items.size()).arg(PAGE_SIZE);
    }
    m_infoLabel->setText(QString(tr("Current Page: %1")).arg(m_page));
    for(int i = 0; i < m_videoInfoVector.size(); i++)
    {
        if(i < items.size()) {
            QJsonObject item = items[i].toObject();
            m_videoInfoVector[i]->setInfo(item["picture"].toString(), item["title"].toString(), item["detail"].toString(), item["price"].toInt(), item["id"].toString());
            m_videoInfoVector[i]->show();
        } else {
            m_videoInfoVector[i]->hide();
        }
    }
    if(page == m_page) {
        m_infoLabel->setText(QString(tr("Current Page: %1, No Next Page")).arg(m_page));
    } else {
        m_page = page;
        m_infoLabel->setText(QString(tr("Current Page: %1")).arg(m_page));
    }
}

void VideoSellWidget::onBuyVideo(QJsonObject obj)
{

}

void VideoSellWidget::onCurrentIndexChanged(int index)
{
    switch(index)
    {
    case 0:
    {
        m_showType = VideoInfo::VideoType::ABSTRACT_MODE;
        break;
    }
    case 1:
    {
        m_showType = VideoInfo::VideoType::DETAIL_MODE;
        break;
    }
    default:
    {
        m_showType = VideoInfo::VideoType::DETAIL_MODE;
    }
    }

    for(auto &one : m_videoInfoVector)
    {
        one->setShowType(m_showType);
    }

    m_contentWidget->updateGeometry();
    m_contentWidget->adjustSize();
}
