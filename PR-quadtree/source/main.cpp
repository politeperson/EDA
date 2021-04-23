#include "QuadTree.hpp"
#include "LeafLetPluggin.hpp"
#include <fstream>
#include <string>
#include <random>
#include <unordered_map>


int main() {
	std::ifstream File_Head;
	std::string filename = "HeadMap.txt", line;
	std::string header = "";
	File_Head.open(filename);
	if (File_Head.is_open()) {
		while (std::getline(File_Head, line))
			header += line + "\n";// we save the header inside header string
		File_Head.close();
	}
	else {
		std::cout << "No se puedo abrir el archivo: " << filename << std::endl;
		exit(1);
	}

	std::ifstream File_Tale;
	filename = "TaleMap.txt";
	File_Tale.open(filename);
	std::string tale = "";
	if (File_Tale.is_open()) {
		while (std::getline(File_Tale, line))
			tale += line + "\n";// we save the header inside header string
		File_Tale.close();
	}
	else {
		std::cout << "No se puedo abrir el archivo: " << filename << std::endl;
		exit(1);
	}


	// reading dataset	of points
	std::ifstream File_DataSet;
	filename = "DataSet.tsv";
	File_DataSet.open(filename);
	double rndlng, rndlat;
	// lng_range = [a, b], lat_range = [c, d], where a<=b and c<=d
	double range_lat[2] = { LDBL_MAX, LDBL_MIN };
	double range_lng[2] = { LDBL_MAX, LDBL_MIN };
	std::vector<std::pair<double, double>> DataSetPoints;

	if (File_DataSet.is_open()) {
		// on the dataset first is the lattitude and later the longitude
		while (File_DataSet >> rndlat >> rndlng) {
			DataSetPoints.push_back(std::make_pair(rndlng, rndlat));
			// lattitude
			range_lat[0] = std::min(range_lat[0], rndlat);
			range_lat[1] = std::max(range_lat[1], rndlat);
			// longitude
			range_lng[0] = std::min(range_lng[0], rndlng);
			range_lng[1] = std::max(range_lng[1], rndlng);
		}
		File_DataSet.close();
	}
	else {
		std::cout << "No se puedo abrir el archivo: " << filename << std::endl;
		exit(1);
	}
	
	unsigned int tree_depth;
	std::cout << "Enter the depth of the PR-quadtree: ";
	std::cin >> tree_depth;

	// lng_range = [a, b], lat_range = [c, d]
	// Box = [lng_range, lat_range]
	std::pair<double, double> Box[2] = { std::make_pair(range_lng[0], range_lng[1]),
										std::make_pair(range_lat[0], range_lat[1]) };

	// the tree charges the points in (longitude, lattitude) form
	QuadTree QTree(Box, tree_depth);
	size_t DataSetSize = DataSetPoints.size();
	for (size_t i = 0; i < DataSetSize; ++i) {
		QTree.insert(DataSetPoints[i]);
	}

	std::vector<RectangleLeaf> Rectangles = leaflet::GenerateLeafRectangles(QTree);
	

	std::unordered_map<std::string, std::string> Rect;
	Rect["declare"] = "L.rectangle(";
	Rect["pol_options_first"] = ", {color: \"";
	Rect["pol_options_second"] = "\", weight: 1, fillOpacity: 1, fill: true}).addTo(mymap);\n";
	std::string little_rects = "";
	int RectanglesSize = Rectangles.size();
	for (int i = 0; i < RectanglesSize; ++i) {
		little_rects += Rect["declare"];
		// adding left-down corner: (lat,lng)
		little_rects += "[[" + std::to_string(Rectangles[i].ldcorner.first) + "," + std::to_string(Rectangles[i].ldcorner.second) + "],";
		// adding right-up corner: (lat, lng)
		little_rects += "[" + std::to_string(Rectangles[i].rucorner.first) + "," + std::to_string(Rectangles[i].rucorner.second) + "]]";
		// adding the color of the Rectangle
		little_rects += Rect["pol_options_first"] + Rectangles[i].color + Rect["pol_options_second"];
	}
	
	
	// here goes the map file
	std::ofstream File_Map;
	filename = "LeafMap.html"; // we generate the map file
	File_Map.open(filename);
	File_Map << header; // first goes the header
	File_Map << little_rects; // then we add the rectangles
	File_Map << tale;
	File_Map.close();
	/**/
	/*
	std::vector<dd> Box = {std::make_pair(13.0, 17.0), std::make_pair(6.0, 10.0)};
	QuadTree QTree(Box, 3);
	// NW
	std::cout << "NW\n";
	std::cout << QTree.insert(std::make_pair(14.68362, 9.71794)) << "\n";
	std::cout << QTree.insert(std::make_pair(14.15057, 9.68833)) << "\n";
	std::cout << QTree.insert(std::make_pair(14.22954, 9.27374)) << "\n";
	std::cout << QTree.insert(std::make_pair(14.7922, 9.07631)) << "\n";

	// NE
	std::cout << "NE\n";
	std::cout << QTree.insert(std::make_pair(16.22353, 9.26387)) << "\n";
	std::cout << QTree.insert(std::make_pair(15.94713, 8.76043)) << "\n";

	// SW
	std::cout << "SW\n";
	std::cout << QTree.insert(std::make_pair(14.23941, 7.10207)) << "\n";

	// SE
	std::cout << "SE\n";
	std::cout << QTree.insert(std::make_pair(15.79906, 7.35872)) << "\n";
	std::cout << QTree.insert(std::make_pair(16.65786, 6.90465)) << "\n";
	/**/
	

	return 0;
}