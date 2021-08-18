#ifndef SPLASHDIALOGUE_H
#define SPLASHDIALOGUE_H

#include <QDialog>
#include "authconfigmanager.h"

class QLineEdit;
class MainWindow;

namespace Ui {
class SplashDialogue;
}

class SplashDialogue : public QDialog
{
    Q_OBJECT

public:
    enum Page
    {
        SplashPage = 0,
        LoginPage,
        LoginInProgressPage,
        LoginFailurePage,
        FatalErrorPage
    };

    explicit SplashDialogue(MainWindow *parent = nullptr);
    ~SplashDialogue();

    void setPage(Page page);

signals:
    void closing();

public slots:
    void attemptInitialAuth();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void usernameOrPasswordUpdated();
    void doRequestLogin();
    void handleAuthProcessComplete();
    void handleAuthProcessFailed(const QString& message);
    void goToLoginPage();

private:
    bool canAttemptLogIn() const;
    QString getUsername() const;
    QString getPassword() const;
    void beginAuth();

    Ui::SplashDialogue *ui;

    MainWindow* m_MainWindow = nullptr;
    AuthConfigManager* m_Auth = nullptr;
};

#endif // SPLASHDIALOGUE_H
