#ifndef VIDEOINFO_H
#define VIDEOINFO_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

enum class VideoType
{
    SELL_MODE,
    SHOW_MODE
};

class VideoInfo : public QWidget
{
    Q_OBJECT
public:
    explicit VideoInfo(VideoType type, QWidget *parent = nullptr);
    void setInfo(QString bgPix, QString title, QString detail, int price, QString bookId);

signals:

public slots:

private:
    QLabel *m_bgPic;
    QLabel *m_title;
    QLabel *m_description;
    QLabel *m_price;
    QPushButton *m_buyBtn;
    QPushButton *m_showDetail;
    QString m_bookId;
    VideoType m_type;
private:
    const int WIDTH = 200;
    const int HEIGHT = 400;
    const int PIC_WIDTH = 200;
    const int PIC_HEIGHT = 200;
};

#endif // VIDEOINFO_H
