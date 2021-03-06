/*
 * SingleClassifierModel.cpp
 *
 *  Created on: 28.03.2013
 *      Author: poschmann
 */

#include "condensation/SingleClassifierModel.hpp"
#include "condensation/Sample.hpp"
#include "imageprocessing/Patch.hpp"
#include "imageprocessing/FeatureExtractor.hpp"
#include "classification/ProbabilisticClassifier.hpp"

using std::make_shared;

namespace condensation {

SingleClassifierModel::SingleClassifierModel(shared_ptr<FeatureExtractor> featureExtractor,
		shared_ptr<ProbabilisticClassifier> classifier) :
				featureExtractor(featureExtractor), classifier(classifier), cache() {}

SingleClassifierModel::~SingleClassifierModel() {}

void SingleClassifierModel::update(shared_ptr<VersionedImage> image) {
	cache.clear();
	featureExtractor->update(image);
}

void SingleClassifierModel::evaluate(Sample& sample) {
	shared_ptr<Patch> patch = featureExtractor->extract(sample.getX(), sample.getY(), sample.getWidth(), sample.getHeight());
	if (patch) {
		pair<bool, double> result = classify(patch);
		sample.setObject(result.first);
		sample.setWeight(result.second);
	} else {
		sample.setObject(false);
		sample.setWeight(0);
	}
}

pair<bool, double> SingleClassifierModel::classify(shared_ptr<Patch> patch) {
	auto resIt = cache.find(patch);
	if (resIt == cache.end()) {
		pair<bool, double> result = classifier->getProbability(patch->getData());
		cache.emplace(patch, result);
		return result;
	}
	return resIt->second;
}

} /* namespace condensation */
