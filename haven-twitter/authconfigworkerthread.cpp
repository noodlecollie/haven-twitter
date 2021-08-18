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

    bool success = false;
    QString errorString;

    if ( !tcObj->oAuthAccessToken() )
    {
        qWarning() << "AuthConfigWorkerThread: Failed to authenticate with Twitter.";
        errorString = "Failed to authenticate with Twitter.";
    }
    else
    {
        tcObj->accountVerifyCredGet();

        if ( receivedErrorResponse(tcObj, errorString) )
        {
            qWarning() << "AuthConfigWorkerThread: Failed to authenticate with Twitter. Response:" << errorString;
        }
        else
        {
            success = true;
            qDebug() << "AuthConfigWorkerThread: OAuth authentication succeeded.";
        }
    }

    emit authFinished(success, tcObj, errorString);
}

bool AuthConfigWorkerThread::receivedErrorResponse(twitCurl* tcObj, QString& errorResponse) const
{
    // For some reason, some requests report that they were successful
    // even though Twitter returned an error. We check for the error here.
    // It's fairly quick and dirty but it should do.

    std::string content;
    tcObj->getLastCurlError(content);

    if ( !content.empty() )
    {
        // Definitely an error.
        errorResponse = QString::fromStdString(content);
        return true;
    }

    tcObj->getLastWebResponse(content);

    QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(content));

    if ( !doc.isObject() )
    {
        return true;
    }

    QJsonObject root = doc.object();

    if ( !root.contains("errors") )
    {
        // Assume all is fine.
        return false;
    }

    QJsonValue errorsJson = root.value("errors");

    if ( errorsJson.isArray() )
    {
        QJsonArray errorsArray = errorsJson.toArray();
        QStringList errorStrings;

        for ( QJsonValue err : errorsArray )
        {
            if ( !err.isObject() )
            {
                continue;
            }

            QJsonObject errObj = err.toObject();

            if ( !errObj.contains("message") )
            {
                continue;
            }

            QJsonValue messageValue = errObj.value("message");

            if ( !messageValue.isString() )
            {
                continue;
            }

            errorStrings.append(messageValue.toString());
        }

        errorResponse = errorStrings.join("; ");
    }

    if ( errorResponse.isEmpty() )
    {
        errorResponse = "Unknown error.";
    }

    return true;
}
