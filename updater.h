#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVersionNumber>
#include <QProgressDialog>
#include <QFile>
#include <QProcess>

class Updater : public QObject
{
    Q_OBJECT

public:
    explicit Updater(const QString &currentVersion, 
                     const QString &githubRepo, 
                     QWidget *parent = nullptr);
    
    void checkForUpdates(bool silent = false);
    QString getCurrentVersion() const { return m_currentVersion; }

signals:
    void updateAvailable(const QString &version, const QString &downloadUrl, const QString &releaseNotes);
    void noUpdateAvailable();
    void updateCheckFailed(const QString &error);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadComplete(const QString &filePath);

private slots:
    void onUpdateCheckFinished();
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onDownloadFinished();

private:
    QString m_currentVersion;
    QString m_githubRepo; // Format: "username/repository"
    QWidget *m_parent;
    QNetworkAccessManager *m_networkManager;
    QNetworkReply *m_currentReply;
    QFile *m_downloadFile;
    bool m_silentCheck;
    
    QString m_latestVersion;
    QString m_downloadUrl;
    QString m_releaseNotes;
    
    bool isNewerVersion(const QString &latest, const QString &current);
    void downloadUpdate(const QString &url);
    void installUpdate(const QString &installerPath);
};

#endif // UPDATER_H