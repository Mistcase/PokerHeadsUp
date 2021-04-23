#pragma once
#ifndef _NOTIFICATIONS_H_
#define _NOTIFICATIONS_H_

#include "Types.h"

#include <string>
#include <vector>


class Notifications
{
public:
	static AnsiString CreateNofiticationMessage(const AnsiString& action, const AnsiString& args = "");
	static AnsiString CreateNofiticationMessage(const AnsiString& action, const std::vector<AnsiString>& namedArgs);
	static AnsiString GetNotificationAction(const AnsiString& msg);
	static AnsiString GetNotificationArgs(const AnsiString& msg);
	static AnsiString GetNotificationNamedArg(const AnsiString& msg, const AnsiString& key);

private:
	static AnsiString GetValueByKey(const AnsiString& str, const AnsiString& key, char separator);
};

#endif