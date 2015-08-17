#ifndef ZEN_FLOAT_PARAMETER_H_INCLUDED
#define ZEN_FLOAT_PARAMETER_H_INCLUDED
//using namespace juce;

namespace juce
{
	class FloatParameter :AudioProcessorParameter
	{
	/*public:
		FloatParameter();
		~FloatParameter();
		float testFloat(float inVal);*/
	public:

		float testFloat(float inVal);
		static float testGainUtil(float inGain);

		float testGain(float inGain);

		virtual bool isMetaParameter() const override;

		virtual int getNumSteps() const override;

		virtual bool isAutomatable() const override;

		virtual void setValue(float newValue) override;

		virtual String getName(int maximumStringLength) const override;

		virtual float getValueForText(const String& text) const override;

		virtual float getDefaultValue() const override;

		virtual bool isOrientationInverted() const override;

		virtual String getLabel() const override;

		virtual float getValue() const override;

		virtual String getText(float value, int) const override;

	private:


	};
	/*/// <summary>
	/// Class FloatParameter.
	/// </summary>
	class FloatParameter : AudioProcessorParameter
	{
	public:


		static float testGainUtil(float inGain);

		float testGain(float inGain);

		virtual bool isMetaParameter() const override;

		virtual int getNumSteps() const override;

		virtual bool isAutomatable() const override;

		virtual void setValue(float newValue) override;

		virtual String getName(int maximumStringLength) const override;

		virtual float getValueForText(const String& text) const override;

		virtual float getDefaultValue() const override;

		virtual bool isOrientationInverted() const override;

		virtual String getLabel() const override;

		virtual float getValue() const override;

		virtual String getText(float value, int) const override;

	private:


	};*/
}
#endif   // ZEN_FLOAT_PARAMETER_H_INCLUDED