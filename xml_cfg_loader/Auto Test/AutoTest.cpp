#include <iostream>

#include "xml.h"
#include <stdexcept>
#include <vector>
#include <utility>
#include <string>
#include <sstream>
#include <chrono>
#include <thread>

#include "Configuration.h"
#include "Commands.h"
#include "SerialConnection.h"
#include "logger.h"

static const std::string EOL = std::string("\n");

int main() {
    using namespace std;
	Configuration cfg = Configuration::Make("config.xml");
	cout << cfg.ToString() << "\n\n";

	SerialConnection dt(cfg.Port, cfg.BaudRate, cfg.GetMode(), cfg.FlowControl);
	if (!dt.IsConnected()) {
		return 1;
	}
	Logger log;
	log.OpenFile();
	auto data = dt.SendCmd(ENTER);
    // loop through the command sequences, running each one the specified times
    Commands cmds = Commands::Make("command.xml");
    for (auto seq : cmds.Sequences) {
        int iteration = 1;
        for (int idx = 0; idx < seq.loop; ++idx) {
            cout << "running sequence '" << seq.name << "' iteration " << iteration++ << " of " << seq.loop << '\n';
            for (auto cmd : seq.Commands) {
                // get the operation to run
                cout << cmd.operation << '\n';
				auto data = dt.SendCmd(cmd.operation + EOL, cmd.mode,
									   cmd.is_mode_debug ? CMD_TYPE::DEBUG_CMD : CMD_TYPE::NORMAL_CMD, 
									   cmd.eol);
				
				if (cmd.is_mode_debug) {
					std::stringstream logtime;
					log.LogData((std::get<0>(data)).c_str());
					logtime << "\ntook: ------- " << std::get<1>(data) << " secs -------\n";
 					cout << logtime.str();
					log.LogData(logtime.str().c_str());
					Sleep(5000);
				}

				else {
					cout << "reply:\n" << std::get<0>(data) << "\n";
					log.LogData((std::get<0>(data)).c_str());
					// wait specified time after 
					Sleep(1000);
				}
            }
            cout << '\n';
        }
    }
	return 0;
}
