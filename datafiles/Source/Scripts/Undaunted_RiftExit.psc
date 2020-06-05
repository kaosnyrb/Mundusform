Scriptname Undaunted_RiftExit extends ObjectReference
import Undaunted_SystemScript

ObjectReference Property exitRed  Auto  

event onActivate(objectReference akActivator)
    akActivator.MoveTo(exitRed)
    SetScriptedDoorsComplete()
endevent