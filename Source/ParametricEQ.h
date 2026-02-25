/*
  ==============================================================================

    ParametricEQ.h
    Created: 13 Apr 2025 11:59:56am
    Author:  zachary patchen

  ==============================================================================
*/
#include <JuceHeader.h>     
#pragma once
class ParametricEQ 
    
{

public:
    ParametricEQ();
	~ParametricEQ();

	void setSampleRate(float sampleRate);
	void setGain(float gain);
	void setCenterFrequency(float frequency);
	void setQFactor(float q);
	float processSample(float inputSample);

	void reset();
	void updateCoefficients();



private:
    float a0, a1, a2, b1, b2, z1, z2, y1, y2;
	float gain, qFactor, sampleRate;
	int centerFrequency;

};
