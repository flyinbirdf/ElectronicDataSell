#include "searchbar.h"
#include <QHBoxLayout>

SearchBar::SearchBar(SEARCH_TYPE type, std::shared_ptr<JsonTcpClient> socket, std::shared_ptr<MessageBuilder> msgBuilder, QWidget *parent) : QWidget(parent)
    , m_searchEdit(new QLineEdit(this))
    , m_searchBtn(new QPushButton(tr("SEARCH"), this))
    , m_socket(socket)
    , m_msgBuilder(msgBuilder)
    , m_type(type)
{
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setSpacing(8);

    layout->addWidget(m_searchEdit);
    layout->addWidget(m_searchBtn);

    this->setLayout(layout);

    connect(m_searchBtn, &QPushButton::clicked, this, &SearchBar::SearchBtnClicked);
    
    // 设置搜索栏样式
    setupStyle();
}

void SearchBar::changeType(SEARCH_TYPE type)
{
    m_type = type;
}

void SearchBar::setupStyle()
{
    setStyleSheet(
        "QLineEdit {"
        "    border: 2px solid #BBDEFB;"
        "    border-radius: 20px;"
        "    padding: 10px 20px;"
        "    font-size: 14px;"
        "    background-color: white;"
        "    color: #333333;"
        "    min-width: 300px;"
        "}"
        "QLineEdit:focus {"
        "    border: 2px solid #2196F3;"
        "    background-color: #F5FAFF;"
        "}"
        ""
        "QPushButton {"
        "    border: none;"
        "    border-radius: 20px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "    color: white;"
        "    background-color: #2196F3;"
        "    padding: 10px 25px;"
        "    min-width: 100px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1565C0;"
        "}"
    );
}

void SearchBar::SearchBtnClicked(bool)
{
    switch(m_type)
    {
    case BOOK: {
        QJsonObject msgbook = m_msgBuilder->reqestPageBookInfo(0, m_searchEdit->text());
        m_socket->sendMessage(msgbook);
        break;
    }
    case VIDEO: {
        QJsonObject msgvideo = m_msgBuilder->reqestPageVideoInfo(0, m_searchEdit->text());
        m_socket->sendMessage(msgvideo);
        break;
    }
    default: {
        qDebug("enter search default branch");
        break;
    }
    }



}
