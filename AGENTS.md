# InteractSystem

Component-based interaction framework for interactable objects, inventory, equipment, and physics grab.

## Capabilities

| Capability | Description |
|------------|-------------|
| **Interaction Management** | Component-based system for defining interactable objects and player interaction logic |
| **Inventory System** | Full-featured inventory with stacking, item attributes, and network replication |
| **Equipment System** | Item equipping, unequipping, and usage with visual feedback |
| **Physics Interaction** | Grab, push, and throw physics objects with configurable parameters |
| **Trigger System** | Chain interactions between multiple interactable objects |
| **Toggleable Interface** | On/off state management for interactive objects |

## Key Classes

| Class | Purpose |
|-------|---------|
| `UInteractorComponent` | Player interaction component |
| `UInteractableComponent` | Defines interactable objects |
| `UInventoryComponent` | Inventory management |
| `UEquipComponent` | Item equipment system |
| `UTriggerComponent` | Chain interaction triggers |
| `UPhysicsGrabComponent` | Physics object interaction |

## Key Structs & Interfaces

| Name | Type | Purpose |
|------|------|---------|
| `FItemHandle` | Struct | Reference to inventory item |
| `FItemData` | Struct | Item data with count/stacking |
| `FInventoryContents` | Struct | Inventory entry with replication |
| `FInventoryContainer` | Struct | Full inventory container |
| `FItemAttribute` | Struct | Item attribute data |
| `IToggleable` | Interface | `Toggle`, `SetToggle` methods |

## Common Pitfalls

- Inventory replication requires authority - `AddToInventory` etc. are `BlueprintAuthorityOnly`
- `FItemHandle::IsValid()` returns false when `HandleID == -1` or `OwningInventory` is null
- `EStackableType::NoStacking`: `GetMaxStack()` returns 1 regardless of MaxStack field

See `.claude/conventions.md` for more patterns.

## Integration Points

- **HorrorFeatures**: Character uses `UInteractableComponent` for interactions
- See `.claude/plugin-integration.md` for cross-plugin dependency matrix

## Human Review Required

- Changes to `IInteractable` interface
- Changes to `FItemData` / `FInventoryContents` / `FInventoryContainer`
- See `.claude/human-review-checklist.md` for full list