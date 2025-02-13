#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "CTRPF.hpp"

namespace CTRPluginFramework
{
    enum ActorType
    {
        MAP = 0,
        TAG = 1,
        AREA = 2,
        OBJ = 3,
        ENEMY = 4,
        NPC = 5,
        NONE = 6
    };

    class Actor
    {
    public:
        Actor(u32 ptr);

        std::string getInternalName(u32 internalNamePtr);
        std::string getGenericName(std::string internalName);

        bool verifyValidity(void);
        ActorType getType(void);
        bool isType(ActorType category);
        bool setLocation(void);
        u32 getLocation(void);
        int getUNIQ(void);
        void setHealth(u32 HP);

    private:
        u32 _memLocation;
        u16 _UNIQ;
        ActorType _type;
        std::string _internalName;
        std::string _genericName;
    };
}
#endif
