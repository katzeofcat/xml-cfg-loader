#include "logger.h"
#include <algorithm>


Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::OpenFile()
{
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	std::string t = asctime(timeinfo);
	fd = 0;
	fd = t.find("\n", 0);
	t.replace(fd, 1,"");
	std::replace(t.begin(), t.end(), ':', '.');
	tm << t << "_log.txt";
	logs.open(tm.str().c_str());
}

void Logger::LogData(const char* data)
{
	logs << data;
}
