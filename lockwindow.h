#ifndef LOCKWINDOW_H
#define LOCKWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class LockWindow; }
QT_END_NAMESPACE

class LockWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LockWindow(QWidget *parent = nullptr);
    ~LockWindow();

private slots:
    void handleLock();
    void handleUnlock();
    
private:
    Ui::LockWindow *ui;
};

#endif