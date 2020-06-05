Scriptname Undaunted_ActivatorObjective extends ObjectReference  
import Undaunted_SystemScript

event onActivate(objectReference akActivator)
    bool isready = false;
	while (!isready)
		if (isSystemReady() == 2)
			isready = true
		else
			Utility.Wait(5.0)
		endif		
	endwhile
    if (isSystemReady())
        SetGroupMemberComplete(self as objectReference)
        Disable(true)
        Delete()
    endif
endevent