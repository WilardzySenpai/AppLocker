#include "lockwindow.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

LockWindow::LockWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("App Locker");
    resize(400, 300);

    QLabel *tagLabel = new QLabel("App Tag:");
    tagInput = new QLineEdit();

    QLabel *passwordLabel = new QLabel("Password:");
    passwordInput = new QLineEdit();
    passwordInput->setEchoMode(QLineEdit::Password);

    QPushButton *lockButton = new QPushButton("Lock");
    QPushButton *unlockButton = new QPushButton("Unlock");

    statusLabel = new QLabel("");

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setSpacing(10);  // Add spacing between widgets
    layout->setContentsMargins(20, 20, 20, 20);  // Add margins around the layout
    
    layout->addWidget(tagLabel);
    layout->addWidget(tagInput);
    layout->addWidget(passwordLabel);
    layout->addWidget(passwordInput);
    layout->addWidget(lockButton);
    layout->addWidget(unlockButton);
    layout->addSpacing(10);  // Add extra space before status label
    layout->addWidget(statusLabel);

    setLayout(layout);

    connect(lockButton, &QPushButton::clicked, this, &LockWindow::handleLock);
    connect(unlockButton, &QPushButton::clicked, this, &LockWindow::handleUnlock);
}

void LockWindow::handleLock()
{
    QString tag = tagInput->text();
    QString pass = passwordInput->text();

    if (tag.isEmpty() || pass.isEmpty()) {
        statusLabel->setText("Please enter both tag and password!");
        return;
    }

    statusLabel->setText("Locked " + tag + " with password.");
}

void LockWindow::handleUnlock()
{
    QString tag = tagInput->text();
    QString pass = passwordInput->text();

    if (tag.isEmpty() || pass.isEmpty()) {
        statusLabel->setText("Please enter both tag and password!");
        return;
    }

    statusLabel->setText("Unlocked " + tag + ".");
}
