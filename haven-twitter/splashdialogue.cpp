#include <QLineEdit>
#include <QTimer>
#include <QDir>
#include <QtDebug>

#include "splashdialogue.h"
#include "ui_splashdialogue.h"
#include "mainwindow.h"

SplashDialogue::SplashDialogue(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::SplashDialogue),
    m_MainWindow(parent),
    m_Auth(new AuthConfigManager(this))
{
    ui->setupUi(this);
    ui->pages->setCurrentIndex(SplashPage);

    if ( m_MainWindow )
    {
        m_Auth->setTwitterApplication(m_MainWindow->twitterApplication());
    }

    connect(m_Auth, &AuthConfigManager::authProcessComplete, this, &SplashDialogue::handleAuthProcessComplete);
    connect(m_Auth, &AuthConfigManager::authProcessFailed, this, &SplashDialogue::handleAuthProcessFailed);
}

SplashDialogue::~SplashDialogue()
{
    delete ui;
}

void SplashDialogue::setPage(Page page)
{
    ui->pages->setCurrentIndex(page);
}

void SplashDialogue::attemptInitialAuth()
{
    if ( !m_Auth->hasApiCredentials() )
    {
        qCritical() << "Unable to obtain API keys.";
        setPage(FatalErrorPage);
        return;
    }

    if ( !m_Auth->loadConfigFromFile(m_Auth->defaultConfigFilePath()) ||
         !m_Auth->canAuthenticate() )
    {
        qDebug() << "Cannot authenticate yet, user login is required.";
        setPage(LoginPage);
        return;
    }

    beginAuth();
}

void SplashDialogue::usernameOrPasswordUpdated()
{
    ui->loginButton->setEnabled(canAttemptLogIn());
}

void SplashDialogue::doRequestLogin()
{
    if ( !canAttemptLogIn() )
    {
        return;
    }

    m_Auth->setUsername(getUsername());
    m_Auth->setPassword(getPassword());

    beginAuth();
}

void SplashDialogue::handleAuthProcessComplete()
{
    qDebug() << "Auth process complete.";

    m_Auth->saveToConfigFile(m_Auth->defaultConfigFilePath());
    close();
}

void SplashDialogue::handleAuthProcessFailed(const QString& message)
{
    qDebug() << "Auth process failed.";

    if ( m_Auth->username().isEmpty() )
    {
        ui->loginFailedTitleLabel->setText(tr("<h2>Login Failed</h2>"));
    }
    else
    {
        ui->loginFailedTitleLabel->setText(tr("<h2>Login Failed For %0</h2>").arg(m_Auth->username()));
    }

    ui->loginFailedDescLabel->setText(message.isEmpty() ? tr("Please try again.") : message);
    setPage(LoginFailurePage);
}

void SplashDialogue::goToLoginPage()
{
    ui->password->clear();
    setPage(LoginPage);
}

bool SplashDialogue::canAttemptLogIn() const
{
    return getUsername().length() > 0 && getPassword().length() > 0;
}

QString SplashDialogue::getUsername() const
{
    return ui->username->text().trimmed();
}

QString SplashDialogue::getPassword() const
{
    return ui->password->text();
}

void SplashDialogue::beginAuth()
{
    qDebug() << "Beginning auth process.";
    m_Auth->beginAuthProcess();
    setPage(LoginInProgressPage);
}

void SplashDialogue::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    qDebug() << "Closing splash dialogue.";
    emit closing();
}
