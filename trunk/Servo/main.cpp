#include "Servo.h"
#include <sstream>

std::string Servo::StringSub(std::string instring,std::string subin,std::string subout)
{
	if(instring.find(subin) != std::string::npos)
		return(instring.replace(instring.find(subin), subin.length(), subout));
	else
		return instring;
}

void Servo::formatCommand(std::string* command)
{
	std::stringstream id;
	id << m_AshitaCore->GetDataModule()->GetTarget()->GetTargetWindow()->ID;

	*command = StringSub(*command, "[me]", m_AshitaCore->GetDataModule()->GetParty()->Member[0].Name);
	*command = StringSub(*command, "[p1]", m_AshitaCore->GetDataModule()->GetParty()->Member[1].Name);
	*command = StringSub(*command, "[p2]", m_AshitaCore->GetDataModule()->GetParty()->Member[2].Name);
	*command = StringSub(*command, "[p3]", m_AshitaCore->GetDataModule()->GetParty()->Member[3].Name);
	*command = StringSub(*command, "[p4]", m_AshitaCore->GetDataModule()->GetParty()->Member[4].Name);
	*command = StringSub(*command, "[p5]", m_AshitaCore->GetDataModule()->GetParty()->Member[5].Name);
	*command = StringSub(*command, "[t]", id.str());
}

void Servo::SendCommand(std::string command)
{ 
	while (!lock.try_lock())
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	buffer.push_back(command);
	lock.unlock();
}

void Servo::RecvCommand(std::string* command)
{
	command->erase();
	if (lock.try_lock())
	{
		if (!buffer.empty())
		{
			command->swap(buffer.front());
			buffer.pop_front();
		}
		lock.unlock();
	}
}

void Servo::SetState(ServoMode::Mode mode)
{
	ServoMode::Mode prevState = state;
	state = ServoMode::Mode::Standby;
	connected = false;
	if (prevState != ServoMode::Mode::Standby)
		conn.join();
	while (!buffer.empty())
		buffer.pop_front();
	state = mode;
}

int __stdcall Servo::Load(IAshitaCore* mAshitaCore, DWORD ExtensionID)
{
		this->m_AshitaCore = mAshitaCore;
		this->m_ExtensionId = ExtensionID;

		cmdParse = new CommandParser;
		state = ServoMode::Mode::Standby;
		lservAddr.lock();
		servAddr = "";
		lservAddr.unlock();
		me = "";

		zContext = zmq::context_t(1);
		conn = std::thread();

		log.SetCore(m_AshitaCore);
		
		log << Chat::Mode::Echo;

		return 1;
}

void Servo::Serv()
{
	zmq::socket_t publisher(zContext, ZMQ_PUB);
	std::string hostmask = "tcp://";
	while (!lservAddr.try_lock())
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	if (!servAddr.empty())
		hostmask.append(servAddr);
	else
		hostmask.append("127.0.0.1");
	lservAddr.unlock();
	hostmask.append(":56556");
	publisher.bind(hostmask.c_str());

	connected = true;

	while (connected) {
		while(!lock.try_lock())
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if (!buffer.empty())
		{
			auto it = buffer.begin();
			zmq::message_t message(it->length()+1);
			strcpy_s((char*)message.data(), (size_t)(it->length() + 1), it->c_str());
			buffer.pop_front();
			lock.unlock();
			publisher.send(message,ZMQ_DONTWAIT);
		}
		else
			lock.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	publisher.close();
}

void Servo::Client()
{
	std::string host = "tcp://";
	while (!lservAddr.try_lock())
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	if (!servAddr.empty())
		host.append(servAddr);
	else
		host.append("localhost");
	lservAddr.unlock();
	host.append(":56556");
	zmq::socket_t subscriber(zContext, ZMQ_SUB);
	subscriber.connect(host.c_str());
	subscriber.setsockopt(ZMQ_SUBSCRIBE, me.c_str(), me.size());
	subscriber.setsockopt(ZMQ_SUBSCRIBE, "::", 2);
	connected = true;
	
	while (connected)
	{
		zmq::message_t update;

		while (subscriber.recv(&update, ZMQ_DONTWAIT) == false)
		{
			if (connected)
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
			else
				break;
		}
		
		std::string s(static_cast<char*>(update.data()));
		if (!s.empty())
		{
			while (!lock.try_lock())
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			buffer.push_back(s);
			lock.unlock();
		}
			
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	subscriber.close();
}

void __stdcall Servo::Unload()
{
	SetState(ServoMode::Mode::Standby);
	zContext.close();
	delete cmdParse;
}

ExtensionInterfaceData __stdcall Servo::GetExtensionData()
{
	return *ModuleData;
}

bool __stdcall Servo::HandleCommand(const char* szCommand, int iType)
{
	if (iType == 0)
	{
		return false;
	}
	
	char szClean[1024];

	m_AshitaCore->GetDataModule()->ParseAutoTrans(szCommand,szClean,1024,false);
	std::string arg;

	cmdParse->InputCommand(szClean);
	cmdParse->GetFirstCommand(&arg);
	
	if(arg == "/servo")
	{
		cmdParse->GetNextCommand(&arg);
		if(arg == "on")
		{
			log << Chat::Format::RoyalBlue << "Servo started in server mode, with a hostmask of: ";
			while (!lservAddr.try_lock())
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			if (cmdParse->GetNextCommand(&arg))
			{
				servAddr = arg;
				log << servAddr;
			}
			else
			{
				servAddr = "";
				log << "localhost";
			}
			lservAddr.unlock();
			
			SetState(ServoMode::Mode::Server);
			conn.swap(std::thread(&Servo::Serv, this));
			log << Chat::Control::flush;
		}
		else if(arg == "sync")
		{
			me = m_AshitaCore->GetDataModule()->GetParty()->Member[0].Name;
			log << Chat::Format::RoyalBlue << "Servo started in client mode, connected to: ";
			while (!lservAddr.try_lock())
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			if (cmdParse->GetNextCommand(&arg))
			{
				servAddr = arg;
				log << servAddr;
			}
			else
			{
				servAddr = "";
				log << "localhost";
			}
			lservAddr.unlock();

			SetState(ServoMode::Mode::Client);
			conn.swap(std::thread(&Servo::Client, this));
			log << Chat::Control::flush;
		}
		else if (arg == "off")
		{
			SetState(ServoMode::Mode::Standby);
			log << Chat::Format::RoyalBlue << "Servo set to standy!" << Chat::Control::flush;
		}
		else if (arg == "command" || arg == "send")
		{
			if (cmdParse->GetRemainingCommands(&arg))
			{
				arg = ":: " + arg;
				formatCommand(&arg);
				SendCommand(arg);
			}

		}
		else if (arg == "sendto")
		{
			if (cmdParse->GetRemainingCommands(&arg))
			{
				formatCommand(&arg);
				SendCommand(arg);
			}

		}
		return true;
	}
	return false;
}

bool __stdcall Servo::DxSetup(IDirect3DDevice8* mDevice)
{
	return true;
}

void __stdcall Servo::DxRender()
{
	if (state.load() == ServoMode::Mode::Client)
	{
		std::string incommand;
		RecvCommand(&incommand);
		if (!incommand.empty())
		{
			incommand = incommand.substr(incommand.find_first_of(" "));
			m_AshitaCore->GetConsoleModule()->HandleCommand(false, incommand.c_str(), IData::CommandInputType::Menu);
		}
	}
}

__declspec( dllexport ) void __stdcall CreateExtensionData(ExtensionInterfaceData* Data)
{
	ModuleData = Data;
	ModuleData->ExtensionVersion		=		1.01;
	ModuleData->InterfaceVersion		=		INTERFACEVERSION;
	ModuleData->RequiresValadation		=		false;
	ModuleData->AutoloadConfiguration	=		true;
	ModuleData->AutoloadLanguage		=		false;

	strncpy_s(ModuleData->Name,"Servo", 256);
	strncpy_s(ModuleData->Author,"bluekirby0", 256);
}

__declspec( dllexport ) IExtension* __stdcall CreateExtension(char* szText)
{
	return (IExtension*)new Servo();
}