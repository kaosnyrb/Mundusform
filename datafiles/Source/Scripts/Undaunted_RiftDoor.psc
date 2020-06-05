Scriptname Undaunted_RiftDoor extends ObjectReference
import Undaunted_SystemScript

ObjectReference Property TargetRef  Auto  
ObjectReference Property Holdingroom  Auto  

ObjectReference Property exitRed  Auto  

ObjectReference[] refs

event onActivate(objectReference akActivator)
    exitRed.MoveTo(akActivator)
    TargetRef = GetRandomRiftStartMarker()
    akActivator.MoveTo(TargetRef)
    SpawnMonsterInCell(236796)
    if (false)
    ;exitRed.MoveTo(akActivator)
    akActivator.MoveTo(Holdingroom)
    ;akActivator.MoveTo(TargetRef)

    refs = GetRiftReferences()
    int refslength = refs.Length
	while(refslength > 0)
        refslength -= 1
        refs[refslength].DisableNoWait(false)
        refs[refslength].Delete()
    endwhile
    refs = SpawnRift(1,TargetRef)
    float[] rotations = GetRiftRotations()
    ;Game.DisablePlayerControls()
	refslength = refs.Length
	while(refslength > 0)
        refslength -= 1
        refs[refslength].SetAngle(rotations[(refslength * 3)],rotations[(refslength * 3)+1],rotations[(refslength * 3)+2])
    endwhile
    ;Game.EnablePlayerControls()
    akActivator.MoveTo(exitRed)
    akActivator.MoveTo(TargetRef)
    Debug.Notification("Rift Complete")
    endif
endEvent

Function DeleteRefs()
	int refslength = refs.Length
	while(refslength > 0)
        refslength -= 1
        refs[refslength].DisableNoWait(false)
        refs[refslength].Delete()
    endwhile
endfunction

