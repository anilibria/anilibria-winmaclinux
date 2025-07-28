#ifndef OSEXTRAS_H
#define OSEXTRAS_H

#include <QObject>
#include <QStringList>

#if defined(_WIN32)
#define CORECLR_DELEGATE_CALLTYPE __stdcall
#else
#define CORECLR_DELEGATE_CALLTYPE
#endif

class OsExtras : public QObject
{
    Q_OBJECT
private:
    typedef int (CORECLR_DELEGATE_CALLTYPE* torrentstreaminitializemethod)(int port, wchar_t* downloadPath, wchar_t* listenAddress, bool showui);
    torrentstreaminitializemethod torrentStreamInitialize = nullptr;

    typedef int (CORECLR_DELEGATE_CALLTYPE* stoptorrentstreammethod)();
    stoptorrentstreammethod torrentStreamStop = nullptr;

public:
    explicit OsExtras(QObject *parent = nullptr);

#ifdef Q_OS_LINUX
    QStringList m_dbusServices { QStringList() };
    QStringList m_dbusPaths { QStringList() };
    uint m_dbusReply { 0 };
#endif

    Q_INVOKABLE void startPreventSleepMode();
    Q_INVOKABLE void stopPreventSleepMode();
    Q_INVOKABLE bool initializeTorrentStream(int port, const QString& pathToLibrary, const QString& downloadPath, const QString& listenAddress, bool showui);

private:
    void* loadLibrary(const QString& path);
    void* getExport(void *h, const char *name);

signals:

};

#endif // OSEXTRAS_H
