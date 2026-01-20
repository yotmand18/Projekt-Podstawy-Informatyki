#include <string>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h>
#endif

std::string getExecutableDir()
{
#ifdef _WIN32
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    std::filesystem::path p(buffer);
    return p.parent_path().string();  // no trailing slash
#elif __linux__
    char buffer[1024];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer)-1);
    if (len != -1) {
        buffer[len] = '\0';
        std::filesystem::path p(buffer);
        return p.parent_path().string();  // no trailing slash
    }
    return ".";  // fallback
#else
    return ".";
#endif
}
