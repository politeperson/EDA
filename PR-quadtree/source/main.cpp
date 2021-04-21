#include "QuadTree.hpp"
#include "LeafLetPluggin.hpp"
#include <fstream>
#include <string>
#include <random>
#include <unordered_map>


int main() {
	// lng_range = [-71.542733, -71.529818], lat_range = [-16.402391, -16.396304]
	double range_lat[2] = { -16.402391, -16.396304 };
	double range_lng[2] = { -71.542733, -71.529818 };

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<>dislat(range_lat[0], range_lat[1]);
	std::uniform_real_distribution<>dislng(range_lng[0], range_lng[1]);

	// lng_range = [-71.542733, -71.529818], lat_range = [-16.402391, -16.396304]
	// Box = [lng_range, lat_range]
	std::pair<double, double> Box[2] = {std::make_pair(range_lng[0], range_lng[1]), 
										std::make_pair(range_lat[0], range_lat[1])};
	
	unsigned int tree_depth;
	std::cout << "Enter the depth of the PR-quadtree: ";
	std::cin >> tree_depth;
	// PR-quadtree of depth 4
	// the tree charges the points in (longitude, lattitude) form
	QuadTree QTree(Box, tree_depth);

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
		std::cout << "No se puedo abrir el archivo" << filename << std::endl;
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
		std::cout << "No se puedo abrir el archivo" << filename << std::endl;
	}

	int num_points;
	std::cout << "Choose Number of Points: ";
	std::cin >> num_points;

	double rndlng, rndlat;
	for (int i = 0; i < num_points; ++i) {
		rndlng = dislng(gen), rndlat = dislat(gen);
		QTree.insert(std::make_pair(rndlng, rndlat));
	}

	std::vector<std::pair<std::pair<double, double>, std::pair<double, double>>> Rectangles = leaflet::GenerateLeafRectangles(QTree);
	/*for (int i = 0; i < Rectangles.size(); ++i) {
			std::cout << "Rectangle " << i + 1 << "\n";
			std::cout << "ldcorner: [" << Rectangles[i].first.first << "," << Rectangles[i].first.second << "]" << std::endl;
			std::cout << "rucorner: [" << Rectangles[i].second.first << "," << Rectangles[i].second.second << "]" << std::endl;
		}/**/

	std::unordered_map<std::string, std::string> Rect;
	Rect["declare"] = "L.rectangle(";
	Rect["pol_options"] = ", {color: \"#0C0C0B\", weight: 1}).addTo(mymap);\n";
	Rect["pol_options_big"] = ", {color: \"#F4A08E\", weight: 1}).addTo(mymap);\n";
	std::string little_rects = "";
	int RectanglesSize = Rectangles.size();
	for (int i = 0; i < RectanglesSize; ++i) {
		little_rects += Rect["declare"];
		little_rects += "[[" + std::to_string(Rectangles[i].first.first) + "," + std::to_string(Rectangles[i].first.second) + "],";
		little_rects += "[" + std::to_string(Rectangles[i].second.first) + "," + std::to_string(Rectangles[i].second.second) + "]]";
		little_rects += Rect["pol_options"];
	}
	
	std::string big_rect = Rect["declare"];
	big_rect += "[[" + std::to_string(range_lat[0]) + "," + std::to_string(range_lng[0]) + "],";
	big_rect += "[" + std::to_string(range_lat[1]) + "," + std::to_string(range_lng[1]) + "]]";
	big_rect += Rect["pol_options_big"];

	// here goes the map file
	std::ofstream File_Map;
	filename = "LeafMap.html"; // we generate the map file
	File_Map.open(filename);
	File_Map << header; // first goes the header
	File_Map << little_rects; // then we add the rectangles
	File_Map << big_rect;
	File_Map << tale;
	File_Map.close();

	/*
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