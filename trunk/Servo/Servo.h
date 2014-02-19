#include <deque>
#include <atomic>
#include <thread>
#include <mutex>
#include "zmq.hpp"
#include "..\..\..\Ashita\trunk\build\Release\Extensions\SDK\Depends\Common\Extension.h"
#include "CommandParser.h"
#include "Chat.hpp"


#pragma comment(lib,"libzmq.lib")

//do not delete this. Ashita will clean it up.
ExtensionInterfaceData* ModuleData;

namespace ServoMode{
	enum Mode{Standby,Server,Client};
};

class Servo : public ExtensionBase
{
private:
	IAshitaCore* m_AshitaCore;
	Chat::Log log;
	std::string me;

	zmq::context_t zContext;

	std::mutex lock;
	std::atomic_bool connected;
	std::thread conn;
	std::deque<std::string> buffer;

	std::atomic<ServoMode::Mode> state;
	std::string servAddr;

	std::mutex lservAddr;

	std::string StringSub(std::string,std::string,std::string);

	CommandParser* cmdParse;
	void formatCommand(std::string* command);
	void SendCommand(std::string command);
	void RecvCommand(std::string* command);
	void SetState(ServoMode::Mode mode);

	//Threads
	void Serv();
	void Client();
public:
	Servo()
	{}
	~Servo()
	{}
	
	int __stdcall Load(IAshitaCore* mAshitaCore, DWORD ExtensionID);

	void __stdcall Unload();

	ExtensionInterfaceData __stdcall GetExtensionData();

	bool __stdcall HandleCommand(const char* szCommand, int iType);

	bool __stdcall DxSetup(IDirect3DDevice8* mDevice);

	void __stdcall DxRender();

};

//Exports
__declspec( dllexport ) void __stdcall CreateExtensionData(ExtensionInterfaceData* Data);

__declspec( dllexport ) IExtension* __stdcall CreateExtension(char* szText);
