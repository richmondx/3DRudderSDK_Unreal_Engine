// Fill out your copyright notice in the Description page of Project Settings.

#include "3DRudderComponent.h"


// Sets default values for this component's properties
U3DRudderComponent::U3DRudderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;	
	// ...
}

void U3DRudderComponent::BeginPlay()
{
	pSdk = ns3dRudder::GetSDK();
	
	events = new EventUnreal(Port, On3dRudderConnected);
	pSdk->SetEvent(events);

	pSdk->Init();

	if (PitchCurve != nullptr)
	{
		curvePitch = new CurveUnreal(PitchCurve);
		curves.SetCurve(ns3dRudder::CurvePitch, curvePitch);
	}
	if (RollCurve != nullptr)
	{
		curveRoll = new CurveUnreal(RollCurve);
		curves.SetCurve(ns3dRudder::CurveRoll, curveRoll);
	}
	if (YawCurve != nullptr)
	{
		curveYaw = new CurveUnreal(YawCurve);
		curves.SetCurve(ns3dRudder::CurveYaw, curveYaw);
	}
	if (UpDownCurve != nullptr)
	{
		curveUpdown = new CurveUnreal(UpDownCurve);
		curves.SetCurve(ns3dRudder::CurveUpDown, curveUpdown);
	}
}

U3DRudderComponent::~U3DRudderComponent()
{
	if (pSdk != nullptr)
	{
		ns3dRudder::EndSDK();
		pSdk = nullptr;
	}
	if (events != nullptr)
	{
		delete events;
		events = nullptr;
	}
	if (curvePitch != nullptr)
	{
		delete curvePitch;
		curvePitch = nullptr;
	}
}

bool U3DRudderComponent::IsConnected()
{
	return pSdk->IsDeviceConnected(Port);
}

FString U3DRudderComponent::GetFirmwareVersion()
{
	FString version = FString::Printf(TEXT("%04x"), pSdk->GetVersion(Port));
	return version;
}

E3DRudderStatus U3DRudderComponent::GetStatus()
{
	return (E3DRudderStatus)pSdk->GetStatus(Port);
}

FString U3DRudderComponent::GetStatusString()
{
	FString statusString = "NoStatus";
	switch (pSdk->GetStatus(Port))
	{
	case ns3dRudder::Status::NoFootStayStill:
		statusString = "No Foot Stay Still";
		break;
	case ns3dRudder::Status::Initialisation:
		statusString = "Initialisation";
		break;
	case ns3dRudder::Status::PutYourFeet:
		statusString = "Put Your Feet";
		break;
	case ns3dRudder::Status::PutSecondFoot:
		statusString = "Put Second Foot";
		break;
	case ns3dRudder::Status::StayStill:
		statusString = "Stay Still";
		break;
	case ns3dRudder::Status::InUse:
		statusString = "In Use";
		break;
	case ns3dRudder::Status::ExtendedMode:
		statusString = "Extended Mode";
		break;
	case ns3dRudder::Status::End:
		statusString = "End";
		break;
	default:
		statusString = "NoStatus";
		break;
	}
	return statusString;
}

E3DRudderError U3DRudderComponent::GetAxis(E3DRudderModeAxis mode, float &axis_x, float &axis_y, float &axis_z, float &axis_rz)
{
	ns3dRudder::Axis axis;
	ns3dRudder::ErrorCode error = pSdk->GetAxis(Port, (ns3dRudder::ModeAxis)mode, &axis, &curves);
	if (error == ns3dRudder::Success)
	{
		axis_x = axis.GetYAxis();
		axis_y = axis.GetXAxis();
		axis_z = axis.GetZAxis();
		axis_rz = axis.GetZRotation();
	}
	else
	{
		axis_x = axis_y = axis_z = axis_rz = 0.0;		
	}
	return (E3DRudderError)error;
}

bool U3DRudderComponent::PlaySound(int32 frequency, int32 duration)
{
	if (pSdk->PlaySnd(Port, frequency, duration) == ns3dRudder::Success)
	{		
		return true;
	}
	return false;
}

void U3DRudderComponent::GetSensor(int32 &sensor1, int32 &sensor2, int32 &sensor3, int32 &sensor4, int32 &sensor5, int32 &sensor6)
{
	sensor1 = pSdk->GetSensor(Port, 0);
	sensor2 = pSdk->GetSensor(Port, 1);
	sensor3 = pSdk->GetSensor(Port, 2);
	sensor4 = pSdk->GetSensor(Port, 3);
	sensor5 = pSdk->GetSensor(Port, 4);
	sensor6 = pSdk->GetSensor(Port, 5);
}

// Called every frame
void U3DRudderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	ns3dRudder::Status _status = status;
	status = pSdk->GetStatus(Port);
	if (status != _status)
	{		
		OnStatusChangedDelegate.Broadcast(GetStatusString());		
	}
	// ...
}

