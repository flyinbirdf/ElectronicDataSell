#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QToolBar>
#include <QStackedWidget>
#include <QAction>
#include <QVector>
#include "booksellwidget.h"
#include "videosellwidget.h"
#include "memory"
#include "myaccountwidget.h"

class Dialog : public QDialog
{
    Q_OBJECT

public slots:
    void actionTriggered(QAction *action);
public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
private:
    void createToolbar();
    void setCheckedAction(QAction *curAction);
private:
    std::shared_ptr<JsonTcpClient> m_tcpClient;
    std::shared_ptr<MessageBuilder> m_msgBuilder;
    QToolBar *m_classifyBar;
    QStackedWidget *m_stackedWidget;
    BookSellWidget *m_bookSellWidget;
    VideoSellWidget *m_videoSellWidget;
    MyAccountWidget *m_myAccountWidget;
    QAction* m_bookAction;
    QAction* m_videoAction;
    QAction* m_myAction;
    QVector<QAction*> m_actionVector;


};
#endif // DIALOG_H
