#ifndef SPLASHDIALOGUE_H
#define SPLASHDIALOGUE_H

#include <QDialog>

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
        LoginInProgressPage
    };

    explicit SplashDialogue(MainWindow *parent = nullptr);
    ~SplashDialogue();

    void setPage(Page page);

signals:
    void requestLogin(const QString& username, const QString& password);

private slots:
    void usernameOrPasswordUpdated();
    void doRequestLogin();

private:
    bool canAttemptLogIn() const;
    QString getUsername() const;
    QString getPassword() const;

    Ui::SplashDialogue *ui;

    MainWindow* m_MainWindow = nullptr;
    QLineEdit* m_UsernameInput = nullptr;
    QLineEdit* m_PasswordInput = nullptr;
    QPushButton* m_LoginButton = nullptr;
};

#endif // SPLASHDIALOGUE_H
