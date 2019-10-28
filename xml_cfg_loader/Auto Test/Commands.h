#pragma once

/*
    a command is just a string specifying 
    an operation to perform and a wait attribute
    that specifies how long to wait after the operation 
    has been performed
*/
struct Command {
    std::string operation;
    int wait = 0;
	std::string eol = ">";
	bool is_mode_debug = false;
	std::string mode;
};

/*
    a sequence is a list of commands that need to be run
    it can be run N times as specified by the loop attribute
    and has a name
*/
struct Sequence {
    std::string name;
    std::vector<Command> Commands;
    int loop = 1;
};

/*
    turns this xml

    <command>
        <sequence loop="2">
            <operation cmd="ETM TEST ' FY110660830'"/>
            <operation cmd="ETM INFO" wait="1000"/>
            <operation cmd="ETM TEST 0"/>
            <operation cmd="ETM INFO" wait="1000"/>
        </sequence>
        <sequence loop="3">
            operation cmd="ETM LIST" wait="3000"/>
            <operation cmd="SYS LIST" wait="1000"/>
        </sequence>
    </command>

    into a Commands object

    VisitEnter is called by the tinyxml parser for each element in the xml it
    encounters when parsing the input via Accept

*/
class Commands : public tinyxml2::XMLVisitor {

    Sequence current_sequence = Sequence();

    /*
        called when an element has been found
    */
    virtual bool VisitEnter(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute* firstAttribute) {
        using namespace tinyxml2;
        using namespace std;
        string tag = element.Value();
        if (tag.data()) {
            if (tag == "operation") {
                Command cmd = Command();
                // pull out all the attributes for the operation tag
                // and build up the Command object
                for (auto attr = firstAttribute; attr != 0; attr = attr->Next()) {
                    if (strcmp(attr->Name(), "cmd") == 0) {
                        cmd.operation = attr->Value();
                    }
                    else if (strcmp(attr->Name(), "wait") == 0) {
                        cmd.wait = stoi(attr->Value());
                    }
					else if (strcmp(attr->Name(), "eol") == 0) {
						cmd.eol = std::string(attr->Value());
					}
					else if (strcmp(attr->Name(), "mode") == 0) {
						cmd.mode = attr->Value();
						cmd.is_mode_debug = true;
					}
				}
                current_sequence.Commands.emplace_back(cmd);
            }
            else if (tag == "sequence") {
                // pull out all the attributes for the sequence tag
                for (auto attr = firstAttribute; attr != 0; attr = attr->Next()) {
                    if (strcmp(attr->Name(), "loop") == 0) {
                        current_sequence.loop = stoi(attr->Value());
                    }
                    else if (strcmp(attr->Name(), "name") == 0) {
                        current_sequence.name = attr->Value();
                    }
                }
            }
        }
        return true;
    }

    /*
        called when an end element has been parsed
    */
    virtual bool VisitExit(const tinyxml2::XMLElement& element) {
        std::string tag = element.Value();
        if (tag.data()) {
            if (tag == "sequence") { 
                Sequences.emplace_back(current_sequence); // pop the current sequence into the sequence list
                current_sequence = Sequence(); // and start another sequence
            }
        }
        return true;
    }

public:
    static Commands Make(const char* fileName) {
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
        Commands cmds;
        doc.Accept(&cmds);
        return cmds;
    }

    std::vector<Sequence> Sequences;
};


