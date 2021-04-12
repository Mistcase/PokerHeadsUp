#include "Notifications.h"

EventMessageString Notifications::CreateNofiticationMessage(const string& action, const string& args)
{
	return "Action:" + action + "/Args:" + args + "/";
}

EventMessageString Notifications::GetNotificationAction(const EventMessageString& msg)
{
	return GetValueByKey(msg, "Action");
}

EventMessageString Notifications::GetNotificationArgs(const EventMessageString& msg)
{
	return GetValueByKey(msg, "Args");
}

EventMessageString Notifications::GetValueByKey(const EventMessageString& str, const EventMessageString& key)
{
	EventMessageString result;

	auto pos = str.find(key + ":");
	if (pos != string::npos)
	{
		pos += key.size() + 1;
		while (str[pos] != '/')
			result += str[pos++];
	}

	return result;
}