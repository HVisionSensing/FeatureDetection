/*
 * IbugLandmarkFormatParser.cpp
 *
 *  Created on: 05.11.2013
 *      Author: Patrik Huber
 */

#include "imageio/IbugLandmarkFormatParser.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/filesystem.hpp"
#include <stdexcept>
#include <utility>
#include <fstream>

using std::copy;
using std::sort;
using std::runtime_error;
using std::make_pair;
using boost::algorithm::trim;
using boost::algorithm::starts_with;
using boost::lexical_cast;
using boost::filesystem::path;
using std::ifstream;
using std::stringstream;
using std::make_shared;
using std::pair;

namespace imageio {

IbugLandmarkFormatParser::~IbugLandmarkFormatParser() {}

const map<path, LandmarkCollection> IbugLandmarkFormatParser::read(path landmarkFilePath)
{
	map<path, LandmarkCollection> allLandmarks;
	LandmarkCollection landmarks;

	ifstream ifLM(landmarkFilePath.string());
	string line;
	getline(ifLM, line); // Skip the first 3 lines, they're header lines
	getline(ifLM, line);
	getline(ifLM, line);

	int landmarkId = 1; // The landmarks are ordered in the .pts file
	while(getline(ifLM, line))
	{
		if (line == "}") {
			break;
		}
		stringstream ssLine(line);
		Vec3f position(0.0f, 0.0f, 0.0f);
		if (!(ssLine >> position[0] >> position[1])) {
			throw std::runtime_error("Landmark format error while parsing a line.");
		}
		// From the iBug website:
		// "Please note that the re-annotated data for this challenge are saved in the matlab convention of 1 being 
		// the first index, i.e. the coordinates of the top left pixel in an image are x=1, y=1."
		// ==> So we shift every point by 1:
		position[0] -= 1.0f;
		position[1] -= 1.0f;
		
		string landmarkIdentifier = lexical_cast<string>(landmarkId);
		bool visible = true; // In comparison to the original LFPW, this information is not available anymore.
		shared_ptr<Landmark> lm = make_shared<ModelLandmark>(landmarkIdentifier, position, visible);

		landmarks.insert(lm);
		
		++landmarkId;
	}

	path imageName = landmarkFilePath.stem();
	allLandmarks.insert(make_pair(imageName, landmarks));
	return allLandmarks;
}

} /* namespace imageio */
