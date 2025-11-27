#include "booksellwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include "bookinfo.h"
#include <QDebug>

BookSellWidget::BookSellWidget(std::shared_ptr<JsonTcpClient> tcpClient, std::shared_ptr<MessageBuilder> msgBuilder, QWidget *parent) : QWidget(parent)
    , m_page(1)
    , m_searchBar(new SearchBar(SearchBar::BOOK, tcpClient, msgBuilder, this))
    , m_content(new QScrollArea(this))
    , m_contentWidget(new QWidget(this))
    , m_prevBtn(new QPushButton(tr("PREVIOUS"), this))
    , m_nextBtn(new QPushButton(tr("NEXT"), this))
    , m_infoLabel(new QLabel(QString(tr("Current Page: %1")).arg(m_page), this))
    , m_showPdfViewer(nullptr)
    , m_tcpClient(tcpClient)
    , m_msgBuilder(msgBuilder)
    , m_showTypeComb(new QComboBox(this))
{
    QGridLayout *contentLayout = new QGridLayout(m_contentWidget);
    QVector<std::shared_ptr<BookInfo*>> pageitems;
    for(int i = 0; i < ROW; i++)
    {
        for(int j = 0; j < COLUMN; j++)
        {
            BookInfo* book = new BookInfo(BookInfo::BookType::DETAIL_MODE, m_contentWidget);
            m_bookInfoVector.push_back(book);
            book->setInfo(QString(":/img/picdemo.jpg"), QString("LOADING"), "This is demo for Book Name", 0, QString(""));
            contentLayout->addWidget(book, i, j);
            connect(book, &BookInfo::showDetail, this, &BookSellWidget::showBookDetail);
        }
    }

    contentLayout->setSpacing(10);
    contentLayout->setContentsMargins(10,10,10,10);
    m_contentWidget->setLayout(contentLayout);
    m_content->setWidget(m_contentWidget);

    QVBoxLayout *mainlayout = new QVBoxLayout(this);

    QStringList searchTypes = { QString(tr("ABSTRACT")), QString(tr("DETAIL")) };
    m_showTypeComb->addItems(searchTypes);
    connect(m_showTypeComb, &QComboBox::currentIndexChanged, this, [=](int index) {
        switch(index)
        {
            case 0:
            {
                m_showType = BookInfo::BookType::ABSTRACT_MODE;
                break;
            }
            case 1:
            {
                m_showType = BookInfo::BookType::DETAIL_MODE;
                break;
            }
            default:
            {
                m_showType = BookInfo::BookType::DETAIL_MODE;
            }
        }

        for(auto &one : m_bookInfoVector)
        {
            one->setShowType(m_showType);
        }
        //m_contentWidget->layout()->invalidate();
        //m_contentWidget->layout()->activate();
        m_contentWidget->updateGeometry();
        m_contentWidget->adjustSize();
    });
    m_showTypeComb->setCurrentIndex(1);

    QHBoxLayout *searchLayout = new QHBoxLayout(this);
    searchLayout->addStretch();
    searchLayout->addWidget(m_showTypeComb);
    searchLayout->addWidget(m_searchBar);
    searchLayout->addStretch();

    mainlayout->addLayout(searchLayout);
    mainlayout->addWidget(m_content);

    QHBoxLayout *lowerLayout = new QHBoxLayout(this);
    lowerLayout->addStretch();
    lowerLayout->addWidget(m_prevBtn);
    lowerLayout->addWidget(m_infoLabel);
    lowerLayout->addWidget(m_nextBtn);
    lowerLayout->addStretch();

    mainlayout->addLayout(lowerLayout);
    this->setLayout(mainlayout);

    connect(m_tcpClient.get(),  &JsonTcpClient::bookMessageReceived, this, &BookSellWidget::refreshPage);

    connect(m_prevBtn, &QPushButton::clicked, this, [=](bool) {
        if(m_page == 0) {
            m_infoLabel->setText(QString(tr("Current Page: %1, No Previous Page")).arg(m_page));
            return;
        }
        auto msg = m_msgBuilder->reqestPageBookInfo(m_page-1, m_searchBar->text());
        m_tcpClient->sendMessage(msg);
    });

    connect(m_prevBtn, &QPushButton::clicked, this, [=](bool) {
        auto msg = m_msgBuilder->reqestPageBookInfo(m_page+1, m_searchBar->text());
        m_tcpClient->sendMessage(msg);
    });

    QJsonObject msg = m_msgBuilder->reqestFavoritePageBookInfo(0);
    m_tcpClient->sendMessage(msg);
}

void BookSellWidget::showBookDetail(QString name, BookInfo::BookType type)
{
    if(m_showPdfViewer == nullptr) {
        m_showPdfViewer = new PDFViewer(this);
    }

    m_showPdfViewer->loadPDF(name);
    m_showPdfViewer->show();
}

void BookSellWidget::refreshPage(QJsonObject obj)
{
    int page = obj["page"].toInt();


    QJsonArray items = obj["items"].toArray();

    if(PAGE_SIZE != items.size())
    {
        qDebug()<<QString("收到：%1，当前页大小：%2。").arg(items.size()).arg(PAGE_SIZE);
    }
    m_infoLabel->setText(QString(tr("Current Page: %1")).arg(m_page));
    for(int i = 0; i < m_bookInfoVector.size(); i++)
    {
        if(i < items.size()) {
            QJsonObject item = items[i].toObject();
            m_bookInfoVector[i]->setInfo(item["picture"].toString(), item["title"].toString(), item["detail"].toString(), item["price"].toInt(), item["id"].toString());
        } else {
            m_bookInfoVector[i]->hide();
        }
    }
    if(page == m_page) {
    m_infoLabel->setText(QString(tr("Current Page: %1, No Next Page")).arg(m_page));
    } else {
        m_page = page;
        m_infoLabel->setText(QString(tr("Current Page: %1")).arg(m_page));
    }
}
