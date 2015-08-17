/* ==============================================================================
//  ZenParamUtils.h
//  Part of the Zentropia JUCE Collection
//  @author Casey Bailey (<a href="SonicZentropy@gmail.com">email</a>)
//  @version 0.1
//  @date 2015/08/16
//  Copyright (C) 2015 by Casey Bailey
//  Provided under the [GNU license]
//
//  Details: Generic Utilities for dealing with JUCE Parameters
//
//  Zentropia is hosted on Github at [https://github.com/SonicZentropy]
==============================================================================
*/


#ifndef ZENPARAMUTILS_H_INCLUDED
#define ZENPARAMUTILS_H_INCLUDED

//using namespace juce;

namespace juce
{
	/// <summary>
	/// Class ZenParamUtils.
	/// </summary>
	class ZenParamUtils
	{
		public:
		//	ZenParamUtils();
		//	~ZenParamUtils();

		/// <summary>
		/// Warps Audio normalization from juce_zen_utils.h fixed
		/// </summary>
		/// <param name="x">Incoming Value</param>
		/// <param name="max">Max of range</param>
		/// <param name="min">Min of Range</param>
		/// <param name="mid">Midpoint for 0.5 skew</param>
		/// <returns>(float) Post-normalized value</returns>
		static float warp(float x, float max, float min, float mid);

		static float inverseWarp(float x, float max, float min, float mid);
		static float warpCoefficients(float max, float min, float mid);

		static bool convertNormalizedValueToBoolViaRange(float inputValue);
		static float convertBoolToFloatNormalized(bool inputBool);


		static float normalizeValueLinear(const double& value, const double& minValue, const double& maxValue);
		static float denormalizeValueLinear(const float& normalized, const double& minValue, const double& maxValue);
		private:
	};
}
#endif
