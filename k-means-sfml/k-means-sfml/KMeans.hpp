#pragma once
#ifndef K_MEANS_HPP_
#define K_MEANS_HPP_
#include <array>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <iostream>

// genera un entero aleatorio entre [a,b]
int getRandomInteger(int a, int b)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(a, b);
	return distrib(gen);
}


float getRandomFloat(float a, float b) 
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distrib(a, b);
	return distrib(gen);
}


template <int dim>
class Point {
public:
	std::array<float, dim> p;

	Point() { p.fill(0.0); }

	Point(std::array<float, dim> copy_p)
	{
		p = copy_p;
	}

	Point(const Point<dim>& some_p)
	{
		p = some_p.p;
	}

	// obtenemos la distancia euclidiana
	float distance(const Point<dim>& some_p)
	{
		float euclidean_distance = 0.0;
		for (int i = 0; i < dim; ++i)
			euclidean_distance += (p[i] - some_p.p[i]) * (p[i] - some_p.p[i]);
		return sqrt(euclidean_distance);
	}

	bool operator==(const Point<dim>& other_point) const 
	{
		return p == other_point.p;
	}

	void setP(std::array<float, dim> other_p)
	{
		p = other_p;
	}


	~Point() {}
};

// dim: la dimensión de los puntos, C: #clusters
template<int dim>
class KM // K-means 
{
	int C; // el número de clusters
	std::vector<Point<dim>> points; // los puntos que conforman el conjunto de datos
	std::vector<int> assignments; // contiene el centroide(índice [0,C]) al cual está asignado un punto
	std::vector<Point<dim>> centroids;


	float adjustCentroids();
	void assignCentroids();
public:
	KM(std::vector<Point<dim>> points_copy, int C_copy);
	// este es el algoritmo k-means
	void KMeans(int iter);
	void KMeans(float err);

	std::vector<int> getAssignments() const { return assignments; }
	std::vector<Point<dim>> getPoints() const { return points; }
	std::vector<Point<dim>> getCentroids() const 
	{ 
		return centroids; 
	}
	int getC() const { return C; }

	~KM();
};
/* MËTODOS PRIVADOS */


// ajustamos los centroides de cada grupo y retornamos la función de costo
template<int dim>
float KM<dim>::adjustCentroids()
{
	std::vector<Point<dim>> new_centroids(C);
	std::vector<int> n_set(C, 0);
	int set_size = points.size();

	for (int i = 0; i < set_size; ++i)
	{
		for (int j = 0; j < dim; ++j)
		{
			new_centroids[assignments[i]].p[j] += points[i].p[j];
			++n_set[assignments[i]];
		}
	}

	float cost = 0.0;

	// copiamos el promedio de los valores de cada una de las dimensiones corespondientes
	// a cada grupo, a cada uno de los centroides
	for (int i = 0; i < C; ++i)
	{
		if(n_set[i] != 0)
		{
			float some_n = static_cast<float>(n_set[i]);
			for (int j = 0; j < dim; ++j)
			{
				new_centroids[i].p[j] /= some_n;
			}
			cost += centroids[i].distance(new_centroids[i]); // vamos incrementando la función de costo
			centroids[i] = new_centroids[i]; // actualizamos nuestro centroides con uno nuevo
		}
	}
	return cost;
}

template<int dim>
void KM<dim>::assignCentroids()
{
	int size_points = points.size();
	float min_dist_to_centroid;
	int selected_centroid;

	for (int i = 0; i < size_points; ++i)
	{
		// al principio la distancia mínima del i-ésimo punto será hacia el centroide 0
		min_dist_to_centroid = points[i].distance(centroids[0]);
		selected_centroid = 0;
		for (int j = 1; j < C; ++j)
		{
			float dist_to_centroid_j = points[i].distance(centroids[j]);
			if (min_dist_to_centroid > dist_to_centroid_j)
			{
				min_dist_to_centroid = dist_to_centroid_j;
				selected_centroid = j;
			}
		}
		// el punto i-ésimo ahora tiene asignado su centroide
		assignments[i] = selected_centroid;
	}
	return;
}

/* MËTODOS PÜBLICOS */
template<int dim>
KM<dim>::KM(std::vector<Point<dim>> points_copy, int C_copy)
{
	C = C_copy;
	points = points_copy;
	assignments.resize(points.size(), 0); // n puntos que pertenecen al principio al cluster 0
	centroids.resize(C);
	
	for (int i = 0; i < C; ++i) {
		bool already_exist;
		int newPos;
		do {
			already_exist = false;
			newPos = getRandomInteger(0, points.size() - 1);

			for (int j = 0; j < i; ++j) {
				if (centroids[j] == points[newPos]) {
					already_exist = true;
					break;
				}
			}
		} while (already_exist);
		centroids[i] = points[newPos];
	}
	/*
	for (int i = 0; i < C; ++i) {
		std::cout << "cluster " << i + 1 << " : \n";
		for (int j = 0; j < dim; ++j)
		{
			std::cout << centroids[i].p[j] << ",";
		}
		std::cout << "\n";
	}/**/
}

template<int dim>
KM<dim>::~KM()
{
	points.clear();
	assignments.clear();
}

template<int dim>
void KM<dim>::KMeans(int iter)
{
	for (int i = 0; i < iter; ++i)
	{
		// primero asignamos un centroide a cada punto, y luego ajustamos los nuevos centroides
		assignCentroids();
		std::cout << "cost function: " << adjustCentroids() << std::endl;
	}
}

template<int dim>
void KM<dim>::KMeans(float err)
{
	int maximun_iter = 5000;
	// en caso se supere el límite de iteraciones paramos
	for (int i = 0; i < maximun_iter; ++i)
	{
		// primero asignamos un centroide a cada punto, y luego ajustamos los nuevos centroides
		// nos detenemos cuando la función de costo retornada es menor a nuestro error
		assignCentroids();
		float new_cost = adjustCentroids();
		std::cout << "cost function: " << new_cost << std::endl;
		if (new_cost < err) break;
	}
}


#endif // K_MEANS_HPP_
