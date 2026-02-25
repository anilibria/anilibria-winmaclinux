#ifndef LOCALCACHECHECKER_H
#define LOCALCACHECHECKER_H

#include <string>
#include <cmath>
#include <cstdlib>
#include <cassert>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#if defined(_WIN32)
#define FLOWBRIDGER_DELEGATE_CALLTYPE __stdcall
#else
#define FLOWBRIDGER_DELEGATE_CALLTYPE
#endif

typedef void (*routine_types_call_back)(bool completed);
typedef void (*latest_releases_progress)(int32_t percent, int32_t processesReleases);
typedef void (*share_cache_call_back)(bool completed, const char* message);

typedef void (FLOWBRIDGER_DELEGATE_CALLTYPE *synchronize_routines)(bool franchises, bool schedule, bool types, const char* path, routine_types_call_back callBack);
typedef void (FLOWBRIDGER_DELEGATE_CALLTYPE *synchronize_changed_releases)(int32_t maximumPages, const char* path, latest_releases_progress callBack, routine_types_call_back finalCallBack);
typedef void (FLOWBRIDGER_DELEGATE_CALLTYPE *synchronize_latest_releases)(int32_t countReleases, int32_t countPages, const char* path, latest_releases_progress callback, routine_types_call_back finalCallBack);
typedef void (FLOWBRIDGER_DELEGATE_CALLTYPE *share_cache)(bool posters, bool torrents, bool releaseCache, const char* cachePath, const char* resultPath, share_cache_call_back callBack);
typedef void (FLOWBRIDGER_DELEGATE_CALLTYPE *load_cache)(const char* cacheFile, const char* cachePath, share_cache_call_back callBack);
typedef void (FLOWBRIDGER_DELEGATE_CALLTYPE *synchronize_posters)(const char* cachePath, bool forceAll, latest_releases_progress callback, routine_types_call_back finalCallBack);

class ImportFunctions {
private:
    void* loadLibrary(const std::wstring& path)
    {
#if defined(_WIN32)
        HMODULE h = ::LoadLibraryW(path.c_str());
        assert(h != nullptr);
        return (void*)h;
#else
        void *h = dlopen(wstringToString(path).c_str(), RTLD_LAZY | RTLD_LOCAL);
        assert(h != nullptr);
        return h;
#endif
    }

    void *getExport(void *h, const char *name)
    {
#if defined(_WIN32)
        void *f = ::GetProcAddress((HMODULE)h, name);
        assert(f != nullptr);
        return f;
#else
        void *f = dlsym(h, name);
        assert(f != nullptr);
        return f;
#endif
    }

    std::string wstringToString(const std::wstring& wstr) {
        size_t size_needed = std::wcstombs(nullptr, wstr.c_str(), 0);

        if (size_needed == (size_t)-1) return "";

        std::vector<char> buffer(size_needed + 1);

        std::wcstombs(buffer.data(), wstr.c_str(), size_needed + 1);

        return std::string(buffer.data());
    }

public:
    ImportFunctions(const std::wstring& pathToLibrary) {
        void *lib = loadLibrary(pathToLibrary);

        synchronizeRoutines = (synchronize_routines)getExport(lib, "synchronize_routines");
        synchronizeChangedReleases = (synchronize_changed_releases)getExport(lib, "synchronize_changed_releases");
        synchronizeLatestReleases = (synchronize_latest_releases)getExport(lib, "synchronize_latest_releases");
        shareCache = (share_cache)getExport(lib, "share_cache");
        loadCache = (load_cache)getExport(lib, "load_cache");
        synchronizePosters = (synchronize_posters)getExport(lib, "synchronize_posters");
    }

    synchronize_routines synchronizeRoutines = nullptr;
    synchronize_changed_releases synchronizeChangedReleases = nullptr;
    synchronize_latest_releases synchronizeLatestReleases = nullptr;
    share_cache shareCache = nullptr;
    load_cache loadCache = nullptr;
    synchronize_posters synchronizePosters = nullptr;
};

#endif // LOCALCACHECHECKER_H