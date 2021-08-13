#ifndef DEBUGOAUTHCREDENTIALINPUT_H
#define DEBUGOAUTHCREDENTIALINPUT_H

#include <QDialog>

namespace Ui {
class DebugOAuthCredentialInput;
}

class DebugOAuthCredentialInput : public QDialog
{
    Q_OBJECT

public:
    explicit DebugOAuthCredentialInput(QWidget *parent = nullptr);
    ~DebugOAuthCredentialInput();

    QString username() const;
    QString password() const;
    QString apiKey() const;
    QString apiSecret() const;
    QString accessToken() const;
    QString accessTokenSecret() const;

private:
    Ui::DebugOAuthCredentialInput *ui;
};

#endif // DEBUGOAUTHCREDENTIALINPUT_H
