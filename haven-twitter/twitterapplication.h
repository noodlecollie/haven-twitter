#ifndef TWITTERAPPLICATION_H
#define TWITTERAPPLICATION_H

#include <QObject>
#include <QSharedPointer>

class twitCurl;

class TwitterApplication : public QObject
{
    Q_OBJECT
public:
    explicit TwitterApplication(QObject *parent = nullptr);

    QSharedPointer<twitCurl> twitCurlObject() const;
    void setTwitCurlObject(const QSharedPointer<twitCurl>& tcObj);

    bool isInitialised() const;

private:
    QSharedPointer<twitCurl> m_TwitCurlObject;
};

#endif // TWITTERAPPLICATION_H
