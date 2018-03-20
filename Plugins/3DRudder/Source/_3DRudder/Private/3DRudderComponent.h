// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#define _3DRUDDER_SDK_STATIC
#include "3DRudderSDK.h"
#include "3DRudderFunctionLibrary.h"
#include "3DRudderComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOn3dRudderConnected, const bool, Connected);

class EventUnreal : public ns3dRudder::IEvent
{
public:
	EventUnreal(int32 port, FOn3dRudderConnected event)
	{
		portNumber = port;
		callback = event;

	}
	virtual void OnConnect(uint32_t nDeviceNumber) { if (nDeviceNumber== portNumber) callback.Broadcast(true); };
	virtual void OnDisconnect(uint32_t nDeviceNumber) { if (nDeviceNumber == portNumber) callback.Broadcast(false); };
private:
	int32 portNumber;
	FOn3dRudderConnected callback;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), DisplayName = "3dRudder Actor")
class U3DRudderComponent : public UActorComponent
{
	GENERATED_BODY()

public:		

	// Sets default values for this component's properties
	U3DRudderComponent();
	~U3DRudderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
		int32 Port;

	UPROPERTY(EditAnywhere)
		UCurveFloat* PitchCurve;
	UPROPERTY(EditAnywhere)
		UCurveFloat* RollCurve;
	UPROPERTY(EditAnywhere)
		UCurveFloat* YawCurve;
	UPROPERTY(EditAnywhere)
		UCurveFloat* UpDownCurve;

	UFUNCTION(BlueprintPure, Category = "3dRudder Actor")
		bool IsConnected();

	UFUNCTION(BlueprintPure, Category = "3dRudder Actor")
		FString GetFirmwareVersion();

	UFUNCTION(BlueprintPure, Category = "3dRudder Actor")
		E3DRudderStatus GetStatus();

	UFUNCTION(BlueprintPure, Category = "3dRudder Actor")
		FString GetStatusString();

	UFUNCTION(BlueprintCallable, Category = "3dRudder Actor")
		bool PlaySound(int32 frequency, int32 duration);

	UFUNCTION(BlueprintPure, Category = "3dRudder Actor")
		E3DRudderError GetAxis(E3DRudderModeAxis mode, float &axis_x, float &axis_y, float &axis_z, float &axis_rz);

	UFUNCTION(BlueprintPure, Category = "3dRudder Actor")
		void GetSensor(int32 &sensor1, int32 &sensor2, int32 &sensor3, int32 &sensor4, int32 &sensor5, int32 &sensor6);

	// This is declaration of "Static Multicast Delegate".
	// Plugin can broadcast it to any other parts of the code that has been binded to it.
	/*DECLARE_MULTICAST_DELEGATE_OneParam(FXPlugin_OnReceivedTestMessageDelegate, const FString&);
		static FXPlugin_OnReceivedTestMessageDelegate OnReceivedTestMessageDelegate;*/

	// This is declaration of "Dynamic Multicast Delegate". 
	// This is the delegate that will be invoking the Blueprint Event.
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusChanged, const FString&, Status);
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On status changed"))
		FOnStatusChanged OnStatusChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = "On 3dRudder connected"))
		FOn3dRudderConnected On3dRudderConnected;
		
private:
	ns3dRudder::CSdk* pSdk;
	ns3dRudder::Status status;

	ns3dRudder::CurveArray curves;
	CurveUnreal* curvePitch;
	CurveUnreal* curveRoll;
	CurveUnreal* curveYaw;
	CurveUnreal* curveUpdown;

	EventUnreal* events;
};
