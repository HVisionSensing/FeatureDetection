/*
 * LandmarkFileGatherer.hpp
 *
 *  Created on: 28.04.2013
 *      Author: Patrik Huber
 */

#ifndef LANDMARKFILEGATHERER_HPP_
#define LANDMARKFILEGATHERER_HPP_

#include "imageio/ImageSource.hpp"
#ifdef WIN32
	#define BOOST_ALL_DYN_LINK	// Link against the dynamic boost lib. Seems to be necessary because we use /MD, i.e. link to the dynamic CRT.
	#define BOOST_ALL_NO_LIB	// Don't use the automatic library linking by boost with VS2010 (#pragma ...). Instead, we specify everything in cmake.
#endif
#include "boost/filesystem.hpp"
#include <map>
#include <memory>
#include <string>

using boost::filesystem::path;
using std::map;
using std::shared_ptr;
using std::string;

namespace imageio {

/**
 * Represents the different possible methods for gathering landmark files.
 */
enum class GatherMethod { // Case inconsistent with "loglevels"
	SEPARATE_FILES,		// Use one or more separate, user-specified files.
	ONE_FILE_PER_IMAGE_SAME_DIR,	// Look for the landmark file in the same directory as the image resides.
	ONE_FILE_PER_IMAGE_DIFFERENT_DIRS // Look for the landmark file in the directory of the image as well as
									  // in all additional directories specified. Uses the first occurence of the file.
	// Todo: separate SAME_DIR / DIFFERENT_DIRS, assign powers of 2 and use logical operators?
};

class LandmarkCollection;
class FilebasedImageSource;
class LandmarkFormatParser;

/**
 * Provides different means of gathering a list of landmark-files
 * from different files and/or directories.
 */
class LandmarkFileGatherer {
public:

	LandmarkFileGatherer();

	virtual ~LandmarkFileGatherer();

	/**
	 * Gathers a list of landmark-files from different files and/or directories,
	 * depending on the input parameters.
	 *
	 * Note: Do we want to load all landmark-files at the start, or just load each one
	 *       when LandmarkSource::get(path) is called? Advantage of the latter would be
	 *       that we already know the path. Disadvantage: Maybe more complicated in the
	 *       case we load all landmarks from one image? And we should not do at run-time
	 *		 what we can do at load-time.
	 *
	 * Todo: ImageSource could also be an optional parameter (only needed for GatherMethod::ONE_FILE_PER_IMAGE_*)
	 *
	 * @param[in] imageSource An ImageSource, needed for knowing the filenames to load.
	 * @param[in] fileFxtension The file extension of the landmark files to load. TODO add doku: with or without the dot? (.tlms or tlms?)
	 * @param[in] gatherMethod The method with which to gather the landmark files.
	 * @param[in] additionalPaths One or more additional paths to files or directories, depending on the type of GatherMethod. Default: empty vector.
	 * @return A vector containing the full paths to all the landmark files gathered.
	 */
	static vector<path> gather(const shared_ptr<const ImageSource> imageSource, const string fileExtension, const GatherMethod gatherMethod, const vector<path> additionalPaths=vector<path>());

};

} /* namespace imageio */
#endif /* LANDMARKFILEGATHERER_HPP_ */
