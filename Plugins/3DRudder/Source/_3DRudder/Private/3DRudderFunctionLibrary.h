// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#define _3DRUDDER_SDK_STATIC
#include "3DRudderSDK.h"
#include "3DRudderFunctionLibrary.generated.h"

/** Defines the mode of axis*/
UENUM(BlueprintType)
enum class E3DRudderModeAxis : uint8
{
	Angle = 0,
	NormalisedValue = 2,
	ValueWithCurve = 3,
	NormalisedValueNonSymmetricalPitch = 4,
	ValueWithCurveNonSymmetricalPitch = 5
};
/** Defines the status of axis*/
UENUM(BlueprintType)
enum class E3DRudderStatus : uint8
{
	NoStatus = 0, 
	NoFootStayStill = 1,
	Initialisation, 
	PutYourFeet,
	PutSecondFoot,
	StayStill,
	InUse,
	ExtendedMode
};
/** Defines the error code*/
UENUM(BlueprintType)
enum class E3DRudderError : uint8
{
	Success = 0, 
	NotConnected, 
	Fail, 
	IncorrectCommand, 
	Timeout, 
	WrongSignature, 
	NotReady	
};
/**
 * Curve Editor 
 */
class CurveUnreal : public ns3dRudder::Curve
{
public:
	CurveUnreal(UCurveFloat* curve)
	{
		pCurve = curve;
	}

	virtual float CalcCurveValue(float fValue)  const 
	{
		//if (pCurve != nullptr)
			return pCurve->GetFloatValue(fValue);
		//return 0.0f;
	}
private:
	UCurveFloat* pCurve;
};
/**
*
*/
UCLASS()
class _3DRUDDER_API U3DRudderFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "3dRudder")
		static FString GetSDKVersion();

	UFUNCTION(BlueprintPure, Category = "3dRudder")
		static int32 GetNumberOfConnectedDevice();

	UFUNCTION(BlueprintPure, Category = "3dRudder")
		static bool IsConnected(int32 portNumber);

	UFUNCTION(BlueprintPure, Category = "3dRudder")
		static FString GetFirmwareVersion(int32 portNumber);

	UFUNCTION(BlueprintPure, Category = "3dRudder")
		static E3DRudderStatus GetStatus(int32 portNumber);

	UFUNCTION(BlueprintPure, Category = "3dRudder")
		static FString GetStatusString(int32 portNumber);

	UFUNCTION(BlueprintCallable, Category = "3dRudder")
		static bool PlaySound(int32 portNumber, int32 frequency, int32 duration);

	UFUNCTION(BlueprintPure, Category = "3dRudder")
		static E3DRudderError GetAxis(int32 portNumber, E3DRudderModeAxis mode, UCurveFloat* curve_pitch, UCurveFloat* curve_roll, UCurveFloat* curve_yaw, UCurveFloat* curve_updown,
			float &axis_x, float &axis_y, float &axis_z, float &axis_rz);

	UFUNCTION(BlueprintPure, Category = "3dRudder")
		static void GetSensor(int32 portNumber, int32 &sensor1, int32 &sensor2, int32 &sensor3, int32 &sensor4, int32 &sensor5, int32 &sensor6);
};
