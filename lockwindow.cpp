#include "lockwindow.h"
#include "ui_lockwindow.h"

LockWindow::LockWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LockWindow)
{
    ui->setupUi(this);

    // Connect signals to slots
    connect(ui->lockButton, &QPushButton::clicked, this, &LockWindow::handleLock);
    connect(ui->unlockButton, &QPushButton::clicked, this, &LockWindow::handleUnlock);
    
    // Set initial subtitle style
    ui->subtitleLabel->setStyleSheet("color: #7f8c8d;");
}

LockWindow::~LockWindow()
{
    delete ui;
}

void LockWindow::handleLock()
{
    QString app = ui->appComboBox->currentText();
    QString appPass = ui->appPasswordEdit->text();
    QString masterPass = ui->masterPasswordEdit->text();

    if (appPass.isEmpty() || masterPass.isEmpty()) {
        ui->subtitleLabel->setText("Please enter both passwords!");
        ui->subtitleLabel->setStyleSheet("color: #d32f2f;"); // Red for error
        return;
    }

    ui->subtitleLabel->setText("Locked " + app + " with password.");
    ui->subtitleLabel->setStyleSheet("color: #388e3c;"); // Green for success
}

void LockWindow::handleUnlock()
{
    QString app = ui->appComboBox->currentText();
    QString appPass = ui->appPasswordEdit->text();
    QString masterPass = ui->masterPasswordEdit->text();

    if (appPass.isEmpty() || masterPass.isEmpty()) {
        ui->subtitleLabel->setText("Please enter both passwords!");
        ui->subtitleLabel->setStyleSheet("color: #d32f2f;"); // Red for error
        return;
    }

    ui->subtitleLabel->setText("Unlocked " + app + ".");
    ui->subtitleLabel->setStyleSheet("color: #388e3c;"); // Green for success
}