Scriptname Mundusform_ExportNavMesh  extends activemagiceffect  
import Mundusform_SystemScript

Event OnEffectStart(Actor Target, Actor Caster)
    Debug.Notification("Exporting Navmesh...")
    ExportNavMesh()
EndEvent
