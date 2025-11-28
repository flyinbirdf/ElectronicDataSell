#include "userbasicinfowidget.h"
#include <QFormLayout>
#include <QMessageBox>

UserBasicInfoWidget::UserBasicInfoWidget(QWidget *parent)
    : QWidget(parent)
    , m_titleLabel(nullptr)
    , m_usernameLabel(nullptr)
    , m_usernameEdit(nullptr)
    , m_emailLabel(nullptr)
    , m_emailEdit(nullptr)
    , m_phoneLabel(nullptr)
    , m_phoneEdit(nullptr)
    , m_nicknameLabel(nullptr)
    , m_nicknameEdit(nullptr)
    , m_saveBtn(nullptr)
    , m_mainLayout(nullptr)
{
    setupUI();
    setupStyle();
}

void UserBasicInfoWidget::setupUI()
{
    // 创建标题
    m_titleLabel = new QLabel(tr("用户基本信息"), this);
    m_titleLabel->setAlignment(Qt::AlignCenter);

    // 创建表单标签和输入框
    m_usernameLabel = new QLabel(tr("用户名:"), this);
    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText(tr("请输入用户名"));
    m_usernameEdit->setMinimumHeight(35);
    m_usernameEdit->setReadOnly(true);  // 用户名通常不允许修改

    m_emailLabel = new QLabel(tr("邮箱:"), this);
    m_emailEdit = new QLineEdit(this);
    m_emailEdit->setPlaceholderText(tr("请输入邮箱地址"));
    m_emailEdit->setMinimumHeight(35);

    m_phoneLabel = new QLabel(tr("手机号:"), this);
    m_phoneEdit = new QLineEdit(this);
    m_phoneEdit->setPlaceholderText(tr("请输入手机号"));
    m_phoneEdit->setMinimumHeight(35);

    m_nicknameLabel = new QLabel(tr("昵称:"), this);
    m_nicknameEdit = new QLineEdit(this);
    m_nicknameEdit->setPlaceholderText(tr("请输入昵称"));
    m_nicknameEdit->setMinimumHeight(35);

    // 创建保存按钮
    m_saveBtn = new QPushButton(tr("保存"), this);
    m_saveBtn->setMinimumHeight(40);
    m_saveBtn->setMinimumWidth(120);

    // 连接保存按钮信号
    connect(m_saveBtn, &QPushButton::clicked, this, [this]() {
        QMessageBox::information(this, tr("提示"), tr("保存成功！"));
        // TODO: 实现实际的保存逻辑
    });

    // 创建表单布局
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(15);
    formLayout->setLabelAlignment(Qt::AlignRight);
    formLayout->addRow(m_usernameLabel, m_usernameEdit);
    formLayout->addRow(m_emailLabel, m_emailEdit);
    formLayout->addRow(m_phoneLabel, m_phoneEdit);
    formLayout->addRow(m_nicknameLabel, m_nicknameEdit);

    // 创建主布局
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(30);
    m_mainLayout->setContentsMargins(50, 50, 50, 50);

    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_titleLabel);
    m_mainLayout->addLayout(formLayout);

    // 添加按钮布局
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(m_saveBtn);
    btnLayout->addStretch();
    m_mainLayout->addLayout(btnLayout);
    m_mainLayout->addStretch();

    setLayout(m_mainLayout);
}

void UserBasicInfoWidget::setupStyle()
{
    setStyleSheet(
        "QWidget {"
        "    background-color: #E3F2FD;"
        "}"
        ""
        "QLabel {"
        "    font-size: 14px;"
        "    color: #424242;"
        "}"
        ""
        "QLabel[titleLabel=\"true\"] {"
        "    font-size: 28px;"
        "    font-weight: bold;"
        "    color: #1976D2;"
        "}"
        ""
        "QLineEdit {"
        "    border: 2px solid #BBDEFB;"
        "    border-radius: 8px;"
        "    padding: 10px 15px;"
        "    font-size: 14px;"
        "    background-color: white;"
        "    color: #333333;"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid #2196F3;"
        "    background-color: #F5FAFF;"
        "}"
        "QLineEdit:read-only {"
        "    background-color: #E8F4F8;"
        "    color: #757575;"
        "}"
        ""
        "QPushButton {"
        "    border: none;"
        "    border-radius: 8px;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "    color: white;"
        "    background-color: #2196F3;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1565C0;"
        "}"
    );

    // 为标题标签设置属性以便样式化
    m_titleLabel->setProperty("titleLabel", true);
}





