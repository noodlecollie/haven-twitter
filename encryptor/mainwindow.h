#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateControlState();
    void encrypt();
    void copyToClipboard();
    void setHexMode(bool enabled);

private:
    bool canEncrypt() const;
    void toBytes(const QString& input, QByteArray& output) const;

    Ui::MainWindow *ui;
    bool m_InterpretAsHex = false;
};
#endif // MAINWINDOW_H
