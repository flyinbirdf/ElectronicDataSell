#include "videoinfo.h"
#include <QVBoxLayout>

VideoInfo::VideoInfo(VideoType type, QWidget *parent): QWidget(parent)
  , m_bgPic(new QLabel(this))
  , m_title(new QLabel(this))
  , m_description(new QLabel(this))
  , m_price(new QLabel(this))
  , m_buyBtn(new QPushButton(tr("Buy"), this))
  , m_showDetail(new QPushButton(tr("Detail"), this))
  , m_type(type)
{
    QVBoxLayout *layout = new QVBoxLayout();

    this->setFixedSize(WIDTH, HEIGHT);

    m_bgPic->setFixedSize(PIC_WIDTH,PIC_HEIGHT);
    layout->addWidget(m_bgPic);
    m_title->setFixedSize(WIDTH, 30);
    layout->addWidget(m_title);
    m_description->setFixedSize(WIDTH, 90);
    layout->addWidget(m_description);

    QHBoxLayout *hlayout = new QHBoxLayout();
    m_price->setFixedSize(WIDTH/3, 30);
    hlayout->addWidget(m_price);
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
    //m_title->setStyleSheet("background-color: rgba(255, 0, 0, 255);");
}

void VideoInfo::setInfo(QPixmap bgPix, QString title, QString detail, int price, QString bookId)
{
    bgPix.scaled(PIC_WIDTH, PIC_HEIGHT);
    m_bgPic->setPixmap(bgPix);
    m_bgPic->setScaledContents(true);
    //this->setStyleSheet("background-image: url(:/img/picdemo.jpg);");

    QFontMetrics fontWidth(m_title->font());//得到每个字符的宽度
    QString elideTitle = fontWidth.elidedText(QString(tr("Book Name: %1")).arg(title), Qt::ElideRight, WIDTH);//最大宽度150像素
    m_title->setText(elideTitle);

    m_description->setWordWrap(true);
    //QFontMetrics descriptionWidth(m_description->font());//得到每个字符的宽度
    //QString elideDescription = fontWidth.elidedText(QString(tr("Abstract: %1")).arg(detail), Qt::ElideRight, WIDTH);//最大宽度150像素
    m_description->setText(detail);
    m_price->setText(QString("%1 RMB").arg(price));

    m_bookId = bookId;

    if(m_type == VideoType::SHOW_MODE)
    {
        m_price->hide();
        m_buyBtn->hide();
    }
}
