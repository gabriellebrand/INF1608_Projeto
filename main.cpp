#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

std::vector<unsigned char> readScanFile(const std::string& path)
{
    //std::vector <unsigned char> data;
    std::ifstream file(path, std::ios::binary) ;
    if (file)
    	return std::vector<unsigned char>(std::istreambuf_iterator<char>(file), 
    									  std::istreambuf_iterator<char>());
    else
    	return std::vector<unsigned char>();
}

void writePGMFile(const std::vector<double> values, int width, int height) {
	std::ofstream f("vriOutput.pgm");

    int maxColorValue = 255;
    f << "P5\n" << width << " " << height << "\n" << maxColorValue << "\n";

    for(int i = 0; i < height; ++i) {
    	for (int j = 0; j < width; ++j) {
    		unsigned char val = 255*values[i*width + j];
    		f << (int)val << " ";
    	}
        f << "\n";
    }

    f.close();
}

double getDensityValue(int i, int j, int k, std::vector<unsigned char> data) {
	int nx = 128, ny = 256, nz = 99;

	return (double) data[k*ny*nx + j*nx + i];
}

int main () {
	std::vector<unsigned char> scanData;

	scanData = readScanFile("vridados-head-8bit.raw");

	if (!scanData.empty())
		std::cout << "data size= " << scanData.size() << std::endl;
	else
		std::cout << "Ocorreu um erro na leitura do scan." << std::endl;

	// for (auto const& value : scanData) {
	// 	std::cout << +value << std::endl;
	// }

	// std::vector<double> values = {0.4, 0.5, 0.1, 1.0, 0.9, 0.2};

	// writePGMFile(values, 3, 2);

}