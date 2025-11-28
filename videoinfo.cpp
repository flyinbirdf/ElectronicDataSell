#include "videoinfo.h"
#include <QVBoxLayout>

VideoInfo::VideoInfo(VideoType type, QWidget *parent): QWidget(parent)
  , m_bgPic(new QLabel(this))
  , m_title(new QLabel(this))
  , m_description(new QLabel(this))
  , m_priceLabel(new QLabel(this))
  , m_buyBtn(new QPushButton(tr("Buy"), this))
  , m_showDetail(new QPushButton(tr("Detail"), this))
  , m_type(type)
{
    QVBoxLayout *layout = new QVBoxLayout();

    m_bgPic->setFixedSize(PIC_WIDTH,PIC_HEIGHT);
    layout->addWidget(m_bgPic);
    m_title->setFixedSize(WIDTH, 30);
    layout->addWidget(m_title);
    m_description->setFixedSize(WIDTH, 90);
    layout->addWidget(m_description);

    QHBoxLayout *hlayout = new QHBoxLayout();
    m_priceLabel->setFixedSize(WIDTH/3, 30);
    hlayout->addWidget(m_priceLabel);
    m_showDetail->setFixedSize(WIDTH/3, 30);
    hlayout->addWidget(m_showDetail);
    m_buyBtn->setFixedSize(WIDTH/3, 30);
    hlayout->addWidget(m_buyBtn);

    layout->addLayout(hlayout);
    this->setLayout(layout);

    this->setFixedSize(WIDTH, HEIGHT);

    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    hlayout->setContentsMargins(0,0,0,0);
    hlayout->setSpacing(0);

    this->setStyleSheet("background: rgba(184, 203, 221, 128);");
}

void VideoInfo::setInfo(QString bgPix, QString title, QString detail, int price, QString videoId)
{
    m_bgPic->setPixmap(QPixmap(bgPix));
    //m_bgPic->setFixedSize(PIC_WIDTH, PIC_HEIGHT);
    m_bgPic->setScaledContents(true);

    QFontMetrics fontWidth(m_title->font());//得到每个字符的宽度
    QString elideTitle = fontWidth.elidedText(QString(tr("Book Name: %1")).arg(title), Qt::ElideRight, WIDTH);//最大宽度150像素
    m_title->setText(elideTitle);

    m_description->setWordWrap(true);
    m_description->setText(detail);

    m_price = price;
    m_priceLabel->setText(QString("%1 Yuan").arg(price));

    m_videoId = videoId;

}

void VideoInfo::setShowType(VideoType type)
{
    m_type = type;
    if(type == VideoType::ABSTRACT_MODE)
    {
        m_bgPic->hide();
        m_description->hide();
        this->setFixedSize(WIDTH, ABSTRACT_HEIGHT);
    }
    else if(type == VideoType::DETAIL_MODE)
    {
        m_bgPic->show();
        m_description->show();
        this->setFixedSize(WIDTH, HEIGHT);
    }
}
