#pragma once
#ifndef LEAF_LET_PLUGGIN_HPP_
#define LEAF_LET_PLUGGIN_HPP_
#include "QuadTree.hpp"
#include <vector>
#include <math.h>

typedef std::pair<double, double> dd;

class RectangleLeaf {
public:
	dd ldcorner;	// left-down corner
	dd rucorner;	// right-up corner
	std::string color; // the color in hexadecimal
};


namespace leaflet {
	// this function generates a vector of rectangles, in the LeafLet API form
	// var bounds = [[54.559322, -5.767822], [56.1210604, -3.021240]];
	// L.rectangle(bounds, {color: "#ff7800", weight: 1}).addTo(map);
	// where bounds[0] represents the left-down corner of a rectangle
	// and bounds[1] represents the right-up corner of a rectangle
	// the format of the coordinates are given in latlng form, hence in bounds[0] = [54.559322, -5.767822]
	// lattitude = 54.559322, longittude = -5.767822
	std::vector<RectangleLeaf> GenerateLeafRectangles(QuadTree& QTree) {
		//std::vector<std::string> HeatColors = { "#C70509","#E90308","#FF342D","#FFED44","#FFCE00","#F9A900" };
		std::vector<std::string> HeatColors = {"#fa1d05","#faa805","#d0f52f"};

		std::vector<RectangleLeaf> Rectangles;
		RectangleLeaf myRect;
		Node* root = QTree.getRoot();
		if (root->IsLeaf()) {
			myRect.ldcorner = std::make_pair(root->Box[1].first, root->Box[0].first);// remenber (lat, lng) format
			myRect.rucorner = std::make_pair(root->Box[1].second, root->Box[0].second);
			myRect.color = HeatColors[0];
			Rectangles.push_back(myRect);
			return Rectangles;
		}
		else {
			std::stack<std::pair<Node*, unsigned int>> NodesPile;
			NodesPile.push(std::make_pair(root, 0)); // root is at depth 0
			while (!NodesPile.empty()) {
				std::pair<Node*, unsigned int> top = NodesPile.top();
				NodesPile.pop();
				for (auto& child : top.first->children)
				{
					if (child) {
						if (child->IsLeaf())
						{
							// remenber (lat, lng) format, Tree works in the form of (lng, lat)
							myRect.ldcorner = std::make_pair(child->Box[1].first, child->Box[0].first);
							myRect.rucorner = std::make_pair(child->Box[1].second, child->Box[0].second);
							double leafdepth = static_cast<double>(top.second) + 1.0;
							
							if (leafdepth >=  0.85 * static_cast<double>(QTree.getDepth())) 
								myRect.color = HeatColors[2]; // some red
							else if(leafdepth >= 0.75 * static_cast<double>(QTree.getDepth())) 
								myRect.color = HeatColors[1]; // some orange
							else
								myRect.color = HeatColors[0]; // some yellow

							Rectangles.push_back(myRect);
						}
						else 
							NodesPile.push(std::make_pair(child, top.second + 1)); // the parent depth plus one
					}
				}
			}
		}

		return Rectangles;
	}

};

#endif // !QUAD_TREE_HPP_