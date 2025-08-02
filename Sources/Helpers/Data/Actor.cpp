#include "Helpers.hpp"

namespace CTRPluginFramework
{
   u32 actorProfilePtr = 0x98;
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
      u16 actorUNIQ;
      u8 actorType;

      Process::Read16(_memLocation + UNIQ_Offset, actorUNIQ);
      Process::Read8(_memLocation + typeOffset, actorType);
      Process::Read32(_memLocation + internalNameOffset, namePtr);

      _ID = retrieveActorID();
      _UNIQ = actorUNIQ;
      _internalName = "";
      _genericName = "";

      // _internalName = getInternalName(namePtr);
      // _genericName = getGenericName(_internalName);

      if (actorType == 0xFF)
         _type = ActorType::SPECIAL;
      else if (actorType > 0x5)
         _type = ActorType::NONE;
      else
         _type = static_cast<ActorType>(actorType);
   }

   // Gets the Actor's ID from the ActorProfile master data block
   u16 Actor::retrieveActorID(void)
   {
      u8 ID_Offset = 0x80;
      u16 actorID = 0x0;
      u32 actorProfileLoc = 0x0;

      Process::Read32(_memLocation + actorProfilePtr, actorProfileLoc);

      if (!GeneralHelpers::isNullPointer(actorProfileLoc))
         Process::Read16(actorProfileLoc + ID_Offset, actorID);

      return actorID;
   }

   // Retrieves the Actor's internal name
   std::string Actor::getInternalName(u32 internalNamePtr)
   {
      std::string internalName = "";
      u32 actorProfile_Name = 0x8C;
      u32 actorProfileLoc = 0x0;
      u8 currChar;

      Process::Read32(_memLocation + actorProfilePtr, actorProfileLoc);

      if (!GeneralHelpers::isNullPointer(actorProfileLoc))
      {
         Process::Read32(actorProfileLoc + actorProfile_Name, actorProfile_Name);
         while (true)
         {
            Process::Read8(internalNamePtr, currChar);
            if (currChar == 0)
               break;

            internalName += static_cast<char>(currChar);
            internalNamePtr++;
         }
      }

      // OSD::Notify(internalName);
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
      u16 rawUNIQ;
      Process::Read16(_memLocation + UNIQ_Offset, rawUNIQ);
      return rawUNIQ == getUNIQ();
   }

   // Sets the Actor's health -- applicable only to ENEMY types
   // Note: Event-driven enemy spawns (such as Blue Rats appearing out of melted snow piles) or stacked enemies 2-3 units tall are categorized as SPECIAL type
   // Note 2: Beamos and Morths are excluded here
   void Actor::setHealth(u32 HP)
   {
      u16 ID = getID();
      u32 currHealth;
      Process::Read32(_memLocation + healthOffset, currHealth);

      if (isType(ActorType::SPECIAL))
      {
         switch (ID)
         {
            case 0x186: // Blue Rats
            case 0x110: // Totem Deku head
            case 0x112: // Totem Deku body
            case 0x113: // Totem Deku body (random height)
            case 0x98: // Terrorpins
            case 0x8F: // Wallmaster
               break;
            default:
               return;
         }
      }
      else if (isType(ActorType::ENEMY))
      {
         switch (ID)
         {
            case 0x1BC: // Beamos (excluded for instakill only)
               if (HP == 0x0) return;
               else break;
            case 0x1EC: // Morth (excluded for instakill + inf)
               return;
            default:
               break;
         }
      }

      if (verifyValidity() && currHealth != 0)
         Process::Write32(_memLocation + healthOffset, HP);
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
   // Returns the Actor ID stored upon initial access of the Actor's data block
   u16 Actor::getID(void)
   {
      return _ID;
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
