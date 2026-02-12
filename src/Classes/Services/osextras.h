#ifndef OSEXTRAS_H
#define OSEXTRAS_H

#include <QObject>
#include <QStringList>
#include <QDebug>

#if defined(_WIN32)
#define CORECLR_DELEGATE_CALLTYPE __stdcall
#else
#define CORECLR_DELEGATE_CALLTYPE
#endif

typedef void (CORECLR_DELEGATE_CALLTYPE *connectedCallback)();
typedef void (CORECLR_DELEGATE_CALLTYPE *refreshedCallback)(bool isResult);
typedef void (CORECLR_DELEGATE_CALLTYPE *startTorrentDownloadCallback)(int id, wchar_t* downloadPath, bool isAdded);

class OsExtras : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool localCacheCheckerConnected READ localCacheCheckerConnected NOTIFY localCacheCheckerConnectedChanged FINAL)

private:
    typedef int (CORECLR_DELEGATE_CALLTYPE* torrentstreaminitializemethod)(int port, wchar_t* downloadPath, wchar_t* listenAddress, bool showui, connectedCallback callback);
    torrentstreaminitializemethod torrentStreamInitialize = nullptr;

    typedef void (CORECLR_DELEGATE_CALLTYPE* stoptorrentstreammethod)();
    stoptorrentstreammethod torrentStreamStop = nullptr;

    typedef void (CORECLR_DELEGATE_CALLTYPE* torrentstreamclearallmethod)(refreshedCallback callback);
    torrentstreamclearallmethod torrentStreamClearAll = nullptr;

    typedef void (CORECLR_DELEGATE_CALLTYPE* torrentstreamclearonlytorrentmethod)(wchar_t* downloadPathPointer, refreshedCallback callback);
    torrentstreamclearonlytorrentmethod torrentStreamClearOnlyTorrent = nullptr;

    typedef void (CORECLR_DELEGATE_CALLTYPE* torrentstreamcleartorrentanddatamethod)(wchar_t* downloadPathPointer, refreshedCallback callback);
    torrentstreamcleartorrentanddatamethod torrentStreamClearTorrentAndData = nullptr;

    typedef void (CORECLR_DELEGATE_CALLTYPE* savestatemethod)();
    savestatemethod torrentStreamSaveState = nullptr;

    typedef wchar_t* (CORECLR_DELEGATE_CALLTYPE* torrentstreamgetallmethod)();
    torrentstreamgetallmethod torrentstreamGetAll = nullptr;

    typedef void (CORECLR_DELEGATE_CALLTYPE* torrentstreamstartdownload)(int id, wchar_t* path, startTorrentDownloadCallback callback);
    torrentstreamstartdownload torrentstreamStartDownload = nullptr;

    void* m_LocalCacheChecker { nullptr };

    bool m_startedSynchronization { false };
    int32_t m_synchronizationReleases { 0 };

    bool m_localCacheCheckerConnected { false };

    bool localCacheCheckerConnected() const noexcept { return m_localCacheCheckerConnected; }

public:
    explicit OsExtras(QObject *parent = nullptr);

    static OsExtras* instance;

#ifdef Q_OS_LINUX
    QStringList m_dbusServices { QStringList() };
    QStringList m_dbusPaths { QStringList() };
    uint m_dbusReply { 0 };
#endif

    Q_INVOKABLE void startPreventSleepMode();
    Q_INVOKABLE void stopPreventSleepMode();
    Q_INVOKABLE bool initializeTorrentStream(int port, const QString& pathToLibrary, const QString& downloadPath, const QString& listenAddress, bool showui);
    Q_INVOKABLE bool initializeLocalCacheChecker(const QString& pathToLibrary);
    Q_INVOKABLE void synchronizeRoutine();
    Q_INVOKABLE void synchronizeChanges();
    Q_INVOKABLE void synchronizeLatest();
    Q_INVOKABLE void synchronizeAllReleases();
    Q_INVOKABLE void synchronizePosters(bool forceAll);
    Q_INVOKABLE void deinitializeTorrentStream() noexcept;
    Q_INVOKABLE void tsClearAll() noexcept;
    Q_INVOKABLE void tsClearOnlyTorrent(const QString& downloadPath) noexcept;
    Q_INVOKABLE void tsClearTorrentAndData(const QString& downloadPath) noexcept;
    Q_INVOKABLE void tsSaveState() noexcept;
    Q_INVOKABLE QString tsGetAll() noexcept;
    Q_INVOKABLE void tsStartFullDownload(int id, QString downloadPath) noexcept;

private:
    void* loadLibrary(const QString& path);
    void* getExport(void *h, const char *name);

public slots:
    void callbackConnected();
    void callbackRefresh(bool isResult);
    void callbackStartDownload(int id, const QString& path, bool isAdded);
    void synchronizationFinished(bool completed);
    void rountineFinished(bool completed);
    void synchronizationLatestChanges(int32_t percent, int32_t processesReleases);
    void synchronizationPosterChanges(int32_t percent, int32_t processesReleases);
    void postersFinished(bool completed);

signals:
    void torrentStreamConnected();
    void torrentStreamRefreshed(bool isResult);
    void torrentStreamStartDownload(int id, const QString& path, bool isAdded);
    void synchronizationRoutine(bool completed);
    void needReloadReleases();
    void localCacheCheckerConnectedChanged();
    void routineSynchronized(const QString& message);
    void releasesSynchronized(const QString& message);
};

static void CORECLR_DELEGATE_CALLTYPE callbackConnectedExternal() {
    if (OsExtras::instance == nullptr) return;

    OsExtras::instance->callbackConnected();
}

static void CORECLR_DELEGATE_CALLTYPE callbackRefreshExternal(bool isResult) {
    if (OsExtras::instance == nullptr) return;

    OsExtras::instance->callbackRefresh(isResult);
}

static void CORECLR_DELEGATE_CALLTYPE callbackStartDownloadExternal(int id, wchar_t* path, bool isAdded) {
    if (OsExtras::instance == nullptr) return;

    QString downloadPath = QString::fromWCharArray(path);
    OsExtras::instance->callbackStartDownload(id, downloadPath, isAdded);
}


#endif // OSEXTRAS_H
