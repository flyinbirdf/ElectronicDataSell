#include "bookinfo.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

BookInfo::BookInfo(BookType type, QWidget *parent): QWidget(parent)
  , m_bgPic(new QLabel(this))
  , m_title(new QLabel(this))
  , m_description(new QLabel(this))
  , m_price(new QLabel(this))
  , m_buyBtn(new QPushButton(tr("Buy"), this))
  , m_showDetail(new QPushButton(tr("Detail"), this))
  , m_type(type)
{
    QVBoxLayout *layout = new QVBoxLayout();

    m_bgPic->setFixedSize(PIC_WIDTH,PIC_HEIGHT);
    layout->addWidget(m_bgPic);
    //0m_title->setFixedSize(WIDTH, 30);
    layout->addWidget(m_title);
    //m_description->setFixedSize(WIDTH, 90);
    layout->addWidget(m_description);

    QHBoxLayout *hlayout = new QHBoxLayout();
    m_price->setFixedSize(WIDTH/4, 30);
    hlayout->addWidget(m_price);
    m_showDetail->setFixedSize(WIDTH/4, 30);
    hlayout->addWidget(m_showDetail);
    m_buyBtn->setFixedSize(WIDTH/4, 30);
    hlayout->addWidget(m_buyBtn);
    hlayout->setContentsMargins(0,0,0,0);
    hlayout->setSpacing(0);

    layout->addLayout(hlayout);
    this->setLayout(layout);

    setShowType(m_type);

    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    hlayout->setContentsMargins(0,0,0,0);
    hlayout->setSpacing(0);

    // 设置卡片样式 - 淡蓝色主题
    setStyleSheet(
        "QWidget {"
        "    background-color: white;"
        "    border: 2px solid #BBDEFB;"
        "    border-radius: 12px;"
        "}"
        ""
        "QLabel {"
        "    background-color: transparent;"
        "    color: #424242;"
        "}"
        ""
        "QPushButton {"
        "    border: none;"
        "    border-radius: 6px;"
        "    font-size: 12px;"
        "    font-weight: bold;"
        "    color: white;"
        "    background-color: #2196F3;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1565C0;"
        "}"
    );
    
    connect(m_showDetail, &QPushButton::clicked, this, [=](bool) {
        qDebug()<<"Send showDetail";
        emit showDetail("C:\\D盘\\gitProject\\ElectronicDataSell\\test.pdf", m_type);
    });
}

void BookInfo::setInfo(QString bgPix, QString title, QString detail, int price, QString bookId)
{
    QPixmap pix = QPixmap(bgPix);
    pix.scaled(PIC_WIDTH, PIC_HEIGHT);
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
}

void BookInfo::setInfo(QString title, int price, QString bookId)
{
    QFontMetrics fontWidth(m_title->font());//得到每个字符的宽度
    QString elideTitle = fontWidth.elidedText(QString(tr("Book Name: %1")).arg(title), Qt::ElideRight, WIDTH);//最大宽度150像素
    m_title->setText(elideTitle);
    m_price->setText(QString("%1 RMB").arg(price));

    m_bookId = bookId;
}

void BookInfo::setShowType(BookType type)
{    if(BookType::DETAIL_MODE == m_type) {

    } else {

    }
    m_type = type;
    switch(type)
    {
        case BookType::ABSTRACT_MODE:
        {
            this->setFixedSize(WIDTH, ABSTRACT_HEIGHT);
            m_bgPic->hide();
            m_description->hide();
            break;
        }
        case BookType::DETAIL_MODE:
        {
            this->setFixedSize(WIDTH, DETAIL_HEIGHT);
            m_bgPic->show();
            m_description->show();
            break;
        }
        default:
        {
            break;
        }
    }
}
