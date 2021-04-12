#pragma once
#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include <string>

using std::string;

class Resources
{
public:
    static string path(const string& resourceName);
};

#endif