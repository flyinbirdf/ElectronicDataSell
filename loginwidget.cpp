#include "loginwidget.h"
#include <QMessageBox>
#include <QDebug>

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
    , m_titleLabel(nullptr)
    , m_usernameEdit(nullptr)
    , m_passwordEdit(nullptr)
    , m_loginBtn(nullptr)
    , m_wechatLoginBtn(nullptr)
    , m_mainLayout(nullptr)
{
    setupUI();
    setupStyle();
}

void LoginWidget::setupUI()
{
    // 创建标题
    m_titleLabel = new QLabel(tr("登录"), this);
    m_titleLabel->setAlignment(Qt::AlignCenter);

    // 创建用户名输入框
    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText(tr("请输入用户名"));
    m_usernameEdit->setMinimumHeight(40);

    // 创建密码输入框
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText(tr("请输入密码"));
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setMinimumHeight(40);

    // 创建登录按钮
    m_loginBtn = new QPushButton(tr("登录"), this);
    m_loginBtn->setMinimumHeight(45);

    // 创建微信登录按钮
    m_wechatLoginBtn = new QPushButton(tr("微信登录"), this);
    m_wechatLoginBtn->setMinimumHeight(45);

    // 连接信号和槽
    connect(m_loginBtn, &QPushButton::clicked, this, &LoginWidget::onLoginClicked);
    connect(m_wechatLoginBtn, &QPushButton::clicked, this, &LoginWidget::onWechatLoginClicked);

    // 创建主布局
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(20);
    m_mainLayout->setContentsMargins(50, 80, 50, 80);

    // 添加控件到布局
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_titleLabel);
    m_mainLayout->addWidget(m_usernameEdit);
    m_mainLayout->addWidget(m_passwordEdit);
    m_mainLayout->addWidget(m_loginBtn);
    
    // 添加分割线或文字
    QLabel *dividerLabel = new QLabel(tr("或"), this);
    dividerLabel->setAlignment(Qt::AlignCenter);
    dividerLabel->setStyleSheet("color: #999999;");
    m_mainLayout->addWidget(dividerLabel);
    
    m_mainLayout->addWidget(m_wechatLoginBtn);
    m_mainLayout->addStretch();

    setLayout(m_mainLayout);
}

void LoginWidget::setupStyle()
{
    // 设置窗口样式
    setStyleSheet(
        "QWidget {"
        "    background-color: #f5f5f5;"
        "}"
        ""
        "QLabel#titleLabel {"
        "    font-size: 32px;"
        "    font-weight: bold;"
        "    color: #333333;"
        "    margin-bottom: 30px;"
        "}"
        ""
        "QLineEdit {"
        "    border: 2px solid #e0e0e0;"
        "    border-radius: 8px;"
        "    padding: 10px 15px;"
        "    font-size: 14px;"
        "    background-color: white;"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid #4CAF50;"
        "}"
        ""
        "QPushButton {"
        "    border: none;"
        "    border-radius: 8px;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "    color: white;"
        "    background-color: #4CAF50;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #3d8b40;"
        "}"
        ""
        "QPushButton#wechatBtn {"
        "    background-color: #07c160;"
        "}"
        "QPushButton#wechatBtn:hover {"
        "    background-color: #06ad56;"
        "}"
        "QPushButton#wechatBtn:pressed {"
        "    background-color: #059648;"
        "}"
    );

    // 为标题标签设置对象名以便样式化
    m_titleLabel->setObjectName("titleLabel");
    m_wechatLoginBtn->setObjectName("wechatBtn");
}

void LoginWidget::onLoginClicked()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();

    // 验证输入
    if (username.isEmpty()) {
        QMessageBox::warning(this, tr("提示"), tr("请输入用户名！"));
        m_usernameEdit->setFocus();
        return;
    }

    if (password.isEmpty()) {
        QMessageBox::warning(this, tr("提示"), tr("请输入密码！"));
        m_passwordEdit->setFocus();
        return;
    }

    // TODO: 这里应该调用实际的登录验证逻辑
    // 暂时只做简单的演示
    qDebug() << "尝试登录 - 用户名:" << username << "密码:" << password;

    // 如果登录成功，发出信号
    // emit loginSuccess(username);

    // 这里可以添加实际的网络请求来验证登录
    // 示例：假设登录成功
    QMessageBox::information(this, tr("提示"), tr("登录功能待实现！\n用户名: %1").arg(username));
    
    // 登录成功后发出信号
    // emit loginSuccess(username);
}

void LoginWidget::onWechatLoginClicked()
{
    qDebug() << "微信登录被点击";
    
    // 发出微信登录请求信号
    emit wechatLoginRequested();
    
    // TODO: 实现微信登录逻辑
    QMessageBox::information(this, tr("提示"), tr("微信登录功能待实现！"));
}
