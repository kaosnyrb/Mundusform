Scriptname Undaunted_MCM extends SKI_ConfigBase

import Undaunted_SystemScript

GlobalVariable Property BountiesComplete  Auto  


; Controls
int			_NumberOfBountiesPerChainOID_S
int			_NumberOfBountiesPerChain	= 3

int			BountyStartDistanceOID_S
int			BountyStartDistance	= 5000

int			BountyMinSpawnDistanceOID_S
int			BountyMinSpawnDistance	= 6000

int			BountyMaxSpawnDistanceOID_S
int			BountyMaxSpawnDistance	= 15000

int			BountyEnemyExteriorSpawnRadiusOID_S
int			BountyEnemyExteriorSpawnRadius	= 2000

int			BountyEnemyPlacementHeightDistanceOID_S
int			BountyEnemyPlacementHeightDistance	= 300

int			AllowChainResetingOID_S
bool		AllowChainReseting	= false

int			RewardsPerKeyOID_S
int			RewardsPerKey	= 3

int			RewardWeaponWeightOID_S
int			RewardWeaponWeight	= 10
int			RewardArmourWeightOID_S
int			RewardArmourWeight	= 10
int			RewardPotionWeightOID_S
int			RewardPotionWeight	= 5
int			RewardScrollWeightOID_S
int			RewardScrollWeight	= 5
int			RewardIngredientWeightOID_S
int			RewardIngredientWeight = 5
int			RewardBookWeightOID_S
int			RewardBookWeight = 5
int			RewardMiscWeightOID_S
int			RewardMiscWeight = 5

int			RewardAllowMiscItemsOID_S
bool		RewardAllowMiscItems	= false
int			RewardAllowClothesOID_S
bool		RewardAllowClothes	= false
int			RewardAllowShieldsOID_S
bool		RewardAllowShields	= false
int			RewardAllowDaedricArtifactsOID_S
bool		RewardAllowDaedricArtifacts	= false


function SetValues()
	SetConfigValue("NumberOfBountiesPerChain", _NumberOfBountiesPerChain)
	SetConfigValue("BountyStartDistance", BountyStartDistance)
	SetConfigValue("BountyMinSpawnDistance", BountyMinSpawnDistance)
	SetConfigValue("BountyMaxSpawnDistance", BountyMaxSpawnDistance)
	SetConfigValue("BountyEnemyExteriorSpawnRadius", BountyEnemyExteriorSpawnRadius)
	SetConfigValue("BountyEnemyPlacementHeightDistance", BountyEnemyPlacementHeightDistance)
	SetConfigValue("BountyEnemyPlacementHeightDistance", BountyEnemyPlacementHeightDistance)
	if (AllowChainReseting)
		SetConfigValue("AllowChainReseting", 1)
	else
		SetConfigValue("AllowChainReseting", 0)
	endIf
	SetConfigValue("RewardsPerKey", RewardsPerKey)
	SetConfigValue("RewardWeaponWeight", RewardWeaponWeight)
	SetConfigValue("RewardArmourWeight", RewardArmourWeight)
	SetConfigValue("RewardPotionWeight", RewardPotionWeight)
	SetConfigValue("RewardScrollWeight", RewardScrollWeight)
	SetConfigValue("RewardIngredientWeight", RewardIngredientWeight)
	SetConfigValue("RewardBookWeight", RewardBookWeight)
	SetConfigValue("RewardMiscWeight", RewardMiscWeight)
	if (RewardAllowMiscItems)
		SetConfigValue("RewardAllowMiscItems", 1)
	else
		SetConfigValue("RewardAllowMiscItems", 0)
	endIf
	if (RewardAllowClothes)
		SetConfigValue("RewardAllowClothes", 1)
	else
		SetConfigValue("RewardAllowClothes", 0)
	endIf
	if (RewardAllowShields)
		SetConfigValue("RewardAllowShields", 1)
	else
		SetConfigValue("RewardAllowShields", 0)
	endIf
	if (RewardAllowDaedricArtifacts)
		SetConfigValue("RewardAllowDaedricArtifacts", 1)
	else
		SetConfigValue("RewardAllowDaedricArtifacts", 0)
	endIf
endfunction

event OnConfigInit()
    SetValues()
endEvent

event OnGameReload()
	parent.OnGameReload() ; Don't forget to call the parent!
	SetValues()
endEvent

; -------------------------------------------------------------------------------------------------
; Pages
event OnPageReset(string page)
    if (page == "")
        AddHeaderOption("BountiesComplete: " + BountiesComplete.Value as int)
    elseIf (page == "Bounty Settings")
		AddHeaderOption("Bounty Settings")
		AddEmptyOption()
        _NumberOfBountiesPerChainOID_S		= AddSliderOption("Number Of Bounties Per Chain: ", _NumberOfBountiesPerChain)
        BountyStartDistanceOID_S		= AddSliderOption("Bounty Start Distance: ", BountyStartDistance)
        BountyMinSpawnDistanceOID_S		= AddSliderOption("Bounty Min Spawn Distance: ", BountyMinSpawnDistance)
        BountyMaxSpawnDistanceOID_S		= AddSliderOption("Bounty Max Spawn Distance: ", BountyMaxSpawnDistance)
        BountyEnemyExteriorSpawnRadiusOID_S		= AddSliderOption("Enemy Exterior Spawn Radius: ", BountyEnemyExteriorSpawnRadius)
		BountyEnemyPlacementHeightDistanceOID_S		= AddSliderOption("Enemy Placement Height: ", BountyEnemyPlacementHeightDistance)
		AddHeaderOption("Debug Settings")
		AddEmptyOption()
		AllowChainResetingOID_S = AddToggleOption("Allow Resetting Bounties: ", AllowChainReseting)
    elseIf (page == "Reward Settings")
		AddHeaderOption("General Reward Settings")
		AddEmptyOption()
		RewardsPerKeyOID_S		= AddSliderOption("Number of rewards per key: ", RewardsPerKey)
		AddEmptyOption()
		AddHeaderOption("Reward Weighting")
		AddEmptyOption()
        RewardWeaponWeightOID_S		= AddSliderOption("Weapon Chance: ", RewardWeaponWeight)
		RewardArmourWeightOID_S		= AddSliderOption("Armour Chance: ", RewardArmourWeight)
		RewardPotionWeightOID_S		= AddSliderOption("Potion Chance: ", RewardPotionWeight)
		RewardScrollWeightOID_S		= AddSliderOption("Scroll Chance: ", RewardScrollWeight)
		RewardIngredientWeightOID_S	= AddSliderOption("Ingredient Chance: ", RewardIngredientWeight)
		RewardBookWeightOID_S	= AddSliderOption("Book Chance: ", RewardBookWeight)
		RewardMiscWeightOID_S	= AddSliderOption("Misc Chance: ", RewardMiscWeight)
		AddEmptyOption()
		AddHeaderOption("Reward Filtering")
		AddEmptyOption()
		RewardAllowMiscItemsOID_S = AddToggleOption("Allow Misc items as rewards ", RewardAllowMiscItems)
		RewardAllowClothesOID_S = AddToggleOption("Allow Clothing as rewards ", RewardAllowClothes)
		RewardAllowShieldsOID_S = AddToggleOption("Allow Shields as rewards ", RewardAllowShields)
		RewardAllowDaedricArtifactsOID_S = AddToggleOption("Allow Daedric Artifacts as rewards", RewardAllowDaedricArtifacts)
    endIf
endEvent

; -------------------------------------------------------------------------------------------------
; @implements SKI_ConfigBase
event OnOptionSliderOpen(int a_option)

	; -------------------------------------------------------
	if (a_option == _NumberOfBountiesPerChainOID_S)
		SetSliderDialogStartValue(_NumberOfBountiesPerChain)
		SetSliderDialogDefaultValue(GetConfigValueInt("NumberOfBountiesPerChain"))
		SetSliderDialogRange(1, 100)
		SetSliderDialogInterval(1)

	ElseIf (a_option == BountyStartDistanceOID_S)
		SetSliderDialogStartValue(BountyStartDistance)
		SetSliderDialogDefaultValue(GetConfigValueInt("BountyStartDistance"))
		SetSliderDialogRange(0, 100000)
		SetSliderDialogInterval(1000)

	ElseIf (a_option == BountyMinSpawnDistanceOID_S)
		SetSliderDialogStartValue(BountyMinSpawnDistance)
		SetSliderDialogDefaultValue(GetConfigValueInt("BountyMinSpawnDistance"))
		SetSliderDialogRange(0, 100000)
		SetSliderDialogInterval(1000)
	ElseIf (a_option == BountyMaxSpawnDistanceOID_S)
		SetSliderDialogStartValue(BountyMaxSpawnDistance)
		SetSliderDialogDefaultValue(GetConfigValueInt("BountyMaxSpawnDistance"))
		SetSliderDialogRange(0, 100000)
		SetSliderDialogInterval(1000)

	ElseIf (a_option == BountyEnemyExteriorSpawnRadiusOID_S)
		SetSliderDialogStartValue(BountyEnemyExteriorSpawnRadius)
		SetSliderDialogDefaultValue(GetConfigValueInt("BountyEnemyExteriorSpawnRadius"))
		SetSliderDialogRange(0, 10000)
		SetSliderDialogInterval(100)

	ElseIf (a_option == BountyEnemyPlacementHeightDistanceOID_S)
		SetSliderDialogStartValue(BountyEnemyPlacementHeightDistance)
		SetSliderDialogDefaultValue(GetConfigValueInt("BountyEnemyPlacementHeightDistance"))
		SetSliderDialogRange(200, 5000)
		SetSliderDialogInterval(100)

	ElseIf (a_option == RewardsPerKeyOID_S)
		SetSliderDialogStartValue(RewardsPerKey)
		SetSliderDialogDefaultValue(GetConfigValueInt("RewardsPerKey"))
		SetSliderDialogRange(1, 100)
		SetSliderDialogInterval(1)

	ElseIf (a_option == RewardWeaponWeightOID_S)
		SetSliderDialogStartValue(RewardWeaponWeight)
		SetSliderDialogDefaultValue(GetConfigValueInt("RewardWeaponWeight"))
		SetSliderDialogRange(0, 100)
		SetSliderDialogInterval(1)
	ElseIf (a_option == RewardArmourWeightOID_S)
		SetSliderDialogStartValue(RewardArmourWeight)
		SetSliderDialogDefaultValue(GetConfigValueInt("RewardArmourWeight"))
		SetSliderDialogRange(0, 100)
		SetSliderDialogInterval(1)
	ElseIf (a_option == RewardPotionWeightOID_S)
		SetSliderDialogStartValue(RewardPotionWeight)
		SetSliderDialogDefaultValue(GetConfigValueInt("RewardPotionWeight"))
		SetSliderDialogRange(0, 100)
		SetSliderDialogInterval(1)
	ElseIf (a_option == RewardScrollWeightOID_S)
		SetSliderDialogStartValue(RewardScrollWeight)
		SetSliderDialogDefaultValue(GetConfigValueInt("RewardScrollWeight"))
		SetSliderDialogRange(0, 100)
		SetSliderDialogInterval(1)
	ElseIf (a_option == RewardIngredientWeightOID_S)
		SetSliderDialogStartValue(RewardIngredientWeight)
		SetSliderDialogDefaultValue(GetConfigValueInt("RewardIngredientWeight"))
		SetSliderDialogRange(0, 100)
		SetSliderDialogInterval(1)
	ElseIf (a_option == RewardBookWeightOID_S)
		SetSliderDialogStartValue(RewardBookWeight)
		SetSliderDialogDefaultValue(GetConfigValueInt("RewardBookWeight"))
		SetSliderDialogRange(0, 100)
		SetSliderDialogInterval(1)
	ElseIf (a_option == RewardMiscWeightOID_S)
		SetSliderDialogStartValue(RewardMiscWeight)
		SetSliderDialogDefaultValue(GetConfigValueInt("RewardMiscWeight"))
		SetSliderDialogRange(0, 100)
		SetSliderDialogInterval(1)
								
	endIf
endEvent

event OnOptionSliderAccept(int a_option, float a_value)

	; -------------------------------------------------------
	if (a_option == _NumberOfBountiesPerChainOID_S)
		_NumberOfBountiesPerChain = a_value as int
        SetSliderOptionValue(a_option, _NumberOfBountiesPerChain)
        SetConfigValue("NumberOfBountiesPerChain", _NumberOfBountiesPerChain)
	
	ElseIf (a_option == BountyStartDistanceOID_S)
		BountyStartDistance = a_value as int
        SetSliderOptionValue(a_option, BountyStartDistance)
		SetConfigValue("BountyStartDistance", BountyStartDistance)
		
	ElseIf (a_option == BountyMinSpawnDistanceOID_S)
		BountyMinSpawnDistance = a_value as int
        SetSliderOptionValue(a_option, BountyMinSpawnDistance)
		SetConfigValue("BountyMinSpawnDistance", BountyMinSpawnDistance)
		
	ElseIf (a_option == BountyMaxSpawnDistanceOID_S)
		BountyMaxSpawnDistance = a_value as int
        SetSliderOptionValue(a_option, BountyMaxSpawnDistance)
        SetConfigValue("BountyMaxSpawnDistance", BountyMaxSpawnDistance)
	
	ElseIf (a_option == BountyEnemyExteriorSpawnRadiusOID_S)
		BountyEnemyExteriorSpawnRadius = a_value as int
        SetSliderOptionValue(a_option, BountyEnemyExteriorSpawnRadius)
        SetConfigValue("BountyEnemyExteriorSpawnRadius", BountyEnemyExteriorSpawnRadius)
	
	ElseIf (a_option == BountyEnemyPlacementHeightDistanceOID_S)
		BountyEnemyPlacementHeightDistance = a_value as int
        SetSliderOptionValue(a_option, BountyEnemyPlacementHeightDistance)
        SetConfigValue("BountyEnemyPlacementHeightDistance", BountyEnemyPlacementHeightDistance)

	ElseIf (a_option == RewardsPerKeyOID_S)
		RewardsPerKey = a_value as int
        SetSliderOptionValue(a_option, RewardsPerKey)
		SetConfigValue("RewardsPerKey", RewardsPerKey)
		
	ElseIf (a_option == RewardWeaponWeightOID_S)
		RewardWeaponWeight = a_value as int
        SetSliderOptionValue(a_option, RewardWeaponWeight)
        SetConfigValue("RewardWeaponWeight", RewardWeaponWeight)

	ElseIf (a_option == RewardArmourWeightOID_S)
		RewardArmourWeight = a_value as int
        SetSliderOptionValue(a_option, RewardArmourWeight)
        SetConfigValue("RewardArmourWeight", RewardArmourWeight)

	ElseIf (a_option == RewardPotionWeightOID_S)
		RewardPotionWeight = a_value as int
        SetSliderOptionValue(a_option, RewardPotionWeight)
        SetConfigValue("RewardPotionWeight", RewardPotionWeight)
	ElseIf (a_option == RewardScrollWeightOID_S)
		RewardScrollWeight = a_value as int
        SetSliderOptionValue(a_option, RewardScrollWeight)
        SetConfigValue("RewardScrollWeight", RewardScrollWeight)
	ElseIf (a_option == RewardIngredientWeightOID_S)
		RewardIngredientWeight = a_value as int
        SetSliderOptionValue(a_option, RewardIngredientWeight)
        SetConfigValue("RewardIngredientWeight", RewardIngredientWeight)
	ElseIf (a_option == RewardBookWeightOID_S)
		RewardBookWeight = a_value as int
        SetSliderOptionValue(a_option, RewardBookWeight)
        SetConfigValue("RewardBookWeight", RewardBookWeight)
	ElseIf (a_option == RewardMiscWeightOID_S)
		RewardMiscWeight = a_value as int
        SetSliderOptionValue(a_option, RewardMiscWeight)
        SetConfigValue("RewardMiscWeight", RewardMiscWeight)

	endif
endEvent

; -----------------
event OnOptionHighlight(int a_option)

	if (a_option == _NumberOfBountiesPerChainOID_S)
		SetInfoText("How many bounties are spawned before a key is rewarded")
	elseIf(a_option == BountyStartDistanceOID_S)
		SetInfoText("The maximum distance between the player and the bounty before it starts")
	elseIf(a_option == BountyMinSpawnDistanceOID_S)
		SetInfoText("The minimum distance at which a bounty will be allowed to be placed at")
	elseIf(a_option == BountyMaxSpawnDistanceOID_S)
		SetInfoText("The maximum distance at which a bounty will be allowed to be placed at")
	elseIf(a_option == BountyEnemyExteriorSpawnRadiusOID_S)
		SetInfoText("The radius around the bounty at which enemies will spawn")
	elseIf(a_option == BountyEnemyPlacementHeightDistanceOID_S)
		SetInfoText("The maximum height distance enemys are allowed to spawn.")
	elseIf(a_option == AllowChainResetingOID_S)
		SetInfoText("Allows use of the power while a bounty is running. Resets progress.")
	elseIf(a_option == RewardsPerKeyOID_S)
		SetInfoText("The number of items that you get per key")
	elseIf(a_option == RewardWeaponWeightOID_S)
		SetInfoText("The chance of getting a weapon")
	elseIf(a_option == RewardArmourWeightOID_S)
		SetInfoText("The chance of getting armour")
	elseIf(a_option == RewardPotionWeightOID_S)
		SetInfoText("The chance of getting a potion")
	elseIf(a_option == RewardScrollWeightOID_S)
		SetInfoText("The chance of getting a scroll")
	elseIf(a_option == RewardIngredientWeightOID_S)
		SetInfoText("The chance of getting an ingredient")
	elseIf(a_option == RewardBookWeightOID_S)
		SetInfoText("The chance of getting a book")
	elseIf(a_option == RewardMiscWeightOID_S)
		SetInfoText("The chance of getting a miscellaneous item")
	endIf
endEvent


event OnOptionSelect(int a_option)
	if (a_option == AllowChainResetingOID_S)
		AllowChainReseting = !AllowChainReseting
		SetToggleOptionValue(a_option, AllowChainReseting)
		if (AllowChainReseting)
			SetConfigValue("AllowChainReseting", 1)
		else
			SetConfigValue("AllowChainReseting", 0)
		endIf
	endif

	if (a_option == RewardAllowMiscItemsOID_S)
		RewardAllowMiscItems = !RewardAllowMiscItems
		SetToggleOptionValue(a_option, RewardAllowMiscItems)
		if (RewardAllowMiscItems)
			SetConfigValue("RewardAllowMiscItems", 1)
		else
			SetConfigValue("RewardAllowMiscItems", 0)
		endIf
	endif
	if (a_option == RewardAllowClothesOID_S)
		RewardAllowClothes = !RewardAllowClothes
		SetToggleOptionValue(a_option, RewardAllowClothes)
		if (RewardAllowClothes)
			SetConfigValue("RewardAllowClothes", 1)
		else
			SetConfigValue("RewardAllowClothes", 0)
		endIf
	endif
	if (a_option == RewardAllowShieldsOID_S)
		RewardAllowShields = !RewardAllowShields
		SetToggleOptionValue(a_option, RewardAllowShields)
		if (RewardAllowShields)
			SetConfigValue("RewardAllowShields", 1)
		else
			SetConfigValue("RewardAllowShields", 0)
		endIf
	endif
	if (a_option == RewardAllowDaedricArtifactsOID_S)
		RewardAllowDaedricArtifacts = !RewardAllowDaedricArtifacts
		SetToggleOptionValue(a_option, RewardAllowDaedricArtifacts)
		if (RewardAllowDaedricArtifacts)
			SetConfigValue("RewardAllowDaedricArtifacts", 1)
		else
			SetConfigValue("RewardAllowDaedricArtifacts", 0)
		endIf
	endif
endEvent
