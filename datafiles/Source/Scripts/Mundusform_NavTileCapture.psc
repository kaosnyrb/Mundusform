Scriptname Mundusform_NavTileCapture extends activemagiceffect  
import Mundusform_SystemScript

spell property SpellToCast auto
Event OnEffectStart(Actor Target, Actor Caster)
    Debug.Notification("NavTileCapture...")
    ;Yeah this is infinite but you shouldn't be using this in a real game...
    while(true)
        CaptureNavTile()
        Utility.Wait(0.1)
    endwhile
EndEvent