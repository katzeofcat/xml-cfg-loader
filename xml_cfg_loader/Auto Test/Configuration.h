#pragma once


class Configuration : public tinyxml2::XMLVisitor {

    /*
        turns this xml

        <config>
            <port>COM3</port>
            <boudrate>115200</boudrate>
            <databits>8</databits>
            <parity>N</parity>
            <stopbit>1</stopbit>
            <flowcontrol>0</flowcontrol>
        </config>

        into a configuration object

        VisitEnter is called by the tinyxml parser for each element in the xml it 
        encounters when parsing the input via Accept
    */
    virtual bool VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* firstAttribute) {
        using namespace tinyxml2;
        using namespace std;
        string tag = element.Value();
        if (tag.data()) {

            if (tag == "port") {
                this->Port = element.GetText();
            }
            else if (tag == "parity") {
                this->Parity = element.GetText();
            }
            else if (tag == "baudrate") {
                this->BaudRate = stoi(element.GetText());
            }
            else if (tag == "databits") {
                this->Databits = stoi(element.GetText());
            }
            else if (tag == "stopbit") {
                this->StopBit = stoi(element.GetText());
            }
            else if (tag == "flowcontrol") {
                this->FlowControl = stoi(element.GetText());
            }
        }
        return true;
    }

public:

    static Configuration Make(const char* fileName) {
        using namespace tinyxml2;
        using namespace std;
        XMLDocument doc;
        auto hr = doc.LoadFile(fileName);
        if (hr != XML_SUCCESS) {
            string err = "couldn't load:" + string(fileName);
            throw runtime_error(err);
        }
        // get tinyxml to "visit" each node in the input,
        // this class provides an implementation of tinyxml2::XMLVisitor
        // which the Accept call will use when it encounters elements in the xml
        Configuration cfg;
        doc.Accept(&cfg);
        return cfg;
    }

    std::string ToString() {
        std::stringstream ss;
        ss << "Port: " << Port << ", ";
        ss << "Parity: " << Parity << ", ";
        ss << "BaudRate: " << BaudRate << ", ";
        ss << "Databits: " << Databits << ", ";
        ss << "StopBit: " << StopBit << ", ";
        ss << "FlowControl: " << FlowControl << ", ";
		ss << "Mode: " << GetMode();
        return ss.str();
    }

    // attributes from the config file defaulted to sane values
    std::string Port;
    std::string Parity = "N";
    int BaudRate = 115200;
    int Databits = 8;
    int StopBit = 1;
    int FlowControl = 0;

	const std::string GetMode() const {
		std::stringstream ss;
		ss << Databits << Parity << StopBit; // << "0";
		return ss.str();
	}
};
