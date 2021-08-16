#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QSysInfo>
#include <QtDebug>

#include "authconfigmanager.h"

static constexpr const char* const CFG_API_KEY = "apiKey";
static constexpr const char* const CFG_API_SECRET = "apiSecret";
static constexpr const char* const CFG_ACCESS_TOKEN = "accessToken";
static constexpr const char* const CFG_ACCESS_TOKEN_SECRET = "accessTokenSecret";
static constexpr const char* const CFG_USERNAME = "username";

static inline QString GetString(const QJsonObject& root, const QString& key)
{
    if ( root.contains(key))
    {
        QJsonValue value = root.value(key);

        if ( value.isString() )
        {
            return value.toString();
        }
    }

    return QString();
}

QAESEncryption AuthConfigManager::m_Encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
QByteArray AuthConfigManager::m_Key(QCryptographicHash::hash(QSysInfo::machineUniqueId() + QByteArray("+ several grains of salt"), QCryptographicHash::Sha256));
QByteArray AuthConfigManager::m_InitVector(QCryptographicHash::hash(QString("A seemingly harmless plant is actually a carnivore with flowers that eat insects").toLocal8Bit(), QCryptographicHash::Md5));

AuthConfigManager::AuthConfigManager(QObject* parent) :
    QObject(parent)
{
    generateConsumerKeyAndSecret();
}

AuthConfigManager::~AuthConfigManager()
{
    if ( m_WorkerThread )
    {
        m_WorkerThread->quit();
        m_WorkerThread->wait();
    }
}

QString AuthConfigManager::username() const
{
    return m_Username;
}

void AuthConfigManager::setUsername(const QString& username)
{
    m_Username = username;
}

QString AuthConfigManager::password() const
{
    return m_Password;
}

void AuthConfigManager::setPassword(const QString& password)
{
    m_Password = password;
}

QString AuthConfigManager::apiKey() const
{
    return m_ApiKey;
}

void AuthConfigManager::setAPIKey(const QString& key)
{
    m_ApiKey = key;
}

QString AuthConfigManager::apiSecret() const
{
    return m_ApiSecret;
}

void AuthConfigManager::setAPISecret(const QString& secret)
{
    m_ApiSecret = secret;
}

QString AuthConfigManager::accessToken() const
{
    return m_AccessToken;
}

void AuthConfigManager::setAccessToken(const QString& token)
{
    m_AccessToken = token;
}

QString AuthConfigManager::accessTokenSecret() const
{
    return m_AccessTokenSecret;
}

void AuthConfigManager::setAccessTokenSecret(const QString& secret)
{
    m_AccessTokenSecret = secret;
}

bool AuthConfigManager::canAuthenticate() const
{
    if ( m_ApiKey.isEmpty() || m_ApiSecret.isEmpty() )
    {
        return false;
    }

    // We need either a username and password, or a token/secret pair.
    return (!m_Username.isEmpty() && !m_Password.isEmpty()) ||
           (!m_AccessToken.isEmpty() && !m_AccessTokenSecret.isEmpty());
}

bool AuthConfigManager::loadConfigFromFile(const QString& path)
{
    qDebug() << "Attempting to load auth config from file:" << path;

    QFile inFile(path);

    if ( !inFile.open(QIODevice::ReadOnly) )
    {
        qDebug() << "No auth config file found on disk.";
        return false;
    }

    QByteArray content = inFile.readAll();
    inFile.close();

    if ( content.length() < 1 )
    {
        qDebug() << "Auth config file was empty.";
        return false;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(decryptJson(content));

    if ( jsonDoc.isNull() )
    {
        qDebug() << "Could not parse auth config file.";
        return false;
    }

    return loadConfigFromJson(jsonDoc);
}

void AuthConfigManager::saveToConfigFile(const QString& path)
{
    QJsonDocument doc;
    saveConfigToJson(doc);

    QFile outFile(path);

    if ( outFile.open(QIODevice::WriteOnly) )
    {
        outFile.write(encryptJson(doc));
        outFile.close();
    }
}

void AuthConfigManager::beginAuthProcess()
{
    if ( m_WorkerThread )
    {
        return;
    }

    m_WorkerThread = new AuthConfigWorkerThread(this);

    m_WorkerThread->setUsername(m_Username);
    m_WorkerThread->setPassword(m_Password);
    m_WorkerThread->setAPIKey(m_ApiKey);
    m_WorkerThread->setAPISecret(m_ApiSecret);
    m_WorkerThread->setAccessToken(m_AccessToken);
    m_WorkerThread->setAccessTokenSecret(m_AccessTokenSecret);

    connect(m_WorkerThread, &AuthConfigWorkerThread::newAccessTokenAcquired, this, &AuthConfigManager::setAccessTokenVars, Qt::QueuedConnection);
    connect(m_WorkerThread, &AuthConfigWorkerThread::finished, this, &AuthConfigManager::authProcessComplete);

    m_WorkerThread->start();
}

void AuthConfigManager::authProcessComplete()
{
    if ( !m_WorkerThread )
    {
        return;
    }

    m_WorkerThread->deleteLater();
    m_WorkerThread = nullptr;
}

void AuthConfigManager::setAccessTokenVars(QString accessToken, QString accessTokenSecret)
{
    m_AccessToken = accessToken;
    m_AccessTokenSecret = accessTokenSecret;
}

bool AuthConfigManager::loadConfigFromJson(const QJsonDocument& doc)
{
    if ( !doc.isObject() )
    {
        qDebug() << "Auth config file was not formatted correctly.";
        return false;
    }

    QJsonObject root = doc.object();

    m_AccessToken = GetString(root, CFG_ACCESS_TOKEN);
    m_AccessTokenSecret = GetString(root, CFG_ACCESS_TOKEN_SECRET);
    m_Username = GetString(root, CFG_USERNAME);

    return true;
}

void AuthConfigManager::saveConfigToJson(QJsonDocument& doc)
{
    if ( !doc.isObject() )
    {
        return;
    }

    QJsonObject root = doc.object();

    root.insert(CFG_ACCESS_TOKEN, m_AccessToken);
    root.insert(CFG_ACCESS_TOKEN_SECRET, m_AccessTokenSecret);
    root.insert(CFG_USERNAME, m_Username);
}

QByteArray AuthConfigManager::encryptJson(const QJsonDocument& doc)
{
    return m_Encryption.encode(doc.toJson(), m_Key, m_InitVector);
}

QByteArray AuthConfigManager::decryptJson(const QByteArray& encryptedInput)
{
    return m_Encryption.removePadding(m_Encryption.decode(encryptedInput, m_Key, m_InitVector));
}

void AuthConfigManager::generateConsumerKeyAndSecret()
{
    QFile inFile(":/ecapikas");

    if ( !inFile.open(QIODevice::ReadOnly) )
    {
        return;
    }

    QByteArray data = inFile.readAll();
    inFile.close();

    QByteArray key;

    {
        // Yeah we can't really hide the credentials in any significant way,
        // given this is open source, but at least we can stop people
        // directly grepping the source code/binaries and make them work for it.

        unsigned char magic[8] =
        {
            0b10100001,
            0b10101010,
            0b00011011,
            0b10011111,
            0b11110001,
            0b01000100,
            0b01010111,
            0b00000111
        };

        char cauldron[16];

        for ( int index = 0; index < static_cast<int>(sizeof(cauldron)); ++index )
        {
            cauldron[index] = 0x30 + ((((magic[(index / 2) % sizeof(magic)] ^ 0xAA) << ((index % 2) * 4)) & 0xFF) >> 4);
            cauldron[index] += (cauldron[index] >= 0x3A ? 0x07 : 0x00);
        }

        key = QCryptographicHash::hash(QByteArray(cauldron, sizeof(cauldron)), QCryptographicHash::Sha256);
    }

    QJsonDocument doc = QJsonDocument::fromJson(m_Encryption.removePadding(m_Encryption.decode(data, key, m_InitVector)));

    if ( doc.isObject() )
    {
        QJsonObject root = doc.object();
        m_ApiKey = GetString(root, CFG_API_KEY);
        m_ApiSecret = GetString(root, CFG_API_SECRET);
    }
}
