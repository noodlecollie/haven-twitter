#ifndef AUTHCONFIGWORKERTHREAD_H
#define AUTHCONFIGWORKERTHREAD_H

#include <QThread>
#include "twitcurl/twitcurl.h"

class AuthConfigWorkerThread : public QThread
{
    Q_OBJECT
public:
    AuthConfigWorkerThread(QObject* parent = nullptr);
    virtual ~AuthConfigWorkerThread();

    // These are not thread-safe. Do not call after calling start().
    void setAPIKey(const QString& key);
    void setAPISecret(const QString& secret);
    void setAccessToken(const QString& token);
    void setAccessTokenSecret(const QString& secret);
    void setUsername(const QString& username);
    void setPassword(const QString& password);

signals:
    void newAccessTokenAcquired(QString accessToken, QString accessTokenSecret);

private:
    void run() override;

    QString m_ApiKey;
    QString m_ApiSecret;
    QString m_AccessToken;
    QString m_AccessTokenSecret;
    QString m_Username;
    QString m_Password;

    twitCurl tcObj;
    bool m_ReceivedNewAccessToken = false;
};

#endif // AUTHCONFIGWORKERTHREAD_H
