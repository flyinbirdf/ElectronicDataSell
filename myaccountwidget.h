#ifndef MYACCOUNTWIDGET_H
#define MYACCOUNTWIDGET_H

#include <QWidget>
#include <QVector>
#include <QToolBar>
#include <QAction>
#include "bookinfo.h"
#include "videoinfo.h"
#include <QGridLayout>

class MyAccountWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyAccountWidget(QWidget *parent = nullptr);

private:
    void createToolbar();
    void actionTriggered(QAction *action);
    void showBook();
    void showVideo();
signals:

public slots:

private:
    QToolBar *m_typeBar;
    QAction *m_bookAction;
    QAction *m_videoAction;
    QGridLayout *m_gridLayout;
    QVector<BookInfo*> m_bookVector;
    QVector<VideoInfo*> m_videoVector;

private:
    const int ROW = 4;
    const int COLUMN = 7;
};

#endif // MYACCOUNTWIDGET_H


