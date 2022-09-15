/*	
 Custom flag: AirshoT (+AT)
 Tank shoots an extra two shots at an angle upward.

 Server Variables:
 _airshotAngle - controls the angle at which the extra bullets shoot upward.
 
 Extra notes:
 - The player world weapon shots make use of metadata 'type' and 'owner'. Type is
   is AT and owner is the playerID.
 
 Copyright 2022 Quinn Carmack
 May be redistributed under either the LGPL or MIT licenses.
 
 ./configure --enable-custom-plugins=airshotFlag
*/
#include "bzfsAPI.h"
#include "plugin_utils.h"
#include <math.h>

using namespace std;

class AirshotFlag : public bz_Plugin
{
    virtual const char* Name ()
    {
        return "Airshot Flag";
    }
    virtual void Init(const char*);
	virtual void Event(bz_EventData*);
	~AirshotFlag();

	virtual void Cleanup(void)
	{
		Flush();
	}
};

BZ_PLUGIN(AirshotFlag)

void AirshotFlag::Init(const char*)
{
	bz_RegisterCustomFlag("AT", "Airshot", "Extra two shots at an angle upward.", 0, eGoodFlag);
	bz_registerCustomBZDBDouble("_airshotAngle", 0.12);			// Angle between ground level and the first shot upward
	Register(bz_eShotFiredEvent);
	Register(bz_ePlayerDieEvent);
}

AirshotFlag::~AirshotFlag() {}

void AirshotFlag::Event(bz_EventData *eventData)
{
	switch (eventData->eventType)
	{
		case bz_eShotFiredEvent:
		{
			bz_ShotFiredEventData_V1* data = (bz_ShotFiredEventData_V1*) eventData;
			bz_BasePlayerRecord *playerRecord = bz_getPlayerByIndex(data->playerID);

			if (playerRecord && playerRecord->currentFlag == "AirshoT (+AT)")
			{
				float pos[3];
				pos[0] = playerRecord->lastKnownState.pos[0] + cos(playerRecord->lastKnownState.rotation)*bz_getBZDBDouble("_muzzleFront");
				pos[1] = playerRecord->lastKnownState.pos[1] + sin(playerRecord->lastKnownState.rotation)*bz_getBZDBDouble("_muzzleFront");
				pos[2] = playerRecord->lastKnownState.pos[2] + bz_getBZDBDouble("_muzzleHeight");
				
				// Middle shot
				float vel1[3];
				vel1[0] = (cos(playerRecord->lastKnownState.rotation) + playerRecord->lastKnownState.velocity[0]/bz_getBZDBDouble("_shotSpeed"))*cos(bz_getBZDBDouble("_airshotAngle"));
				vel1[1] = (sin(playerRecord->lastKnownState.rotation) + playerRecord->lastKnownState.velocity[1]/bz_getBZDBDouble("_shotSpeed"))*cos(bz_getBZDBDouble("_airshotAngle"));
				vel1[2] = sin(bz_getBZDBDouble("_airshotAngle"));
				uint32_t shot1GUID = bz_fireServerShot("AT", pos, vel1, playerRecord->team);
				bz_setShotMetaData(shot1GUID, "type", bz_getPlayerFlag(data->playerID));
				bz_setShotMetaData(shot1GUID, "owner", data->playerID);
				
				// Top shot
				float vel2[3];
				vel2[0] = (cos(playerRecord->lastKnownState.rotation) + playerRecord->lastKnownState.velocity[0]/bz_getBZDBDouble("_shotSpeed"))*cos(bz_getBZDBDouble("_airshotAngle")*2);
				vel2[1] = (sin(playerRecord->lastKnownState.rotation) + playerRecord->lastKnownState.velocity[1]/bz_getBZDBDouble("_shotSpeed"))*cos(bz_getBZDBDouble("_airshotAngle")*2);
				vel2[2] = sin(bz_getBZDBDouble("_airshotAngle")*2);
				uint32_t shot2GUID = bz_fireServerShot("AT", pos, vel2, playerRecord->team);
				bz_setShotMetaData(shot2GUID, "type", bz_getPlayerFlag(data->playerID));
				bz_setShotMetaData(shot2GUID, "owner", data->playerID);
			}
			
			bz_freePlayerRecord(playerRecord);
		} break;
		case bz_ePlayerDieEvent:
		{
			bz_PlayerDieEventData_V1 *data = (bz_PlayerDieEventData_V1*) eventData;
			uint32_t shotGUID = bz_getShotGUID(data->killerID, data->shotID);

			if (bz_shotHasMetaData(shotGUID, "type") && bz_shotHasMetaData(shotGUID, "owner"))
			{
			    std::string flagType = bz_getShotMetaDataS(shotGUID, "type");
			    
			    if (flagType == "AT")
			    {
			        data->killerID = bz_getShotMetaDataI(shotGUID, "owner");
			        data->killerTeam = bz_getPlayerTeam(data->killerID);
			    }
			}
		} break;
		default:
			break;
	}
}
				
