#ifndef LOGGER_H
#define LOGGER_H
#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

class Logger
{
	int fd;
	std::string change;
	std::ofstream logs;
	std::stringstream tm;

public:
	Logger();
	~Logger();
	void OpenFile();
	void LogData(const char*);


};

#endif // !LOGGER_H

