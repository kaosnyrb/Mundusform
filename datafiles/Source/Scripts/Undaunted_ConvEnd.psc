;BEGIN FRAGMENT CODE - Do not edit anything between this and the end comment
;NEXT FRAGMENT INDEX 5
Scriptname Undaunted_ConvEnd Extends TopicInfo Hidden

;BEGIN FRAGMENT Fragment_4
Function Fragment_4(ObjectReference akSpeakerRef)
Actor akSpeaker = akSpeakerRef as Actor
;BEGIN CODE
Game.GetPlayer().AddSpell(givespell)
dialougestage.SetValue(10)
undauntedQuest.SetCurrentStageID(0)
;END CODE
EndFunction
;END FRAGMENT

;END FRAGMENT CODE - Do not edit anything between this and the begin comment

SPELL Property givespell  Auto  

GlobalVariable Property dialougestage  auto

Quest Property undauntedQuest  Auto  
