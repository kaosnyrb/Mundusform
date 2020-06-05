Scriptname Undaunted_DestroyObjective extends ObjectReference  
import Undaunted_SystemScript

Int Property OldStage = 3 Auto
{The destruction stage that is before the stage you want to trigger at
- Default = 3}

Int Property NewStage = 4 Auto
{The destruction stage that you want the trigger to happen at
- Default = 4}

ObjectReference CollisionLink 

Event OnLoad()
	if GetCurrentDestructionStage() < NewStage
        CollisionLink = GetLinkedRef()
        if (CollisionLink)
            CollisionLink.Enable()
        endif
	EndIf
EndEvent

Event OnDestructionStageChanged(int aiOldStage, int aiCurrentStage)
    if GetCurrentDestructionStage() == 4
        if (isSystemReady())
            SetGroupMemberComplete(self as objectReference)
            Disable(true)
            Delete()
        endif
    endif
EndEvent

;Event OnHit(ObjectReference akAggressor, Form akSource, Projectile akProjectile, bool abPowerAttack, bool abSneakAttack, bool abBashAttack, bool abHitBlocked)
;    if (isSystemReady())
;        SetGroupMemberComplete(self as objectReference)
;        Disable(true)
;        Delete()
;    endif
;EndEvent