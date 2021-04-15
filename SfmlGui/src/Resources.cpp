#include "Resources.h"

AnsiString Resources::path(const AnsiString& resourceName)
{
    return std::filesystem::current_path() + "/Data/" + resourceName;
}