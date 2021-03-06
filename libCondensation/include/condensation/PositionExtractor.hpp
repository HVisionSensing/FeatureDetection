/*
 * PositionExtractor.hpp
 *
 *  Created on: 19.07.2012
 *      Author: poschmann
 */

#ifndef POSITIONEXTRACTOR_HPP_
#define POSITIONEXTRACTOR_HPP_

#include "condensation/Sample.hpp"
#include "boost/optional.hpp"
#include <vector>

using boost::optional;
using std::vector;

namespace condensation {

/**
 * Extractor of a bounding box around the estimated object position.
 */
class PositionExtractor {
public:

	virtual ~PositionExtractor() {}

	/**
	 * Estimates the most probable object position from a set of samples.
	 *
	 * @param[in] samples The samples.
	 * @return The the most probable object position if there is one.
	 */
	virtual optional<Sample> extract(const vector<Sample>& samples) = 0;
};

} /* namespace condensation */
#endif /* POSITIONEXTRACTOR_HPP_ */
