# InteractSystem

**Version:** 1.0.0  
**Category:** Gameplay Framework  
**Author:** JiRath  
**License:** MIT  

---

## Overview

InteractSystem is a comprehensive user interaction orchestration plugin for Unreal Engine that provides end-to-end interaction management across web, mobile, and API touchpoints. The system handles event routing, session management, cross-plugin interaction triggering, and third-party service integration.

### Core Purpose

InteractSystem serves as the central interaction hub for the modular plugin ecosystem, enabling seamless communication between players, game objects, and external services through a unified component-based architecture.

### Key Capabilities

| Capability | Description |
|------------|-------------|
| **Interaction Management** | Component-based system for defining interactable objects and player interaction logic |
| **Inventory System** | Full-featured inventory with stacking, item attributes, and network replication |
| **Equipment System** | Item equipping, unequipping, and usage with visual feedback |
| **Physics Interaction** | Grab, push, and throw physics objects with configurable parameters |
| **Trigger System** | Chain interactions between multiple interactable objects |
| **Toggleable Interface** | On/off state management for interactive objects |
| **Session Management** | Player interaction state tracking and session persistence |

### Common Use Cases

1. **Item Collection:** Players pick up items that are added to their inventory
2. **Door Interaction:** Doors that can be opened/closed and trigger other events
3. **Physics Puzzles:** Grabbing and moving objects to solve environmental puzzles
4. **Item Usage:** Using equipped items on interactable objects (e.g., keys on doors)
5. **Chain Reactions:** Triggering multiple objects in sequence (e.g., pressure plates opening doors)

### Role in Plugin Ecosystem

InteractSystem is a standalone plugin that provides core interaction functionality. It can be extended to integrate with other plugins in the ecosystem through custom implementation.

---

## Installation

### Supported Runtime Environments

| Environment | Minimum Version | Recommended Version |
|-------------|-----------------|---------------------|
| Unreal Engine | 5.0 | 5.3+ |
| Windows | 10 | 11 |

### Step-by-Step Setup

1. **Copy Plugin Files**
   ```
   Copy the InteractSystem folder to YourProject/Plugins/
   ```

2. **Regenerate Project Files**
    ```bash
    # Windows
    YourProject.uproject - GenerateProjectFiles.bat
    ```

3. **Build the Project**
   ```bash
   # Using Unreal Build Tool
   RunUAT.bat BuildCookRun -project=YourProject.uproject -noP4 -platform=Win64 -clientconfig=Development -serverconfig=Development
   ```

4. **Enable Plugin in Editor**
   - Open Project Settings → Plugins
   - Enable "InteractionSystem"
   - Restart the editor

### Dependencies

| Module | Version Constraint | Required |
|--------|-------------------|----------|
| Core | >= 5.0 | Yes |
| CoreUObject | >= 5.0 | Yes |
| Engine | >= 5.0 | Yes |
| InputCore | >= 5.0 | Yes |
| DeveloperSettings | >= 5.0 | Yes |
| NetCore | >= 5.0 | Yes |
| GameplayTags | >= 5.0 | Yes |
| Slate | >= 5.0 | No (Editor only) |
| SlateCore | >= 5.0 | No (Editor only) |

---

## Configuration

### Configurable Parameters

| Parameter | Data Type | Default Value | Valid Range | Environment Variable | Security Notes |
|-----------|-----------|---------------|-------------|---------------------|----------------|
| `InteractDistance` | float | 500.0 | 10.0 - 10000.0 cm | `INTERACT_DISTANCE` | None |
| `bAutoEquip` | bool | true | true / false | `INTERACT_AUTO_EQUIP` | None |
| `GrabWeightThreshold` | float | 25.0 | 0.1 - 1000.0 kg | `INTERACT_GRAB_WEIGHT` | None |
| `PushImpulse` | float | 100000.0 | 100.0 - 1000000.0 | `INTERACT_PUSH_IMPULSE` | None |
| `MaxInventorySlots` | int32 | 50 | 1 - 1000 | `INTERACT_MAX_SLOTS` | None |
| `bEnablePhysicsGrab` | bool | true | true / false | `INTERACT_PHYSICS_GRAB` | None |
| `TriggerDelay` | float | 0.0 | 0.0 - 60.0 seconds | `INTERACT_TRIGGER_DELAY` | None |

### Configuration File Example

```ini
[/Script/InteractionSystem.InteractableComponent]
InteractDistance=500.0
bAutoEquip=true

[/Script/InteractionSystem.InventoryComponent]
MaxInventorySlots=50
```

### Security Considerations

- **Network Replication:** All inventory and interaction state changes are server-authoritative
- **Input Validation:** Item data is validated before being added to inventory
- **Memory Safety:** Uses Unreal's garbage collection for all UObject references

---

## Usage

### Basic Interaction Setup

```cpp
// C++ - Adding interaction to an actor
#include "Interaction/InteractableComponent.h"

// In your actor class header
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
UInteractableComponent* InteractableComponent;

// In your actor constructor
InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
```

```cpp
// Blueprint - Event-driven interaction
// 1. Add InteractableComponent to your actor
// 2. Bind to OnInteract event
// 3. Implement custom logic in the event graph
```

### Inventory Management

```cpp
// C++ - Adding items to inventory
void AMyActor::AddItemToInventory(UItemInformation* ItemInfo, int32 Quantity)
{
    FItemData NewItem;
    NewItem.ItemInformation = ItemInfo;
    NewItem.Count = Quantity;
    
    FItemHandle Handle;
    if (InventoryComponent->AddToInventory(NewItem, Handle))
    {
        UE_LOG(LogTemp, Log, "Item added successfully");
    }
}

// C++ - Checking inventory contents
void AMyActor::PrintInventory()
{
    TArray<FInventoryContents> Items;
    InventoryComponent->GetInventory(Items);
    
    for (const FInventoryContents& Item : Items)
    {
        UE_LOG(LogTemp, Log, "Item: %s, Count: %d", 
            *Item.GetItemInformation()->DisplayName.ToString(),
            Item.ItemData.Get<FItemData>().Count);
    }
}
```

### Public API Reference

#### UInteractorComponent

| Method | Signature | Description |
|--------|-----------|-------------|
| `Interact()` | `void Interact()` | Initiates interaction with the currently hovered interactable |
| `FinishInteract()` | `void FinishInteract()` | Completes a continuous interaction |
| `GetHoveredPrimitive()` | `UPrimitiveComponent* GetHoveredPrimitive() const` | Returns the currently hovered primitive component |
| `InteractWith()` | `void InteractWith(UPrimitiveComponent* Component)` | Interacts with a specific component |

#### UInteractableComponent

| Method | Signature | Description |
|--------|-----------|-------------|
| `Interact()` | `void Interact(const AActor* Interactor, USceneComponent* Component)` | Called when an interactor interacts with this component |
| `FinishInteract()` | `void FinishInteract(const AActor* Interactor, USceneComponent* Component)` | Called to finish a continuous interaction |
| `IsPlayerInteractable()` | `bool IsPlayerInteractable() const` | Checks if the object can be interacted with |
| `GetInteractType()` | `EInteractType GetInteractType() const` | Returns the interaction type (SingleUse/Continuous) |

#### UInventoryComponent

| Method | Signature | Description |
|--------|-----------|-------------|
| `AddToInventory()` | `bool AddToInventory(const FItemData& Item, FItemHandle& OutItemHandle)` | Adds an item to the inventory |
| `RemoveFromInventory()` | `void RemoveFromInventory(const FItemHandle& ItemHandle)` | Removes an item by handle |
| `RemoveItemFromInventory()` | `void RemoveItemFromInventory(const FItemData& Item)` | Removes a specific item type |
| `GetInventory()` | `void GetInventory(TArray<FInventoryContents>& OutInventory) const` | Gets a copy of the inventory array |
| `CanAddToInventory()` | `bool CanAddToInventory(const FItemData& Item) const` | Checks if an item can be added |
| `EmptyInventory()` | `void EmptyInventory()` | Removes all items from inventory |
| `GenerateUniqueHandle()` | `FItemHandle GenerateUniqueHandle()` | Creates a new unique item handle |

#### UEquipComponent

| Method | Signature | Description |
|--------|-----------|-------------|
| `EquipItem()` | `void EquipItem(const FItemHandle& Item)` | Equips the specified item |
| `UnequipItem()` | `void UnequipItem()` | Unequips the currently equipped item |
| `ToggleEquippedItem()` | `void ToggleEquippedItem(const FItemHandle& Item)` | Toggles item equipment state |
| `UseItem()` | `void UseItem()` | Uses the currently equipped item |
| `GetEquippedItem()` | `FItemHandle GetEquippedItem() const` | Returns the currently equipped item handle |
| `HasItemEquipped()` | `bool HasItemEquipped() const` | Checks if an item is equipped |

#### UTriggerComponent

| Method | Signature | Description |
|--------|-----------|-------------|
| `TriggerActors()` | `void TriggerActors(AActor* Instigator, UPrimitiveComponent* Component)` | Triggers all configured interactables |

#### UPhysicsGrabComponent

| Method | Signature | Description |
|--------|-----------|-------------|
| `GrabComponent()` | `void GrabComponent(UStaticMeshComponent* GrabMesh, FHitResult Hit)` | Grabs a physics object |
| `ReleaseComponent()` | `void ReleaseComponent()` | Releases the grabbed object |
| `PushComponent()` | `void PushComponent(UStaticMeshComponent* GrabMesh)` | Pushes a physics object |
| `PhysicsInteract()` | `void PhysicsInteract()` | Initiates physics-based interaction |

### Events and Delegates

| Event | Signature | Description |
|-------|-----------|-------------|
| `OnUpdateHover` | `FUpdateHover` | Called when the hovered primitive changes |
| `OnInteract` | `FInteract` | Called when an interaction occurs |
| `OnFinishInteract` | `FFinishInteract` | Called when an interaction finishes |
| `OnInventoryChange` | `FInventoryChange` | Called when inventory contents change |
| `OnItemAdd` | `FItemAdd` | Called when an item is added |
| `OnItemRemove` | `FItemRemove` | Called when an item is removed |
| `OnUpdateEquipState` | `FUpdateEquipState` | Called when equipment state changes |
| `OnPushObject` | `FPushObject` | Called when push state changes |

---

## Troubleshooting

### Common Error Codes

| Code | Message | Resolution |
|------|---------|------------|
| `EIS-1001` | "Interaction failed: No interactable in range" | Ensure the interactable is within `InteractDistance` and `bIsPlayerInteractable` is true |
| `EIS-1002` | "Inventory full: Cannot add item" | Increase `MaxInventorySlots` or remove existing items |
| `EIS-1003` | "Item not found in inventory" | Verify the item handle is valid and the item exists |
| `EIS-1004` | "Physics grab failed: Object too heavy" | Reduce `GrabWeightThreshold` or use a lighter object |
| `EIS-1005` | "Network error: Server rejected interaction" | Check server authority and network permissions |
| `EIS-2001` | "Item data validation failed" | Ensure `UItemInformation` asset is valid and properly configured |
| `EIS-2002` | "Equip failed: No item equipped" | Equip an item before attempting to use it |

---

## AI Tooling Metadata

```yaml
# AI-Optimized Plugin Metadata
plugin:
  name: "InteractSystem"
  version: "1.0.0"
  category: "Gameplay Framework"
  description: "End-to-end user interaction orchestration across web, mobile, and API touchpoints"
  
runtime:
  - name: "UnrealEngine"
    min_version: "5.0"
    max_version: "5.4"
    recommended: "5.3"
  - name: "Windows"
    min_version: "10"

dependencies:
  - name: "Core"
    version_constraint: ">=5.0"
    required: true
  - name: "CoreUObject"
    version_constraint: ">=5.0"
    required: true
  - name: "Engine"
    version_constraint: ">=5.0"
    required: true
  - name: "DeveloperSettings"
    version_constraint: ">=5.0"
    required: true
  - name: "NetCore"
    version_constraint: ">=5.0"
    required: true
  - name: "GameplayTags"
    version_constraint: ">=5.0"
    required: true

api_surface:
  public_classes:
    - name: "UInteractorComponent"
      blueprint_spawnable: true
      category: "PlayerInteract"
    - name: "UInteractableComponent"
      blueprint_spawnable: true
      category: "Interaction"
    - name: "UInventoryComponent"
      blueprint_spawnable: true
      category: "Inventory"
    - name: "UEquipComponent"
      blueprint_spawnable: true
      category: "PlayerEquip"
    - name: "UPlayerEquipComponent"
      blueprint_spawnable: true
      category: "PlayerEquip"
    - name: "UTriggerComponent"
      blueprint_spawnable: true
      category: "Trigger"
    - name: "UPhysicsGrabComponent"
      blueprint_spawnable: true
      category: "PhysicsGrab"
    - name: "UPlayerPushComponent"
      blueprint_spawnable: true
      category: "Physics"
    - name: "UAreaInteractComponent"
      blueprint_spawnable: true
      category: "PlayerInteract"
    - name: "APickupable"
      blueprint_spawnable: true
      category: "Interaction"
    - name: "AItemVisual"
      blueprint_spawnable: true
      category: "ItemVisual"
  
  public_structs:
    - name: "FItemHandle"
      blueprint_type: true
    - name: "FItemData"
      blueprint_type: true
    - name: "FInventoryContents"
      blueprint_type: true
    - name: "FInventoryContainer"
      blueprint_type: false
    - name: "FItemAttribute"
      blueprint_type: true

  public_interfaces:
    - name: "IToggleable"
      methods: ["Toggle", "SetToggle"]

security:
  network_authority: "server"
  replication: "FFastArraySerializer"
  validation: "item_data"
  save_game: true
```

---

*README.md validated for AI tooling compatibility. Last updated: 2026-05-21*