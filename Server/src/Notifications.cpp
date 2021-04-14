#include "Notifications.h"

AnsiString Notifications::CreateNofiticationMessage(const AnsiString& action, const AnsiString& args)
{
	return "Action:" + action + "/Args:" + args + "/";
}

AnsiString Notifications::CreateNofiticationMessage(const AnsiString& action, const std::vector<AnsiString>& namedArgs)
{
	AnsiString result = ("Action:" + action + "/Args:");
	for (auto& na : namedArgs)
		result += (na + "|");
	return result + "/";
}

AnsiString Notifications::GetNotificationAction(const AnsiString& msg)
{
	return GetValueByKey(msg, "Action", '/');
}

AnsiString Notifications::GetNotificationArgs(const AnsiString& msg)
{
	return GetValueByKey(msg, "Args", '/');
}

AnsiString Notifications::GetNotificationNamedArg(const AnsiString& msg, const AnsiString& key)
{
	return GetValueByKey(GetNotificationArgs(msg), key, '|');
}

AnsiString Notifications::GetValueByKey(const AnsiString& str, const AnsiString& key, char separator)
{
	AnsiString result;

	auto pos = str.find(key + ":");
	if (pos != AnsiString::npos)
	{
		pos += key.size() + 1;
		while (str[pos] != separator)
			result += str[pos++];
	}

	return result;
}