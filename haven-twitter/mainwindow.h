#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TwitterApplication;
class SplashDialogue;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    TwitterApplication* twitterApplication() const;

public slots:
    void showSplash();

private slots:
    void onAuthCompleted();

private:
    Ui::MainWindow *ui;
    TwitterApplication* m_App = nullptr;
    SplashDialogue* m_Splash = nullptr;
};
#endif // MAINWINDOW_H
