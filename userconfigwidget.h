#ifndef USERCONFIGWIDGET_H
#define USERCONFIGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>

class UserConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserConfigWidget(QWidget *parent = nullptr);

private:
    void setupUI();
    void setupConnection();
    void setupStyle();

private:
    QLabel *m_titleLabel;
    QLabel *m_languageLabel;
    QComboBox *m_languageCombo;
    QLabel *m_themeLabel;
    QComboBox *m_themeCombo;
    QCheckBox *m_notificationCheck;
    QCheckBox *m_autoLoginCheck;
    QPushButton *m_saveBtn;
    QPushButton *m_resetBtn;
    QVBoxLayout *m_mainLayout;
    QLineEdit *m_sellPath;
    QPushButton *m_openSellPath;
    QLineEdit *m_buyPath;
    QPushButton *m_openBuyPath;
    QLabel *m_hintLabel;
};

#endif // USERCONFIGWIDGET_H



