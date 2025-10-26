#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QComboBox>
#include <QSettings>
#include <QTimer>
#include <QSet>
#include <QMap>
#include <QDebug>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include <QCryptographicHash>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcess>
#include <QStandardPaths>
#include <QDir>
#include <QSharedMemory>
#include <QCoreApplication>
#include <QFileInfo>

#include "version.h"

#include <windows.h>
#include <tlhelp32.h>

bool isVersionGreaterThan(const QString &v1, const QString &v2)
{
    QString version1 = v1;
    if (version1.startsWith("v"))
    {
        version1 = version1.mid(1);
    }
    QString version2 = v2;
    if (version2.startsWith("v"))
    {
        version2 = version2.mid(1);
    }

    QStringList v1_parts = version1.split('.');
    QStringList v2_parts = version2.split('.');

    for (int i = 0; i < qMax(v1_parts.size(), v2_parts.size()); ++i)
    {
        int part1 = (i < v1_parts.size()) ? v1_parts[i].toInt() : 0;
        int part2 = (i < v2_parts.size()) ? v2_parts[i].toInt() : 0;

        if (part1 > part2)
        {
            return true;
        }
        if (part1 < part2)
        {
            return false;
        }
    }
    return false;
}

QSet<QString> lockedApps;
QSet<QString> allowedProcesses;
QMap<QString, QString> appPaths;
QMap<QString, QString> appPasswords; // Map of app name -> hashed password
QString masterPasswordHash = "";     // Store hashed master password

// Hash password for security
QString hashPassword(const QString &password)
{
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}

// Helper function to get the full path for a data file
QString getDataFilePath(const QString &fileName)
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataPath);
    if (!dir.exists())
    {
        dir.mkpath(".");
    }
    return appDataPath + "/" + fileName;
}

// Load master password from file
void loadMasterPassword()
{
    QFile file(getDataFilePath("master_password.dat"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        masterPasswordHash = in.readLine().trimmed();
        file.close();
    }
}

// Save master password to file
void saveMasterPassword(const QString &password)
{
    masterPasswordHash = hashPassword(password);
    QFile file(getDataFilePath("master_password.dat"));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << masterPasswordHash;
        file.close();
    }
}

// Verify master password
bool verifyMasterPassword(const QString &password)
{
    return hashPassword(password) == masterPasswordHash;
}

// Load app passwords from file
void loadAppPasswords()
{
    appPasswords.clear();
    QFile file(getDataFilePath("app_passwords.dat"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty())
            {
                QStringList parts = line.split("|");
                if (parts.size() == 2)
                {
                    appPasswords[parts[0]] = parts[1]; // app name -> hashed password
                }
            }
        }
        file.close();
    }
}

// Save app passwords to file
void saveAppPasswords()
{
    QFile file(getDataFilePath("app_passwords.dat"));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        for (auto it = appPasswords.begin(); it != appPasswords.end(); ++it)
        {
            out << it.key() << "|" << it.value() << "\n";
        }
        file.close();
    }
}

// Verify app password
bool verifyAppPassword(const QString &appName, const QString &password)
{
    if (!appPasswords.contains(appName))
        return false;
    return hashPassword(password) == appPasswords[appName];
}

// Dialog to set up master password (first run)
class SetupPasswordDialog : public QDialog
{
    Q_OBJECT
public:
    SetupPasswordDialog(QWidget *parent = nullptr) : QDialog(parent)
    {
        setWindowTitle("Set Master Password");
        setModal(true);
        setWindowFlags((windowFlags() & ~Qt::WindowContextHelpButtonHint) | Qt::WindowStaysOnTopHint);

        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *msgLabel = new QLabel("Welcome to App Locker!\nPlease set your master password.\n\nThis password will be used to access the App Locker interface.");
        msgLabel->setWordWrap(true);
        layout->addWidget(msgLabel);

        QLabel *passLabel = new QLabel("New Master Password:");
        layout->addWidget(passLabel);

        passwordInput = new QLineEdit(this);
        passwordInput->setEchoMode(QLineEdit::Password);
        passwordInput->setPlaceholderText("Enter new master password");
        layout->addWidget(passwordInput);

        QLabel *confirmLabel = new QLabel("Confirm Master Password:");
        layout->addWidget(confirmLabel);

        confirmInput = new QLineEdit(this);
        confirmInput->setEchoMode(QLineEdit::Password);
        confirmInput->setPlaceholderText("Re-enter master password");
        layout->addWidget(confirmInput);

        QPushButton *okBtn = new QPushButton("Set Password", this);
        layout->addWidget(okBtn);

        connect(okBtn, &QPushButton::clicked, this, &SetupPasswordDialog::setPassword);
        connect(confirmInput, &QLineEdit::returnPressed, this, &SetupPasswordDialog::setPassword);

        resize(400, 250);
    }

private slots:
    void setPassword()
    {
        QString pass = passwordInput->text();
        QString confirm = confirmInput->text();

        if (pass.isEmpty())
        {
            QMessageBox::warning(this, "Empty Password", "Master password cannot be empty!");
            return;
        }

        if (pass.length() < 4)
        {
            QMessageBox::warning(this, "Weak Password", "Master password must be at least 4 characters!");
            return;
        }

        if (pass != confirm)
        {
            QMessageBox::warning(this, "Password Mismatch", "Passwords do not match!");
            confirmInput->clear();
            confirmInput->setFocus();
            return;
        }

        saveMasterPassword(pass);
        QMessageBox::information(this, "Success", "Master password has been set successfully!");
        accept();
    }

private:
    QLineEdit *passwordInput;
    QLineEdit *confirmInput;
};

// Dialog to verify master password before showing main window
class MasterPasswordDialog : public QDialog
{
    Q_OBJECT
public:
    MasterPasswordDialog(QWidget *parent = nullptr) : QDialog(parent)
    {
        setWindowTitle("App Locker - Authentication");
        setModal(true);
        setWindowFlags((windowFlags() & ~Qt::WindowContextHelpButtonHint) | Qt::WindowStaysOnTopHint);

        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *msgLabel = new QLabel("Enter your master password to access App Locker:");
        msgLabel->setWordWrap(true);
        layout->addWidget(msgLabel);

        passwordInput = new QLineEdit(this);
        passwordInput->setEchoMode(QLineEdit::Password);
        passwordInput->setPlaceholderText("Master password");
        layout->addWidget(passwordInput);

        QPushButton *okBtn = new QPushButton("Unlock", this);
        QPushButton *cancelBtn = new QPushButton("Cancel", this);

        QHBoxLayout *btnLayout = new QHBoxLayout();
        btnLayout->addWidget(okBtn);
        btnLayout->addWidget(cancelBtn);
        layout->addLayout(btnLayout);

        connect(okBtn, &QPushButton::clicked, this, &MasterPasswordDialog::checkPassword);
        connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
        connect(passwordInput, &QLineEdit::returnPressed, this, &MasterPasswordDialog::checkPassword);

        resize(350, 150);
    }

private slots:
    void checkPassword()
    {
        if (verifyMasterPassword(passwordInput->text()))
        {
            accept();
        }
        else
        {
            QMessageBox::warning(this, "Wrong Password", "Incorrect master password!");
            passwordInput->clear();
            passwordInput->setFocus();
        }
    }

private:
    QLineEdit *passwordInput;
};

// Dialog to change master password
class ChangePasswordDialog : public QDialog
{
    Q_OBJECT
public:
    ChangePasswordDialog(QWidget *parent = nullptr) : QDialog(parent)
    {
        setWindowTitle("Change Master Password");
        setModal(true);
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *oldLabel = new QLabel("Current Master Password:");
        layout->addWidget(oldLabel);

        oldPasswordInput = new QLineEdit(this);
        oldPasswordInput->setEchoMode(QLineEdit::Password);
        oldPasswordInput->setPlaceholderText("Enter current master password");
        layout->addWidget(oldPasswordInput);

        QLabel *newLabel = new QLabel("New Master Password:");
        layout->addWidget(newLabel);

        newPasswordInput = new QLineEdit(this);
        newPasswordInput->setEchoMode(QLineEdit::Password);
        newPasswordInput->setPlaceholderText("Enter new master password");
        layout->addWidget(newPasswordInput);

        QLabel *confirmLabel = new QLabel("Confirm New Master Password:");
        layout->addWidget(confirmLabel);

        confirmInput = new QLineEdit(this);
        confirmInput->setEchoMode(QLineEdit::Password);
        confirmInput->setPlaceholderText("Re-enter new master password");
        layout->addWidget(confirmInput);

        QPushButton *okBtn = new QPushButton("Change Password", this);
        QPushButton *cancelBtn = new QPushButton("Cancel", this);

        QHBoxLayout *btnLayout = new QHBoxLayout();
        btnLayout->addWidget(okBtn);
        btnLayout->addWidget(cancelBtn);
        layout->addLayout(btnLayout);

        connect(okBtn, &QPushButton::clicked, this, &ChangePasswordDialog::changePassword);
        connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

        resize(350, 250);
    }

private slots:
    void changePassword()
    {
        QString oldPass = oldPasswordInput->text();
        QString newPass = newPasswordInput->text();
        QString confirm = confirmInput->text();

        if (!verifyMasterPassword(oldPass))
        {
            QMessageBox::warning(this, "Wrong Password", "Current master password is incorrect!");
            oldPasswordInput->clear();
            oldPasswordInput->setFocus();
            return;
        }

        if (newPass.isEmpty())
        {
            QMessageBox::warning(this, "Empty Password", "New master password cannot be empty!");
            return;
        }

        if (newPass.length() < 4)
        {
            QMessageBox::warning(this, "Weak Password", "Master password must be at least 4 characters!");
            return;
        }

        if (newPass != confirm)
        {
            QMessageBox::warning(this, "Password Mismatch", "New passwords do not match!");
            confirmInput->clear();
            confirmInput->setFocus();
            return;
        }

        saveMasterPassword(newPass);
        QMessageBox::information(this, "Success", "Master password has been changed successfully!");
        accept();
    }

private:
    QLineEdit *oldPasswordInput;
    QLineEdit *newPasswordInput;
    QLineEdit *confirmInput;
};

// Password dialog that appears when locked app tries to open
class PasswordDialog : public QDialog
{
    Q_OBJECT
public:
    PasswordDialog(const QString &appName, QWidget *parent = nullptr)
        : QDialog(parent), appName(appName)
    {
        setWindowTitle("App Locked - " + appName);
        setModal(true);
        setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *msgLabel = new QLabel("This app is locked. Enter the app password to use it:");
        msgLabel->setWordWrap(true);
        layout->addWidget(msgLabel);

        QLabel *appLabel = new QLabel("<b>" + appName + "</b>");
        layout->addWidget(appLabel);

        passwordInput = new QLineEdit(this);
        passwordInput->setEchoMode(QLineEdit::Password);
        passwordInput->setPlaceholderText("Enter app password");
        layout->addWidget(passwordInput);

        QPushButton *okBtn = new QPushButton("OK", this);
        QPushButton *cancelBtn = new QPushButton("Cancel", this);

        QHBoxLayout *btnLayout = new QHBoxLayout();
        btnLayout->addWidget(okBtn);
        btnLayout->addWidget(cancelBtn);
        layout->addLayout(btnLayout);

        connect(okBtn, &QPushButton::clicked, this, &PasswordDialog::checkPassword);
        connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
        connect(passwordInput, &QLineEdit::returnPressed, this, &PasswordDialog::checkPassword);

        resize(350, 180);
    }

private slots:
    void checkPassword()
    {
        if (verifyAppPassword(appName, passwordInput->text()))
        {
            accept();
        }
        else
        {
            QMessageBox::warning(this, "Wrong Password", "Incorrect app password! Access denied.");
            passwordInput->clear();
            passwordInput->setFocus();
        }
    }

private:
    QLineEdit *passwordInput;
    QString appName;
};

bool isProcessRunning(const QString &processName, DWORD *outPID = nullptr)
{
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE)
        return false;

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    bool found = false;

    if (Process32First(hSnap, &pe32))
    {
        do
        {
            QString exe = QString::fromWCharArray(pe32.szExeFile);
            if (exe.compare(processName, Qt::CaseInsensitive) == 0)
            {
                if (outPID)
                    *outPID = pe32.th32ProcessID;
                found = true;
                break;
            }
        } while (Process32Next(hSnap, &pe32));
    }
    CloseHandle(hSnap);
    return found;
}

QString getProcessPath(const QString &processName)
{
    DWORD pid;
    if (!isProcessRunning(processName, &pid))
        return "";

    HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (hProc == nullptr)
        return "";

    wchar_t path[MAX_PATH];
    DWORD size = MAX_PATH;

    if (QueryFullProcessImageNameW(hProc, 0, path, &size))
    {
        CloseHandle(hProc);
        return QString::fromWCharArray(path);
    }

    CloseHandle(hProc);
    return "";
}

void launchProcess(const QString &path)
{
    if (path.isEmpty())
        return;

    STARTUPINFOW si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    std::wstring wPath = path.toStdWString();

    if (CreateProcessW(
            wPath.c_str(),
            nullptr,
            nullptr,
            nullptr,
            FALSE,
            0,
            nullptr,
            nullptr,
            &si,
            &pi))
    {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        qDebug() << "Launched:" << path;
    }
}

void killProcessByName(const QString &processName)
{
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE)
        return;

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnap, &pe32))
    {
        do
        {
            QString exe = QString::fromWCharArray(pe32.szExeFile);
            if (exe.compare(processName, Qt::CaseInsensitive) == 0)
            {
                HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                if (hProc != nullptr)
                {
                    TerminateProcess(hProc, 0);
                    CloseHandle(hProc);
                    qDebug() << "Terminated:" << exe;
                }
            }
        } while (Process32Next(hSnap, &pe32));
    }
    CloseHandle(hSnap);
}

void loadLockedApps()
{
    lockedApps.clear();
    QFile file(getDataFilePath("locked_apps.txt"));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString app = in.readLine().trimmed();
            if (!app.isEmpty())
                lockedApps.insert(app);
        }
        file.close();
    }
}

// Custom widget class to override closeEvent
class AppLockerWindow : public QWidget
{
    Q_OBJECT
public:
    QSystemTrayIcon *trayIcon;
    QMap<QString, bool> wasRunning;

    AppLockerWindow(QWidget *parent = nullptr) : QWidget(parent), trayIcon(nullptr) {}

    ~AppLockerWindow()
    {
        if (trayIcon)
        {
            trayIcon->hide();
            delete trayIcon;
        }
    }

    void checkLockedProcesses()
    {
        for (const QString &appName : lockedApps)
        {
            bool isRunning = isProcessRunning(appName);
            bool prevRunning = wasRunning.value(appName, false);

            if (isRunning && !prevRunning && !allowedProcesses.contains(appName))
            {
                QString path = getProcessPath(appName);
                if (!path.isEmpty() && !appPaths.contains(appName))
                {
                    appPaths[appName] = path;
                    qDebug() << "Saved path for" << appName << ":" << path;
                }

                killProcessByName(appName);

                PasswordDialog dialog(appName, this);
                int result = dialog.exec();

                if (result == QDialog::Accepted)
                {
                    allowedProcesses.insert(appName);

                    if (appPaths.contains(appName))
                    {
                        QTimer::singleShot(500, [appName]()
                                           { launchProcess(appPaths[appName]); });
                        trayIcon->showMessage("Access Granted",
                                              appName + " is being launched...",
                                              QSystemTrayIcon::Information, 2000);
                    }
                    else
                    {
                        trayIcon->showMessage("Access Granted",
                                              appName + " is now allowed. Please open it manually.",
                                              QSystemTrayIcon::Information, 3000);
                    }
                }
                else
                {
                    trayIcon->showMessage("Access Denied",
                                          appName + " was blocked. Access denied.",
                                          QSystemTrayIcon::Warning, 2000);
                }
                continue;
            }

            if (isRunning && !allowedProcesses.contains(appName))
            {
                killProcessByName(appName);
            }

            if (!isRunning && prevRunning)
            {
                allowedProcesses.remove(appName);
            }

            wasRunning[appName] = isRunning;
        }
    }

protected:
    void closeEvent(QCloseEvent *event) override
    {
        event->ignore();
        hide();
        if (trayIcon)
        {
            trayIcon->showMessage("App Locker",
                                  "App Locker is still running in the background.",
                                  QSystemTrayIcon::Information, 3000);
        }
    }
};

QMap<QString, QString> getInstalledApps()
{
    QMap<QString, QString> installedApps;

    // ✅ Registry locations (include 32-bit and 64-bit apps)
    QStringList uninstallKeys = {
        "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
        "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
        "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall"};

    for (const QString &uninstallPath : uninstallKeys)
    {
        QSettings settings(uninstallPath, QSettings::NativeFormat);
        QStringList subKeys = settings.childGroups();

        for (const QString &subKey : subKeys)
        {
            settings.beginGroup(subKey);
            QString displayName = settings.value("DisplayName").toString().trimmed();
            QString displayIcon = settings.value("DisplayIcon").toString().trimmed();
            QString installLocation = settings.value("InstallLocation").toString().trimmed();
            QString uninstallString = settings.value("UninstallString").toString().trimmed();
            bool systemComponent = settings.value("SystemComponent", 0).toInt() != 0;

            if (displayName.isEmpty() || systemComponent)
            {
                settings.endGroup();
                continue;
            }

            QString exeName;

            // Prefer DisplayIcon if valid
            if (!displayIcon.isEmpty())
            {
                QString cleanedIcon = displayIcon.split(',').first().remove('"');
                QFileInfo fi(cleanedIcon);
                if (fi.exists() && fi.suffix().compare("exe", Qt::CaseInsensitive) == 0)
                    exeName = fi.fileName();
            }

            // Fallback: try InstallLocation
            if (exeName.isEmpty() && !installLocation.isEmpty())
            {
                QDir dir(installLocation);
                QStringList exes = dir.entryList(QStringList("*.exe"), QDir::Files);
                if (!exes.isEmpty())
                    exeName = exes.first();
            }

            // Fallback: parse uninstall string
            if (exeName.isEmpty() && uninstallString.contains(".exe", Qt::CaseInsensitive))
            {
                QString candidate = uninstallString.split(' ').first().remove('"');
                QFileInfo fi(candidate);
                if (fi.exists())
                    exeName = fi.fileName();
            }

            if (!exeName.isEmpty() && !installedApps.contains(displayName))
                installedApps.insert(displayName, exeName);

            settings.endGroup();
        }
    }

    return installedApps;
}

QMap<QString, QString> getUwpApps()
{
    QMap<QString, QString> uwpApps;

    QProcess process;
    process.start("powershell", QStringList() << "Get-StartApps | Select-Object Name, AppID");
    process.waitForFinished();

    QString output = process.readAllStandardOutput();
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);

    for (const QString &line : lines.mid(2)) // skip headers and separator
    {
        QString trimmed = line.trimmed();
        if (trimmed.isEmpty()) continue;

        // Find the position of the AppID, which is the last part of the string
        int lastSpace = trimmed.lastIndexOf(' ');
        if (lastSpace == -1) continue;

        QString name = trimmed.left(lastSpace).trimmed();
        QString appId = trimmed.mid(lastSpace + 1).trimmed();

        if (!name.isEmpty() && !appId.isEmpty() && !uwpApps.contains(name))
        {
            uwpApps.insert(name, appId);
        }
    }

    return uwpApps;
}

QMap<QString, QString> getAllInstalledApps()
{
    QMap<QString, QString> allApps = getInstalledApps();  // Win32 apps
    QMap<QString, QString> uwpApps = getUwpApps();        // UWP apps

    for (auto it = uwpApps.begin(); it != uwpApps.end(); ++it)
    {
        if (!allApps.contains(it.key()))
            allApps.insert(it.key(), it.value());
    }

    return allApps;
}

int main(int argc, char *argv[])
{
    // CRITICAL FIX: Initialize Qt resources before anything else
    Q_INIT_RESOURCE(resources);

    QApplication app(argc, argv);

    QSharedMemory sharedMemory;
    sharedMemory.setKey("AppLocker_SingleInstance");

    if (sharedMemory.attach())
    {
        QMessageBox::warning(nullptr, "App Locker", "App Locker is already running.");
        return 0;
    }

    if (!sharedMemory.create(1))
    {
        QMessageBox::critical(nullptr, "App Locker", "Failed to create shared memory segment.");
        return 0;
    }

    // Load master password
    loadMasterPassword();

    // If no master password is set, show setup dialog
    if (masterPasswordHash.isEmpty())
    {
        SetupPasswordDialog setupDialog;
        if (setupDialog.exec() != QDialog::Accepted)
        {
            return 0; // User cancelled, exit
        }
    }

    // Load app passwords
    loadAppPasswords();

    QIcon icon;

    // Try loading icon from resources first
    if (QFile::exists(":/logo/icon_32x.png"))
    {
        icon = QIcon(":/logo/icon_32x.png");
        qDebug() << "Icon loaded from resources.";
    }
    else
    {
        QString appDirIconPath = QCoreApplication::applicationDirPath() + "/logo/icon.ico";
        if (QFile::exists(appDirIconPath))
        {
            icon = QIcon(appDirIconPath);
            qDebug() << "Icon loaded from application directory.";
        }
        else
        {
            qDebug() << "Icon not found. Using default.";
            icon = QApplication::style()->standardIcon(QStyle::SP_ComputerIcon);
        }
    }

    // Set application-wide icon
    app.setWindowIcon(icon);

    AppLockerWindow window;
    window.setWindowTitle("App Locker");
    window.resize(320, 320);
    window.setWindowIcon(icon);

    QLabel *label = new QLabel("Select App to Lock:", &window);
    label->move(30, 30);

    QComboBox *appComboBox = new QComboBox(&window);
    appComboBox->move(30, 60);
    appComboBox->resize(240, 25);
    QMap<QString, QString> installedApps = getAllInstalledApps();
    for (auto it = installedApps.begin(); it != installedApps.end(); ++it)
    {
        appComboBox->addItem(it.key(), it.value());
    }

    QLabel *passLabel = new QLabel("App Password:", &window);
    passLabel->move(30, 100);

    QLineEdit *passwordInput = new QLineEdit(&window);
    passwordInput->setPlaceholderText("Enter password for this app");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->move(30, 125);
    passwordInput->resize(240, 25);

    QLabel *masterPassLabel = new QLabel("Master Password:", &window);
    masterPassLabel->move(30, 165);

    QLineEdit *masterPasswordInput = new QLineEdit(&window);
    masterPasswordInput->setPlaceholderText("Enter your master password");
    masterPasswordInput->setEchoMode(QLineEdit::Password);
    masterPasswordInput->move(30, 190);
    masterPasswordInput->resize(240, 25);

    QPushButton *lockBtn = new QPushButton("Lock", &window);
    lockBtn->move(50, 230);

    QPushButton *unlockBtn = new QPushButton("Unlock", &window);
    unlockBtn->move(160, 230);

    QPushButton *changePassBtn = new QPushButton("Change Master Password", &window);
    changePassBtn->move(55, 270);
    changePassBtn->resize(180, 30);

    QPushButton *updateBtn = new QPushButton("Check for Updates", &window);
    updateBtn->move(55, 310);
    updateBtn->resize(180, 30);

    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(&app);

    QIcon trayIconImage(":/logo/icon_16x.png");

    if (!trayIconImage.isNull())
    {
        trayIcon->setIcon(trayIconImage);
        qDebug() << "Tray icon loaded successfully from embedded resource.";
    }
    else
    {
        qDebug() << "Failed to load tray icon. Using fallback system icon.";
        trayIcon->setIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));
    }

    trayIcon->setToolTip("App Locker - running in background");
    trayIcon->show();

    window.trayIcon = trayIcon;

    QMenu *trayMenu = new QMenu();
    QAction *showAction = new QAction("Show", &window);
    QAction *changePasswordAction = new QAction("Change Master Password", &window);
    QAction *exitAction = new QAction("Exit", &window);

    trayMenu->addAction(showAction);
    trayMenu->addAction(changePasswordAction);
    trayMenu->addSeparator();
    trayMenu->addAction(exitAction);

    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();

    // Tray actions
    QObject::connect(showAction, &QAction::triggered, [&]()
                     {
        // Require master password to show window
        MasterPasswordDialog authDialog(&window);
        if (authDialog.exec() == QDialog::Accepted) {
            window.showNormal();
            window.raise();
            window.activateWindow();
        } });

    QObject::connect(changePasswordAction, &QAction::triggered, [&]()
                     {
        ChangePasswordDialog dialog(&window);
        dialog.exec(); });

    QObject::connect(exitAction, &QAction::triggered, [&]()
                     {
        QMessageBox::StandardButton reply = QMessageBox::question(&window, 
            "Exit App Locker", 
            "Are you sure you want to exit App Locker?\nThis will stop monitoring locked applications.",
            QMessageBox::Yes | QMessageBox::No);
            
        if (reply == QMessageBox::Yes) {
            // Ask for master password before exiting
            MasterPasswordDialog authDialog(&window);
            authDialog.setWindowTitle("Confirm Exit - Authentication Required");
            
            if (authDialog.exec() == QDialog::Accepted) {
                trayIcon->hide();
                app.quit();
            }
        } });

    // Change password button
    QObject::connect(changePassBtn, &QPushButton::clicked, [&]()
                     {
        ChangePasswordDialog dialog(&window);
        dialog.exec(); });

    loadLockedApps();

    QObject::connect(lockBtn, &QPushButton::clicked, [&]()
                     {
        if (appComboBox->currentIndex() < 0) return;
        QString appName = appComboBox->currentData().toString();
        QString appPass = passwordInput->text().trimmed();
        QString masterPass = masterPasswordInput->text().trimmed();

        if (!verifyMasterPassword(masterPass)) {
            QMessageBox::warning(&window, "Access Denied", "Incorrect master password!");
            masterPasswordInput->clear();
            return;
        }

        if (appName.isEmpty()) {
            QMessageBox::warning(&window, "Warning", "Please enter an app name!");
            return;
        }

        if (appPass.isEmpty()) {
            QMessageBox::warning(&window, "Warning", "Please enter a password for this app!");
            return;
        }

        if (appPass.length() < 4) {
            QMessageBox::warning(&window, "Weak Password", "App password must be at least 4 characters!");
            return;
        }

        if (lockedApps.contains(appName)) {
            QMessageBox::information(&window, "Already Locked", appName + " is already locked!");
            return;
        }

        // Add to locked apps list
        QFile file(getDataFilePath("locked_apps.txt"));
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << appName << "\n";
            file.close();
        }

        // Store app password
        appPasswords[appName] = hashPassword(appPass);
        saveAppPasswords();

        lockedApps.insert(appName);
        QMessageBox::information(&window, "Locked", appName + " has been locked with its own password!");
        passwordInput->clear();
        masterPasswordInput->clear(); });

    QObject::connect(unlockBtn, &QPushButton::clicked, [&]()
                     {
        if (appComboBox->currentIndex() < 0) return;
        QString appName = appComboBox->currentData().toString();
        QString masterPass = masterPasswordInput->text().trimmed();

        if (!verifyMasterPassword(masterPass)) {
            QMessageBox::warning(&window, "Access Denied", "Incorrect master password!");
            masterPasswordInput->clear();
            return;
        }

        if (appName.isEmpty()) {
            QMessageBox::warning(&window, "Warning", "Please enter an app name!");
            return;
        }

        if (!lockedApps.contains(appName)) {
            QMessageBox::information(&window, "Not Locked", appName + " is not locked!");
            return;
        }

        // Remove from locked apps
        lockedApps.remove(appName);
        allowedProcesses.remove(appName);
        appPasswords.remove(appName);

        // Update files
        QFile file(getDataFilePath("locked_apps.txt"));
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            for (const QString &a : lockedApps)
                out << a << "\n";
            file.close();
        }

        saveAppPasswords();

        QMessageBox::information(&window, "Unlocked", appName + " has been unlocked!");
        passwordInput->clear();
        masterPasswordInput->clear(); });

    // Timer to monitor locked processes
    QTimer *timer = new QTimer(&window);
    QObject::connect(timer, &QTimer::timeout, [&]()
                     { window.checkLockedProcesses(); });
    timer->start(1000);

    // Update check
    QNetworkAccessManager *manager = new QNetworkAccessManager(&window);
    QObject::connect(updateBtn, &QPushButton::clicked, [&]()
                     {
        QNetworkRequest request(QUrl("https://api.github.com/repos/WilardzySenpai/AppLocker/releases/latest"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QNetworkReply *reply = manager->get(request);
        QObject::connect(reply, &QNetworkReply::finished, [reply, &window]() {
            if (reply->error() == QNetworkReply::NoError) {
                QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
                QJsonObject obj = doc.object();
                QString latestVersion = obj["tag_name"].toString();
                if (isVersionGreaterThan(latestVersion, APP_VERSION)) {
                    QMessageBox::StandardButton buttonReply = QMessageBox::question(&window, "Update Available", "A new version of App Locker is available. Do you want to download and install it?", QMessageBox::Yes|QMessageBox::No);
                    if (buttonReply == QMessageBox::Yes) {
                        QJsonArray assets = obj["assets"].toArray();
                        if (assets.size() > 0) {
                            QString downloadUrl = assets[0].toObject()["browser_download_url"].toString();
                            QProcess::startDetached("explorer", QStringList() << downloadUrl);
                        }
                    }
                } else {
                    QMessageBox::information(&window, "No Update", "You are using the latest version of App Locker.");
                }
            } else {
                QMessageBox::warning(&window, "Error", "Failed to check for updates: " + reply->errorString());
            }
            reply->deleteLater();
        }); });

    window.show();
    return app.exec();
}

#include "main.moc"