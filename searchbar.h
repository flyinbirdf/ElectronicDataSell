#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QJsonObject>
#include <tcpsocket.h>
#include <memory>
#include "messagebuilder.h"

class SearchBar : public QWidget
{
    Q_OBJECT
public:
    typedef enum {
        BOOK,
        VIDEO
    } SEARCH_TYPE;
public:
    explicit SearchBar(SEARCH_TYPE type, std::shared_ptr<JsonTcpClient> socket, std::shared_ptr<MessageBuilder> msgBuilder, QWidget *parent = nullptr);
    QString text() { return m_searchEdit->text(); };
    void changeType(SEARCH_TYPE type);

signals:
    //void onSearchBtnClicked(QString text);
public slots:
    void SearchBtnClicked(bool);
private:
    void setupStyle();
private:
    QLineEdit *m_searchEdit;
    QPushButton *m_searchBtn;
    std::shared_ptr<JsonTcpClient> m_socket;
    std::shared_ptr<MessageBuilder> m_msgBuilder;
    SEARCH_TYPE m_type;
};

#endif // SEARCHBAR_H
