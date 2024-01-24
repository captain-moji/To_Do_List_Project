#include "comment.h"

Comment::Comment() {}

Comment::Comment(QString t, QString d)
    : text(t), comment_date(d)
{

}

Comment::Comment(const Comment & comment)
    : text(comment.text), comment_date(comment.comment_date)
{
    for (int var = 0; var < comment.replys.size(); ++var) {
        replys[var] = new Comment[1];
        replys[var][0] = comment.replys[var][0];
    }
}

Comment::~Comment()
{
    for (int var = 0; var < replys.size(); ++var) {
        delete[] replys[var];
    }
}
