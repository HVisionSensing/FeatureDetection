/*
 * BobotLandmarkSource.cpp
 *
 *  Created on: 22.05.2013
 *      Author: poschmann
 */

#include "imageio/BobotLandmarkSource.hpp"
#include "imageio/ImageSource.hpp"
#include "imageio/RectLandmark.hpp"
#include <fstream>
#include <sstream>
#include <utility>
#include <memory>

using std::make_pair;
using std::make_shared;

namespace imageio {

const string BobotLandmarkSource::landmarkName = "target";

BobotLandmarkSource::BobotLandmarkSource(shared_ptr<ImageSource> imageSource, const string& filename) :
		imageSource(imageSource), positions(), name2index(), index(-1), collection() {
	string name;
	string line;
	std::ifstream file(filename.c_str());
	if (file.is_open()) {
		if (file.good())
			std::getline(file, line); // ignore first line (contains video file name)
		Rect_<float> position;
		while (file.good()) {
			if (!std::getline(file, line))
				break;
			// read values from line
			std::istringstream lineStream(line);
			lineStream >> name;
			lineStream >> position.x;
			lineStream >> position.y;
			lineStream >> position.width;
			lineStream >> position.height;
			positions.push_back(position);
			name2index.emplace(name, positions.size() - 1);
		}
	}
}

void BobotLandmarkSource::reset() {
	index = -1;
}

bool BobotLandmarkSource::next() {
	index++;
	return index < static_cast<int>(positions.size());
}

LandmarkCollection BobotLandmarkSource::get() {
	next();
	return getLandmarks();
}

LandmarkCollection BobotLandmarkSource::get(const path& imagePath) {
	auto iterator = name2index.find(imagePath.string());
	if (iterator == name2index.end())
		index = -1;
	else
		index = static_cast<int>(iterator->second);
	return getLandmarks();
}

LandmarkCollection BobotLandmarkSource::getLandmarks() const {
	collection.clear(); // Todo: As we don't return a reference anymore, I think this could now be a local variable.
	if (index >= 0 && index < static_cast<int>(positions.size())) {
		const cv::Mat& image = imageSource->getImage();
		const Rect_<float>& relativePosition = positions[index];
		if (relativePosition.x == 0 && relativePosition.y == 0 && relativePosition.width == 0 && relativePosition.height == 0) {
			collection.insert(make_shared<RectLandmark>(landmarkName)); // invisible landmark
		} else {
			cv::Rect_<float> rect(relativePosition.x * image.cols, relativePosition.y * image.rows,
					relativePosition.width * image.cols, relativePosition.height * image.rows);
			collection.insert(make_shared<RectLandmark>(landmarkName, rect));
		}
	}
	return collection;
}

} /* namespace imageio */
