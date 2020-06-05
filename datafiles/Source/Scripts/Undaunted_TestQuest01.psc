;BEGIN FRAGMENT CODE - Do not edit anything between this and the end comment
;NEXT FRAGMENT INDEX 8
Scriptname Undaunted_TestQuest01 Extends Quest Hidden

;BEGIN ALIAS PROPERTY BountyMarker
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_BountyMarker Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY Player
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_Player Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY RewardLocation
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_RewardLocation Auto
;END ALIAS PROPERTY

;BEGIN ALIAS PROPERTY PlayerAlias
;ALIAS PROPERTY TYPE ReferenceAlias
ReferenceAlias Property Alias_PlayerAlias Auto
;END ALIAS PROPERTY

;BEGIN FRAGMENT Fragment_0
Function Fragment_0()
;BEGIN CODE
SetObjectiveCompleted(0, true)
SetObjectiveCompleted(10, false)
SetObjectiveCompleted(20, false)
SetObjectiveCompleted(30, false)
SetObjectiveDisplayed(0, abDisplayed = false, abForce = true)
SetObjectiveDisplayed(10, abDisplayed = true, abForce = true)
SetObjectiveDisplayed(20, abDisplayed = false, abForce = true)
SetObjectiveDisplayed(30, abDisplayed = false, abForce = true)
;END CODE
EndFunction
;END FRAGMENT

;BEGIN FRAGMENT Fragment_5
Function Fragment_5()
;BEGIN CODE
SetObjectiveCompleted(10, true)
SetObjectiveCompleted(20, false)
SetObjectiveDisplayed(10, abDisplayed = false, abForce = true)
SetObjectiveDisplayed(20, abDisplayed = true, abForce = true)
;END CODE
EndFunction
;END FRAGMENT

;BEGIN FRAGMENT Fragment_4
Function Fragment_4()
;BEGIN CODE
SetObjectiveCompleted(0, false)
SetObjectiveCompleted(10, false)
SetObjectiveCompleted(20, false)
SetObjectiveCompleted(30, false)
SetObjectiveDisplayed(0, abDisplayed = true, abForce = true)
SetObjectiveDisplayed(10, abDisplayed = false,  abForce = true)
SetObjectiveDisplayed(20, abDisplayed = false, abForce = true)
SetObjectiveDisplayed(30, abDisplayed = false, abForce = true)
;END CODE
EndFunction
;END FRAGMENT

;BEGIN FRAGMENT Fragment_7
Function Fragment_7()
;BEGIN CODE
SetObjectiveCompleted(10, true)
SetObjectiveCompleted(20, true)
SetObjectiveCompleted(30, false)
SetObjectiveDisplayed(10, abDisplayed = false, abForce = true)
SetObjectiveDisplayed(20, abDisplayed = false, abForce = true)
SetObjectiveDisplayed(30, abDisplayed = false, abForce = true)
;END CODE
EndFunction
;END FRAGMENT

;END FRAGMENT CODE - Do not edit anything between this and the begin comment


import Undaunted_SystemScript

Message Property QuestTextMessage Auto  
GlobalVariable Property QuestStage auto
GlobalVariable Property TotalBounties auto
Key Property keyform Auto
STATIC Property XMakerStatic  Auto  


ObjectReference XMarkerRef
int numberOfBountiesNeeded = 2
int numberOfBountiesCurrently = 0
string currentbounty = "loading"
int bountyId = -1

Function RestartEvent()
	if (QuestStage.GetValue() != 10)
		return
	endif
	bountyId = CreateBounty()	
	;XMarkerRef = Game.GetPlayer().PlaceAtMe(XMakerStatic)
	SetXMarker(bountyId,XMarkerRef)
	Alias_BountyMarker.ForceRefTo(XMarkerRef)
	SetBountyMessageRef(bountyId,QuestTextMessage)
	;QuestTextMessage.SetName(currentbounty)
	RestartNamedBounty(bountyId,currentbounty)
	SetCurrentStageID(10)
	QuestStage.SetValue(10)
	RegisterForSingleUpdate(GetConfigValueInt("BountyUpdateRate"))	
endFunction

int Function StartEvent(bool nearby)
	bountyId = CreateBounty()
	;Pass the refs the plugin will edit
	XMarkerRef = Game.GetPlayer().PlaceAtMe(XMakerStatic)
	SetXMarker(bountyId,XMarkerRef)
	Alias_BountyMarker.ForceRefTo(XMarkerRef)
	SetBountyMessageRef(bountyId,QuestTextMessage)
	if ( numberOfBountiesCurrently + 1 == numberOfBountiesNeeded)
		StartEliteBounty(bountyId,true)
	else
		StartBounty(bountyId,true)
	endif
	SetCurrentStageID(10)
	QuestStage.SetValue(10)
	RegisterForSingleUpdate(GetConfigValueInt("BountyUpdateRate"))
	currentbounty = GetBountyName(bountyId)
	;QuestTextMessage.SetName(currentbounty)
endFunction

int Function ClearBountyStatus()
	numberOfBountiesCurrently = 0;
endFunction

Function CleanUpBounty()
	ObjectReference[] all = GetBountyObjectRefs(bountyId,"ALL")		
	int alllength = all.Length
	while(alllength > 0)
		alllength -= 1
		all[alllength].Delete()
	endwhile
	ObjectReference[] decorations = GetBountyObjectRefs(bountyId,"BountyDecoration")		
	int decorationslength = decorations.Length
	while(decorationslength > 0)
	decorationslength -= 1
		decorations[decorationslength].DisableNoWait(true)
	endwhile
	ObjectReference[] ScriptedDoors = GetBountyObjectRefs(bountyId,"ScriptedDoor")		
	int ScriptedDoorslength = ScriptedDoors.Length
	while(ScriptedDoorslength > 0)
		ScriptedDoorslength -= 1
		ScriptedDoors[ScriptedDoorslength].DisableNoWait(false)
	endwhile
endFunction

Event OnUpdate()
	bool isready = false;
	while (!isready)
		if (isSystemReady() == 2)
			isready = true
		else
			Utility.Wait(5.0)
		endif		
	endwhile
	numberOfBountiesNeeded = GetConfigValueInt("NumberOfBountiesPerChain")
	;Enemy check
	ObjectReference[] enemies = GetBountyObjectRefs(bountyId,"Enemy")		
	int enemieslength = enemies.Length
	while(enemieslength > 0)
		enemieslength -= 1
		if (enemies[enemieslength] as Actor).IsDead()
			SetGroupMemberComplete(enemies[enemieslength])
		endif
	endwhile

	;Delete check
	ObjectReference[] Deletes = GetBountyObjectRefs(bountyId,"DELETE")		
	int Deleteslength = Deletes.Length
	while(Deleteslength > 0)
		Deleteslength -= 1
		Deletes[Deleteslength].DisableNoWait(false)
		Deletes[Deleteslength].Delete()
	endwhile

	bool complete = isBountyComplete(bountyId)
	;Debug.Notification("Bounty State: " + complete)
	if (QuestStage.GetValue() == 10)
		If complete
			numberOfBountiesCurrently += 1
			TotalBounties.SetValueInt(TotalBounties.GetValueInt() + 1)
			;UnregisterForUpdate()
			CleanUpBounty()
			if (numberOfBountiesCurrently < numberOfBountiesNeeded)
				;questProperty.SetCurrentStageID(20)
				StartEvent(true)
			Else
				Game.GetPlayer().AddItem(keyform, 1, false)
				SetCurrentStageID(20)
				QuestStage.SetValue(20)
				;UnregisterForUpdate()
			EndIf
		Else
			RegisterForSingleUpdate(GetConfigValueInt("BountyUpdateRate"))
		endif
	EndIf
EndEvent

