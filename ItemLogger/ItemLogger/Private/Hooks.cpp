#include "Hooks.h"
#include "Main.h"

namespace ItemLogger::Hooks
{
    DECLARE_HOOK(UPrimalInventoryComponent_DropItem, void, UPrimalInventoryComponent*, FItemNetInfo *, bool, FVector *, FRotator *, bool, bool, bool, bool);
    DECLARE_HOOK(UPrimalInventoryComponent_RemoveItem, bool, UPrimalInventoryComponent*, FItemNetID *, bool, bool, bool, bool);
    DECLARE_HOOK(UPrimalInventoryComponent_ServerCraftItem, void, UPrimalInventoryComponent*, FItemNetID *, AShooterPlayerController *);
    DECLARE_HOOK(UPrimalInventoryComponent_RemoveArkTributeItem, bool, UPrimalInventoryComponent*, FItemNetID *, unsigned int);
    DECLARE_HOOK(UPrimalInventoryComponent_OnArkTributeItemsAdded, void, UPrimalInventoryComponent*, bool, TArray<FItemNetInfo> *);
    DECLARE_HOOK(UPrimalInventoryComponent_OnArkTributeItemsRemoved, void, UPrimalInventoryComponent*, bool, TArray<FItemNetInfo> *, TArray<FItemNetInfo> *, int, FString *, bool);
    DECLARE_HOOK(UPrimalItem_AddToInventory, void, UPrimalItem*, UPrimalInventoryComponent *, bool, bool, FItemNetID *, bool, bool, bool);
    DECLARE_HOOK(UPrimalItem_RemoveItemFromArkTributeInventory, bool, UPrimalItem*);
    DECLARE_HOOK(UPrimalItem_RemoveItemFromInventory, bool, UPrimalItem*, bool, bool);
    DECLARE_HOOK(AShooterGameMode_SaveWorld, void, AShooterGameMode *);

    void Hook_UPrimalInventoryComponent_DropItem(UPrimalInventoryComponent* _this, FItemNetInfo* theInfo, bool bOverrideSpawnTransform, FVector* LocationOverride, FRotator* RotationOverride, bool bPreventDropImpulse, bool bThrow, bool bSecondryAction, bool bSetItemDropLocation)
    {
        Log::GetLog()->info("UPrimalInventoryComponent_DropItem");
        UPrimalInventoryComponent_DropItem_original(_this, theInfo, bOverrideSpawnTransform, LocationOverride, RotationOverride, bPreventDropImpulse, bThrow, bSecondryAction, bSetItemDropLocation);
    }

    bool Hook_UPrimalInventoryComponent_RemoveItem(UPrimalInventoryComponent* _this, FItemNetID* itemID, bool bDoDrop, bool bSecondryAction, bool bForceRemoval, bool showHUDMessage)
    {
        return UPrimalInventoryComponent_RemoveItem_original(_this, itemID, bDoDrop, bSecondryAction, bForceRemoval, showHUDMessage);
    }

    void Hook_UPrimalInventoryComponent_ServerCraftItem(UPrimalInventoryComponent* _this, FItemNetID* itemID, AShooterPlayerController* ByPC)
    {
        Log::GetLog()->info("UPrimalInventoryComponent_ServerCraftItem");
        UPrimalInventoryComponent_ServerCraftItem_original(_this, itemID, ByPC);
    }

    bool Hook_UPrimalInventoryComponent_RemoveArkTributeItem(UPrimalInventoryComponent* _this, FItemNetID* itemID, unsigned int Quantity)
    {
        Log::GetLog()->info("UPrimalInventoryComponent_RemoveArkTributeItem");
        return UPrimalInventoryComponent_RemoveArkTributeItem_original(_this, itemID, Quantity);
    }

    void Hook_UPrimalInventoryComponent_OnArkTributeItemsAdded(UPrimalInventoryComponent* _this, bool Success, TArray<FItemNetInfo>* AddedItems)
    {
        Log::GetLog()->info("UPrimalInventoryComponent_OnArkTributeItemsAdded");
        UPrimalInventoryComponent_OnArkTributeItemsAdded_original(_this, Success, AddedItems);
    }

    void Hook_UPrimalInventoryComponent_OnArkTributeItemsRemoved(UPrimalInventoryComponent* _this, bool Success, TArray<FItemNetInfo>* RemovedItems, TArray<FItemNetInfo>* NotFoundItems, int FailureResponseCode, FString* FailureResponseMessage, bool bAllowForcedItemDownload)
    {
        Log::GetLog()->info("UPrimalInventoryComponent_OnArkTributeItemsRemoved");
        UPrimalInventoryComponent_OnArkTributeItemsRemoved_original(_this, Success, RemovedItems, NotFoundItems, FailureResponseCode, FailureResponseMessage, bAllowForcedItemDownload);
    }

    void Hook_UPrimalItem_AddToInventory(UPrimalItem* _this, UPrimalInventoryComponent* toInventory, bool bEquipItem, bool AddToSlotItems, FItemNetID* InventoryInsertAfterItemID, bool ShowHUDNotification, bool bDontRecalcSpoilingTime, bool bIgnoreAbsoluteMaxInventory)
    {
        AShooterPlayerController* toOwner = toInventory->GetOwnerController();
        if (toOwner && !_this->bIsEngram().Get())
        {
            std::string sourcePlayerName = "";
            uint64 sourceSteamId = 0;
            AShooterCharacter* sp = _this->LastOwnerPlayerField().Get();
            if (sp)
            {
                FString sourceName;
                sp->ClassField()->GetDefaultObject(true)->GetFullName(&sourceName, nullptr);
                sourcePlayerName = sp->PlayerNameField().ToString();
                /*APlayerController* sourceController = sp->GetOwnerController();
                if (sourceController) {
                sourceSteamId = ArkApi::IApiUtils::GetSteamIdFromController(sourceController);
                }*/
            }

            const uint64 destSteamId = ArkApi::IApiUtils::GetSteamIdFromController(toOwner);

            FString full_name;
            _this->ClassField()->GetDefaultObject(true)->GetFullName(&full_name, nullptr);

            FString newOwnerName;
            toOwner->GetPlayerCharacterName(&newOwnerName);

            FString itemName;
            _this->GetItemName(&itemName, false, false, nullptr);

            FString type;
            _this->GetItemTypeString(&type);

            const bool res = database->SaveItemLog(Add, sourceSteamId, sourcePlayerName, destSteamId, newOwnerName.ToString(), _this->ItemIDField().ItemID1, _this->ItemIDField().ItemID2, type.ToString(), _this->ItemQuantityField(), (int)_this->ItemQualityIndexField(), _this->bIsBlueprint().Get(), itemName.ToString(), full_name.ToString());
        }

        UPrimalItem_AddToInventory_original(_this, toInventory, bEquipItem, AddToSlotItems, InventoryInsertAfterItemID, ShowHUDNotification, bDontRecalcSpoilingTime, bIgnoreAbsoluteMaxInventory);
    }

    bool Hook_UPrimalItem_RemoveItemFromArkTributeInventory(UPrimalItem* _this)
    {
        Log::GetLog()->info("UPrimalItem_RemoveItemFromArkTributeInventory");
        return UPrimalItem_RemoveItemFromArkTributeInventory_original(_this);
    }

    bool Hook_UPrimalItem_RemoveItemFromInventory(UPrimalItem* _this, bool bForceRemoval, bool showHUDMessage)
    {
        return UPrimalItem_RemoveItemFromInventory_original(_this, bForceRemoval, showHUDMessage);
    }

    void Hook_AShooterGameMode_SaveWorld(AShooterGameMode* aShooterGameMode)
    {
        Log::GetLog()->info("World Saving!");
        AShooterGameMode_SaveWorld_original(aShooterGameMode);
    }

    void Init()
    {
        ArkApi::GetHooks().SetHook("UPrimalInventoryComponent.DropItem",
            &Hook_UPrimalInventoryComponent_DropItem,
            &UPrimalInventoryComponent_DropItem_original);

        ArkApi::GetHooks().SetHook("UPrimalInventoryComponent.RemoveItem",
            &Hook_UPrimalInventoryComponent_RemoveItem,
            &UPrimalInventoryComponent_RemoveItem_original);

        ArkApi::GetHooks().SetHook("UPrimalInventoryComponent.ServerCraftItem",
            &Hook_UPrimalInventoryComponent_ServerCraftItem,
            &UPrimalInventoryComponent_ServerCraftItem_original);

        ArkApi::GetHooks().SetHook("UPrimalInventoryComponent.RemoveArkTributeItem",
            &Hook_UPrimalInventoryComponent_RemoveArkTributeItem,
            &UPrimalInventoryComponent_RemoveArkTributeItem_original);

        ArkApi::GetHooks().SetHook("UPrimalInventoryComponent.OnArkTributeItemsAdded",
            &Hook_UPrimalInventoryComponent_OnArkTributeItemsAdded,
            &UPrimalInventoryComponent_OnArkTributeItemsAdded_original);

        ArkApi::GetHooks().SetHook("UPrimalInventoryComponent.OnArkTributeItemsRemoved",
            &Hook_UPrimalInventoryComponent_OnArkTributeItemsRemoved,
            &UPrimalInventoryComponent_OnArkTributeItemsRemoved_original);

        ArkApi::GetHooks().SetHook("UPrimalItem.AddToInventory",
            &Hook_UPrimalItem_AddToInventory,
            &UPrimalItem_AddToInventory_original);

        ArkApi::GetHooks().SetHook("UPrimalItem.RemoveItemFromArkTributeInventory",
            &Hook_UPrimalItem_RemoveItemFromArkTributeInventory,
            &UPrimalItem_RemoveItemFromArkTributeInventory_original);

        ArkApi::GetHooks().SetHook("UPrimalItem.RemoveItemFromInventory",
            &Hook_UPrimalItem_RemoveItemFromInventory,
            &UPrimalItem_RemoveItemFromInventory_original);

        ArkApi::GetHooks().SetHook("AShooterGameMode.SaveWorld",
            &Hook_AShooterGameMode_SaveWorld,
            &AShooterGameMode_SaveWorld_original);
    }

    void Unload()
    {
        ArkApi::GetHooks().DisableHook("UPrimalInventoryComponent.DropItem", &Hook_UPrimalInventoryComponent_DropItem);
        ArkApi::GetHooks().DisableHook("UPrimalInventoryComponent.RemoveItem", &Hook_UPrimalInventoryComponent_RemoveItem);
        ArkApi::GetHooks().DisableHook("UPrimalInventoryComponent.ServerCraftItem", &Hook_UPrimalInventoryComponent_ServerCraftItem);
        ArkApi::GetHooks().DisableHook("UPrimalInventoryComponent.RemoveArkTributeItem", &Hook_UPrimalInventoryComponent_RemoveArkTributeItem);
        ArkApi::GetHooks().DisableHook("UPrimalInventoryComponent.OnArkTributeItemsAdded", &Hook_UPrimalInventoryComponent_OnArkTributeItemsAdded);
        ArkApi::GetHooks().DisableHook("UPrimalInventoryComponent.OnArkTributeItemsRemoved", &Hook_UPrimalInventoryComponent_OnArkTributeItemsRemoved);
        ArkApi::GetHooks().DisableHook("UPrimalItem.AddToInventory", &Hook_UPrimalItem_AddToInventory);
        ArkApi::GetHooks().DisableHook("UPrimalItem.RemoveItemFromArkTributeInventory", &Hook_UPrimalItem_RemoveItemFromArkTributeInventory);
        ArkApi::GetHooks().DisableHook("UPrimalItem.RemoveItemFromInventory", &Hook_UPrimalItem_RemoveItemFromInventory);
        ArkApi::GetHooks().DisableHook("AShooterGameMode.SaveWorld", &Hook_AShooterGameMode_SaveWorld);
    }
}