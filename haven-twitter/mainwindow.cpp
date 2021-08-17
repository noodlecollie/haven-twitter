#include <QTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "splashdialogue.h"
#include "twitterapplication.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_App(new TwitterApplication(this))
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

TwitterApplication* MainWindow::twitterApplication() const
{
    return m_App;
}

void MainWindow::showSplash()
{
    if ( !m_Splash )
    {
        m_Splash = new SplashDialogue(this);
        connect(m_Splash, &SplashDialogue::authCompleted, this, &MainWindow::onAuthCompleted);
    }

    QTimer::singleShot(1000, m_Splash, &SplashDialogue::attemptInitialAuth);
    m_Splash->open();
}

void MainWindow::onAuthCompleted()
{
    delete m_Splash;
    m_Splash = nullptr;

    show();
}
