Scriptname Mundusform_ExportNavMesh  extends activemagiceffect  
import Mundusform_SystemScript

Event OnEffectStart(Actor Target, Actor Caster)
    
    ExportNavMesh()
    Debug.Notification("Exported Navmesh.")
EndEvent
