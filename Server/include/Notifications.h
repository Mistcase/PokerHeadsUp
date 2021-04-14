#pragma once
#ifndef _NOTIFICATIONS_H_
#define _NOTIFICATIONS_H_

#include "Types.h"
#include "Observer.h"



class Notifications
{
public:
	static EventMessageString CreateNofiticationMessage(const AnsiString& action, const AnsiString& args);
	static EventMessageString CreateNofiticationMessage(const AnsiString& action, const vector<AnsiString>& namedArgs);
	static EventMessageString GetNotificationAction(const EventMessageString& msg);
	static EventMessageString GetNotificationArgs(const EventMessageString& msg);
	static EventMessageString GetNotificationNamedArg(const EventMessageString& msg, const AnsiString& key);

private:
	static EventMessageString GetValueByKey(const EventMessageString& str, const EventMessageString& key, char separator);
};

#endif