#include "debugoauthcredentialinput.h"
#include "ui_debugoauthcredentialinput.h"

DebugOAuthCredentialInput::DebugOAuthCredentialInput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugOAuthCredentialInput)
{
    ui->setupUi(this);
}

DebugOAuthCredentialInput::~DebugOAuthCredentialInput()
{
    delete ui;
}

QString DebugOAuthCredentialInput::username() const
{
    return ui->username->text();
}

QString DebugOAuthCredentialInput::password() const
{
    return ui->password->text();
}

QString DebugOAuthCredentialInput::apiKey() const
{
    return ui->apiKey->text();
}

QString DebugOAuthCredentialInput::apiSecret() const
{
    return ui->apiSecret->text();
}

QString DebugOAuthCredentialInput::accessToken() const
{
    return ui->accessToken->text();
}

QString DebugOAuthCredentialInput::accessTokenSecret() const
{
    return ui->accessTokenSecret->text();
}
