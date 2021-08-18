#ifndef AUTHCONFIGMANAGER_H
#define AUTHCONFIGMANAGER_H

#include <QString>
#include <QObject>
#include <QPointer>

#include "authconfigworkerthread.h"
#include "qaesencryption.h"

class QJsonDocument;
class TwitterApplication;

class AuthConfigManager : public QObject
{
    Q_OBJECT
public:
    AuthConfigManager(QObject* parent = nullptr);
    virtual ~AuthConfigManager();

    void setTwitterApplication(TwitterApplication* app);

    QString username() const;
    void setUsername(const QString& username);

    QString password() const;
    void setPassword(const QString& password);

    QString apiKey() const;
    void setAPIKey(const QString& key);

    QString apiSecret() const;
    void setAPISecret(const QString& secret);

    QString accessToken() const;
    void setAccessToken(const QString& token);

    QString accessTokenSecret() const;
    void setAccessTokenSecret(const QString& secret);

    QString defaultConfigFilePath() const;
    bool loadConfigFromFile(const QString& path);
    void saveToConfigFile(const QString& path);

    bool hasApiCredentials() const;
    bool canAuthenticate() const;
    void beginAuthProcess();

signals:
    void authProcessComplete();
    void authProcessFailed(const QString& message);

private slots:
    void onAuthProcessComplete(bool success, twitCurl* tcObj, QString errorString);
    void disposeOfWorkerThread();

private:
    bool loadConfigFromJson(const QJsonDocument& doc);
    void saveConfigToJson(QJsonDocument& doc);
    QByteArray encryptJson(const QJsonDocument& doc);
    QByteArray decryptJson(const QByteArray& encryptedInput);
    void generateConsumerKeyAndSecret();

    static QAESEncryption m_Encryption;
    static QByteArray m_Key;
    static QByteArray m_InitVector;

    AuthConfigWorkerThread* m_WorkerThread = nullptr;
    QPointer<TwitterApplication> m_TwitterApplication;

    QString m_Username;
    QString m_Password;
    QString m_ApiKey;
    QString m_ApiSecret;
    QString m_AccessToken;
    QString m_AccessTokenSecret;
};

#endif // AUTHCONFIGMANAGER_H
