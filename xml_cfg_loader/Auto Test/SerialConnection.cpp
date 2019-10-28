#include "SerialConnection.h"
#include <stdexcept>
#include <chrono>
#include <vector>
#include <tuple> 

const int BUFFER_SIZE = 4096 * 40;

SerialConnection::SerialConnection(const std::string& port, int bourdrate, const std::string& mode, int flowcontrol)
	: _connected(true)
{
	_port_idx = RS232_findComPort(port.c_str());
	if (_port_idx == -1) {
		printf("CONNECTION NOT FOUND RECONNECTING ON PORT %s\n", port.c_str());
		return;
	}
	
	int countdown = 9;
	while (RS232_OpenComport(_port_idx, bourdrate, mode.c_str(), flowcontrol) != 0)
	{
		printf("CONNECTION NOT FOUND RECONNECTING ON PORT %s\n", port.c_str());
		Sleep(1000);
		if (!(countdown--)) {
			printf("GAVE UP CONNECTING TO PORT %s\n", port.c_str());
			_connected = false;
			break;
		}
	}
}

SerialConnection::~SerialConnection()
{
	if (_connected) {
		RS232_CloseComport(_port_idx);
	}
}

std::tuple<std::string, long long> SerialConnection::SendCmd(const std::string& cmd, std::string mode, CMD_TYPE ctype, std::string eol) {
	auto start = std::chrono::high_resolution_clock::now();
	std::vector<unsigned char> buf(BUFFER_SIZE);
	RS232_cputs(_port_idx, cmd.c_str());
	if (ctype == CMD_TYPE::DEBUG_CMD) {
		RS232_cputs(_port_idx, ESC);
		Sleep(1000);
		RS232_cputs(_port_idx, mode.c_str());
		if (eol.back() != '\n') {
			eol += '\n';
		}
	}
	std::string back;
	while (true) {
		Sleep(20);
		auto read = RS232_PollComport(_port_idx, buf.data(), BUFFER_SIZE);
		if (read) {
			back += std::string((const char*)buf.data(), read);
			if (ctype == CMD_TYPE::DEBUG_CMD) {
				buf[read] = 0;
				std::cout << buf.data() << "\n";
			}
			if (back.substr(back.size() - eol.size(), back.size()) == eol) {
				break;
			}
		}
	}
	if (ctype == CMD_TYPE::DEBUG_CMD) {
		RS232_cputs(_port_idx, ENTER);
		RS232_PollComport(_port_idx, buf.data(), BUFFER_SIZE);
		Sleep(1000);
	}
	auto end = std::chrono::high_resolution_clock::now();
	auto took = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
	return std::make_pair(back, took);
}
