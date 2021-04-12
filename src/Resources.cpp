#include "Resources.h"

#include <string>

#if defined(__APPLE__)
#include <mach-o/dyld.h>
#include <limits.h>
#endif

string Resources::path(const string& resourceName)
{
    string folderPath("");
#if defined(_WIN32)

#elif defined(__APPLE__)
    constexpr int MAX_PATH = 256;
    string binaryPath;
    binaryPath.resize(MAX_PATH, '\0');

    uint32_t bufSize = MAX_PATH;
    _NSGetExecutablePath(const_cast<char*>(binaryPath.c_str()), &bufSize);
    for (int i = 0; i < binaryPath.size() - 1; i++)
    {
        if (binaryPath[i] == '.' && binaryPath[i + 1] == '/')
            break;
        folderPath += binaryPath[i];
    }
#endif


    return folderPath + "Data/" + resourceName;
}