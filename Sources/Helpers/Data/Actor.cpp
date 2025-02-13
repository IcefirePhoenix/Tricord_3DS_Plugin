#include "Helpers.hpp"

namespace CTRPluginFramework
{
   u32 typeOffset = 0x90;
   u32 UNIQ_Offset = 0xC0;
   u32 internalNameOffset = 0x1E0;
   u32 healthOffset = 0x598;
   u32 positionOffset = 0x1F0;
   u32 scaleOffset = 0x1FC;
   u32 playerTargetOffset = 0x608; // check?

   /* ------------------ */

   Actor::Actor(u32 ptr) : _memLocation(ptr)
   {
      u32 namePtr;
      u16 actorID;
      u8 actorType;

      Process::Read16(_memLocation + UNIQ_Offset, actorID);
      Process::Read8(_memLocation + typeOffset, actorType);
      Process::Read32(_memLocation + internalNameOffset, namePtr);

      _UNIQ = actorID;
      _internalName = getInternalName(namePtr);
      _genericName = getGenericName(_internalName);

      if (actorType > 5)
         _type = static_cast<ActorType>(ActorType::NONE);
      else
         _type = static_cast<ActorType>(actorType);
   }

   // Retrieves the Actor's internal name
   std::string Actor::getInternalName(u32 internalNamePtr)
   {
      std::string internalName = "";
      u8 currChar;

      while (true)
      {
         Process::Read8(internalNamePtr, currChar);
         if (currChar == 0)
            break;

         internalName += static_cast<char>(currChar);
         internalNamePtr++;
      }
      return internalName;
   }

   // Returns the generic name of the Actor
   std::string Actor::getGenericName(std::string internalName)
   {
      if (!verifyValidity())
         return NULL;

      auto iterator = GameData::actorNames.find(internalName);
      if (iterator != GameData::actorNames.end())
         return iterator->second;
      else
         return "";
   }

   // Ensures the current data block holds the correct referenced Actor instance
   bool Actor::verifyValidity(void)
   {
      u8 rawUNIQ;
      Process::Read8(_memLocation + UNIQ_Offset, rawUNIQ);
      return rawUNIQ == getUNIQ();
   }

   // Sets the Actor's health -- applicable only to ENEMY types
   void Actor::setHealth(u32 HP)
   {
      u32 healthOffset = 0x598;
      u32 currHealth;

      if (verifyValidity() && isType(ENEMY))
      {
         Process::Read32(_memLocation + healthOffset, currHealth);

         if (currHealth != 0)
            Process::Write32(_memLocation + healthOffset, HP);
      }
   }

   // Returns the ActorType
   bool Actor::isType(ActorType category)
   {
      if (!verifyValidity())
         return false;

      return _type == category;
   }

   // Returns the ActorType
   ActorType Actor::getType()
   {
      if (!verifyValidity())
         return ActorType::NONE;

      return _type;
   }

   // Returns the UNIQ ID stored upon initial access of the Actor's data block
   int Actor::getUNIQ(void)
   {
      return _UNIQ;
   }

   // TODO: Sets the current position of the Actor
   bool Actor::setLocation(void)
   {
      verifyValidity();
      return true;
   }

   // TODO: Gets the current position of the Actor
   // Position Actor::getLocation(void)
   u32 Actor::getLocation(void)
   {
      verifyValidity();
      return 0;
   }
}
