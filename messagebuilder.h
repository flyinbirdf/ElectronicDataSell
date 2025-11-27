#ifndef MESSAGEBUILDER_H
#define MESSAGEBUILDER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonValue>

class MessageBuilder : public QObject
{
    Q_OBJECT
public:
    explicit MessageBuilder(QObject *parent = nullptr);

    QJsonObject reqestPageBookInfo(int page, QString searchText);
    QJsonObject reqestFavoritePageBookInfo(int page);
    QJsonObject reqestPageVideoInfo(int page, QString searchText);
signals:
};

#endif // MESSAGEBUILDER_H
