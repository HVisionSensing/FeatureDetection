/*
 * MorphableModel.cpp
 *
 *  Created on: 30.09.2013
 *      Author: Patrik Huber
 */

#include "shapemodels/MorphableModel.hpp"
#include "logging/LoggerFactory.hpp"
#include "opencv2/core/core.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/filesystem/path.hpp"
#include <exception>
#include <fstream>

using logging::LoggerFactory;
using cv::Mat;
using cv::Vec3f;
using cv::Vec4f;
using boost::lexical_cast;
using std::vector;
using std::string;

namespace shapemodels {

MorphableModel::MorphableModel()
{
	
}

shapemodels::MorphableModel MorphableModel::load(boost::property_tree::ptree configTree)
{
	MorphableModel morphableModel;
	boost::filesystem::path filename = configTree.get<string>("filename");
	if (filename.extension().string() == ".scm") {
		string vertexMappingFile = configTree.get<string>("vertexMapping");
		morphableModel = shapemodels::MorphableModel::loadScmModel(filename.string(), vertexMappingFile);
	}
	else if (filename.extension().string() == ".h5") {
		morphableModel = shapemodels::MorphableModel::loadStatismoModel(filename.string());
	}
	else
	{
		throw std::runtime_error("MorphableModel: Unknown file extension. Neither .scm nor .h5.");
	}
	return morphableModel;
}

shapemodels::MorphableModel MorphableModel::loadScmModel(std::string h5file, std::string landmarkVertexMappingFile)
{
	MorphableModel model;
	model.shapeModel = PcaModel::loadScmModel(h5file, landmarkVertexMappingFile, PcaModel::ModelType::SHAPE);
	model.colorModel = PcaModel::loadScmModel(h5file, landmarkVertexMappingFile, PcaModel::ModelType::COLOR);
	return model;
}

shapemodels::MorphableModel MorphableModel::loadStatismoModel(std::string h5file)
{
	MorphableModel model;
	model.shapeModel = PcaModel::loadStatismoModel(h5file, PcaModel::ModelType::SHAPE);
	model.colorModel = PcaModel::loadStatismoModel(h5file, PcaModel::ModelType::COLOR);
	return model;
}


shapemodels::PcaModel MorphableModel::getShapeModel() const
{
	return shapeModel;
}

shapemodels::PcaModel MorphableModel::getColorModel() const
{
	return colorModel;
}

render::Mesh MorphableModel::getMean() const
{
	render::Mesh mean;
	
	mean.tvi = shapeModel.getTriangleList();
	mean.tci = colorModel.getTriangleList();
	
	Mat shapeMean = shapeModel.getMean();
	Mat colorMean = colorModel.getMean();

	unsigned int numVertices = shapeModel.getDataDimension() / 3;
	unsigned int numVerticesColor = colorModel.getDataDimension() / 3;
	if (numVertices != numVerticesColor) {
		// TODO throw ERROR!
	}

	mean.vertex.resize(numVertices);

	for (unsigned int i = 0; i < numVertices; ++i) {
		mean.vertex[i].position = Vec4f(shapeMean.at<float>(i*3 + 0), shapeMean.at<float>(i*3 + 1), shapeMean.at<float>(i*3 + 2), 1.0f);
		mean.vertex[i].color = Vec3f(colorMean.at<float>(i*3 + 0), colorMean.at<float>(i*3 + 1), colorMean.at<float>(i*3 + 2));        // order in hdf5: RGB. Order in OCV: BGR. But order in vertex.color: RGB
	}

	mean.hasTexture = false;

	return mean;
}

render::Mesh MorphableModel::drawSample(float sigma /*= 1.0f*/)
{
	render::Mesh mean;

	mean.tvi = shapeModel.getTriangleList();
	mean.tci = colorModel.getTriangleList();

	Mat shapeMean = shapeModel.drawSample(sigma);
	Mat colorMean = colorModel.drawSample(sigma);

	unsigned int numVertices = shapeModel.getDataDimension() / 3;
	unsigned int numVerticesColor = colorModel.getDataDimension() / 3;
	if (numVertices != numVerticesColor) {
		string msg("MorphableModel: The number of vertices of the shape and color models are not the same: " + lexical_cast<string>(numVertices) + " != " + lexical_cast<string>(numVerticesColor));
		Loggers->getLogger("shapemodels").debug(msg);
		throw std::runtime_error(msg);
	}

	mean.vertex.resize(numVertices);

	for (unsigned int i = 0; i < numVertices; ++i) {
		mean.vertex[i].position = Vec4f(shapeMean.at<float>(i*3 + 0), shapeMean.at<float>(i*3 + 1), shapeMean.at<float>(i*3 + 2), 1.0f);
		mean.vertex[i].color = Vec3f(colorMean.at<float>(i*3 + 0), colorMean.at<float>(i*3 + 1), colorMean.at<float>(i*3 + 2));        // order in hdf5: RGB. Order in OCV: BGR. But order in vertex.color: RGB
	}

	mean.hasTexture = false;

	return mean;
}

render::Mesh MorphableModel::drawSample(vector<float> shapeCoefficients, vector<float> colorCoefficients)
{
	render::Mesh mean;

	mean.tvi = shapeModel.getTriangleList();
	mean.tci = colorModel.getTriangleList();

	Mat shapeSample;
	Mat colorSample;

	if (shapeCoefficients.empty()) {
		shapeSample = shapeModel.getMean();
	} else {
		shapeSample = shapeModel.drawSample(shapeCoefficients);
	}
	if (colorCoefficients.empty()) {
		colorSample = colorModel.getMean();
	} else {
		colorSample = colorModel.drawSample(colorCoefficients);
	}

	unsigned int numVertices = shapeModel.getDataDimension() / 3;
	unsigned int numVerticesColor = colorModel.getDataDimension() / 3;
	if (numVertices != numVerticesColor) {
		string msg("MorphableModel: The number of vertices of the shape and color models are not the same: " + lexical_cast<string>(numVertices) + " != " + lexical_cast<string>(numVerticesColor));
		Loggers->getLogger("shapemodels").debug(msg);
		throw std::runtime_error(msg);
	}

	mean.vertex.resize(numVertices);

	for (unsigned int i = 0; i < numVertices; ++i) {
		mean.vertex[i].position = Vec4f(shapeSample.at<float>(i*3 + 0), shapeSample.at<float>(i*3 + 1), shapeSample.at<float>(i*3 + 2), 1.0f);
		mean.vertex[i].color = Vec3f(colorSample.at<float>(i*3 + 0), colorSample.at<float>(i*3 + 1), colorSample.at<float>(i*3 + 2));        // order in hdf5: RGB. Order in OCV: BGR. But order in vertex.color: RGB
	}

	mean.hasTexture = false;

	return mean;
}

/*
unsigned int matIdx = 0;
for (auto& v : mesh.vertex) {
v.position = Vec4f(modelSample.at<double>(matIdx), modelSample.at<double>(matIdx+1), modelSample.at<double>(matIdx+2), 1.0f);
matIdx += 3;
}
*/

}