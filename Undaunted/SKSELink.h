#ifndef SKSELink
#define SKSELink
#include <skse64/GameTypes.h>
#include <skse64/NiTypes.h>
#include <skse64/PapyrusGameData.h>
#include <skse64/GameReferences.h>
#include <skse64/GameData.h>
#include <skse64/PluginAPI.h>
#include <skse64/PapyrusCell.h>
#include <skse64/GameForms.h>
#include <skse64/PapyrusVM.h>
#include <time.h>
#include <skse64/GameRTTI.h>
#include <skse64/GameReferences.h>
#include "skse64/PapyrusVM.h"
#include "skse64/PapyrusArgs.h"
#include "skse64/PapyrusNativeFunctions.h"
#include <set>
#include <shlobj.h>				// CSIDL_MYCODUMENTS
#include "skse64/PluginAPI.h"		// super
#include "skse64_common/skse_version.h"	// What version of SKSE is running?
#include <skse64/PluginManager.h>
#include <skse64/Serialization.h>
#include "addrlib/relocation.h"
#include "addrlib/offsets.h"
#include "addrlib/versiondb.h"

//#include "RE/Skyrim.h"
namespace Undaunted
{
	PlayerCharacter* GetPlayer();
	TESObjectREFR* PlaceAtMe(VMClassRegistry* registry, int count, TESObjectREFR* ref, TESForm* spawnForm, int something, bool ForcePersist, bool InitiallyDisabled);
	DataHandler* GetDataHandler();
	
	BSFixedString GetCurrentWorldspaceName();


	void MoveRef(TESObjectREFR* object, TESObjectCELL* cell, TESWorldSpace* worldspace, NiPoint3 pos, NiPoint3 rot);

	TESForm* LookupFormByID(UInt32 id);


	class NavMesh;
	class NavMeshArray
	{
	public:
		VMArray<NavMesh> navMeshes;  // 00
	};
}
#endif