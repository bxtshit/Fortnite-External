#pragma once
#ifndef OFFSETS_HPP
#define OFFSETS_HPP

class offset
{
public:
	static constexpr uintptr_t GWorld = null;

	static constexpr uintptr_t GameInstance = 0x1d8;

	static constexpr uintptr_t LocalPlayers = 0x38;

	static constexpr uintptr_t PlayerController = 0x30;

	static constexpr uintptr_t AcknowledgedPawn = 0x338;

	static constexpr uintptr_t PlayerState = 0x2b0;

	static constexpr uintptr_t RootComponent = 0x198;

	static constexpr uintptr_t CharacterMovement = 0x320;

	static constexpr uintptr_t LastUpdateVelocity = 0x348;

	static constexpr uintptr_t GameState = 0x160;

	static constexpr uintptr_t Mesh = 0x318;

	static constexpr uintptr_t ComponentToWorld = 0x1c0;

	static constexpr uintptr_t PawnPrivate = 0x308;

	static constexpr uintptr_t PlayerArray = 0x2a8;

	static constexpr uintptr_t TeamIndex = 0x1211;

	static constexpr uintptr_t HabaneroComponent = 0x9f0;

	static constexpr uintptr_t KillScore = 0x1224;

	static constexpr uintptr_t bIsDying = 0x758;

	static constexpr uintptr_t bIsDBNO = 0x982;

	static constexpr uintptr_t bIsABot = 0x29a;

	static constexpr uintptr_t CurrentVehicle = 0x29A0;

	static constexpr uintptr_t CurrentWeapon = 0xA68;

	static constexpr uintptr_t WeaponCoreAnimation = 0x1300;

	static constexpr uintptr_t MaxTargetingAimAdjustPerSecond = 0x1a10;

	static constexpr uintptr_t Rarity = 0x9a;

	static constexpr uintptr_t WeaponData = 0x500;

	static constexpr uintptr_t ItemName = 0x40;

	static constexpr uintptr_t BoneArray = 0x5B8;

	static constexpr uintptr_t BoneCached = 0x600;

	static constexpr uintptr_t TargetedFortPawn = 0x18A8;

	static constexpr uintptr_t ViewPitchMin = 0x248c;

	static constexpr uintptr_t ViewPitchMax = 0x2490;

	static constexpr uintptr_t ViewYawMin = 0x2494;

	static constexpr uintptr_t ViewYawMax = 0x2498;

}; inline std::unique_ptr<offset> offsets = std::make_unique<offset>( );

#endif