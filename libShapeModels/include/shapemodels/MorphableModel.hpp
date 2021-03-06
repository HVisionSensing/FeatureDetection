/*
 * MorphableModel.hpp
 *
 *  Created on: 30.09.2013
 *      Author: Patrik Huber
 */

#pragma once

#ifndef MORPHABLEMODEL_HPP_
#define MORPHABLEMODEL_HPP_

#include "shapemodels/PcaModel.hpp"

#include "render/Mesh.hpp"

#ifdef WIN32
	#define BOOST_ALL_DYN_LINK	// Link against the dynamic boost lib. Seems to be necessary because we use /MD, i.e. link to the dynamic CRT.
	#define BOOST_ALL_NO_LIB	// Don't use the automatic library linking by boost with VS2010 (#pragma ...). Instead, we specify everything in cmake.
#endif
#include "boost/property_tree/ptree.hpp"

namespace shapemodels {

/**
 * Desc
 */
class MorphableModel  {
public:

	/**
	 * Constructs a new Morphable Model.
	 *
	 * @param[in] a b
	 */
	MorphableModel();

	/**
	* Load a morphable model from a property tree node in a config file.
	* The function uses the file extension to determine which load
	* function to call.
	* Throws a std::runtime_exception if the extension is unrecognised.
	*
	* @param[in] configTree A node of a ptree.
	* @return A morphable model.
	*/
	static MorphableModel load(boost::property_tree::ptree configTree);
	
	/**
	 * Todo.
	 *
	 * @param[in] h5file Todo.
	 * @param[in] landmarkVertexMappingFile Todo.
	 * @return TODO.
	 */
	static MorphableModel loadScmModel(std::string h5file, std::string landmarkVertexMappingFile);

	static MorphableModel loadStatismoModel(std::string h5file);
	
	PcaModel getShapeModel() const;
	PcaModel getColorModel() const;

	/**
	 * Returns the mean of the shape- and color model
	 * as a Mesh.
	 *
	 * @return The mean of the model.
	 */
	render::Mesh getMean() const;

	/**
	 * Return the value of the mean at a given landmark.
	 *
	 * @param[in] landmarkIdentifier A landmark identifier (e.g. "center.nose.tip").
	 * @return A Vec3f containing the values at the given landmark.
	 * @throws out_of_range exception if the landmarkIdentifier does not exist in the model. // TODO test the javadoc!
	 */
	//cv::Vec3f getMeanAtPoint(std::string landmarkIdentifier) const;

	/**
	 * Return the value of the mean at a given vertex id.
	 *
	 * @param[in] vertexIndex A vertex id.
	 * @return A Vec3f containing the values at the given vertex id.
	 */
	//cv::Vec3f getMeanAtPoint(unsigned int vertexIndex) const;

	/**
	 * Draws a random sample from the model, where the coefficients
	 * for the shape- and color models are both drawn from a standard
	 * normal (or with the given standard deviation).
	 *
	 * @param[in] sigma The standard deviation. (TODO find out which one, sigma=var, sigmaSq=sdev)
	 * @return A random sample from the model.
	 */
	render::Mesh drawSample(float sigma = 1.0f); // Todo sigmaShape, sigmaColor? or 2 functions?

	/**
	 * Returns a sample from the model with the given shape- and
	 * color PCA coefficients. 
	 * If a vector is empty, the mean is used.
	 *
	 * @param[in] shapeCoefficients The PCA coefficients used to generate the shape sample.
	 * @param[in] colorCoefficients The PCA coefficients used to generate the shape sample.
	 * @return A model instance with given coefficients.
	 */
	render::Mesh drawSample(std::vector<float> shapeCoefficients, std::vector<float> colorCoefficients);
	
private:
	PcaModel shapeModel; ///< A PCA model over the shape
	PcaModel colorModel; ///< A PCA model over vertex color information

};

} /* namespace shapemodels */
#endif /* MORPHABLEMODEL_HPP_ */
