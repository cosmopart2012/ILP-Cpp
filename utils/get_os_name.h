#pragma once
#ifndef GET_OS_NAME_H
#define GET_OS_NAME_H

#include <string>

using namespace std;

string getOsName()
{
#ifdef _WIN32
    return "Windows";
#elif __linux__
    return "Linux";
#elif __APPLE__ && __MACH__
    return "MacOS";
#else
    return "Unknown";
#endif
}

string get_extension()
{
    string os_name = getOsName();
    if (os_name == "Windows")
    {
        return ".dll";
    }
    else if (os_name == "Linux")
    {
        return ".so";
    }
    else if (os_name == "MacOS")
    {
        return ".dylib";
    }
    else
    {
        return "";
    }
}
#endif // GET_OS_NAME_H