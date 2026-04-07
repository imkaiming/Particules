#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#include "../framework/Core.h"
#include "../framework/audio/LookUpTable.h"
#include "../utils/math/EnvelopeMath.h"
#include "../utils/math/Lerp.h"
#include "../utils/math/MathConstants.h"

// SVG Generation

struct SVGConfig
{
    int width = 400;
    int height = 200;
    int padding = 20;
    std::string strokeColor = "black";
    int strokeWidth = 2;
    std::string backgroundColor = "white";
};

// Function to generate an SVG path from a LookUpTable
std::string generateSVGPath(const particules::LookUpTable& lut, const SVGConfig& config)
{
    int plotWidth = config.width - 2 * config.padding;
    int plotHeight = config.height - 2 * config.padding;

    std::string pathData = "M ";

    // Standardizing on number of points to sample for visualization (e.g., 200 points)
    const int numSamples = 200;
    for(int i = 0; i < numSamples; ++i)
    {
        float phase = static_cast<float>(i) / (numSamples - 1);
        float value = lut.getValue(phase);

        // Map phase to X coordinate
        float x = config.padding + phase * plotWidth;
        // Map value to Y coordinate (invert Y because SVG origin is top-left)
        float y = config.padding + (1.0f - value) * plotHeight;

        pathData += std::to_string(x) + " " + std::to_string(y) + " ";
        if(i == 0)
            pathData += "L ";
    }

    return pathData;
}

// Function to save the full SVG file
void saveSVGFile(const std::string& filename, const std::string& pathData, const std::string& title, const SVGConfig& config)
{
    std::ofstream file(filename);
    if(!file.is_open())
    {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
    file << "<svg width=\"" << config.width << "\" height=\"" << config.height << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";

    // Background and Title
    //file << "  <rect width=\"100%\" height=\"100%\" fill=\"" << config.backgroundColor << "\" />\n";
    //file
    //    << "  <text x=\"50%\" y=\"15\" dominant-baseline=\"middle\" text-anchor=\"middle\" font-family=\"sans-serif\" font-size=\"12px\" fill=\"black\">"
    //    << title << "</text>\n";

    // Axes
    float xAxisY = config.height - config.padding;
    float yAxisX = config.padding;
    file << "  <line x1=\"" << yAxisX << "\" y1=\"" << xAxisY << "\" x2=\"" << (config.width - config.padding) << "\" y2=\""
         << xAxisY << "\" stroke=\"gray\" stroke-width=\"1\" />\n"; // X-axis
    file << "  <line x1=\"" << yAxisX << "\" y1=\"" << config.padding << "\" x2=\"" << yAxisX << "\" y2=\"" << xAxisY
         << "\" stroke=\"gray\" stroke-width=\"1\" />\n"; // Y-axis

    // Grid labels (0 and 1)
    file << "  <text x=\"" << (yAxisX - 5) << "\" y=\"" << (config.padding + 5)
         << "\" text-anchor=\"end\" font-family=\"sans-serif\" font-size=\"10px\" fill=\"gray\">1.0</text>\n";
    file << "  <text x=\"" << (yAxisX - 5) << "\" y=\"" << xAxisY
         << "\" text-anchor=\"end\" font-family=\"sans-serif\" font-size=\"10px\" fill=\"gray\">0.0</text>\n";
    file << "  <text x=\"" << yAxisX << "\" y=\"" << (xAxisY + 12)
         << "\" text-anchor=\"middle\" font-family=\"sans-serif\" font-size=\"10px\" fill=\"gray\">0.0</text>\n";
    file << "  <text x=\"" << (config.width - config.padding) << "\" y=\"" << (xAxisY + 12)
         << "\" text-anchor=\"middle\" font-family=\"sans-serif\" font-size=\"10px\" fill=\"gray\">1.0</text>\n";

    // Plotting the curve
    file << "  <path d=\"" << pathData << "\" stroke=\"" << config.strokeColor << "\" stroke-width=\"" << config.strokeWidth
         << "\" fill=\"none\" />\n";

    file << "</svg>";
    file.close();
    std::cout << "Generated: " << filename << std::endl;
}

int main()
{
    SVGConfig config;

    particules::LookUpTable hannLUT, linearLUT, sqrtLUT, gaussianLUT, expLUT;

    hannLUT.populate(particules::initHann);
    linearLUT.populate(particules::initLinear);
    sqrtLUT.populate(particules::initSqrt);
    gaussianLUT.populate(particules::initGaussian);
    expLUT.populate(particules::initExp);

    saveSVGFile("env_hann.svg", generateSVGPath(hannLUT, config), "Hann Envelope", config);
    saveSVGFile("env_linear.svg", generateSVGPath(linearLUT, config), "Linear (Triangle) Envelope", config);
    saveSVGFile("env_sqrt.svg", generateSVGPath(sqrtLUT, config), "Sqrt (Concave) Envelope", config);
    saveSVGFile("env_gaussian.svg", generateSVGPath(gaussianLUT, config), "Gaussian Envelope (Sigma 0.18)", config);
    saveSVGFile("env_exp.svg", generateSVGPath(expLUT, config), "Double-Exponential (K=6) Envelope", config);

    std::cout << "\nGeneration complete. Open the .svg files in a web browser to view." << std::endl;

    return 0;
}