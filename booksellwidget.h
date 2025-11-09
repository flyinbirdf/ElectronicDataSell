#ifndef BOOKSELLWIDGET_H
#define BOOKSELLWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QPixmap>
#include "searchbar.h"
#include <QScrollArea>
#include <QVector>
#include "bookinfo.h"
#include "showpdfdoc.h"

class BookSellWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BookSellWidget(QWidget *parent = nullptr);

signals:

public slots:
    void showBookDetail(QString name, BookType type);

private:
    SearchBar *m_searchBar;
    QScrollArea *m_content;
    QWidget *m_contentWidget;
    QVector<BookInfo*> m_bookInfo;
    ShowPdfDoc *m_showPdfDoc;
private:
    const int COLUMN = 7;
    const int ROW = 4;
};

#endif // BOOKSELLWIDGET_H
