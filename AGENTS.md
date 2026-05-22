# AGENTS.md

This file provides guidance to agents when working with the InteractSystem plugin.

## Plugin Overview

**InteractSystem** is a comprehensive user interaction orchestration plugin for Unreal Engine that provides end-to-end interaction management. The system handles event routing, session management, cross-plugin interaction triggering, and third-party service integration.

### Key Capabilities

| Capability | Description |
|------------|-------------|
| **Interaction Management** | Component-based system for defining interactable objects and player interaction logic |
| **Inventory System** | Full-featured inventory with stacking, item attributes, and network replication |
| **Equipment System** | Item equipping, unequipping, and usage with visual feedback |
| **Physics Interaction** | Grab, push, and throw physics objects with configurable parameters |
| **Trigger System** | Chain interactions between multiple interactable objects |
| **Toggleable Interface** | On/off state management for interactive objects |

## Runtime Requirements

| Environment | Minimum Version | Recommended Version |
|-------------|-----------------|---------------------|
| Unreal Engine | 5.0 | 5.3+ |
| Windows | 10 | 11 |

## Dependencies

| Module | Version Constraint | Required |
|--------|-------------------|----------|
| Core | >= 5.0 | Yes |
| CoreUObject | >= 5.0 | Yes |
| Engine | >= 5.0 | Yes |
| InputCore | >= 5.0 | Yes |
| DeveloperSettings | >= 5.0 | Yes |
| NetCore | >= 5.0 | Yes |
| GameplayTags | >= 5.0 | Yes |

## Security Considerations

- **Network Replication:** All inventory and interaction state changes are server-authoritative
- **Replication:** Uses `FFastArraySerializer` for inventory replication
- **Validation:** Item data is validated before being added to inventory
- **Save Game:** Supports save game serialization

## Key Classes

| Class | Purpose |
|-------|---------|
| `UInteractorComponent` | Player interaction component |
| `UInteractableComponent` | Defines interactable objects |
| `UInventoryComponent` | Inventory management |
| `UEquipComponent` | Item equipment system |
| `UTriggerComponent` | Chain interaction triggers |
| `UPhysicsGrabComponent` | Physics object interaction |

## Key Structs

| Struct | Blueprint Type |
|--------|----------------|
| `FItemHandle` | Yes |
| `FItemData` | Yes |
| `FInventoryContents` | Yes |
| `FInventoryContainer` | No |
| `FItemAttribute` | Yes |

## Key Interfaces

| Interface | Methods |
|-----------|---------|
| `IToggleable` | `Toggle`, `SetToggle` |