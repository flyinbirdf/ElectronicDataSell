#include "userconfigwidget.h"
#include <QFormLayout>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>

UserConfigWidget::UserConfigWidget(QWidget *parent)
    : QWidget(parent)
    , m_titleLabel(nullptr)
    , m_languageLabel(nullptr)
    , m_languageCombo(nullptr)
    , m_themeLabel(nullptr)
    , m_themeCombo(nullptr)
    , m_notificationCheck(nullptr)
    , m_autoLoginCheck(nullptr)
    , m_saveBtn(nullptr)
    , m_resetBtn(nullptr)
    , m_mainLayout(nullptr)
    , m_sellPath(nullptr)
    , m_openSellPath(nullptr)
    , m_buyPath(nullptr)
    , m_openBuyPath(nullptr)
    , m_hintLabel(nullptr)
{
    setupUI();
    setupConnection();
    setupStyle();
}

void UserConfigWidget::setupUI()
{
    // 创建标题
    m_titleLabel = new QLabel(tr("用户基本配置"), this);
    m_titleLabel->setAlignment(Qt::AlignCenter);

    // // 创建语言选择
    // m_languageLabel = new QLabel(tr("语言设置:"), this);
    // m_languageCombo = new QComboBox(this);
    // m_languageCombo->addItem(tr("中文"), "zh_CN");
    // m_languageCombo->addItem(tr("English"), "en_US");
    // m_languageCombo->setMinimumHeight(35);
    // m_languageCombo->setMinimumWidth(200);

    // // 创建主题选择
    // m_themeLabel = new QLabel(tr("主题设置:"), this);
    // m_themeCombo = new QComboBox(this);
    // m_themeCombo->addItem(tr("浅色"), "light");
    // m_themeCombo->addItem(tr("深色"), "dark");
    // m_themeCombo->setMinimumHeight(35);
    // m_themeCombo->setMinimumWidth(200);

    // 需要读取的文件夹
    QLabel *sellPathLabel = new QLabel(tr("卖出资料路径:"), this);
    m_sellPath = new QLineEdit(this);
    m_sellPath->setEnabled(false);
    m_openSellPath = new QPushButton(tr("打开文件夹"), this);
    QHBoxLayout *sellLayout = new QHBoxLayout();
    sellLayout->addWidget(sellPathLabel);
    sellLayout->addWidget(m_sellPath);
    sellLayout->addWidget(m_openSellPath);
    m_openSellPath->setMinimumHeight(40);
    m_openSellPath->setMinimumWidth(120);

    // 买电子数据的保存路径
    QLabel *buyPathLabel = new QLabel(tr("买入资料路径:"), this);
    m_buyPath = new QLineEdit(this);
    m_buyPath->setEnabled(false);
    m_openBuyPath = new QPushButton(tr("打开文件夹"), this);
    QHBoxLayout *buyLayout = new QHBoxLayout();
    buyLayout->addWidget(buyPathLabel);
    buyLayout->addWidget(m_buyPath);
    buyLayout->addWidget(m_openBuyPath);
    m_openBuyPath->setMinimumHeight(40);
    m_openBuyPath->setMinimumWidth(120);

    // 用于显示文件夹是否正常
    m_hintLabel = new QLabel(tr("文件夹能正常解析"), this);

    // // 创建通知复选框
    // m_notificationCheck = new QCheckBox(tr("接收通知"), this);
    // m_notificationCheck->setChecked(true);

    // // 创建自动登录复选框
    // m_autoLoginCheck = new QCheckBox(tr("自动登录"), this);
    // m_autoLoginCheck->setChecked(false);

    // 创建开始解析按钮
    m_saveBtn = new QPushButton(tr("开始解析"), this);
    m_saveBtn->setMinimumHeight(40);
    m_saveBtn->setMinimumWidth(120);
    m_saveBtn->setMaximumWidth(180);

    // // 创建重置按钮
    // m_resetBtn = new QPushButton(tr("重置"), this);
    // m_resetBtn->setMinimumHeight(40);
    // m_resetBtn->setMinimumWidth(120);

    // 创建表单布局
    // QFormLayout *formLayout = new QFormLayout();
    // formLayout->setSpacing(20);
    // formLayout->setLabelAlignment(Qt::AlignRight);
    // formLayout->addRow(m_languageLabel, m_languageCombo);
    // formLayout->addRow(m_themeLabel, m_themeCombo);
    // formLayout->addRow(tr(""), m_notificationCheck);
    // formLayout->addRow(tr(""), m_autoLoginCheck);

    // 创建主布局
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(30);
    m_mainLayout->setContentsMargins(50, 50, 50, 50);

    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_titleLabel);
    //m_mainLayout->addLayout(formLayout);
    m_mainLayout->addLayout(sellLayout);
    m_mainLayout->addLayout(buyLayout);
    m_mainLayout->addWidget(m_hintLabel);
    m_mainLayout->setAlignment(m_hintLabel, Qt::AlignCenter);
    m_mainLayout->addWidget(m_saveBtn);
    m_mainLayout->setAlignment(m_saveBtn, Qt::AlignCenter);
    m_mainLayout->addStretch();

    // // 添加按钮布局
    // QHBoxLayout *btnLayout = new QHBoxLayout();
    // btnLayout->addStretch();
    // btnLayout->addWidget(m_saveBtn);
    // btnLayout->addSpacing(20);
    // btnLayout->addWidget(m_resetBtn);
    // btnLayout->addStretch();
    // m_mainLayout->addLayout(btnLayout);
    // m_mainLayout->addStretch();

    this->setLayout(m_mainLayout);
}

void UserConfigWidget::setupConnection()
{
    // // 连接信号
    // connect(m_saveBtn, &QPushButton::clicked, this, [this]() {
    //     QString language = m_languageCombo->currentData().toString();
    //     QString theme = m_themeCombo->currentData().toString();
    //     bool notification = m_notificationCheck->isChecked();
    //     bool autoLogin = m_autoLoginCheck->isChecked();

    //     qDebug() << "保存配置 - 语言:" << language
    //              << "主题:" << theme
    //              << "通知:" << notification
    //              << "自动登录:" << autoLogin;

    //     QMessageBox::information(this, tr("提示"), tr("配置保存成功！"));
    //     // TODO: 实现实际的保存逻辑
    // });

    connect(m_openSellPath, &QPushButton::clicked, this, [this]() {
        // 选择现有文件夹
        QString directory = QFileDialog::getExistingDirectory(
            this,
            tr("选择卖出文件夹"),
            "C:\\",
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
            );
        if (!directory.isEmpty()) {
            m_sellPath->setText(directory);
        }
        if(!m_sellPath->text().isEmpty() && !m_buyPath->text().isEmpty())
        {
            if(m_sellPath->text().startsWith(m_buyPath->text()) || m_buyPath->text().startsWith(m_sellPath->text()))
            {
                m_hintLabel->setText(tr("卖出文件夹和买入文件夹相互包含,不能解析"));
                m_saveBtn->setEnabled(false);
            }
            else
            {
                m_hintLabel->setText(tr("文件夹正常,可以解析"));
                m_saveBtn->setEnabled(true);
            }
        }
        else if(!m_sellPath->text().isEmpty())
        {
            m_hintLabel->setText(tr("文件夹正常,可以解析"));
            m_saveBtn->setEnabled(true);
        }
    });

    connect(m_openBuyPath, &QPushButton::clicked, this, [this]() {
        // 选择现有文件夹
        QString directory = QFileDialog::getExistingDirectory(
            this,
            tr("选择卖出文件夹"),
            "C:\\",
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
            );
        if (!directory.isEmpty()) {
            m_buyPath->setText(directory);
        }
        if(!m_sellPath->text().isEmpty() && !m_buyPath->text().isEmpty())
        {
            if(m_sellPath->text().startsWith(m_buyPath->text()) || m_buyPath->text().startsWith(m_sellPath->text()))
            {
                m_hintLabel->setText(tr("卖出文件夹和买入文件夹相互包含,不能解析"));
                m_saveBtn->setEnabled(false);
            }
            else
            {
                m_hintLabel->setText(tr("文件夹正常,可以解析"));
                m_saveBtn->setEnabled(true);
            }
        }
        else if(!m_sellPath->text().isEmpty())
        {
            m_hintLabel->setText(tr("文件夹正常,可以解析"));
            m_saveBtn->setEnabled(true);
        }
    });
}

void UserConfigWidget::setupStyle()
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
        "QComboBox {"
        "    border: 2px solid #BBDEFB;"
        "    border-radius: 8px;"
        "    padding: 8px 12px;"
        "    font-size: 14px;"
        "    background-color: white;"
        "    color: #333333;"
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
        "QComboBox::drop-down:hover {"
        "    background-color: #1976D2;"
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
        "QCheckBox {"
        "    font-size: 14px;"
        "    color: #424242;"
        "    spacing: 8px;"
        "}"
        "QCheckBox::indicator {"
        "    width: 20px;"
        "    height: 20px;"
        "    border: 2px solid #BBDEFB;"
        "    border-radius: 4px;"
        "    background-color: white;"
        "}"
        "QCheckBox::indicator:checked {"
        "    background-color: #2196F3;"
        "    border: 2px solid #2196F3;"
        "}"
        "QCheckBox::indicator:hover {"
        "    border: 2px solid #90CAF9;"
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
        ""
        "QPushButton#resetBtn {"
        "    background-color: #90CAF9;"
        "    color: #1976D2;"
        "}"
        "QPushButton#resetBtn:hover {"
        "    background-color: #64B5F6;"
        "}"
        "QPushButton#resetBtn:pressed {"
        "    background-color: #42A5F5;"
        "}"
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
        "QLabel[hintLabel=\"true\"] {"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    color: #0000FF;"
        "}"
    );

    // 为标题标签设置属性以便样式化
    m_titleLabel->setProperty("titleLabel", true);
    m_hintLabel->setProperty("hintLabel", true);
    //m_resetBtn->setObjectName("resetBtn");
}

