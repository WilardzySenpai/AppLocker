#ifndef LOCKWINDOW_H
#define LOCKWINDOW_H

#include <QWidget>

class QLabel;
class QLineEdit;

class LockWindow : public QWidget
{
    Q_OBJECT

public:
    LockWindow(QWidget *parent = nullptr);

private slots:
    void handleLock();
    void handleUnlock();

private:
    QLineEdit *tagInput;
    QLineEdit *passwordInput;
    QLabel *statusLabel;
};

#endif // LOCKWINDOW_H
