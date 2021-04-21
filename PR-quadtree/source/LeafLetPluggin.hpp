#pragma once
#ifndef LEAF_LET_PLUGGIN_HPP_
#define LEAF_LET_PLUGGIN_HPP_
#include "QuadTree.hpp"
#include <vector>

typedef std::pair<double, double> dd;

namespace leaflet {
	// this function generates a vector of rectangles, in the LeafLet API form
	// var bounds = [[54.559322, -5.767822], [56.1210604, -3.021240]];
	// L.rectangle(bounds, {color: "#ff7800", weight: 1}).addTo(map);
	// where bounds[0] represents the left-down corner of a rectangle
	// and bounds[1] represents the right-up corner of a rectangle
	// the format of the coordinates are given in latlng form, hence in bounds[0] = [54.559322, -5.767822]
	// lattitude = 54.559322, longittude = -5.767822
	std::vector<std::pair<dd, dd>> GenerateLeafRectangles(QuadTree& QTree) {
		std::vector<std::pair<dd, dd>> Rectangles;
		dd ldcorner, rucorner;
		Node* root = QTree.getRoot();
		if (root->IsLeaf()) {
			ldcorner = std::make_pair(root->Box[1].first, root->Box[0].first); // remenber (lat, lng) format
			rucorner = std::make_pair(root->Box[1].second, root->Box[0].second);
			Rectangles.push_back({std::make_pair(ldcorner, rucorner)});
			return Rectangles;
		}
		else {
			std::stack<Node*> NodesPile;
			NodesPile.push(root);
			while (!NodesPile.empty()) {
				Node* top = NodesPile.top();
				NodesPile.pop();
				for (auto& child : top->children)
				{
					if (child) {
						if (child->IsLeaf())
						{
							// remenber (lat, lng) format, Tree works in the form of (lng, lat)
							ldcorner = std::make_pair(child->Box[1].first, child->Box[0].first);
							rucorner = std::make_pair(child->Box[1].second, child->Box[0].second);
							Rectangles.push_back({ std::make_pair(ldcorner, rucorner) });
						}
						else
							NodesPile.push(child);
					}
				}
			}
		}

		return Rectangles;
	}

};

#endif // !QUAD_TREE_HPP_