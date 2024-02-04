#include "comment.h"

Comment::Comment() {}

Comment::Comment(QString t, QString d)
    : text(t), comment_date(d)
{

}

Comment::~Comment()
{

}

void Comment::commentSetPerson(QString p)
{
    comment_person = p;
}

QString Comment::commentGetPerson()
{
    return comment_person;
}

void Comment::commentSetText(QString t)
{
    text = t;
}

QString Comment::commentGetText()
{
    return text;
}

const QString Comment::commentGetText() const
{
    return text;
}

void Comment::commentSetDate(QString d)
{
    comment_date = d;
}

QString Comment::commentGetDate()
{
    return comment_date;
}

const QString Comment::commentGetDate() const
{
    return comment_date;
}

QVector<Comment> &Comment::commentGetReplies()
{
    return replies;
}

void Comment::fromJson(const QJsonObject & json)
{
    comment_person = json["comment_person"].toString();
    text = json["comment_text"].toString();
    comment_date = json["comment_date"].toString();

    QJsonArray repliesJson = json["comment_replies"].toArray();
    for (int i = 0; i < repliesJson.size(); ++i) {
        Comment reply;
        reply.fromJson(repliesJson[i].toObject());
        replies.append(reply);
    }

}

QJsonObject Comment::toJson() const
{
    QJsonObject json;
    json["comment_person"] = comment_person;
    json["comment_text"] = text;
    json["comment_date"] = comment_date;

    QJsonArray repliesJson;
    for (const Comment &reply : replies) {
        repliesJson.append(reply.toJson());
    }

    json["comment_replies"] = repliesJson;

    return json;
}
