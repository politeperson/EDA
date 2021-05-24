#include "SuffixTree.hpp"
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;



int main() {
	//SUFFIX_TREE STree;
	ifstream dataSet;
	dataSet.open("data_set.txt");
	if (!dataSet) {
		cout << "error al abrir el archivo formatted_metadata.txt" << endl;
		exit(1);
	}


	int n_lines = 10, i = 0;
	string id, translated_text;

	auto t_init = chrono::high_resolution_clock::now();
	while (i < n_lines && dataSet >> id >> translated_text) 
	{
		++i;
	}
	auto t_end = chrono::high_resolution_clock::now();
	auto duration_ms = chrono::duration_cast<chrono::microseconds>(t_end - t_init).count();
	auto duration_s = chrono::duration_cast<chrono::seconds>(t_end - t_init).count();

	cout << "tiempo en ms: " << duration_ms << endl;
	cout << "tiempo en s: " << duration_s << endl;

	dataSet.close();

	return 0;
}