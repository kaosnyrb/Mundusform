Scriptname Undaunted_SystemScript   

int Function CreateBounty() global native
float Function StartBounty(int bountyId, bool nearby) global native
float Function StartEliteBounty(int bountyId, bool nearby) global native
float Function StartNamedBounty(int bountyId,bool nearby, String bountyName) global native
float Function RestartNamedBounty(int bountyId,String bountyName) global native
float Function StartNamedBountyNearRef(int bountyId,bool nearby, String bountyName, ObjectReference ref, string WorldspaceName) global native

bool Function InitSystem(int playerLevel) global native
int Function isSystemReady() global native
bool Function ClaimStartupLock() global native

bool Function isBountyComplete(int bountyId) global native
Function SetGroupMemberComplete(objectReference objref) global native
string Function GetBountyName(int bountyId) global native
bool Function SetXMarker(int bountyId,objectReference markerref) global native
bool Function SetBountyMessageRef(int bountyId,Message messageref) global native

bool Function AddBadRegion(int regionid) global native
int Function AddGroup(String questtext,String modRequirement, int minLevel, int maxLevel, int playerLevel) global native
Function AddMembertoGroup(int groupid, int memberformid, string bountyType, string modelFilepath) global native
int Function GetModForm(String ModName, int FormId) global native
Form Function SpawnRandomReward(int rewardOffset,int playerlevel) global native

Function SetConfigValue(string key, string value) global native
int Function GetConfigValueInt(string key) global native

string Function GetPlayerWorldSpaceName() global native
bool Function isPlayerInWorldSpace(String WorldspaceName) global native  
;bool Function IsGroupMemberUsed(objectReference member) global native
Function PlayerTraveled(float distance) global native
Function SetScriptedDoorsComplete() global native
ObjectReference[] Function GetBountyObjectRefs(int bountyId,string bountyType) global native

string Function GetRandomBountyName() global native


ObjectReference[] Function SpawnRift(int bountyId,ObjectReference ref) global native
ObjectReference[] Function GetRiftReferences() global native
Function CaptureCellData() global native
float[] Function GetRiftRotations() global native

ObjectReference Function GetRandomRiftStartMarker() global native

ObjectReference Function SpawnMonsterInCell(int FormId) global native

Function CaptureArea() global native
Function CaptureNavTile() global native
Function ExportNavMesh() global native
