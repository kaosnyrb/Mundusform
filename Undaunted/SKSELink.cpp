#include "SKSELink.h"
#include <Undaunted\WorldCellList.h>

//Big shout out to https://github.com/mwilsnd for his project https://github.com/mwilsnd/SkyrimSE-SmoothCam which helped amazingly with the address lib stuff!

Undaunted::RelocAddr<_PlaceAtMe_Native> PlaceAtMerec(0x009951F0);

Undaunted::RelocPtr<PlayerCharacter*> thePlayer(0x02F26EF8);

Undaunted::RelocPtr <DataHandler*> dataHandler(0x01EBE428);

Undaunted::RelocAddr<_MoveRefrToPosition> MoveReffunc(0x009AE5C0);

Undaunted::RelocAddr <_LookupFormByID> FormByID(0x00194230);


DataHandler* Undaunted::GetDataHandler() {
	return *dataHandler;
}

BSFixedString Undaunted::GetCurrentWorldspaceName()
{
	//You're thinking "Why is this here? we can already get this from the player?"
	//Answer is this call doesn't work on the VR version, so this is centralising the differences.
	return GetPlayer()->currentWorldSpace->editorId.Get();
}

PlayerCharacter* Undaunted::GetPlayer()
{
	return *thePlayer;
}

TESObjectREFR* Undaunted::PlaceAtMe(VMClassRegistry* registry, int count, TESObjectREFR* ref, TESForm* spawnForm, int something, bool ForcePersist, bool InitiallyDisabled)
{
	return PlaceAtMerec(registry, count, ref, spawnForm, 1, ForcePersist, InitiallyDisabled);
}

void Undaunted::MoveRef(TESObjectREFR* object, TESObjectCELL* cell, TESWorldSpace* worldspace, NiPoint3 pos, NiPoint3 rot)
{
	UInt32 nullHandle = *g_invalidRefHandle;
	MoveReffunc(object, &nullHandle, cell, worldspace, &pos, &rot);
}

TESForm* Undaunted::LookupFormByID(UInt32 id)
{
	return FormByID(id);
}

static VersionDb db;

VersionDb Offsets::GetDB()
{
	return db;
}

bool Offsets::Initialize() {
	return db.Load();
}

constexpr uintptr_t Offsets::GetByVersionAddr(uintptr_t addr) {
	return addrMap.at(addr);
}

uintptr_t Offsets::GetVersionAddress(uintptr_t addr) {
	return GetOffset(addrMap.at(addr));
}

uintptr_t Offsets::GetOffset(uintptr_t id) {
	uintptr_t ret;
	GetDB().FindOffsetById(id, ret);
	return ret;
}

#ifdef _DEBUG
void Offsets::DumpDatabaseTextFile() {
	if (!GetDB().Load(1, 5, 97, 0)) {
	}
	GetDB().Dump("offsets.txt");
}
#endif
