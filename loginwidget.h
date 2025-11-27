#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = nullptr);

signals:
    void loginSuccess(const QString &username);  // 登录成功信号
    void wechatLoginRequested();  // 微信登录请求信号

private slots:
    void onLoginClicked();  // 用户名密码登录
    void onWechatLoginClicked();  // 微信登录

private:
    void setupUI();  // 设置界面
    void setupStyle();  // 设置样式

private:
    QLabel *m_titleLabel;  // 标题标签
    QLineEdit *m_usernameEdit;  // 用户名输入框
    QLineEdit *m_passwordEdit;  // 密码输入框
    QPushButton *m_loginBtn;  // 登录按钮
    QPushButton *m_wechatLoginBtn;  // 微信登录按钮
    QVBoxLayout *m_mainLayout;  // 主布局
};

#endif // LOGINWIDGET_H
