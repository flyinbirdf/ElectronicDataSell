#ifndef BOOKINFO_H
#define BOOKINFO_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>

class BookInfo : public QWidget
{
    Q_OBJECT
public:
    enum class BookType
    {
        DETAIL_MODE,
        ABSTRACT_MODE
    };
public:
    explicit BookInfo(enum BookType type, QWidget *parent = nullptr);
    void setInfo(QString bgPix, QString title, QString detail, int price, QString bookId);
    void setInfo(QString title, int price, QString bookId);
    void setShowType(BookType type);
signals:
    void showDetail(QString name, BookType type);
private:
    QLabel *m_bgPic;
    QLabel *m_title;
    QLabel *m_description;
    QLabel *m_price;
    QPushButton *m_buyBtn;
    QPushButton *m_showDetail;
    QString m_bookId;
    BookType m_type;
private:
    const int WIDTH = 300;
    const int DETAIL_HEIGHT = 400;
    const int ABSTRACT_HEIGHT = 100;
    const int PIC_WIDTH = 300;
    const int PIC_HEIGHT = 200;
};

#endif // BOOKINFO_H
