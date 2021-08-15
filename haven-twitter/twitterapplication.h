#ifndef TWITTERAPPLICATION_H
#define TWITTERAPPLICATION_H

#include <QObject>

class TwitterApplication : public QObject
{
    Q_OBJECT
public:
    explicit TwitterApplication(QObject *parent = nullptr);

signals:

};

#endif // TWITTERAPPLICATION_H
