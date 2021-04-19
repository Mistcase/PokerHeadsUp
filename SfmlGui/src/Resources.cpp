#include "Resources.h"

AnsiString Resources::path(const AnsiString& resourceName)
{
#if defined (_WIN32)
	return "Data/" + resourceName;
#else
    return std::filesystem::current_path() + "/Data/" + resourceName;
#endif
}