/*
 * ProbabilisticSvmClassifier.hpp
 *
 *  Created on: 25.02.2013
 *      Author: Patrik Huber
 */
#pragma once

#ifndef PROBABILISTICSVMCLASSIFIER_HPP_
#define PROBABILISTICSVMCLASSIFIER_HPP_

#include "classification/ProbabilisticClassifier.hpp"
#include "boost/property_tree/ptree.hpp"
#include <memory>

using boost::property_tree::ptree;
using std::shared_ptr;
using std::string;

namespace classification {

class SvmClassifier;

/**
 * SVM classifier that produces pseudo-probabilistic output. The hyperplane distance of a feature vector will be transformed
 * into a probability using a logistic function p(x) = 1 / (1 + exp(a + b * x)) with x being the hyperplane distance and a and
 * b being parameters.
 */
class ProbabilisticSvmClassifier : public ProbabilisticClassifier {
public:

	/**
	 * Constructs a new empty probabilistic SVM classifier.
	 */
	ProbabilisticSvmClassifier();

	/**
	 * Constructs a new probabilistic SVM classifier.
	 *
	 * @param[in] svm The actual SVM.
	 * @param[in] logisticA Parameter a of the logistic function for pseudo-probabilistic output p(x) = 1 / (1 + exp(a + b * x)).
	 * @param[in] logisticB Parameter b of the logistic function for pseudo-probabilistic output p(x) = 1 / (1 + exp(a + b * x)).
	 */
	explicit ProbabilisticSvmClassifier(shared_ptr<SvmClassifier> svm, double logisticA = 0.00556, double logisticB = -2.95);

	~ProbabilisticSvmClassifier();

	bool classify(const Mat& featureVector) const;

	pair<bool, double> getConfidence(const Mat& featureVector) const;

	pair<bool, double> getProbability(const Mat& featureVector) const;

	/**
	 * Changes the logistic parameters of this probabilistic SVM.
	 *
	 * @param[in] logisticA Parameter a of the logistic function for pseudo-probabilistic output p(x) = 1 / (1 + exp(a + b * x)).
	 * @param[in] logisticB Parameter b of the logistic function for pseudo-probabilistic output p(x) = 1 / (1 + exp(a + b * x)).
	 */
	void setLogisticParameters(double logisticA, double logisticB);

	/**
	 * Creates a new probabilistic WVM classifier from the parameters given in some Matlab file. Loads the logistic function's
	 * parameters from the matlab file, then passes the loading to the underlying WVM which loads the vectors and thresholds
	 * from the matlab file. TODO update doc
	 *
	 * @param[in] classifierFilename The name of the file containing the WVM parameters.
	 * @param[in] thresholdsFilename The name of the file containing the thresholds of the filter levels and the logistic function's parameters.
	 * @return The newly created probabilistic WVM classifier.
	 */
	static std::pair<double, double> loadSigmoidParamsFromMatlab(const string& thresholdsFilename);

	/**
	 * Creates a new probabilistic SVM classifier from the parameters given in some Matlab file. Loads the logistic function's
	 * parameters from the matlab file, then passes the loading to the underlying SVM which loads the vectors and thresholds
	 * from the matlab file.
	 *
	 * @param[in] classifierFilename The name of the file containing the SVM parameters.
	 * @param[in] logisticFilename The name of the file containing the logistic function's parameters.
	 * @return The newly created probabilistic SVM classifier. TODO: This could be renamed just to "load(...)". But NOTE: The classifier will then be loaded with
	 * default settings, and any deviation from that (e.g. adjusting the thresholds) must be done manually.
	 */
	static shared_ptr<ProbabilisticSvmClassifier> loadFromMatlab(const string& classifierFilename, const string& logisticFilename);

	/**
	 * Creates a new probabilistic SVM classifier from the parameters given in the ptree sub-tree. Loads the logistic function's
	 * parameters, then passes the loading to the underlying SVM which loads the vectors and thresholds
	 * from the matlab file.
	 *
	 * @param[in] subtree The subtree containing the config information for this classifier.
	 * @return The newly created probabilistic WVM classifier.
	 */
	static shared_ptr<ProbabilisticSvmClassifier> load(const ptree& subtree);

	/**
	 * @return The actual SVM.
	 */
	shared_ptr<SvmClassifier> getSvm() {
		return svm;
	}

	/**
	 * @return The actual SVM.
	 */
	const shared_ptr<SvmClassifier> getSvm() const {
		return svm;
	}

private:

	shared_ptr<SvmClassifier> svm; ///< The actual SVM.
	double logisticA; ///< Parameter a of the logistic function for pseudo-probabilistic output p(x) = 1 / (1 + exp(a + b * x)).
	double logisticB; ///< Parameter b of the logistic function for pseudo-probabilistic output p(x) = 1 / (1 + exp(a + b * x)).
};

} /* namespace classification */
#endif /* PROBABILISTICSVMCLASSIFIER_HPP_ */

