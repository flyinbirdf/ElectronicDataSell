#include "dialog.h"
#include <QVBoxLayout>
#include <QDebug>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , m_tcpClient(std::make_shared<JsonTcpClient>())
    , m_msgBuilder(std::make_shared<MessageBuilder>())
    , m_classifyBar(new QToolBar(this))
    , m_stackedWidget(new QStackedWidget(this))
    , m_bookSellWidget(nullptr)
    , m_videoSellWidget(nullptr)
    , m_myAccountWidget(nullptr)
    , m_pdfViewer(nullptr)
{

    createToolbar();

    m_bookSellWidget = new BookSellWidget(m_tcpClient, m_msgBuilder, this);

    m_stackedWidget->addWidget(m_bookSellWidget);

    QVBoxLayout *layout = new QVBoxLayout();

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addStretch();
    hlayout->addWidget(m_classifyBar);
    hlayout->addStretch();
    layout->addLayout(hlayout);
    layout->addWidget(m_stackedWidget);

    this->setLayout(layout);

    connect(m_bookSellWidget, &BookSellWidget::showBookDetail, this, [=](QString name, BookInfo::BookType type) {
        qDebug()<<"Enter pdfViewer show";
        if(m_pdfViewer == nullptr) {
            m_pdfViewer = new PDFViewer(this);
            m_stackedWidget->addWidget(m_pdfViewer);
            connect(m_pdfViewer, &PDFViewer::returnBtnClicked, this, &Dialog::onPdfViewerReturnBtnCliced);

        }
        m_pdfViewer->loadPDF(name);
        m_stackedWidget->setCurrentWidget(m_pdfViewer);
    });
    // 设置窗口标题
    setWindowTitle(tr("electronic data sell system"));

    // 设置窗口标志 - 包含最小化、最大化、关闭按钮
    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint |
                  Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
}

Dialog::~Dialog()
{
}

void Dialog::createToolbar()
{
    m_classifyBar->setOrientation(Qt::Orientation::Horizontal);
    m_bookAction = m_classifyBar->addAction(tr("BOOK"));
    m_bookAction->setCheckable(true);
    m_videoAction = m_classifyBar->addAction(tr("VIDEO"));
    m_videoAction->setCheckable(true);
    m_myAction = m_classifyBar->addAction(tr("MY ACCOUNT"));
    m_myAction->setCheckable(true);

    m_actionVector.push_back(m_bookAction);
    m_actionVector.push_back(m_videoAction);
    m_actionVector.push_back(m_myAction);

    connect(m_classifyBar, &QToolBar::actionTriggered, this, &Dialog::actionTriggered);

    m_bookAction->setChecked(true);
}

void Dialog::setCheckedAction(QAction *curAction)
{
    for(auto *one : m_actionVector)
    {
        if(one == curAction)
        {
            one->setChecked(true);
        }
        else
        {
            one->setChecked(false);
        }
    }
}

void Dialog::actionTriggered(QAction *action)
{
    if(action->text() == tr("BOOK"))
    {
        m_stackedWidget->setCurrentWidget(m_bookSellWidget);
    }
    else if(action->text() == tr("VIDEO"))
    {
        if(m_videoSellWidget == nullptr)
        {
            m_videoSellWidget = new VideoSellWidget(m_tcpClient, m_msgBuilder, this);
            m_stackedWidget->addWidget(m_videoSellWidget);

            connect(m_videoSellWidget, &VideoSellWidget::showVideoDetail, this, [=](QString name) {
                qDebug()<<"Enter pdfViewer show";
                if(m_videoPlayer == nullptr) {
                    m_videoPlayer = new VideoPlayerWidget(this);
                    m_stackedWidget->addWidget(m_videoPlayer);
                    //connect(m_videoPlayer, &PDFViewer::returnBtnClicked, this, &Dialog::onPdfViewerReturnBtnCliced);

                }
                m_videoPlayer->loadVideo(name);
                m_stackedWidget->setCurrentWidget(m_videoPlayer);
            });
        }
        m_stackedWidget->setCurrentWidget(m_videoSellWidget);
    }
    else if(action->text() == tr("MY ACCOUNT"))
    {
        if(m_myAccountWidget == nullptr)
        {
            m_myAccountWidget = new MyAccountWidget(m_tcpClient, m_msgBuilder, this);
            m_stackedWidget->addWidget(m_myAccountWidget);
        }
        m_stackedWidget->setCurrentWidget(m_myAccountWidget);
    }

    setCheckedAction(action);
}

void Dialog::onPdfViewerReturnBtnCliced()
{
    m_stackedWidget->setCurrentWidget(m_bookSellWidget);
}
