#include <QLineEdit>
#include <QTimer>
#include <QDir>

#include "splashdialogue.h"
#include "ui_splashdialogue.h"
#include "mainwindow.h"

SplashDialogue::SplashDialogue(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::SplashDialogue),
    m_MainWindow(parent)
{
    ui->setupUi(this);

    ui->pages->setCurrentIndex(SplashPage);
    QWidget* loginPage = ui->pages->widget(LoginPage);

    if ( loginPage )
    {
        m_UsernameInput = loginPage->findChild<QLineEdit*>("username");
        m_PasswordInput = loginPage->findChild<QLineEdit*>("password");
        m_LoginButton = loginPage->findChild<QPushButton*>("loginButton");
    }
}

SplashDialogue::~SplashDialogue()
{
    delete ui;
}

void SplashDialogue::setPage(Page page)
{
    ui->pages->setCurrentIndex(page);
}

void SplashDialogue::usernameOrPasswordUpdated()
{
    if ( !m_LoginButton )
    {
        return;
    }

    m_LoginButton->setEnabled(canAttemptLogIn());
}

void SplashDialogue::doRequestLogin()
{
    if ( !canAttemptLogIn() )
    {
        return;
    }

    emit requestLogin(getUsername(), getPassword());
}

bool SplashDialogue::canAttemptLogIn() const
{
    return getUsername().length() > 0 && getPassword().length() > 0;
}

QString SplashDialogue::getUsername() const
{
    return m_UsernameInput ? m_UsernameInput->text().trimmed() : QString();
}

QString SplashDialogue::getPassword() const
{
    return m_PasswordInput ? m_PasswordInput->text() : QString();
}
