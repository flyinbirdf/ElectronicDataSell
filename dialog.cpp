#include "dialog.h"
#include <QVBoxLayout>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , m_tcpClient(std::make_shared<JsonTcpClient>())
    , m_msgBuilder(std::make_shared<MessageBuilder>())
    , m_classifyBar(new QToolBar(this))
    , m_stackedWidget(new QStackedWidget(this))
    , m_bookSellWidget(nullptr)
    , m_videoSellWidget(nullptr)
    , m_myAccountWidget(nullptr)
{

    createToolbar();

    m_bookSellWidget = new BookSellWidget(m_tcpClient, m_msgBuilder, this);

    m_stackedWidget->addWidget(m_bookSellWidget);
    //m_stackedWidget->addWidget(m_videoSellWidget);
    //m_stackedWidget->addWidget(m_myAccountWidget);
    //m_stackedWidget->setCurrentWidget(m_bookSellWidget);

    QVBoxLayout *layout = new QVBoxLayout();

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->addStretch();
    hlayout->addWidget(m_classifyBar);
    hlayout->addStretch();
    layout->addLayout(hlayout);
    layout->addWidget(m_stackedWidget);

    this->setLayout(layout);

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
        if(m_bookSellWidget == nullptr)
        {
            m_bookSellWidget = new BookSellWidget(m_tcpClient, m_msgBuilder, this);
            m_stackedWidget->addWidget(m_videoSellWidget);
        }
        m_stackedWidget->setCurrentWidget(m_bookSellWidget);
        //m_videoAction->setChecked(false);
        //m_myAction->setChecked(false);
        m_bookAction->setChecked(true);

    }
    else if(action->text() == tr("VIDEO"))
    {
        if(m_bookSellWidget == nullptr)
        {
            m_videoSellWidget = new VideoSellWidget(m_tcpClient, m_msgBuilder, this);
            m_stackedWidget->addWidget(m_videoSellWidget);
        }
        m_stackedWidget->setCurrentWidget(m_videoSellWidget);
        //m_bookAction->setChecked(false);
        //m_myAction->setChecked(false);
        m_videoAction->setChecked(true);
    }
    else if(action->text() == tr("MY ACCOUNT"))
    {
        if(m_myAccountWidget == nullptr)
        {
            m_myAccountWidget = new MyAccountWidget(m_tcpClient, m_msgBuilder, this);
            m_stackedWidget->addWidget(m_myAccountWidget);
        }
        m_stackedWidget->setCurrentWidget(m_myAccountWidget);
        // m_stackedWidget->setCurrentWidget(m_myAccountWidget);
        // m_bookAction->setChecked(false);
        // m_videoAction->setChecked(false);
        m_myAction->setChecked(true);
    }

    setCheckedAction(action);
}
