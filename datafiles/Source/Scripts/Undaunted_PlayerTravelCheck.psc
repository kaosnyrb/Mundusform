Scriptname Undaunted_PlayerTravelCheck extends ReferenceAlias  
import Undaunted_SystemScript
import Undaunted_Activator

Quest Property undauntedQuest Auto

Function LoadJsonData()
	if (ClaimStartupLock())
		InitSystem(Game.GetPlayer().GetLevel())
		Debug.Notification("Undaunted initialised")
    EndIf
endFunction

Event OnPlayerLoadGame()
	;Debug.Notification("Undaunted OnPlayerLoadGame")
	LoadJsonData()
	;Tell the Undaunted Activator that we've loaded.
	(undauntedQuest as Undaunted_TestQuest01).RestartEvent()
EndEvent


Event OnInit()
    LoadJsonData()
EndEvent

Event OnPlayerFastTravelEnd(float afTravelGameTimeHours)
    PlayerTraveled(afTravelGameTimeHours)
    ;Debug.Notification("Player traveled: " + afTravelGameTimeHours)
endEvent