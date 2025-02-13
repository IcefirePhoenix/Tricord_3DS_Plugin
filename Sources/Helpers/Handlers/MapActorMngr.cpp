#include "Helpers.hpp"

namespace CTRPluginFramework
{
    std::map<int, Actor> actorList;

    /* ------------------ */

    // Retrieves a list of all Actors in the current area
    std::map<int, Actor> MapActorMngr::retrieveActorList(void)
    {
        u32 currActorPtr;

        // get first entry of Actor pointer chain...
        Process::Read32(AddressList::getAddress("ActorListStart"), currActorPtr);
        actorList.clear();

        // circular; the end of the list always directs back to the start of the chain...
        while (currActorPtr != AddressList::getAddress("ActorListStart"))
        {
            if (!GeneralHelpers::isNullPointer(currActorPtr))
            {
                Actor newActor(currActorPtr);
                if (newActor.getType() != ActorType::NONE)
                    actorList.emplace(newActor.getUNIQ(), newActor);

                // update with next Actor address...
                Process::Read32(currActorPtr, currActorPtr);
            }
            else break;
        }
        return actorList;
    }

    // Retrieves interactive Actor count -- note: maximum limit is 90 Actors
    u8 MapActorMngr::getActorCount(ActorType category)
    {
        u8 actorCount;

        // differs from total Actor count (limit 180)
        Process::Read8(AddressList::getAddress("ActorPtrChainLen"), actorCount);
        return actorCount;
    }

    // Despawns all Actor instances in the current area
    bool MapActorMngr::despawnAllActors(ActorType category)
    {
        // TODO: not enough research
        return true;
    }

    // Resets the current actor table -> preferably called during loading screens
    void MapActorMngr::clearActorList(void)
    {
        actorList.clear();
    }
}
