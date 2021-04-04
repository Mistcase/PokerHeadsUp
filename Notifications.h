#pragma once
#ifndef _NOTIFICATIONS_H_
#define _NOTIFICATIONS_H_

#include <string>

#include "Observer.h"

using std::string;

class Notifications
{
public:
	static EventMessageString CreateNofiticationMessage(const string& action, const string& args);
	static EventMessageString GetNotificationAction(const EventMessageString& msg);
	static EventMessageString GetNotificationArgs(const EventMessageString& msg);

private:
	static EventMessageString GetValueByKey(const EventMessageString& str, const EventMessageString& key);
};

#endif