#include "twitterapplication.h"

TwitterApplication::TwitterApplication(QObject *parent) : QObject(parent)
{
}

QSharedPointer<twitCurl> TwitterApplication::twitCurlObject() const
{
    return m_TwitCurlObject;
}

void TwitterApplication::setTwitCurlObject(const QSharedPointer<twitCurl>& tcObj)
{
    m_TwitCurlObject = tcObj;
}

bool TwitterApplication::isInitialised() const
{
    return m_TwitCurlObject.get() != nullptr;
}
