#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <iomanip>

using namespace std;


// gets the euclidian distance of two vectors
double euclidian_distance(vector<double>& a, vector<double>& b)
{
	double result = 0;
	for (int i = 0; i < a.size(); i++)
		result += (a[i] - b[i]) * (a[i] - b[i]); // (ai - bi)^2
	return sqrt(result);
}

// construct set of size "n", with vectors of dimension "k", with random integers.
void construct_VD(vector<vector<double>>& VD, int k, int n)
{
  random_device rd;
	mt19937 gen(rd());
	uniform_real_distribution<>dis(1.0, 2.0); // random values are from 1 to 10000
	vector<double> temp;                                     //temporal vector
	for (int i = 0; i < n; i++)
	{
	  for (int j = 0; j < k; j++)
		  temp.push_back(dis(gen));            // adding random values to our vector
		VD.push_back(temp);                    // adding a vector to our set of vectors
		temp.clear();                          //clear the temporal vector
	}
	return;
}

// print a set of vectors
void printSetVectors(vector<vector<double>>& VD)
{
	for (int i = 0; i < VD.size(); i++)
	{
		for (int j = 0; j < VD[i].size(); j++)
			cout << VD[i][j] << " ";
		cout << endl;
	}
	return;
}

void compare(vector<vector<double>>& VD)
{
  int vsize = VD.size();
	for (int i = 0; i < vsize; i++)
		for (int j = i + 1; j < vsize; j++)
		  euclidian_distance(VD[i], VD[j]);
}
int main()
{
	vector<vector<double>> VD;
	int dimensions[4] = {10, 15, 20, 25}, cardinals[4] = {10000, 15000, 20000, 25000};
  cout << "Matrix of times (ms) size\\dim \n       ";
  for(auto dim:dimensions){
        cout << setw(10);
        cout << dim << "  ";
  }
  cout << endl;
  for(auto cardi:cardinals){
    cout << setw(5);
    cout << cardi << "  ";
    for(auto dim:dimensions){
      construct_VD(VD, dim, cardi);
      auto t1 = chrono::high_resolution_clock::now();
      compare(VD);
      auto t2 = chrono::high_resolution_clock::now();
      cout << setw(10);
      cout << chrono::duration_cast<chrono::microseconds>(t2 - t1).count() << "  ";
	  VD.clear();
	}

    cout << endl;
  }
	return 0;
}