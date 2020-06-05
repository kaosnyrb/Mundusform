#include <Undaunted\MyPlugin.h>
#include <Undaunted\ConfigUtils.h>
#include <Undaunted\SKSELink.h>
#include <Undaunted\StartupManager.h>


static PluginHandle					g_pluginHandle = kPluginHandle_Invalid;
static SKSEPapyrusInterface         * g_papyrus = NULL;
SKSESerializationInterface* g_serialization = NULL;
SKSEMessagingInterface* g_messageInterface = NULL;

extern "C"	{

	bool SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info)	{	// Called by SKSE to learn about this plugin and check that it's safe to load it
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\Mundusform.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_Error);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);
	
		_MESSAGE("Mundusform");

		// populate info structure
		info->infoVersion =	PluginInfo::kInfoVersion;
		info->name =		"Mundusform";
		info->version =		1;

		// store plugin handle so we can identify ourselves later
		g_pluginHandle = skse->GetPluginHandle();

		if(skse->isEditor)
		{
			_MESSAGE("loaded in editor, marking as incompatible");

			return false;
		}
		//else if(skse->runtimeVersion != RUNTIME_VERSION_1_5_97)
		//{
		//	_MESSAGE("unsupported runtime version %08X", skse->runtimeVersion);

		//	return false;
		//}
		
		if (!Offsets::Initialize()) {
			_ERROR("Failed to load game offset database. Visit https://www.nexusmods.com/skyrimspecialedition/mods/32444 to aquire the correct database file.");			
			return false;
		}

		g_serialization = (SKSESerializationInterface*)skse->QueryInterface(kInterface_Serialization);
		g_messageInterface = (SKSEMessagingInterface*)skse->QueryInterface(kInterface_Messaging);
		// ### do not do anything else in this callback
		// ### only fill out PluginInfo and return true/false

		// supported runtime version
		return true;
	}

	void SKSEMessageReceptor(SKSEMessagingInterface::Message* msg)
	{
		static bool active = true;
		if (!active)
			return;

		if (msg->type == SKSEMessagingInterface::kMessage_PreLoadGame)
		{
			_MESSAGE("kMessage_PreLoadGame rechieved, clearing bounty data.");
			
		}
		//Register to recieve interface from Enchantment Framework
		//if (msg->type == SKSEMessagingInterface::kMessage_PostLoad)


		//kMessage_InputLoaded only sent once, on initial Main Menu load
		//else if (msg->type == SKSEMessagingInterface::kMessage_InputLoaded)

	}

	bool SKSEPlugin_Load(const SKSEInterface * skse)	{	// Called by SKSE to load this plugin
		_MESSAGE("Loading Mundusform..");

		g_papyrus = (SKSEPapyrusInterface *)skse->QueryInterface(kInterface_Papyrus);
		g_messageInterface->RegisterListener(g_pluginHandle, "SKSE", SKSEMessageReceptor);
		//Check if the function registration was a success...
		bool btest = g_papyrus->Register(Undaunted::RegisterFuncs);
		

		Undaunted::GetDataHandler();
		Undaunted::GetPlayer();

		if (btest) {
			_MESSAGE("Register Succeeded");
		}

		return true;
	}
};