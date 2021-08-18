#ifndef AUTHCONFIGWORKERTHREAD_H
#define AUTHCONFIGWORKERTHREAD_H

#include <QThread>
#include "twitcurl.h"

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
    // The caller takes ownership of the twitCurl object.
    // It is guaranteed to always be valid.
    void authFinished(bool success, twitCurl* tcObj, QString errorString);

private:
    void run() override;
    bool receivedErrorResponse(twitCurl* tcObj, QString& errorResponse) const;

    QString m_ApiKey;
    QString m_ApiSecret;
    QString m_AccessToken;
    QString m_AccessTokenSecret;
    QString m_Username;
    QString m_Password;

    bool m_ReceivedNewAccessToken = false;
};

#endif // AUTHCONFIGWORKERTHREAD_H
