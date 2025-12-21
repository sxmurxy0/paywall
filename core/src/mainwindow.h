#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "successwidget.h"
#include "rsaverifier.h"
#include "storagemanager.h"
#include "networkmanager.h"


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

const int LICENSE_KEY_SIZE = 32;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SuccessWidget *successWidget;
    RSAVerifier *verifier;
    StorageManager *storage;
    NetworkManager *networkManager;

    void showSuccessWidget();

public slots:
    void onActivateButtonClick();
    void onLicenseButtonClick();

    void onSignatureRecieve(const QString &signature);
    void onNetworkRequestFinish();

    void onErrorOccur(const QString &error);
};
#endif // MAINWINDOW_H
