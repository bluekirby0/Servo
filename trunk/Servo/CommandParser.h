#include <vector>
#include <stdint.h>

class CommandParser
{
private:
	std::vector<char*> thisList;
	uint8_t argC;
	uint8_t argIdx;
public:
	CommandParser()
	{}
	~CommandParser()
	{}
	
	void InputCommand(const char* szBuffer);
	bool GetFirstCommand(std::string*);
	bool GetCommandAt(unsigned int,std::string*);
	bool GetNextCommand(std::string*);
	bool GetRemainingCommands(std::string*);
	unsigned int GetArgCount();
};

void CommandParser::InputCommand(const char* szBuffer)
{
	thisList.clear();
	argIdx = 0;
	argC = 0;
	uint8_t span = 0;
	char* szTemp = const_cast<char*>(szBuffer);
	char* tokenBuf;
	for(char* substr = strtok_s(szTemp, " ",&tokenBuf);substr; substr = strtok_s(NULL, " ",&tokenBuf))
	{
		thisList.push_back(substr);
		argC += 1;
	}
}

bool CommandParser::GetFirstCommand(std::string* sReturn)
{
	argIdx = 0;
	return GetNextCommand(&*sReturn);
}

bool CommandParser::GetCommandAt(unsigned int idx, std::string* sReturn)
{
	argIdx = idx;
	return GetNextCommand(&*sReturn);
}

bool CommandParser::GetNextCommand(std::string* sReturn)
{
	if(argIdx < argC)
	{
		*sReturn = thisList[argIdx];
		argIdx += 1;
		return true;
	}
	return false;
}

bool CommandParser::GetRemainingCommands(std::string* sReturn)
{
	bool retVal = false;
	*sReturn = "";
	while(argIdx < argC)
	{
		sReturn->append(thisList[argIdx]);
		sReturn->append(1,'\x20');
		argIdx += 1;
		retVal = true;
	}
	return retVal;
}

unsigned int CommandParser::GetArgCount()
{
	return argC;
}