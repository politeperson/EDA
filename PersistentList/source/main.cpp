// Librerías estándar C++
#include <typeinfo>
#include <stdlib.h>

// Librerías propias
#include "persistent_list.hpp"


int main() {

	PerList per_list_sample;

	std::cout << "INSERTING NODES TO PERSISTENT LIST\n";
	// INSERTING NODES
	per_list_sample.setFieldsToNode(per_list_sample.head);

	std::cout << (*per_list_sample.head) << std::endl;

	std::shared_ptr<PerListNode> node2 = std::make_shared<PerListNode>(per_list_sample.getCurrVersion());
	per_list_sample.setFieldsToNode(node2);
	per_list_sample.insert(per_list_sample.head, NEXT_NODE_FIELD, "", node2);

	std::cout << (*per_list_sample.head->nextNodesTable[per_list_sample.getCurrVersion() - 1]) << std::endl;

	std::shared_ptr<PerListNode> node3 = std::make_shared<PerListNode>(per_list_sample.getCurrVersion());
	per_list_sample.setFieldsToNode(node3);
	per_list_sample.insert(per_list_sample.head->nextNodesTable[2], NEXT_NODE_FIELD, "", node3);
	std::cout << *(*per_list_sample.head->nextNodesTable[2]).nextNodesTable[3];

	per_list_sample.insert(per_list_sample.head->nextNodesTable[2], "edad", "25");


	std::shared_ptr<PerListNode> node4 = std::make_shared<PerListNode>(per_list_sample.getCurrVersion());
	per_list_sample.setFieldsToNode(node4);
	per_list_sample.insert(per_list_sample.head->nextNodesTable[2], NEXT_NODE_FIELD, "", node4);
	

	// FINDING VALUES BY VERSION & FIELDS
	
	std::cout << "FINDING FIELDS BY VALUE\n";
	per_list_sample.find(4, "edad");
	std::cout << "\n\n";
	per_list_sample.find(5, NEXT_NODE_FIELD);

	system("PAUSE");
	return 0;
}