#include "Chat.hpp"

namespace Chat{
    void Log::putAutoTransOpen()
    {
        s.put('\xFF');
        s.put('\xEF');
        s.put('\x27');
    }

    void Log::putAutoTransClose()
    {
        s.put('\xFF');
        s.put('\xEF');
        s.put('\x28');
    }

    IAutoTrans Log::Region(unsigned char RegionID)
    {
        IAutoTrans i;
        i.x = m_Ashita->GetResources()->GetRegionName(RegionID);
        return i;
    }
    
    IAutoTrans Log::Zone(unsigned int ZoneID)
    {
        IAutoTrans i;
        i.x = m_Ashita->GetResources()->GetZoneName(ZoneID);
        return i;
    }
    
    IAutoTrans Log::Keyitem(unsigned int ID)
    {
        IAutoTrans i;
        i.x = m_Ashita->GetResources()->GetKeyitemID(ID);
        return i;
    }
    
    Log::Log() : m_Ashita(NULL)
    {
        mode = RecvdSay;
        s = std::ostringstream();
    }
    Log::~Log()
    {
    }

    void Log::SetCore(IAshitaCore* mAshitaCore)
    {
        m_Ashita = mAshitaCore;
    }
};