#ifndef VIDEOINFO_H
#define VIDEOINFO_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QJsonObject>

class VideoInfo : public QWidget
{
    Q_OBJECT
public:
    enum class VideoType
    {
        DETAIL_MODE,
        ABSTRACT_MODE
    };
public:
    explicit VideoInfo(VideoType type, QWidget *parent = nullptr);
    void setInfo(QString bgPix, QString title, QString detail, int price, QString bookId);
    void setShowType(VideoType type);

signals:
    void showDetail(QString fileName);
    void buyVideo(QJsonObject obj);
public slots:

private:
    QLabel *m_bgPic;
    QLabel *m_title;
    QLabel *m_description;
    QLabel *m_priceLabel;
    QPushButton *m_buyBtn;
    QPushButton *m_showDetail;
    QString m_videoId;
    VideoType m_type;
    int m_price;
private:
    const int WIDTH = 300;
    const int HEIGHT = 400;
    const int ABSTRACT_HEIGHT = 100;
    const int PIC_WIDTH = 300;
    const int PIC_HEIGHT = 200;
};

#endif // VIDEOINFO_H
