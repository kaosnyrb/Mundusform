Scriptname Mundusform_CaptureAreaEffect extends activemagiceffect  
import Mundusform_SystemScript

Event OnEffectStart(Actor Target, Actor Caster)
    CaptureArea()
    Debug.Notification("Cell saved to UndauntedRift.pas")
EndEvent