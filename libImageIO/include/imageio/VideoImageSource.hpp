/*
 * VideoImageSource.hpp
 *
 *  Created on: 20.08.2012
 *      Author: poschmann
 */

#ifndef VIDEOIMAGESOURCE_HPP_
#define VIDEOIMAGESOURCE_HPP_

#include "imageio/ImageSource.hpp"
#include "opencv2/highgui/highgui.hpp"

using cv::VideoCapture;
using std::string;

namespace imageio {

/**
 * Image source that takes images from a video file.
 */
class VideoImageSource : public ImageSource {
public:

	/**
	 * Constructs a new video image source.
	 *
	 * @param[in] video The name of the video file.
	 */
	explicit VideoImageSource(string video);

	virtual ~VideoImageSource();

	void reset();

	bool next();

	const Mat getImage() const;

	path getName() const;

	vector<path> getNames() const;

private:

	string video;         ///< The name of the video file.
	VideoCapture capture; ///< The video capture.
	Mat frame;            ///< The current frame.
	unsigned long frameCounter; ///< The current frame number since the capture was started.
};

} /* namespace imageio */
#endif /* VIDEOIMAGESOURCE_HPP_ */
