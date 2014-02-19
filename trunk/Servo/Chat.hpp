#pragma once
#include <stdint.h>
#include <sstream>
#include <string>
#include "../../../SDK/Depends/Common/Extension.h"


#include <iostream>

namespace Chat
{
    //typedef const char* IAutoTrans;
    struct IAutoTrans{
        const char* x;
    };

    enum Mode : short
    {
        // Info mostly taken from http://www.ffevo.net/wiki/index.php/ChatIDs

        //----------------------
        // Common "Chat" Modes
        //----------------------
        Unknown = 0,            // Catch all. Do not use...it sometimes breaks the chat log.
        SentSay = 0x1,          // Outgoing Say
        SentSay2 = 0x2,         // Outgoing Say
        SentTell = 0x4,         // Outgoing Tell
        SentParty = 0x5,        // Outgoing Party
        SentLinkShell = 0x6,    // Outgoing Linkshell
        SentEmote = 0x7,        // Outgoing Emote, untargetted
        RecvdSay = 0x9,         // Incoming Say
        RecvdSay2 = 0xA,        // Incoming Say
        RcvdTell = 0xC,         // Incoming Tell
        RcvdTell2 = 0xD,        // Incoming Tell
        RcvdLinkShell = 0xE,    // Incoming Linkshell
        SentEmoteTo = 0xF,      // Outgoing Emote, targeted
        Echo = 0xCE,            // Echo
        
        //----------------------
        // Action Modes
        //----------------------
        
        ActCallHelp = 0x10,         // Call for Help!
        ActFriendReq = 0x12,        // Sent Friend Request
        ActCastMagic = 0x13,        //
        ActHitDamage = 0x14,        //
        ActMiss = 0x15,             //
        ActDrained = 0x16,          //
        ActPTHitAbsorbed = 0x17,    //
        ActPTRecoverHP = 0x18,      //
        ActPTHitDamage = 0x19,      //
        ActPTMiss = 0x1A,           //
        ActPTDrained = 0x1B,        //
        ActDamaged = 0x1C,          //
        ActAvoidDamage = 0x1D,      //
        ActRecoverHealth = 0x1E,    //
        ActRecoverHP = 0x1F,        //
        ActPTDamaged = 0x20,        //
        ActPTAvoidDamage = 0x21,    //
        ActPCGainHealth = 0x22,     //
        ActPTHealedOther = 0x23,    //
        ActDefeats = 0x24,          //
        ActPCDeath = 0x25,          //
        ActDeath = 0x26,            //
        ActPTDeath = 0x27,          //
        ActPCDamage = 0x28,         //
        ActPCMiss = 0x29,           //
        ActPCRecoverHealth = 0x2A,  //
        ActPCHealsPC = 0x2B,        //
        ActPCDefeated = 0x2C,       //
        
        //----------------------
        // Special Action Modes
        //----------------------
                
        SAct = 0x32,                //
        SActPT = 0x33,              //
        SActPC = 0x34,              //
        SActRecvdBenefit = 0x38,    //
        SActRecvdDetriment = 0x39,  //
        SActNoEffect = 0x3B,        //
        SActPTRecvdBenefit = 0x3C,  //
        SActPTRecvdDetriment = 0x3D,//
        SActPTNoEffect = 0x3F,      // Needs confirmation
        SActPCRecvdBenefit = 0x40,  //
        SActPCRecvdDetriment = 0x41,//
        SActPCNoEffect = 0x43,      //
        SActPTResisted = 0x44,      //
        SActOtherResisted = 0x45,   //
        
        SActSAttUse = 0x65,         //
        SActOtherBenefit = 0x6F,    //
        SActOtherDetriment = 0x70,  //
        SActOtherMissed = 0x72,     //
        
        SActSpellEffect = 0xBB,     //
        
        //----------------------
        // Item Modes
        //----------------------
        
        ItemRecvdEffect = 0x51,     //
        ItemPCUsed = 0x55,          //
        ItemPCUsed2 = 0x5A,         //
        ItemLearnedSpell = 0x5B,    //
        
        //----------------------
        // Mob Special Action Modes
        //----------------------
        
        MobActSAttUse = 0x64,       //
        MobActDetriment = 0x66,     //
        MobActSAttEvaded = 0x68,    //
        MobActSAttAoE = 0x69,       //
        MobActSupport = 0x6A,       //
        MobActDrain = 0x6B,         //
        MobActRAttMissed = 0x6D,    //
        MobActSAttInit = 0x6E,      //
        
        //----------------------
        // System Message Modes
        //----------------------
        
        Sys1 = 0x79,                // Target out of range, AH & synth feedback, Item feedback, trade feedback
        Sys2 = 0x7A,                // Entity chasing out of range, entity intimidated, raise, paralyzed, cannot see entity, 
                                    // casting interrupted, cannot attack, target already claimed, no exp gained
        Sys3 = 0x7B,                // Sneak/invis wearing, wait longer to perform action, chat not sent/recvd
        SysObtainedItem = 0x7F,     //
        SysSkillGained = 0x81,      //
        SysExpLvlMerit = 0x83,      //
        SysInvitedToParty = 0x87,   //
        SysLogoutCountdown = 0x88,  //
        SysBazaar = 0x8A,           //
        SysTimePlayed = 0x8B,       //
        SysClock = 0x8C,            //
        SysDynamisWarning = 0x8D,   //
        SysTaunts = 0x8E,           //
        SysTutorialMoogle = 0x90,   //
        SysCaughtFish = 0x92,       //
        Sys4 = 0x94,                // Digging/Fishing/Door/Ballista Feedback, Mission Gil Reward, Accepted Mission, Receive 
                                    // Mission Key Item, Gained Conquest Points
        SysTimeRestriction = 0x95,  //
        SysTutorialNPC = 0x98,      //
        SysCommandError = 0x9D,     // Also /help output
        SysConquestUpdate = 0xA1,   //
        
        SysNeedKey = 0xBE,          //
        Sys5 = 0xBF,                // Effect wears off, Detriment wears off, no longer stunned, fall to level
        SysMOTD = 0xC8,             // Also recast times (but not /recast feedback)
        SysSearchComment = 0xCC,    //
        SysLinkshellMOTD = 0xCD,    //
        SysExamined = 0xD0,         //
        
        //----------------------
        // Alliance Action Modes
        //----------------------
        
        AllyHealAlly = 0xA2,        //
        AllyDamage = 0xA3,          //
        AllyMiss = 0xA4,            //
        AllyDefeats = 0xA6,         //
        AllyDefeated = 0xA7,        //
        AllySAct = 0xA8,            //
        AllyNoEffect = 0xAA,        //
        AllyItemUse = 0xAB,         //
        AllyRecvdDetriment = 0xAE,  //
        AllyRecvdBenefit = 0xAF,    //
        
        AllyRecvdDetrimentAoE = 0xB6,//
        AllyDamaged = 0xB9,         //
        AllyEvades = 0xBA,          //
        
        //----------------------
        // NPC Action Modes
        //----------------------
        
        NPCSAct = 0xB1,             //
        
        NPCActDetriment = 0xB5,     //
        
        //----------------------
        // Unknown or Unused Modes
        //----------------------
        
        SentYell = 0x03,
        RecvdYell = 0x11,
        //0x3
        //0x8
        //0xB
        //0x11
        //0x2D-0x31
        //0x35-0x37
        //0x3A
        //0x3E
        //0x42
        //0x46-0x50
        //0x52-0x54
        //0x56-0x59
        //0x5C-0x63
        //0x67
        //0x6C
        //0x71
        //0x73-0x78
        //0x7C-0x7E
        //0x80
        //0x82
        //0x84-0x86
        //0x89
        //0x8F
        //0x91
        //0x93
        //0x96
        //0x97
        //0x99-0x9C
        //0x9E-0xA0
        //0xA5
        //0xA9
        //0xAC
        //0xAD
        //0xB0
        //0xB2-0xB4
        //0xB7
        //0xB8
        //0xBC
        //0xBD
        //0xC0-0xC7
        //0xC9-0xCB
        //0xCF
        //0xD1-0xFF
    };
    
    enum Format {
        LineBreak,// = 0x07,           // Inserts an inline line break (does not flush)
        Reset,// = 0x1E01,
        Green,// = 0x1E02,
        Blue,// = 0x1E03,
        Purple,// = 0x1E05,
        Aqua,// = 0x1E06,
        Peach,// = 0x1E07,
        White,// = 0x1E1A,
        Gray,// = 0x1E1B,
        Lilac,// = 0x1E3A,
        BlueWhite,// = 0x1E3C,
        InvisibleBlue,// = 0x1E62,     // Ridiculously hard to read
        Red,// = 0x1E69,
        Yellow,// = 0x1E6E,
        RoyalBlue,// = 0x1E71,
        Invisible,// = 0x1E86,         // This one really is invisible as far as I can tell
        GreenWhite,// = 0x1E96,
        SageGreen,// = 0x1EA0,
        AutoTransOpen,// = 0xFFEF27,
        AutoTransClose,// = 0xFFEF28,
        EnterToContinue,// = 0x7F3101  // Waits one second before making prompt available. Can alter the last byte to change the timing.
    };
    
    enum Control
    {
        flush,  //Flush the output to chat log and clears the buffer
        clear,       //Clears the buffer without writing it out
    };

    class Log{
    private:
        Mode mode;
        std::ostringstream s;
        IAshitaCore* m_Ashita;

        void putAutoTransOpen();
        void putAutoTransClose();
    public:
        Log &operator<<(Chat::Mode x)
        {
            mode = x;
            return *this;
        }

        Log &operator<<(Chat::Format x)
        {
            switch (x)
            {
            case LineBreak:
                s.put('\x07');
                break;
            case Reset:
                s.put('\x1E');
                s.put('\x01');
                break;
            case Green:
                s.put('\x1E');
                s.put('\x02');
                break;
            case Blue:
                s.put('\x1E');
                s.put('\x03');
                break;
            case Purple:
                s.put('\x1E');
                s.put('\x05');
                break;
            case Aqua:
                s.put('\x1E');
                s.put('\x06');
                break;
            case Peach:
                s.put('\x1E');
                s.put('\x07');
                break;
            case White:
                s.put('\x1E');
                s.put('\x1A');
                break;
            case Gray:
                s.put('\x1E');
                s.put('\x1B');
                break;
            case Lilac:
                s.put('\x1E');
                s.put('\x3A');
                break;
            case BlueWhite:
                s.put('\x1E');
                s.put('\x3C');
                break;
            case InvisibleBlue:
                s.put('\x1E');
                s.put('\x62');
                break;
            case Red:
                s.put('\x1E');
                s.put('\x69');
                break;
            case Yellow:
                s.put('\x1E');
                s.put('\x6E');
                break;
            case RoyalBlue:
                s.put('\x1E');
                s.put('\x71');
                break;
            case Invisible:
                s.put('\x1E');
                s.put('\x86');
                break;
            case GreenWhite:
                s.put('\x1E');
                s.put('\x96');
                break;
            case SageGreen:
                s.put('\x1E');
                s.put('\xA0');
                break;
            case AutoTransOpen:
                putAutoTransOpen();
                break;
            case AutoTransClose:
                putAutoTransClose();
                break;
            case EnterToContinue:
                s.put('\x7F');
                s.put('\x31');
                s.put('\x01');
                break;
            }
            return *this;
        }
        
        Log &operator<<(ISpell* x)
        {
            putAutoTransOpen();
            s << x->Name;
            putAutoTransClose();
            return *this;
        }
        
        Log &operator<<(IAbility* x)
        {
            putAutoTransOpen();
            s << x->Name;
            putAutoTransClose();
            return *this;
        }
        
        Log &operator<<(IItem* x)
        {
            putAutoTransOpen();
            s << x->Name;
            putAutoTransClose();
            return *this;
        }
        
        Log &operator<<(Chat::IAutoTrans x)
        {
            putAutoTransOpen();
            s << x.x;
            putAutoTransClose();
            return *this;
        }
        
        Log &operator<<(Chat::Control x)
        {
            switch(x)
            {
            case Control::flush:
                m_Ashita->GetDataModule()->AddChatLine(mode,s.str().c_str());
            case Control::clear:
                s = std::ostringstream();
                break;
            }
            return *this;
        }
        
        template <typename T>
		Log &operator<<(T x)
		{
			s << x;
			return *this;
		}

        Log();
        ~Log();

        IAutoTrans Region(unsigned char RegionID);
        IAutoTrans Zone(unsigned int ZoneID);
        IAutoTrans Keyitem(unsigned int ID);

        void SetCore(IAshitaCore* mAshitaCore);
    };

};