#ifndef LOCKWINDOW_H
#define LOCKWINDOW_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QLabel;

class LockWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LockWindow(QWidget *parent = nullptr);

private slots:
    void handleLock();
    void handleUnlock();

private:
    QLineEdit *passwordInput;
    QLineEdit *tagInput;
    QLabel *statusLabel;
};

#endif // LOCKWINDOW_H
