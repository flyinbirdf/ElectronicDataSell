#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QJsonObject>
#include <tcpsocket.h>

class SearchBar : public QWidget
{
    Q_OBJECT
public:
    explicit SearchBar(QString searchClass, QWidget *parent = nullptr);

signals:

public slots:
    void SearchBtnClicked(bool);
private:
    QLineEdit *m_searchEdit;
    QPushButton *m_searchBtn;
    QString m_searchClass;
    TcpSocket *m_socket;
};

#endif // SEARCHBAR_H
