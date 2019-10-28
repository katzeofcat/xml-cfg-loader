#ifndef SERIAL_CONNECTION_H
#define SERIAL_CONNECTION_H
#include <iostream>
#include "rs232.h"

static const char* ESC = "\x1B";
static const char* ENTER = "\r";

enum class CMD_TYPE {
	NORMAL_CMD,
	DEBUG_CMD,
};

class SerialConnection
{
	int _port_idx;
	bool _connected;

public:
	SerialConnection(const std::string& port, int bourdrate, const std::string& mode, int flowcontrol);
	~SerialConnection();
	std::tuple<std::string, long long>  SendCmd(const std::string& cmd, std::string mode = "", CMD_TYPE ctype = CMD_TYPE::NORMAL_CMD, std::string end = ">");

	bool IsConnected() const {
		return _connected;
	}

	int ComPort() const {
		return _port_idx;
	}
};





#endif // !SERIAL_CONNECTION_H

