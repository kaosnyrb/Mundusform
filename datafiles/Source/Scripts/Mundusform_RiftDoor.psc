Scriptname Mundusform_RiftDoor extends ObjectReference
import Mundusform_SystemScript

ObjectReference Property TargetRef  Auto  
ObjectReference[] refs

event onActivate(objectReference akActivator)
    akActivator.MoveTo(TargetRef)
    refs = SpawnRift(TargetRef)
    float[] rotations = GetRiftRotations()

    int refslength = refs.Length
    while(refslength > 0)
        Debug.Notification("Rotating")
        refslength -= 1
        refs[refslength].SetAngle(rotations[(refslength * 3)],rotations[(refslength * 3)+1],rotations[(refslength * 3)+2])
    endwhile

    Debug.Notification("Rift Complete")
endEvent