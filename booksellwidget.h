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
#include <QMap>
#include <memory>
#include <QComboBox>
#include "bookinfo.h"
#include "pdfviewer.h"
#include "tcpsocket.h"
#include "messagebuilder.h"

class BookSellWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BookSellWidget(std::shared_ptr<JsonTcpClient> tcpClient, std::shared_ptr<MessageBuilder> msgBuilder, QWidget *parent = nullptr);

signals:
    void showBookDetail(QString name, BookInfo::BookType type);

public slots:
    void onCurrentIndexChanged(int index);

private:
    void refreshPage(QJsonObject obj);
    void setupStyle();

private:
    int m_page;
    SearchBar *m_searchBar;
    QScrollArea *m_content;
    QWidget *m_contentWidget;
    // 存储了页数对应要加载的
    QVector<BookInfo*> m_bookInfoVector;
    QPushButton *m_prevBtn;
    QPushButton *m_nextBtn;
    QLabel *m_infoLabel;
    PDFViewer *m_showPdfViewer;
    std::shared_ptr<JsonTcpClient> m_tcpClient;
    std::shared_ptr<MessageBuilder> m_msgBuilder;
    QString m_searchText;
    BookInfo::BookType m_showType;
    QComboBox *m_showTypeComb;

private:
    const int COLUMN = 10;
    const int ROW = 8;
    const int PAGE_SIZE = 80;
};

#endif // BOOKSELLWIDGET_H
