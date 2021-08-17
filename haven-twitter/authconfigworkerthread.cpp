#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "authconfigworkerthread.h"

AuthConfigWorkerThread::AuthConfigWorkerThread(QObject* parent) :
    QThread(parent)
{
}

AuthConfigWorkerThread::~AuthConfigWorkerThread()
{
}

void AuthConfigWorkerThread::setAPIKey(const QString& key)
{
    m_ApiKey = key;
}

void AuthConfigWorkerThread::setAPISecret(const QString& secret)
{
    m_ApiSecret = secret;
}

void AuthConfigWorkerThread::setAccessToken(const QString& token)
{
    m_AccessToken = token;
}

void AuthConfigWorkerThread::setAccessTokenSecret(const QString& secret)
{
    m_AccessTokenSecret = secret;
}

void AuthConfigWorkerThread::setUsername(const QString& username)
{
    m_Username = username;
}

void AuthConfigWorkerThread::setPassword(const QString& password)
{
    m_Password = password;
}

void AuthConfigWorkerThread::run()
{
    m_ReceivedNewAccessToken = false;

    twitCurl* tcObj = new twitCurl();

    qDebug() << "AuthConfigWorkerThread: Setting credentials.";
    tcObj->setTwitterUsername(m_Username.toStdString());
    tcObj->setTwitterPassword(m_Password.toStdString());
    tcObj->getOAuth().setConsumerKey(m_ApiKey.toStdString());
    tcObj->getOAuth().setConsumerSecret(m_ApiSecret.toStdString());

    if ( m_AccessToken.isEmpty() || m_AccessTokenSecret.isEmpty() )
    {
        qDebug() << "AuthConfigWorkerThread: Requesting new access token credentials.";

        m_ReceivedNewAccessToken = true;

        std::string authURL;
        tcObj->oAuthRequestToken(authURL);
        tcObj->oAuthHandlePIN(authURL);
    }
    else
    {
        qDebug() << "AuthConfigWorkerThread: Access token credentials were provided, not re-requesting.";

        // Set from details that we have.
        tcObj->getOAuth().setOAuthTokenKey(m_AccessToken.toStdString());
        tcObj->getOAuth().setOAuthTokenSecret(m_AccessTokenSecret.toStdString());
    }

    qDebug() << "AuthConfigWorkerThread: Performing OAuth authentication.";

    const bool success = tcObj->oAuthAccessToken() &&
        tcObj->accountVerifyCredGet() &&
        requestWasSuccessful(tcObj);

    if ( success )
    {
        qDebug() << "AuthConfigWorkerThread: OAuth authentication succeeded.";

        std::string replyMsg;
        tcObj->getLastWebResponse(replyMsg);
        qDebug() << replyMsg.c_str();
    }
    else
    {
        std::string replyMsg;
        tcObj->getLastCurlError(replyMsg);

        qWarning() << "AuthConfigWorkerThread: Failed to authenticate with Twitter. Response:" << replyMsg.c_str();
    }

    emit authFinished(success, tcObj);
}

bool AuthConfigWorkerThread::requestWasSuccessful(twitCurl* tcObj) const
{
    // For some reason, some requests report that they were successful
    // even though Twitter returned an error. We check for the error here.
    // It's fairly quick and dirty but it should do.

    std::string content;
    tcObj->getLastWebResponse(content);

    QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(content));

    if ( !doc.isObject() )
    {
        return false;
    }

    QJsonObject root = doc.object();

    return !(root.contains("errors") &&
        root.value("errors").isArray() &&
        root.value("errors").toArray().count() > 0);
}
