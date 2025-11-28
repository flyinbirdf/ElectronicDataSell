#ifndef USERBASICINFOWIDGET_H
#define USERBASICINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class UserBasicInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserBasicInfoWidget(QWidget *parent = nullptr);

private:
    void setupUI();
    void setupStyle();

private:
    QLabel *m_titleLabel;
    QLabel *m_usernameLabel;
    QLineEdit *m_usernameEdit;
    QLabel *m_emailLabel;
    QLineEdit *m_emailEdit;
    QLabel *m_phoneLabel;
    QLineEdit *m_phoneEdit;
    QLabel *m_nicknameLabel;
    QLineEdit *m_nicknameEdit;
    QPushButton *m_saveBtn;
    QVBoxLayout *m_mainLayout;
};

#endif // USERBASICINFOWIDGET_H





