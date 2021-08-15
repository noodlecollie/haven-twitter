#include <QCryptographicHash>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qaesencryption.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateControlState()
{
    bool allowEncryption = canEncrypt();

    ui->encryptButton->setEnabled(allowEncryption);
}

void MainWindow::encrypt()
{
    if ( !canEncrypt() )
    {
        return;
    }

    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);

    QString inputStr = ui->inputPlainText->toPlainText();
    QString key = ui->inputKey->text();
    QString iv = ui->inputIV->text();

    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);
    QByteArray encodeText = encryption.encode(inputStr.toLocal8Bit(), hashKey, hashIV).toHex();

    QString content;

    int halfLength = encodeText.length() / 2;

    for ( int index = 0; index < halfLength; ++index )
    {
        content += QString("\\x%0%1").arg(encodeText[2 * index]).arg(encodeText[(2 * index) + 1]);
    }

    ui->outputCiphertext->setPlainText(content);
}

void MainWindow::copyToClipboard()
{
    ui->outputCiphertext->selectAll();
    ui->outputCiphertext->copy();
}

bool MainWindow::canEncrypt() const
{
    return
        !ui->inputKey->text().isEmpty() &&
        !ui->inputIV->text().isEmpty() &&
        !ui->inputPlainText->toPlainText().isEmpty();
}
