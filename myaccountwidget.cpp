#include "myaccountwidget.h"
#include <QVBoxLayout>

MyAccountWidget::MyAccountWidget(std::shared_ptr<JsonTcpClient> tcpClient, std::shared_ptr<MessageBuilder> msgBuilder, QWidget *parent) :
    QWidget(parent)
    , m_tcpClient(tcpClient)
    , m_msgBuidler(msgBuilder)
    , m_toolBar(nullptr)
    , m_stackedWidget(nullptr)
    , m_basicInfoWidget(nullptr)
    , m_configWidget(nullptr)
    , m_loginWidget(nullptr)
    , m_basicInfoAction(nullptr)
    , m_configAction(nullptr)
    , m_loginAction(nullptr)
    , m_mainLayout(nullptr)
{
    setupUI();
}

void MyAccountWidget::setupUI()
{
    // 创建工具栏
    createToolbar();

    // 创建堆叠窗口
    m_stackedWidget = new QStackedWidget(this);

    // 创建三个界面
    m_basicInfoWidget = new UserBasicInfoWidget(this);
    m_configWidget = new UserConfigWidget(this);
    m_loginWidget = new LoginWidget(this);

    // 将界面添加到堆叠窗口
    m_stackedWidget->addWidget(m_basicInfoWidget);
    m_stackedWidget->addWidget(m_configWidget);
    m_stackedWidget->addWidget(m_loginWidget);

    // 设置默认显示用户基本信息界面
    m_stackedWidget->setCurrentWidget(m_basicInfoWidget);
    if (m_basicInfoAction) {
        m_basicInfoAction->setChecked(true);
    }

    // 创建主布局：左侧工具栏，右侧堆叠窗口
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setSpacing(10);
    m_mainLayout->setContentsMargins(10, 10, 10, 10);

    // 添加工具栏到左侧
    m_mainLayout->addWidget(m_toolBar);
    
    // 添加堆叠窗口到右侧
    m_mainLayout->addWidget(m_stackedWidget, 1);  // 设置拉伸因子为1，让右侧占据剩余空间

    setLayout(m_mainLayout);
    
    // 设置样式
    setupStyle();
}

void MyAccountWidget::createToolbar()
{
    m_toolBar = new QToolBar(this);
    // 设置工具栏为垂直方向
    m_toolBar->setOrientation(Qt::Vertical);
    // 设置工具栏样式 - 垂直工具栏只显示文字
    m_toolBar->setToolButtonStyle(Qt::ToolButtonTextOnly);
    // 设置工具栏宽度
    m_toolBar->setMinimumWidth(150);
    m_toolBar->setMaximumWidth(200);

    // 创建三个动作
    m_basicInfoAction = m_toolBar->addAction(tr("用户基本信息"));
    m_basicInfoAction->setCheckable(true);

    m_configAction = m_toolBar->addAction(tr("用户基本配置"));
    m_configAction->setCheckable(true);

    m_loginAction = m_toolBar->addAction(tr("用户登录"));
    m_loginAction->setCheckable(true);

    // 添加到动作向量
    m_actionVector.push_back(m_basicInfoAction);
    m_actionVector.push_back(m_configAction);
    m_actionVector.push_back(m_loginAction);

    // 连接信号和槽
    connect(m_toolBar, &QToolBar::actionTriggered, this, &MyAccountWidget::onActionTriggered);
}

void MyAccountWidget::onActionTriggered(QAction *action)
{
    if (action == m_basicInfoAction)
    {
        m_stackedWidget->setCurrentWidget(m_basicInfoWidget);
        setCheckedAction(action);
    }
    else if (action == m_configAction)
    {
        m_stackedWidget->setCurrentWidget(m_configWidget);
        setCheckedAction(action);
    }
    else if (action == m_loginAction)
    {
        m_stackedWidget->setCurrentWidget(m_loginWidget);
        setCheckedAction(action);
    }
}

void MyAccountWidget::setCheckedAction(QAction *curAction)
{
    for (auto *one : m_actionVector)
    {
        if (one == curAction)
        {
            one->setChecked(true);
        }
        else
        {
            one->setChecked(false);
        }
    }
}

void MyAccountWidget::setupStyle()
{
    setStyleSheet(
        "QWidget {"
        "    background-color: #E3F2FD;"
        "}"
        ""
        "QToolBar {"
        "    background-color: #BBDEFB;"
        "    border: none;"
        "    border-radius: 8px;"
        "    padding: 10px;"
        "    spacing: 5px;"
        "}"
        ""
        "QToolBar QToolButton {"
        "    background-color: transparent;"
        "    border: 2px solid transparent;"
        "    border-radius: 6px;"
        "    padding: 12px 15px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    color: #1976D2;"
        "    text-align: left;"
        "}"
        ""
        "QToolBar QToolButton:hover {"
        "    background-color: #90CAF9;"
        "    color: #0D47A1;"
        "}"
        ""
        "QToolBar QToolButton:checked {"
        "    background-color: #42A5F5;"
        "    color: white;"
        "    border: 2px solid #1976D2;"
        "}"
        ""
        "QToolBar QToolButton:checked:hover {"
        "    background-color: #2196F3;"
        "}"
        ""
        "QStackedWidget {"
        "    background-color: #E3F2FD;"
        "}"
    );
}


