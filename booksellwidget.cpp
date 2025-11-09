#include "booksellwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include "bookinfo.h"
#include <QDebug>

BookSellWidget::BookSellWidget(QWidget *parent) : QWidget(parent)
  , m_searchBar(new SearchBar("BOOK", this))
  , m_content(new QScrollArea(this))
  , m_contentWidget(new QWidget(this))
  , m_showPdfDoc(new ShowPdfDoc(this))
{
    QGridLayout *contentLayout = new QGridLayout(m_contentWidget);

    for(int i = 0; i < ROW; i++)
    {
        for(int j = 0; j < COLUMN; j++)
        {
            BookInfo *book = new BookInfo(BookType::SELL_MODE, m_contentWidget);
            book->setInfo(QPixmap(":/img/picdemo.jpg"), "Book Name", "This is demo for Book Name", 15, QString("111"));
            m_bookInfo.push_back(book);
            contentLayout->addWidget(book, i, j);
            connect(book, &BookInfo::showDetail, this, &BookSellWidget::showBookDetail);
        }
    }

    m_contentWidget->setLayout(contentLayout);
    m_contentWidget->setFixedSize(m_bookInfo[0]->width() * COLUMN + 50 * COLUMN, m_bookInfo[0]->height() * ROW + 5 * ROW);
    m_content->setWidget(m_contentWidget);

    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(m_searchBar);
    layout->addWidget(m_content);
    layout->addWidget(m_showPdfDoc);

    this->setLayout(layout);

    //m_showPdfDoc->hide();
}

void BookSellWidget::showBookDetail(QString name, BookType type)
{
    if(type != BookType::SELL_MODE)
    {
        qDebug()<<"Not sell mode in book sell widget";
        return;
    }

    m_showPdfDoc->showPdfDocument(name);
    m_showPdfDoc->show();
}

