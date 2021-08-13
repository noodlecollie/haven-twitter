#ifndef AUTHCONFIGMANAGER_H
#define AUTHCONFIGMANAGER_H

#include <QString>
#include <QObject>

#include "authconfigworkerthread.h"
#include "qt-aes/qaesencryption.h"

class QJsonDocument;

class AuthConfigManager : public QObject
{
    Q_OBJECT
public:
    AuthConfigManager(QObject* parent = nullptr);
    virtual ~AuthConfigManager();

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

    bool loadConfigFromFile(const QString& path);
    void saveToConfigFile(const QString& path);

    bool canAuthenticate() const;
    void beginAuthProcess();

private slots:
    void authProcessComplete();
    void setAccessTokenVars(QString accessToken, QString accessTokenSecret);

private:
    bool loadConfigFromJson(const QJsonDocument& doc);
    void saveConfigToJson(QJsonDocument& doc);
    QByteArray encryptJson(const QJsonDocument& doc);
    QByteArray decryptJson(const QByteArray& encryptedInput);

    static QAESEncryption m_Encryption;
    static QByteArray m_Key;
    static QByteArray m_InitVector;

    AuthConfigWorkerThread* m_WorkerThread = nullptr;

    QString m_Username;
    QString m_Password;
    QString m_ApiKey;
    QString m_ApiSecret;
    QString m_AccessToken;
    QString m_AccessTokenSecret;
};

#endif // AUTHCONFIGMANAGER_H
