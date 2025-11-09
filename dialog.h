#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QToolBar>
#include <QStackedWidget>
#include <QAction>
#include <QVector>
#include "booksellwidget.h"
#include "videosellwidget.h"
//#include "myaccountwidget.h"

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

private:
    QToolBar *m_classifyBar;
    QStackedWidget *m_stackedWidget;
    BookSellWidget *m_bookSellWidget;
    VideoSellWidget *m_videoSellWidget;
    //MyAccountWidget *m_myAccountWidget;
    QAction* m_bookAction;
    QAction* m_videoAction;
    QAction* m_myAction;
};
#endif // DIALOG_H
