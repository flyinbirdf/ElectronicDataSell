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

    layout->addWidget(m_searchEdit);
    layout->addWidget(m_searchBtn);

    this->setLayout(layout);

    connect(m_searchBtn, &QPushButton::clicked, this, &SearchBar::SearchBtnClicked);
}

void SearchBar::changeType(SEARCH_TYPE type)
{
    m_type = type;
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
