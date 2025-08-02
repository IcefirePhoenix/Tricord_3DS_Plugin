#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "CTRPF.hpp"

namespace CTRPluginFramework
{
    enum ActorType
    {
        MAP = 0x0,
        TAG = 0x1,
        AREA = 0x2,
        OBJ = 0x3,
        ENEMY = 0x4,
        NPC = 0x5,
        NONE = 0x6,
        SPECIAL = 0xFF
    };

    class Actor
    {
    public:
        Actor(u32 ptr);

        std::string getInternalName(u32 internalNamePtr);
        std::string getGenericName(std::string internalName);

        u16 retrieveActorID(void);
        bool verifyValidity(void);
        ActorType getType(void);
        bool isType(ActorType category);
        bool setLocation(void);
        u32 getLocation(void);
        int getUNIQ(void);
        u16 getID(void);
        void setHealth(u32 HP);

    private:
        u32 _memLocation;
        u16 _UNIQ;
        u16 _ID;
        ActorType _type;
        std::string _internalName;
        std::string _genericName;
    };
}
#endif
