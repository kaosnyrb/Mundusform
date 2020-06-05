Scriptname Undaunted_TeleportPlayer extends ObjectReference  
import Undaunted_SystemScript

ObjectReference Property TargetLocation  Auto  
Cell Property ResetCellRef  Auto  

event onActivate(objectReference akActivator)
	akActivator.MoveTo(TargetLocation)
	SetScriptedDoorsComplete()
	ResetCellRef.Reset()
endEvent
