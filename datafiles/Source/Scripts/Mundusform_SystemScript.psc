Scriptname Mundusform_SystemScript   

bool Function InitSystem(int playerLevel) global native
string Function GetPlayerWorldSpaceName() global native
bool Function isPlayerInWorldSpace(String WorldspaceName) global native  
Function SetConfigValue(string key, string value) global native
int Function GetConfigValueInt(string key) global native
int Function GetModForm(String ModName, int FormId) global native
ObjectReference[] Function SpawnRift(int bountyId,ObjectReference ref) global native
float[] Function GetRiftRotations() global native
ObjectReference[] Function GetRiftReferences() global native
Function CaptureArea() global native
Function CaptureNavTile() global native
Function ExportNavMesh() global native