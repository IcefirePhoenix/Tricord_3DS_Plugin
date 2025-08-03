#ifndef MAPACTORMNGR_HPP
#define MAPACTORMNGR_HPP

#include "CTRPF.hpp"

namespace CTRPluginFramework
{
    class MapActorMngr
    {
    public:
        static std::multimap<int, Actor> retrieveActorList(void);
        static u8 getActorCount(ActorType category = NONE);
        static bool despawnAllActors(ActorType category = ENEMY);
        static void clearActorList(void);
    };
}
#endif