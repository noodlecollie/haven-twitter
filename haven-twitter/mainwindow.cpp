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
    SplashDialogue* splash = new SplashDialogue(this);
    splash->open();
}
