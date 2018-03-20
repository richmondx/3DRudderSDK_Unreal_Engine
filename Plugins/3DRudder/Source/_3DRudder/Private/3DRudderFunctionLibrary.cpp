// Fill out your copyright notice in the Description page of Project Settings.

#include "3DRudderFunctionLibrary.h"
#include "3DRudderPrivatePCH.h"

U3DRudderFunctionLibrary::U3DRudderFunctionLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

FString U3DRudderFunctionLibrary::GetSDKVersion()
{
	FString version = FString::Printf(TEXT("%04x"), ns3dRudder::GetSDK()->GetSDKVersion());
	return version;
}

int32 U3DRudderFunctionLibrary::GetNumberOfConnectedDevice()
{
	return ns3dRudder::GetSDK()->GetNumberOfConnectedDevice();
}

bool U3DRudderFunctionLibrary::IsConnected(int32 portNumber)
{
	return ns3dRudder::GetSDK()->IsDeviceConnected(portNumber);
}

FString U3DRudderFunctionLibrary::GetFirmwareVersion(int32 portNumber)
{
	FString version = FString::Printf(TEXT("%04x"), ns3dRudder::GetSDK()->GetVersion(portNumber));
	return version;
}

E3DRudderStatus U3DRudderFunctionLibrary::GetStatus(int32 portNumber)
{
	return (E3DRudderStatus)ns3dRudder::GetSDK()->GetStatus(portNumber);
}

FString U3DRudderFunctionLibrary::GetStatusString(int32 portNumber)
{
	FString status = "NoStatus";
	switch (ns3dRudder::GetSDK()->GetStatus(portNumber))
	{
		case ns3dRudder::Status::NoFootStayStill:
			status = "No Foot Stay Still";
		break;
		case ns3dRudder::Status::Initialisation:
			status = "Initialisation";
		break;
		case ns3dRudder::Status::PutYourFeet:
			status = "Put Your Feet";
		break;
		case ns3dRudder::Status::PutSecondFoot:
			status = "Put Second Foot";
		break;
		case ns3dRudder::Status::StayStill:
			status = "Stay Still";
		break;
		case ns3dRudder::Status::InUse:
			status = "In Use";
		break;
		case ns3dRudder::Status::ExtendedMode:
			status = "Extended Mode";
		break;
		case ns3dRudder::Status::End:
			status = "End";
		break;
		default:
			status = "NoStatus";
		break;
	}
	return status;
}

E3DRudderError U3DRudderFunctionLibrary::GetAxis(int32 portNumber, E3DRudderModeAxis mode, UCurveFloat* curve_pitch, UCurveFloat* curve_roll, UCurveFloat* curve_yaw, UCurveFloat* curve_updown,
	float &axis_x, float &axis_y, float &axis_z, float &axis_rz)
{	
	ns3dRudder::Axis axis;
	ns3dRudder::CurveArray curves;
	if (curve_pitch != nullptr)
		curves.SetCurve(ns3dRudder::CurvePitch, new CurveUnreal(curve_pitch));
	if (curve_roll != nullptr)
		curves.SetCurve(ns3dRudder::CurveRoll, new CurveUnreal(curve_roll));
	if (curve_yaw != nullptr)
		curves.SetCurve(ns3dRudder::CurveYaw, new CurveUnreal(curve_yaw));
	if (curve_updown != nullptr)
		curves.SetCurve(ns3dRudder::CurveUpDown, new CurveUnreal(curve_updown));

	ns3dRudder::ErrorCode error = ns3dRudder::GetSDK()->GetAxis(portNumber, (ns3dRudder::ModeAxis)mode, &axis, &curves);
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

bool U3DRudderFunctionLibrary::PlaySound(int32 portNumber, int32 frequency, int32 duration)
{
	if(ns3dRudder::GetSDK()->PlaySnd(portNumber, frequency, duration) == ns3dRudder::Success)
	{ 
		return true;
	}
	return false;
}

void U3DRudderFunctionLibrary::GetSensor(int32 portNumber, int32 &sensor1, int32 &sensor2, int32 &sensor3, int32 &sensor4, int32 &sensor5, int32 &sensor6)
{	
	sensor1 = ns3dRudder::GetSDK()->GetSensor(portNumber, 0);
	sensor2 = ns3dRudder::GetSDK()->GetSensor(portNumber, 1);
	sensor3 = ns3dRudder::GetSDK()->GetSensor(portNumber, 2);
	sensor4 = ns3dRudder::GetSDK()->GetSensor(portNumber, 3);
	sensor5 = ns3dRudder::GetSDK()->GetSensor(portNumber, 4);
	sensor6 = ns3dRudder::GetSDK()->GetSensor(portNumber, 5);
}