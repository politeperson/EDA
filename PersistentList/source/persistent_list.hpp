#pragma once
#ifndef PERSISTENT_LIST_HPP_
#define PERSISTENT_LIST_HPP_
// C++ librerías estándar
#include <memory>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
// Librerías propias
#include "multiple_data.hpp"

#define NEXT_NODE_FIELD "NextNode"
class modsTableRecord {
public:
	std::string field, value;
	uint32_t version;
	modsTableRecord() { version = 0; }
	modsTableRecord(uint32_t _version, const std::string& _field, const std::string& _value)
	{
		version = _version;
		field = _field;
		value = _value;
	}
	modsTableRecord(const modsTableRecord& someRecord) {
		version = someRecord.version;
		field = someRecord.field;
		value = someRecord.value;
	}

	~modsTableRecord() {
		value.clear();
		field.clear();
	}
};

class PerListNode {
public:
	uint32_t version; // version of the node
	std::map<std::string, std::string> dataTable;
	std::vector<modsTableRecord> modsTable;
	std::map<uint32_t, std::shared_ptr<PerListNode>> nextNodesTable;
	PerListNode() { version = 0; nextNodesTable[version] = nullptr; }
	PerListNode(uint32_t _version) { version = _version; nextNodesTable[version] = nullptr; }
	PerListNode(uint32_t _version, std::map<std::string, std::string> _dataTable) 
	{
		dataTable = _dataTable;
		version = _version;
		nextNodesTable[version] = nullptr;
	}

	void addNextNodeRecord(uint32_t _version, std::shared_ptr<PerListNode> _nextNode = nullptr) {
		nextNodesTable[_version] = _nextNode;
	}
	void addModRecord(uint32_t version, const std::string& field, const std::string& value) {
		modsTable.push_back(modsTableRecord(version, field, value));
	}

	void setFields() {
		if (dataTable.empty()) {
			std::cout << "Insert fields and its values to the current node (do not use spaces):\n";
			std::string field, value;
			char op;
			do
			{
				std::cout << "Enter field name: ";
				std::cin >> field;
				std::cout << "Enter value of the field: ";
				std::cin >> value;
				dataTable[field] = value;
				std::cout << "continue inserting fields? [y/n]: ";
				std::cin >> op;
			} while (op == 'y');
		}
		else
			std::cerr << "cannot modify or insert fields on a node for second time, use insert function\n";
	}

	friend std::ostream& operator<<(std::ostream& os, const PerListNode& PLNode);

	~PerListNode(){
		nextNodesTable.clear();
		modsTable.clear();
		dataTable.clear();
		//std::cout << "me voy...\n";
	}
};

std::ostream& operator<<(std::ostream& os, const PerListNode& PLNode)
{
	os << "V(" << PLNode.version << ")\n";
	os << "+++++++++++++++++++\n";
	os << "+    DATA TABLE   +\n";
	os << "+++++++++++++++++++\n";
	os << "[<field> : <value>]\n";
	
	if (PLNode.dataTable.empty())
		os << "empty DATA TABLE\n";
	
	for (auto& data : PLNode.dataTable) {
		os << "[" << data.first << " : " << data.second << "]\n";
	}
	os << "+++++++++++++++++++\n";
	os << "+    MODS TABLE   +\n";
	os << "+++++++++++++++++++\n";
	os << "V(<version>) [<field>: <value>]\n";
	
	if (PLNode.modsTable.empty())
		os << "empty MODS TABLE\n";

	for (auto& mod : PLNode.modsTable) {
		os << "V(" << mod.version << ") [" << mod.field << " : " << mod.value << "]\n";
	}
	os << "++++++++++++++++++++\n";
	os << "+ NEXT_NODES TABLE +\n";
	os << "++++++++++++++++++++\n";
	os << "V(<version>) [<address_value>]\n";
	
	if (PLNode.nextNodesTable.empty())
		os << "empty NEXT_NODES TABLE\n";
	
	for (auto& node : PLNode.nextNodesTable) {
		os << "V(" << node.first << ") [" << node.second << "]\n";
	}
	return os;
}


class PerList {
	uint32_t curr_version = 0; // initial version is one

	void find(uint32_t version, const std::string& field, std::shared_ptr<PerListNode> firstPtr) {
		try
		{
			if (head == nullptr)
				throw 500;
			if (version >= curr_version || version < 0)
				throw 303;
		}
		catch (int error)
		{
			if (error == 500)
				std::cerr << "cannot find any field on an empty list\n";
			if (error == 303)
				std::cerr << "version given is invalid range permitted is: [0 , " << curr_version - 1 << "]\n";
			return;
		}

		std::shared_ptr<PerListNode> pivot = firstPtr;
		if (pivot != nullptr) {
			if (version == pivot->version) {
				if (field == NEXT_NODE_FIELD) {
					if (pivot->nextNodesTable.find(version) != pivot->nextNodesTable.end())
					{
						std::cout << "Node found, showing data:\n";
						std::cout << *pivot->nextNodesTable[version];
					}
					else
						std::cout << "Node not found\n";
					return;
				}
				if (pivot->dataTable.find(field) != head->dataTable.end())
				{
					std::cout << "field found:\n";
					std::cout << "V(<version>) [<field> : <value>]\n";
					std::cout << "V(" << version << ") [" << field << " : " << pivot->dataTable[field] << "]\n";
				}
				else {
					std::cout << "field not found\n";
				}
				return;
			}
			else {
				if (field == NEXT_NODE_FIELD) {
					if (pivot->nextNodesTable.find(version) != pivot->nextNodesTable.end())
					{
						std::cout << "Node found, showing data:\n";
						std::cout << *pivot->nextNodesTable[version];
						return;
					}
				}
				std::vector<modsTableRecord>::iterator it;
				if ((it = std::find_if(pivot->modsTable.begin(), pivot->modsTable.end(),
					[&](modsTableRecord record) {
						return record.version == version && record.field == field;
					})) != pivot->modsTable.end())
				{
					std::cout << "field found:\n";
					std::cout << "V(<version>) [<field> : <value>]\n";
					std::cout << "V(" << version << ") [" << field << " : " << (*it).value << "]\n";
					return;
				}
			}

			for (auto& next_node : pivot->nextNodesTable) {
				if (next_node.second != nullptr) {
					find(version, field, next_node.second);
				}
			}
		}
		else
			std::cout << "field not found\n";
		return;
	}

public:
	std::shared_ptr<PerListNode> head;
	
	PerList() {
		head = nullptr;
	}

	void setFieldsToNode(std::shared_ptr<PerListNode> nodePtr) {
		try
		{
			if (head == nullptr && nodePtr != head)
				throw 500;
		}
		catch (int error)
		{
			if (error == 500)
				std::cerr << "cannot set any field on an empty list\n";
			return;
		}
		if (nodePtr == head && head == nullptr) {
			++curr_version;
			head = std::make_shared<PerListNode>(curr_version);
			nodePtr = head;
			++curr_version;
		}
		nodePtr->setFields();
	}

	void insert(std::shared_ptr<PerListNode> nodePtr, const std::string& field,
		const std::string& value, std::shared_ptr<PerListNode> newNode = nullptr)
	{
		try
		{
			if (field.empty())
				throw 404;
		}
		catch (int error)
		{
			std::cerr << "error 404, empty field\n";
			return;
		}
		bool created_head = false;
		if (head == nullptr && nodePtr == head) {
			++curr_version;
			created_head = true;
			head = std::make_shared<PerListNode>(curr_version);
			nodePtr = head;
		}

		if (field == NEXT_NODE_FIELD) 
			(*nodePtr).nextNodesTable[curr_version] = newNode;
		else
			(*nodePtr).addModRecord(curr_version, field, value);

		if(!created_head)
			++curr_version; // we augment current version if not already update it
	}

	void find(uint32_t version, const std::string& field) {
		try
		{
			if (head == nullptr)
				throw 500;
			if (version >= curr_version || version < 0)
				throw 303;
		}
		catch (int error)
		{
			if(error == 500)
				std::cerr << "cannot find any field on an empty list\n";
			if(error == 303)
				std::cerr << "version given is invalid range permitted is: [0 , "<< curr_version-1 << "]\n";
			return;
		}

		std::shared_ptr<PerListNode> pivot = head;
		if (pivot != nullptr) {
			if (version == pivot->version) {
				if (field == NEXT_NODE_FIELD) {
					if (pivot->nextNodesTable.find(version) != pivot->nextNodesTable.end())
					{
						std::cout << "Node found, showing data:\n";
						std::cout << (*pivot->nextNodesTable[version]) << std::endl;
					}
					else
						std::cout << "Node not found\n";
					return;
				}

				if (pivot->dataTable.find(field) != head->dataTable.end())
				{
					std::cout << "field found:\n";
					std::cout << "V(<version>) [<field> : <value>]\n";
					std::cout << "V(" << version << ") [" << field << " : " << pivot->dataTable[field] << "]\n";
				}
				else {
					std::cout << "field not found\n";
				}
				return;
			}
			else {
				if (field == NEXT_NODE_FIELD) {
					if (pivot->nextNodesTable.find(version) != pivot->nextNodesTable.end())
					{
						std::cout << "Node found, showing data:\n";
						std::cout << *pivot->nextNodesTable[version];
						return;
					}
				}

				std::vector<modsTableRecord>::iterator it;
				if ((it = std::find_if(pivot->modsTable.begin(), pivot->modsTable.end(),
					[&](modsTableRecord record) {
						return record.version == version && record.field == field;
					})) != pivot->modsTable.end())
				{
					std::cout << "field found:\n";
					std::cout << "V(<version>) [<field> : <value>]\n";
					std::cout << "V(" << version << ") [" << field << " : " << (*it).value << "]\n";
					return;
				}
			}

			for(auto& next_node : pivot->nextNodesTable) {
				if (next_node.second != nullptr) {
					find(version, field, next_node.second);
				}
			}
		}
		else
			std::cout << "field not found\n";
		return;
	}

	uint32_t getCurrVersion() const { return curr_version; }

	~PerList() {
		head.reset();
	}
};


#endif // !PERSISTENT_LIST_HPP_
