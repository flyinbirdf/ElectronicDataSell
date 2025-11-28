#ifndef MYACCOUNTWIDGET_H
#define MYACCOUNTWIDGET_H

#include <QWidget>
#include <QVector>
#include <QToolBar>
#include <QStackedWidget>
#include <QAction>
#include <QHBoxLayout>
#include <tcpsocket.h>
#include <messagebuilder.h>
#include "userbasicinfowidget.h"
#include "userconfigwidget.h"
#include "loginwidget.h"

class MyAccountWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccountWidget(std::shared_ptr<JsonTcpClient> tcpClient, std::shared_ptr<MessageBuilder> msgBuilder, QWidget *parent = nullptr);

signals:

public slots:
    void onActionTriggered(QAction *action);

private:
    void setupUI();
    void createToolbar();
    void setCheckedAction(QAction *curAction);
    void setupStyle();

private:
    std::shared_ptr<JsonTcpClient> m_tcpClient;
    std::shared_ptr<MessageBuilder> m_msgBuidler;
    QToolBar *m_toolBar;
    QStackedWidget *m_stackedWidget;
    UserBasicInfoWidget *m_basicInfoWidget;
    UserConfigWidget *m_configWidget;
    LoginWidget *m_loginWidget;
    QAction *m_basicInfoAction;
    QAction *m_configAction;
    QAction *m_loginAction;
    QVector<QAction*> m_actionVector;
    QHBoxLayout *m_mainLayout;
};

#endif // MYACCOUNTWIDGET_H


