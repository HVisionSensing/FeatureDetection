/*
 * DefaultLandmarkSink.hpp
 *
 *  Created on: 08.10.2013
 *      Author: Patrik Huber
 */
#pragma once

#ifndef DEFAULTLANDMARKSINK_HPP_
#define DEFAULTLANDMARKSINK_HPP_

#ifdef WIN32
	#define BOOST_ALL_DYN_LINK	// Link against the dynamic boost lib. Seems to be necessary because we use /MD, i.e. link to the dynamic CRT.
	#define BOOST_ALL_NO_LIB	// Don't use the automatic library linking by boost with VS2010 (#pragma ...). Instead, we specify everything in cmake.
#endif
#include "boost/filesystem/path.hpp"


namespace imageio {

class LandmarkCollection;

/**
 * Landmark source that writes rectangular landmarks to a
 * file using a format of one line per landmark in the form 
 * of 'landmarkName topLeftX topLeftY width height'.
 * Generates one landmark file for each image (i.e. for
 * each call of the 'write' method).
 *
 */
class DefaultLandmarkSink {
public:

	/**
	 * Constructs a new default landmark sink.
	 *
	 * @param[in] outputPath A path to the folder to which the landmark files should be written.
	 */
	explicit DefaultLandmarkSink(const boost::filesystem::path& outputPath);

	~DefaultLandmarkSink();

	/**
	 * Creates a file and writes the landmarks to it.
	 *
	 * @param[in] collection All the rect landmarks to write to the file.
	 * @param[in] imageFilename The filename of the image. Only the basename will be used.
	 */
	void write(const LandmarkCollection& collection, const boost::filesystem::path imageFilename);

private:

	const boost::filesystem::path outputPath; ///< The folder to which the landmark files should be saved.

};

} /* namespace imageio */
#endif /* DEFAULTLANDMARKSINK_HPP_ */
