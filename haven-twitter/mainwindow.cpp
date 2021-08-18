#include <QTimer>
#include <QtDebug>
#include <QApplication>

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
        connect(m_Splash, &SplashDialogue::closing, this, &MainWindow::onSplashClose);
    }

    QTimer::singleShot(1000, m_Splash, &SplashDialogue::attemptInitialAuth);
    m_Splash->open();
}

void MainWindow::onSplashClose()
{
    if ( m_Splash )
    {
        delete m_Splash;
        m_Splash = nullptr;
    }

    if ( !m_App->isInitialised() )
    {
        qDebug() << "Auth process did not complete, shutting down.";
        qApp->quit();
    }
}
