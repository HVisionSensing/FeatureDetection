/*
 * ZeroMeanUnitVarianceFilter.hpp
 *
 *  Created on: 21.02.2013
 *      Author: poschmann
 */

#ifndef ZEROMEANUNITVARIANCEFILTER_HPP_
#define ZEROMEANUNITVARIANCEFILTER_HPP_

#include "imageprocessing/ImageFilter.hpp"

namespace imageprocessing {

/**
 * Image filter that normalizes the pixel values to have zero mean and unit variance. The input
 * image must not have more than one channel. The filtered image is of type CV_32F.
 */
class ZeroMeanUnitVarianceFilter : public ImageFilter {
public:

	/**
	 * Constructs a new zero-mean unit-variance filter.
	 */
	ZeroMeanUnitVarianceFilter();

	~ZeroMeanUnitVarianceFilter();

	using ImageFilter::applyTo;

	Mat applyTo(const Mat& image, Mat& filtered);

	void applyInPlace(Mat& image);
};

} /* namespace imageprocessing */
#endif /* ZEROMEANUNITVARIANCEFILTER_HPP_ */
