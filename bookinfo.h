#ifndef BOOKINFO_H
#define BOOKINFO_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>

enum class BookType
{
    SELL_MODE,
    SHOW_MODE
};

class BookInfo : public QWidget
{
    Q_OBJECT

public:
    explicit BookInfo(BookType type, QWidget *parent = nullptr);
    void setInfo(QPixmap bgPix, QString title, QString detail, int price, QString bookId);

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
    const int WIDTH = 200;
    const int HEIGHT = 400;
    const int PIC_WIDTH = 200;
    const int PIC_HEIGHT = 200;
};

#endif // BOOKINFO_H
