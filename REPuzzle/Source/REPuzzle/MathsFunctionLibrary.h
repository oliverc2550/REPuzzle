// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/UnrealMathUtility.h"

namespace Maths
{
	namespace Percent
	{
		FORCEINLINE double CalculateDecimalPercent( double nonDecimalPercent )
		{
			return nonDecimalPercent / 100.0;
		}
	}
	namespace Rotator
	{
		FORCEINLINE double AbsoluteNormalizeAxis( double angle)
		{
			return FMath::Abs( FRotator::NormalizeAxis( angle ) );
		}

		//Returns a Non-Zero Tolerance Value based on the values of the input axis and percent. If input axis is zero it will be given a tolerence of 1.0f divided by the input percent value
		FORCEINLINE double CalculateAxisTolerenceValue( double axis, double percentTolerance )
		{
			double targetAxisAbsoluteNormalized = AbsoluteNormalizeAxis( axis );
			double nonZeroTargetAxisValue = ( targetAxisAbsoluteNormalized == 0.0 ) ? 1.0 : targetAxisAbsoluteNormalized;
			return nonZeroTargetAxisValue * Percent::CalculateDecimalPercent( percentTolerance );
		}


		//EqualsWithinPercentTolerence is used over FRotator::Equal due to the Tolerance param of FRotator::Equal being the same for all three axes. This is undesirable if you want to allow the setting of a percent based tolerance
		//that can be used across every axes. Because of this EqualsWithinPercentTolerance was written to allow each axis to be compared using a differenct tolerance value that is based off of the input tolerance percent.

		//Returns true if each of the axes of the currentRotator param are within the percentTolerance of targetRotator param, returns false if any of the axes are outside the percentTolerance of targetRotator
		FORCEINLINE bool EqualsWithinPercentTolerence( const FRotator& currentRotator, const FRotator& targetRotator, double percentTolerance )
		{
			double targetPitchTolerance = CalculateAxisTolerenceValue( targetRotator.Pitch, percentTolerance );
			double targetYawTolerance = CalculateAxisTolerenceValue( targetRotator.Yaw, percentTolerance );
			double targetRollTolerance = CalculateAxisTolerenceValue( targetRotator.Roll, percentTolerance );

			return ( AbsoluteNormalizeAxis( currentRotator.Pitch - targetRotator.Pitch ) <= targetPitchTolerance )
				&& ( AbsoluteNormalizeAxis( currentRotator.Yaw - targetRotator.Yaw ) <= targetYawTolerance )
				&& ( AbsoluteNormalizeAxis( currentRotator.Roll - targetRotator.Roll ) <= targetRollTolerance );
		}

		FORCEINLINE FRotator CombineRotators( const FRotator& rotatorA, const FRotator& rotatorB )
		{
			FQuat quatA = FQuat( rotatorA );
			FQuat quatB = FQuat( rotatorB );

			return FRotator( quatB * quatA );
		}
	}
	namespace Vector
	{
		FORCEINLINE FVector CalculateUnitDirectionVectorNormalised(const FVector& fromVector, const FVector& toVector)
		{
			return ( toVector - fromVector ).GetSafeNormal();
		}
	}
}
