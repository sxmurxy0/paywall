#include <QMessageBox>
#include <QDesktopServices>
#include "mainwindow.h"
#include "ui/ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    successWidget(new SuccessWidget(this)),
    verifier(new RSAVerifier(this)),
    storage(new StorageManager(verifier, this)),
    networkManager(new NetworkManager(verifier, this))
{
    successWidget->setVisible(false);

    ui->setupUi(this);

    connect(ui->activateButton, &QPushButton::clicked, 
            this, &MainWindow::onActivateButtonClick);
    connect(ui->licenseButton, &QPushButton::clicked, 
            this, &MainWindow::onLicenseButtonClick);

    connect(networkManager, &NetworkManager::signatureRecieved,
        this, &MainWindow::onSignatureRecieve);
    connect(networkManager, &NetworkManager::networkRequestFinished,
        this, &MainWindow::onNetworkRequestFinish);
    
    connect(networkManager, &NetworkManager::networkReplyErrorOccurred,
        this, &MainWindow::onErrorOccur);
    
    if (storage->hasValidSignature()) {
        showSuccessWidget();
    } else {
        ui->paywallWidget->setEnabled(false);
        networkManager->getSignatureRequest();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showSuccessWidget()
{
    ui->paywallWidget->setVisible(false);
    successWidget->setVisible(true);
    this->setCentralWidget(successWidget);
    
    successWidget->startAnimation();
}

void MainWindow::onActivateButtonClick()
{
    QString key = ui->keyInput->text();
    if (key.length() != LICENSE_KEY_SIZE) {
        QString str = "License key must be %1 characters long!";
        QMessageBox::warning(this, "Invalid license key!", str.arg(LICENSE_KEY_SIZE));
        return;
    }

    ui->paywallWidget->setEnabled(false);
    networkManager->activateKeyRequest(key);
}

void MainWindow::onLicenseButtonClick()
{
    QDesktopServices::openUrl(QUrl(BASE_URL + "/generate-key"));
}

void MainWindow::onSignatureRecieve(const QString &signature)
{
    storage->saveSignature(signature);
    showSuccessWidget();
}

void MainWindow::onNetworkRequestFinish()
{
    if (centralWidget() == ui->paywallWidget) {
        ui->paywallWidget->setEnabled(true);
    }
}

void MainWindow::onErrorOccur(const QString &error)
{
    QMessageBox::critical(this, "An error occurred!", error);
}
