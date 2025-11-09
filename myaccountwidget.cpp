#include "myaccountwidget.h"
#include <QVBoxLayout>

MyAccountWidget::MyAccountWidget(QWidget *parent) : QWidget(parent), m_typeBar(new QToolBar(this))
{
    for(int i = 0; i < ROW; i++)
    {
        for(int j = 0; j < COLUMN; j++)
        {
            BookInfo *book = new BookInfo(BookType::SHOW_MODE, this);
            book->setInfo(QPixmap(":/img/picdemo.jpg"), "Book Name", "This is demo for Book Name", 15, QString("111"));
            m_bookVector.push_back(book);
        }
    }

    for(int i = 0; i < ROW; i++)
    {
        for(int j = 0; j < COLUMN; j++)
        {
            VideoInfo *video = new VideoInfo(VideoType::SHOW_MODE, this);
            video->setInfo(QPixmap(":/img/picdemo.jpg"), "Video Name", "This is demo for Video Name", 15, QString("222"));
            m_videoVector.push_back(video);
            video->hide();
        }
    }

    QVBoxLayout *layout = new QVBoxLayout(this);

    createToolbar();
    layout->addWidget(m_typeBar);

    m_gridLayout = new QGridLayout(this);

    for(int i = 0; i < ROW; i++)
    {
        for(int j = 0; j < COLUMN; j++)
        {
            if(i * COLUMN + j < m_bookVector.size())
            {
                m_gridLayout->addWidget(m_bookVector[i*COLUMN+j], i, j);
            }
        }
    }

    layout->addLayout(m_gridLayout);

    this->setLayout(layout);
}

void MyAccountWidget::createToolbar()
{
    m_typeBar->setOrientation(Qt::Orientation::Horizontal);
    m_bookAction = m_typeBar->addAction(tr("BOOK"));
    m_bookAction->setCheckable(true);
    m_bookAction->setChecked(true);
    m_videoAction = m_typeBar->addAction(tr("VIDEO"));
    m_videoAction->setCheckable(true);

    connect(m_typeBar, &QToolBar::actionTriggered, this, &MyAccountWidget::actionTriggered);
}

void MyAccountWidget::actionTriggered(QAction *action)
{
    if(action->text() == tr("BOOK"))
    {
        m_videoAction->setChecked(false);
        showBook();

    }
    else if(action->text() == tr("VIDEO"))
    {
        m_bookAction->setChecked(false);
        showVideo();
    }
}

void MyAccountWidget::showBook()
{
    for(int i = 0; i < m_videoVector.size(); i++)
    {
        m_videoVector[i]->hide();
        m_gridLayout->removeWidget(m_videoVector[i]);
    }

    for(int i = 0; i < ROW; i++)
    {
        for(int j = 0; j < COLUMN; j++)
        {
            if(i * COLUMN + j < m_bookVector.size())
            {
                m_gridLayout->addWidget(m_bookVector[i*COLUMN+j], i, j);
                m_bookVector[i*COLUMN+j]->show();
            }
        }
    }
}

void MyAccountWidget::showVideo()
{
    for(int i = 0; i < m_bookVector.size(); i++)
    {
        m_bookVector[i]->hide();
        m_gridLayout->removeWidget(m_bookVector[i]);
    }

    for(int i = 0; i < ROW; i++)
    {
        for(int j = 0; j < COLUMN; j++)
        {
            if(i * COLUMN + j < m_videoVector.size())
            {
                m_gridLayout->addWidget(m_videoVector[i*COLUMN+j], i, j);
                m_videoVector[i*COLUMN+j]->show();
            }
        }
    }
}
