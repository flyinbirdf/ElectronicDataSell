#include "dialog.h"
#include <QVBoxLayout>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , m_classifyBar(new QToolBar(this))
    , m_stackedWidget(new QStackedWidget(this))
    , m_bookSellWidget(new BookSellWidget(this))
    , m_videoSellWidget(new VideoSellWidget(this))
    //, m_myAccountWidget(new MyAccountWidget(this))
{
    createToolbar();

    m_stackedWidget->addWidget(m_bookSellWidget);
    m_stackedWidget->addWidget(m_videoSellWidget);
    //m_stackedWidget->addWidget(m_myAccountWidget);
    m_stackedWidget->setCurrentWidget(m_bookSellWidget);

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
    m_bookAction->setChecked(true);
    m_videoAction = m_classifyBar->addAction(tr("VIDEO"));
    m_videoAction->setCheckable(true);
    m_myAction = m_classifyBar->addAction(tr("MY ACCOUNT"));
    m_myAction->setCheckable(true);

    connect(m_classifyBar, &QToolBar::actionTriggered, this, &Dialog::actionTriggered);
}

void Dialog::actionTriggered(QAction *action)
{
    if(action->text() == tr("BOOK"))
    {
        m_stackedWidget->setCurrentWidget(m_bookSellWidget);
        m_videoAction->setChecked(false);
        m_myAction->setChecked(false);

    }
    else if(action->text() == tr("VIDEO"))
    {
        m_stackedWidget->setCurrentWidget(m_videoSellWidget);
        m_bookAction->setChecked(false);
        m_myAction->setChecked(false);
    }
    else if(action->text() == tr("MY ACCOUNT"))
    {
        // m_stackedWidget->setCurrentWidget(m_myAccountWidget);
        // m_bookAction->setChecked(false);
        // m_videoAction->setChecked(false);
    }
}
