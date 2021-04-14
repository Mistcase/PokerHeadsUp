#include "Notifications.h"

EventMessageString Notifications::CreateNofiticationMessage(const AnsiString& action, const AnsiString& args)
{
	return "Action:" + action + "/Args:" + args + "/";
}

EventMessageString Notifications::CreateNofiticationMessage(const AnsiString& action, const vector<AnsiString>& namedArgs)
{
	AnsiString result = ("Action:" + action + "/Args:");
	for (auto& na : namedArgs)
		result += (na + "|");
	return result + "/";
}

EventMessageString Notifications::GetNotificationAction(const EventMessageString& msg)
{
	return GetValueByKey(msg, "Action", '/');
}

EventMessageString Notifications::GetNotificationArgs(const EventMessageString& msg)
{
	return GetValueByKey(msg, "Args", '/');
}

EventMessageString Notifications::GetNotificationNamedArg(const EventMessageString& msg, const AnsiString& key)
{
	return GetValueByKey(GetNotificationArgs(msg), key, '|');
}

EventMessageString Notifications::GetValueByKey(const EventMessageString& str, const EventMessageString& key, char separator)
{
	EventMessageString result;

	auto pos = str.find(key + ":");
	if (pos != AnsiString::npos)
	{
		pos += key.size() + 1;
		while (str[pos] != separator)
			result += str[pos++];
	}

	return result;
}