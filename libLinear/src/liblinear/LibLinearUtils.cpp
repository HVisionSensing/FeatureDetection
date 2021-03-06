/*
 * LibLinearUtils.cpp
 *
 *  Created on: 03.12.2013
 *      Author: poschmann
 */

#include "liblinear/LibLinearUtils.hpp"
#include "linear.h"
#include <stdexcept>

using cv::Mat;
using std::vector;
using std::unique_ptr;
using std::unordered_map;
using std::invalid_argument;

namespace liblinear {

LibLinearUtils::LibLinearUtils() : matType(CV_32F), dimensions(0) {}

LibLinearUtils::~LibLinearUtils() {}

size_t LibLinearUtils::getDimensions() const {
	return dimensions;
}

unique_ptr<struct feature_node[]> LibLinearUtils::createNode(const Mat& vector, bool bias) const {
	matType = vector.type();
	dimensions = vector.total();
	unique_ptr<struct feature_node[]> node(new struct feature_node[dimensions + (bias ? 2 : 1)]);
	if (matType == CV_32F)
		fillNode<float>(node.get(), vector, dimensions);
	else if (matType == CV_64F)
		fillNode<double>(node.get(), vector, dimensions);
	else
		throw invalid_argument("LibLinearUtils: vector has to be of type CV_32F or CV_64F to create a node of");
	if (bias) {
		node[dimensions].index = dimensions + 1;
		node[dimensions].value = 1;
		node[dimensions + 1].index = -1;
	} else {
		node[dimensions].index = -1;
	}
	return move(node);
}

template<class T>
void LibLinearUtils::fillNode(struct feature_node *node, const Mat& vector, int size) const {
	if (!vector.isContinuous())
		throw invalid_argument("LibLinearUtils: vector has to be continuous");
	const T* values = vector.ptr<T>();
	for (int i = 0; i < size; ++i) {
		node[i].index = i + 1;
		node[i].value = values[i];
	}
}

double LibLinearUtils::computeSvmOutput(struct model *model, const struct feature_node *x) const {
	double* dec_values = new double[1];
	predict_values(model, x, dec_values);
	double svmOutput = dec_values[0];
	delete[] dec_values;
	return svmOutput;
}

vector<Mat> LibLinearUtils::extractSupportVectors(struct model *model) const {
	vector<Mat> supportVectors(1);
	supportVectors[0].create(1, model->nr_feature, matType);
	if (matType == CV_32F)
		extractWeightVector<float>(supportVectors[0], model);
	else if (matType == CV_64F)
		extractWeightVector<double>(supportVectors[0], model);
	return supportVectors;
}

template<class T>
void LibLinearUtils::extractWeightVector(Mat& vector, const struct model *model) const {
	T* values = vector.ptr<T>();
	for (int i = 0; i < model->nr_feature; ++i)
		values[i] = static_cast<T>(model->w[model->nr_class * i]);
}

vector<float> LibLinearUtils::extractCoefficients(struct model *model) const {
	vector<float> coefficients(1);
	coefficients[0] = 1;
	return coefficients;
}

double LibLinearUtils::extractBias(struct model *model) const {
	return model->bias >= 0 ? model->w[model->nr_class * model->nr_feature] : 0;
}

void ParameterDeleter::operator()(struct parameter *param) const {
	destroy_param(param);
	delete param;
}

void ProblemDeleter::operator()(struct problem *problem) const {
	delete[] problem->x;
	delete[] problem->y;
	delete problem;
}

void ModelDeleter::operator()(struct model *model) const {
	free_and_destroy_model(&model);
}

} /* namespace liblinear */
