#include "messagebuilder.h"

MessageBuilder::MessageBuilder(QObject *parent)
    : QObject{parent}
{}

QJsonObject MessageBuilder::reqestPageBookInfo(int page, QString searchText)
{
    QJsonObject obj;
    obj["reqest"] = "bookInfo";
    obj["page"] = page;
    obj["searchText"] = searchText;

    return obj;
}

QJsonObject MessageBuilder::reqestPageVideoInfo(int page, QString searchText)
{
    QJsonObject obj;
    obj["reqest"] = "videoInfo";
    obj["page"] = page;
    obj["searchText"] = searchText;

    return obj;
}

QJsonObject MessageBuilder::reqestFavoritePageBookInfo(int page)
{
    QJsonObject obj;
    obj["reqest"] = "favoritebookInfo";
    obj["page"] = page;

    return obj;
}
