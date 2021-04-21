#pragma once
#ifndef QUAD_TREE_HPP_
#define QUAD_TREE_HPP_
#include <iostream>
#include <vector>
#include <utility>
#include <stack>

class Node
{ 
public:
	// Each node has a Box of the form (I_1, I_2)
	// where I_1 = [i,j] is an interval on "x" axis
	// and   I_2 = [i,j] is an interval on "y" axis
	std::pair<double, double> Box[2];
	bool colored;
	Node* children[4];
	Node(std::pair<double, double> _Box[], bool _colored = false) {
		Box[0] = _Box[0];
		Box[1] = _Box[1];
		colored = _colored;          // initially a node is not colored
		for (auto& child : children) // initially all children are null
			child = nullptr;
	}
	Node(std::vector<std::pair<double, double>> _Box, bool _colored = false) {
		Box[0] = _Box[0];
		Box[1] = _Box[1];
		colored = _colored;          // initially a node is not colored
		for (auto& child : children) // initially all children are null
			child = nullptr;
	}
	Node(Node &_node) { // correct testing
		Box[0] = _node.Box[0];
		Box[1] = _node.Box[1];
		colored = _node.colored;
		for (int i = 0; i < 4; ++i)
			children[i] = _node.children[i];
	}

	void SetBox(std::pair<double, double> _Box[], short lenght = 2) {
		Box[0] = _Box[0];
		Box[1] = _Box[1];
	}

	// if a node is not colored and has no children, then it is a leaf
	bool IsLeaf() {
		for (auto& child : children)
			if (child) return false;
		return colored;
	}


	// generates an inner box depending of the index of the childs that you pass, returns the
	// ranges of that box, and returns an empty Box if nothing happens. Remenber: NW=0, NE=1, SW=2, SE=3
	std::vector<std::pair<double,double>> GenerateBox(int quadrant) { // correct testing
		if (quadrant < 0 || quadrant > 3) return std::vector<std::pair<double,double>>(); // an empty box
		std::vector<std::pair<double, double>> NewBox(2); // new box has size 2
		
		double mid1 = Box[0].first + (Box[0].second - Box[0].first) / 2; // middle of the first interval
		double mid2 = Box[1].first + (Box[1].second - Box[1].first) / 2; // middle of the second interval
		switch (quadrant)
		{
		case 0: // NW
			NewBox.assign({ std::make_pair(Box[0].first, mid1), std::make_pair(mid2, Box[1].second) });
			break;
		case 1: // NE
			NewBox.assign({ std::make_pair(mid1, Box[0].second), std::make_pair(mid2, Box[1].second) });
			break;
		case 2: // SW
			NewBox.assign({ std::make_pair(Box[0].first, mid1), std::make_pair(Box[1].first, mid2) });
			break;
		case 3: // SE
			NewBox.assign({ std::make_pair(mid1, Box[0].second), std::make_pair(Box[1].first, mid2) });
			break;
		}
		return NewBox;
	}

	// this function receives a point, returns true if the point is
	// inside the box, returns false if the point is outside the box
	bool IsOn(std::pair<double, double> P) { // correct testing
		return (Box[0].first <= P.first && Box[0].second >= P.first)
			&& (Box[1].first <= P.second && Box[1].second >= P.second);
	}

	// receive a point, returns the quadrant at which the point belongs: 
	// NW=0, NE=1, SW=2, SE=3; otherwise the function returns -1
	// if the query point is out of range of the box
	short ChooseLeaf(std::pair<double, double> P) { // correct testing
		if (!IsOn(P))
			return -1; // the point is outside the box

		double mid1 = Box[0].first + (Box[0].second - Box[0].first) / 2; // middle of the first interval
		double mid2 = Box[1].first + (Box[1].second - Box[1].first) / 2; // middle of the second interval

		// NW
		if ((Box[0].first <= P.first && P.first <= mid1) && (mid2 <= P.second && P.second <= Box[1].second))
			return 0; // NW = 0
		// NE
		if ((mid1 <= P.first && P.first <= Box[0].second) && (mid2 <= P.second && P.second <= Box[1].second))
			return 1; // NE = 1
		// SW
		if ((Box[0].first <= P.first && P.first <= mid1) && (Box[1].first <= P.second && P.second <= mid2))
			return 2; // SW = 2
		// SE
		return 3; // SE = 3 
	}


	~Node() {

		for (auto& child : children)
			delete child;
		 /*std::cout << "deleting Box: " << Box[0].first << " " << Box[0].second << "\n";
		 std::cout << "deleting Box: " << Box[1].first << " " << Box[1].second << "\n";*/
	}
};

class QuadTree
{
private:
	unsigned int TreeDepth;
	Node* root;
public:
	QuadTree(std::pair<double,double> _Box[], unsigned int _TreeDepth) {
		root = new Node(_Box);
		TreeDepth = _TreeDepth;
	}
	QuadTree(std::vector<std::pair<double, double>> _Box, unsigned int _TreeDepth) {
		root = new Node(_Box);
		TreeDepth = _TreeDepth;
	}
	QuadTree(Node _root, unsigned int _TreeDepth) {
		root = new Node(_root);
		TreeDepth = _TreeDepth;
	}

	Node* getRoot() {
		return root;
	}

	// receive a point, returns true if the point is present, false otherwise
	// the function also takes an argument depth by reference, and modify this
	// value to meet the depth at where the point was founded
	bool find(std::pair<double, double> P, unsigned int& depth) {
		Node *ptr = root;
		// if the pointer is null, or the pointer is only a leaf, or the point is outside the
		// box pointer, it means, the point is not on the tree
		int quadrant;
		depth = 0;
		if (!ptr || !ptr->IsOn(P))
			return false;
		// while the node is not a leaf, we proceed the search
		while (!ptr->IsLeaf()) {
			quadrant = ptr->ChooseLeaf(P);
			// the child is null, hence the point is not here
			if (!ptr->children[quadrant]) return false;
			// the child is not a leaf, hence we continue the search
			ptr = ptr->children[quadrant];
			++depth; // we increment the depth, at which the search ends on the tree
		}
		// the root is a leaf, hence it is colored
		return true;
	}


	// receive a point, returns true if the point is present, false otherwise
	// the function also takes an argument depth by reference, and modify this
	// value to meet the depth at where the point was founded
	bool find(std::pair<double, double> P, unsigned int& depth, std::stack<Node**>& ptrs) {
		// first pointer is always root
		ptrs.push(&root);
		int quadrant;
		depth = 0; // level 0 is the level of the root
		// if the pointer is null, or the pointer is only a leaf, or the point is outside the
		// box pointer, it means, the point is not on the tree
		if (!(*ptrs.top()) || !(*ptrs.top())->IsOn(P))
			return false;
		
		// while the node is not a leaf, we proceed the search
		while (!(*ptrs.top())->IsLeaf()) {
			quadrant = (*ptrs.top())->ChooseLeaf(P);
			// the child is null, hence the point is not here
			if (!(*ptrs.top())->children[quadrant]) return false;
			// the child is not a leaf, hence we continue the search
			ptrs.push(&(*ptrs.top())->children[quadrant]);
			++depth; // we increment the depth, at which the search ends on the tree
		}
		// we reach at a node that is a leaf, hence is colored
		return true;
	}

	// inserts a point into the PR-Quadtree, and returns the depth at the node, where the
	// point was inserted, returns -1 if the node could not be inserted
	short insert(std::pair<double, double> P) {
		std::stack<Node**> ptrs;
		unsigned int depth;
		// if the point is already covered by quadrants, we return -1
		if (find(P, depth, ptrs))
			return -1;
		int quadrant;
		std::vector<std::pair<double, double>> NewBox;
		// depth is positive or 0
		while (depth < TreeDepth) {
			// as P is inside the root box, is proved that quadrant could not be less than 0 or greather than 3
			quadrant = (*ptrs.top())->ChooseLeaf(P); 
			NewBox = (*ptrs.top())->GenerateBox(quadrant);
			// a new node with the respectively quadrant dimensions
			(*ptrs.top())->children[quadrant] = new Node(NewBox);
			ptrs.push(&(*ptrs.top())->children[quadrant]);
			++depth;
		}

		// the top of stack is the last child inserted
		(*ptrs.top())->colored = true;
		// we remove this node from the stack, because the parents needs to evaluate if they are
		// full now
		ptrs.pop(); 

		while (!ptrs.empty()) {
			// now the last step is to check if the parent of the inserted point is full of nodes
			bool is_full = 1; // first we say "yes, is full"
			// that is the purpose of aux_ptr
			for (auto& child : (*ptrs.top())->children)
				if (!child || !child->IsLeaf()) is_full = 0; // if any child is not a leaf or is null

			if (is_full) {
				// delete all children, and make this node a new leaf 
				for (auto& child : (*ptrs.top())->children) {
					delete child; // first free the memory to avoid memory leak
					child = nullptr; // and then you can point to null
				}
				// now the parent is a leaf, because it not have children, and is colored
				(*ptrs.top())->colored = true;
			}
			ptrs.pop();
		}
		return depth;
	}

	// inserts a point into the PR-Quadtree, and returns the depth at the node, where the
	// point was inserted, returns -1 if the node could not be inserted
	bool remove(std::pair<double, double> P) {
		std::stack<Node**> ptrs;
		unsigned int depth;
		// if the point is already covered by quadrants, we return -1
		if (!find(P, depth, ptrs)) return false;
		int quadrant;
		
		while (depth < TreeDepth) {
			quadrant = (*ptrs.top())->ChooseLeaf(P);
			for (int i = 0; i < 4; i++)
				(*ptrs.top())->children[i] = new Node((*ptrs.top())->GenerateBox(i), true);
			
			(*ptrs.top())->colored = false; // it is no more a leaf, decolorize the parent
			ptrs.push(&(*ptrs.top())->children[quadrant]);
			++depth;
		}

		// the top of stack is the last child removed
		delete *ptrs.top();
		*ptrs.top() = nullptr;
		// we remove this node from the stack, because the parents needs to evaluate if they are
		// empty done this operation
		ptrs.pop();

		while (!ptrs.empty()) {
			bool is_empty = 1;
			for (auto& child : (*ptrs.top())->children)
				if (child) is_empty = 0;

			if (is_empty && *ptrs.top() != root) {
				delete* ptrs.top();
				*ptrs.top() = nullptr;
			}
			ptrs.pop();
		}
		return true;
	}


	~QuadTree() {
		delete root;
	}
};
/**/


#endif // !QUAD_TREE_HPP_
