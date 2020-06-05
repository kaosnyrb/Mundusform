Scriptname Undaunted_NavTileCapture extends activemagiceffect  
import Undaunted_SystemScript

spell property SpellToCast auto
Event OnEffectStart(Actor Target, Actor Caster)
    Debug.Notification("Undaunted_NavTileCapture Start")
EndEvent

Event OnEffectFinish(Actor akTarget, Actor akCaster)
    Debug.Notification("Undaunted_NavTileCapture OnEffectFinish")
    CaptureNavTile()
    SpellToCast.Cast(akCaster, akTarget)
endEvent