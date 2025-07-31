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

class OsExtras : public QObject
{
    Q_OBJECT
private:
    typedef int (CORECLR_DELEGATE_CALLTYPE* torrentstreaminitializemethod)(int port, wchar_t* downloadPath, wchar_t* listenAddress, bool showui, connectedCallback callback);
    torrentstreaminitializemethod torrentStreamInitialize = nullptr;

    typedef int (CORECLR_DELEGATE_CALLTYPE* stoptorrentstreammethod)();
    stoptorrentstreammethod torrentStreamStop = nullptr;

public:
    explicit OsExtras(QObject *parent = nullptr);

    inline static OsExtras* instance = nullptr;

#ifdef Q_OS_LINUX
    QStringList m_dbusServices { QStringList() };
    QStringList m_dbusPaths { QStringList() };
    uint m_dbusReply { 0 };
#endif

    Q_INVOKABLE void startPreventSleepMode();
    Q_INVOKABLE void stopPreventSleepMode();
    Q_INVOKABLE bool initializeTorrentStream(int port, const QString& pathToLibrary, const QString& downloadPath, const QString& listenAddress, bool showui);
    Q_INVOKABLE void deinitializeTorrentStream() noexcept;

private:
    void* loadLibrary(const QString& path);
    void* getExport(void *h, const char *name);

public slots:
    void callbackConnected();
    void callbackRefresh();

signals:
    void torrentStreamConnected();
};

static void CORECLR_DELEGATE_CALLTYPE callbackConnectedExternal() {
    if (OsExtras::instance == nullptr) return;

    OsExtras::instance->callbackConnected();
}

static void CORECLR_DELEGATE_CALLTYPE callbackRefreshExternal() {
    if (OsExtras::instance == nullptr) return;

    OsExtras::instance->callbackRefresh();
}

#endif // OSEXTRAS_H
