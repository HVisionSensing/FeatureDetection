/*
 * LandmarkBasedSupervisedDescentTraining.hpp
 *
 *  Created on: 04.02.2014
 *      Author: Patrik Huber
 */

#pragma once

#ifndef LANDMARKBASEDSUPERVISEDDESCENTTRAINING_HPP_
#define LANDMARKBASEDSUPERVISEDDESCENTTRAINING_HPP_

#include "shapemodels/SdmLandmarkModel.hpp"

#include "opencv2/core/core.hpp"

namespace shapemodels {

/**
 * Desc
 * The class provides reasonable default arguments, so when calling
 * train without setting any, it works.
 * However, more detailed stuff can be set by the setters.
 */
class LandmarkBasedSupervisedDescentTraining  {
public:

	/**
	 * Constructs a new LandmarkBasedSupervisedDescentTraining.
	 *
	 * @param[in] a b
	 */
	//LandmarkBasedSupervisedDescentTraining() {};

	struct GaussParameter {
		double mu = 0.0;
		double sigma = 1.0; // Note: sigma = stddev. sigma^2 = variance.
							// Notation is $\mathcal{N}(mu, sigma^2)$ (mean, variance).
							// std::normal_distribution takes (mu, sigma) as arguments.
	};

	// Todo: Note sure what exactly this measures. Think about it.
	struct ModelVariance {
		GaussParameter tx; // translation in x-direction
		GaussParameter ty; // ...
		GaussParameter sx;
		GaussParameter sy;
	};

	// Holds the regularisation parameters for the training.
	struct Regularisation {
		float factor = 0.5f;
		bool regulariseAffineComponent = false;
		bool regulariseWithEigenvalueThreshold = false;
	};

	enum class AlignGroundtruth { // what to do with the GT LMs before mean is taken.
		NONE, // no prealign, stay in img-coords
		NORMALIZED_FACEBOX// translate/scale to facebox, that is a normalized square [-0.5, ...] x ...
	};

	enum class MeanNormalization { // what to do with the mean coords after the mean has been calculated
		NONE,
		UNIT_SUM_SQUARED_NORMS // orig paper
	};

	// deals with both row and col vecs. Assumes first half x, second y.
	void saveShapeInstanceToMLtxt(cv::Mat shapeInstance, std::string filename);

	cv::Mat calculateMean(std::vector<cv::Mat> landmarks, AlignGroundtruth alignGroundtruth, MeanNormalization meanNormalization, std::vector<cv::Rect> faceboxes=std::vector<cv::Rect>());

	// trainingImages: debug only
	// trainingFaceboxes: for normalizing the variances by the face-box
	// groundtruthLandmarks, initialShapeEstimateX0: calc variances
	ModelVariance calculateModelVariance(std::vector<cv::Mat> trainingImages, std::vector<cv::Rect> trainingFaceboxes, cv::Mat groundtruthLandmarks, cv::Mat initialShapeEstimateX0);

	// Rescale the model-mean, and the mean variances as well: (only necessary if our mean is not normalized to V&J face-box directly in first steps)
	std::pair<cv::Mat, ModelVariance> rescaleModel(cv::Mat modelMean, ModelVariance modelVariance);

	cv::Mat putInDataAndGenerateSamples(std::vector<cv::Mat> trainingImages, std::vector<cv::Rect> trainingFaceboxes, cv::Mat modelMean, cv::Mat initialShape, ModelVariance modelVariance, int numSamplesPerImage);

	// TODO: Move to MatHelpers::duplicateRows(...)
	Mat duplicateGroundtruthShapes(Mat groundtruthLandmarks, int numSamplesPerImage) {
		Mat groundtruthShapes = Mat::zeros((numSamplesPerImage + 1) * groundtruthLandmarks.rows, groundtruthLandmarks.cols, CV_32FC1); // 10 samples + the original data = 11
		for (int currImg = 0; currImg < groundtruthLandmarks.rows; ++currImg) {
			Mat groundtruthLandmarksRow = groundtruthLandmarks.row(currImg);
			for (int j = 0; j < numSamplesPerImage + 1; ++j) {
				Mat groundtruthShapesRow = groundtruthShapes.row(currImg*(numSamplesPerImage + 1) + j);
				groundtruthLandmarksRow.copyTo(groundtruthShapesRow);
			}
		}
		return groundtruthShapes;
	};

	void setNumSamplesPerImage(int numSamplesPerImage) {
		this->numSamplesPerImage = numSamplesPerImage;
	};

	void setNumCascadeSteps(int numCascadeSteps) {
		this->numCascadeSteps = numCascadeSteps;
	};

	void setRegularisation(Regularisation regularisation) {
		this->regularisation = regularisation;
	};

	void setAlignGroundtruth(AlignGroundtruth alignGroundtruth) {
		this->alignGroundtruth = alignGroundtruth;
	};

	void setMeanNormalization(MeanNormalization meanNormalization) {
		this->meanNormalization = meanNormalization;
	};

public:

	SdmLandmarkModel train(std::vector<cv::Mat> trainingImages, std::vector<cv::Mat> trainingGroundtruthLandmarks, std::vector<cv::Rect> trainingFaceboxes /*maybe optional bzw weglassen hier?*/, std::vector<std::string> modelLandmarks, std::vector<std::string> descriptorTypes, std::vector<std::shared_ptr<FeatureDescriptorExtractor>> descriptorExtractors);
	
private:
	int numSamplesPerImage = 10; ///< todo
	int numCascadeSteps = 5; ///< todo
	Regularisation regularisation; ///< todo
	AlignGroundtruth alignGroundtruth = AlignGroundtruth::NONE; ///< For mean calc: todo
	MeanNormalization meanNormalization = MeanNormalization::UNIT_SUM_SQUARED_NORMS; ///< F...Mean: todo

	// Transforms one row...
	// Takes the face-box as [-0.5, 0.5] x [-0.5, 0.5] and transforms the landmarks into that rectangle.
	// lms are x1 x2 .. y1 y2 .. row-vec
	cv::Mat transformLandmarksNormalized(cv::Mat landmarks, cv::Rect box);

	// assumes modelMean is row-vec, first half x, second y.
	cv::Mat meanNormalizationUnitSumSquaredNorms(cv::Mat modelMean);

	// public?
	// Initial estimate x_0: Center the mean face at the [-0.5, 0.5] x [-0.5, 0.5] square (assuming the face-box is that square)
	// More precise: Take the mean as it is (assume it is in a space [-0.5, 0.5] x [-0.5, 0.5]), and just place it in the face-box as
	// if the box is [-0.5, 0.5] x [-0.5, 0.5]. (i.e. the mean coordinates get upscaled)
	// - makes a copy of mean, not inplace
	// - optional: scaling/trans that gets added to the mean (before scaling up to the facebox)
	// Todo/Note: Is this the same as in SdmModel::alignRigid?
	cv::Mat alignMean(cv::Mat mean, cv::Rect faceBox, float scalingX=1.0f, float scalingY=1.0f, float translationX=0.0f, float translationY=0.0f);

	float calculateTranslationVariance(cv::Mat groundtruth, cv::Mat estimate, float normalization);

	float calculateScaleVariance(cv::Mat groundtruth, cv::Mat estimate);

};

} /* namespace shapemodels */
#endif /* LANDMARKBASEDSUPERVISEDDESCENTTRAINING_HPP_ */
