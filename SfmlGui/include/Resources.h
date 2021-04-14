#pragma once
#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include <string>

typedef std::string AnsiString;

class Resources
{
public:
    static AnsiString path(const AnsiString& resourceName);
};

#endif