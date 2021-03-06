/*
 * GridSampler.hpp
 *
 *  Created on: 15.08.2012
 *      Author: poschmann
 */

#ifndef GRIDSAMPLER_HPP_
#define GRIDSAMPLER_HPP_

#include "condensation/Sampler.hpp"

namespace condensation {

/**
 * Creates new samples according to a grid (sliding-window like).
 */
class GridSampler : public Sampler {
public:

	/**
	 * Constructs a new grid sampler.
	 *
	 * @param[in] minSize The minimum size of a sample.
	 * @param[in] maxSize The maximum size of a sample.
	 * @param[in] sizeScale The scale factor of the size (beginning from the minimum size). Has to be greater than one.
	 * @param[in] stepSize The step size relative to the sample size.
	 */
	GridSampler(int minSize, int maxSize, float sizeScale, float stepSize);

	~GridSampler();

	void init(const Mat& image);

	void sample(const vector<Sample>& samples, vector<Sample>& newSamples, const Mat& image, const optional<Sample>& target);

private:

	int minSize;     ///< The minimum size of a sample.
	int maxSize;     ///< The maximum size of a sample.
	float sizeScale; ///< The scale factor of the size (beginning from the minimum size).
	float stepSize;  ///< The step size relative to the sample size.
};

} /* namespace condensation */
#endif /* GRIDSAMPLER_HPP_ */
