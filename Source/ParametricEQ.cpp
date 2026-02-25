/*
  ==============================================================================

    ParametricEQ.cpp
    Created: 13 Apr 2025 11:59:56am
    Author:  zachary patchen

  ==============================================================================
*/

#include "ParametricEQ.h"
#include <cmath>
ParametricEQ::ParametricEQ()
{
	// Initialize parameters
	sampleRate = 44100.0f;
	gain = 0.0f;
	centerFrequency = 4000;
	qFactor = 1.0f;
	// Initialize coefficients
	a0 = a1 = a2 = b1 = b2 = z1 = z2 = y1 = y2 = 0.0f;
}
ParametricEQ::~ParametricEQ()
{
}
void ParametricEQ::setSampleRate(float newSampleRate)
{
	sampleRate = newSampleRate;
	updateCoefficients();
}
void ParametricEQ::setGain(float newGain)
{
	gain = newGain;
	updateCoefficients();
}
void ParametricEQ::setCenterFrequency(float newFrequency)
{
	centerFrequency = newFrequency;
	updateCoefficients();
}
void ParametricEQ::setQFactor(float newQ)
{
	qFactor = newQ;
	updateCoefficients();
}
float ParametricEQ::processSample(float inputSample)
{
	// Apply the filter to the input sample
	float outputSample = a0 * inputSample + a1 * z1 + a2 * z2 - b1 * y1 - b2 * y2;
	// Update delay lines
	z2 = z1;
	z1 = inputSample;
	y2 = y1;
	y1 = outputSample;
	return outputSample;
}
void ParametricEQ::reset()
{
	// Reset delay lines
	z1 = z2 = y1 = y2 = 0.0f;
}
void ParametricEQ::updateCoefficients()
{
	// Calculate coefficients based on the current parameters
	float omega = 2.0f * juce::MathConstants<float>::pi * centerFrequency / sampleRate;
	float alpha = sin(omega) / (2.0f * qFactor);
	float A = pow(10.0f, gain / 40.0f);

	float norm = 1 + alpha / A;
	a0 = (1 + alpha * A) / norm;
	a1 = (-2 * cos(omega)) / norm;
	a2 = (1 - alpha * A) / norm;
	b1 = (-2 * cos(omega)) / norm;
	b2 = (1 - alpha / A) / norm;

}
