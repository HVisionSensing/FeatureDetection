/*
 * ParallelFilter.hpp
 *
 *  Created on: 08.08.2013
 *      Author: poschmann
 */

#ifndef PARALLELFILTER_HPP_
#define PARALLELFILTER_HPP_

#include "imageprocessing/ImageFilter.hpp"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::vector;

namespace imageprocessing {

/**
 * Image filter that applies several filters on the input image and combines the results by merging the result's channels
 * into a single image. The size and depth of the filter results have to be equal.
 */
class ParallelFilter : public ImageFilter {
public:

	/**
	 * Constructs a new parallel image filter without filters.
	 */
	ParallelFilter();

	/**
	 * Constructs a new parallel image filter.
	 *
	 * @param[in] filters The image filters whose results should be combined.
	 */
	explicit ParallelFilter(vector<shared_ptr<ImageFilter>> filters);

	/**
	 * Constructs a new parallel image filter that combines the results of two filters.
	 *
	 * @param[in] filter1 The first image filter.
	 * @param[in] filter2 The second image filter.
	 */
	ParallelFilter(shared_ptr<ImageFilter> filter1, shared_ptr<ImageFilter> filter2);

	/**
	 * Constructs a new parallel image filter that combines the results of three filters.
	 *
	 * @param[in] filter1 The first image filter.
	 * @param[in] filter2 The second image filter.
	 * @param[in] filter3 The third image filter.
	 */
	ParallelFilter(shared_ptr<ImageFilter> filter1, shared_ptr<ImageFilter> filter2, shared_ptr<ImageFilter> filter3);

	~ParallelFilter();

	/**
	 * Adds a new filter whose result should be combined with other filters.
	 *
	 * @param[in] filter The new filter.
	 */
	void add(shared_ptr<ImageFilter> filter);

	using ImageFilter::applyTo;

	Mat applyTo(const Mat& image, Mat& filtered) const;

private:

	vector<shared_ptr<ImageFilter>> filters; ///< The image filters whose results should be combined.
};

} /* namespace imageprocessing */
#endif /* PARALLELFILTER_HPP_ */
