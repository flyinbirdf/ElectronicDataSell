#include "searchbar.h"
#include <QHBoxLayout>

SearchBar::SearchBar(QString searchClass, QWidget *parent) : QWidget(parent)
  , m_searchEdit(new QLineEdit(this))
  , m_searchBtn(new QPushButton(tr("SEARCH"), this))
  , m_searchClass(searchClass)
  , m_socket(new TcpSocket(this))
{
    QHBoxLayout *layout = new QHBoxLayout();

    layout->addWidget(m_searchEdit);
    layout->addWidget(m_searchBtn);

    this->setLayout(layout);

    connect(m_searchBtn, &QPushButton::clicked, this, &SearchBar::SearchBtnClicked);
}

void SearchBar::SearchBtnClicked(bool)
{
    m_socket->sendSearchText(m_searchEdit->text());
}
