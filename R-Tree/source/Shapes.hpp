#pragma once
#ifndef SHAPES_HPP_
#define SHAPES_HPP_
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Interval {
public:
	double low, upp;
	explicit Interval(){
		low = 0.0;
		upp = 0.0;
	}
	
	explicit Interval(const Interval& _Ival) {
		low = _Ival.low;
		upp = _Ival.upp;
	}

	explicit Interval(double _low, double _upp) {
		low = _low;
		upp = _upp;
	}

	~Interval(){
		//std::cout << "was deleted (" << low << "," << upp <<")" << std::endl;
	}
};

template<unsigned int Dim>
class HyperRect
{
public:
	Interval Ivals[Dim];
	explicit HyperRect(){}
	explicit HyperRect(const Interval _Ivals[]) {
		for (size_t i = 0; i < Dim; i++)
			Ivals[i] = _Ivals[i];
	}
	// no hacerlo expl�cito
	HyperRect(const HyperRect<Dim>& _Hrect) {
		for (size_t i = 0; i < Dim; i++)
			Ivals[i] = _Hrect.Ivals[i];
	}

	void setIntervals(const Interval _Ivals[]) {
		for (size_t i = 0; i < Dim; i++)
			Ivals[i] = _Ivals[i];
	}

	double HyperVolume() {
		double total_volume = 1.0;
		for (size_t i = 0; i < Dim; ++i)
			total_volume *= (Ivals[i].upp - Ivals[i].low);
		return total_volume;
	}
	// muestra los intervalos por dimensi�n
	void show_intervals() {
		for (size_t i = 0; i < Dim; ++i) 
			std::cout << i+1 << ": (" << Ivals[i].low << "," << Ivals[i].upp << ")" << std::endl;
	}

	~HyperRect(){}
};


// Estas son algunas funciones relacionadas a los hiper-rect�ngulos que se usan en el 
// �rbol R, funciones auiliares

// Esta funci�n verifica si dos hiper-rect�ngulos se superponen
// Entradas: 
// A, B: son dos hiperect�ngulos, evaluaremos si se superponen
// Retorno: retornamos true si se superponen, sino false
template<unsigned int Dim>
bool Overlap(HyperRect<Dim>& A, HyperRect<Dim>& B) {
	// Paso las pruebas
	for (size_t i = 0; i < Dim; ++i)
		if (A.Ivals[i].low >= B.Ivals[i].upp || A.Ivals[i].upp <= B.Ivals[i].low)
			return false;
	return true;
}

// Funci�n minimun bounding box (MBR)
// Esta funci�n genera un nuevo Hiper-rect�ngulo para un conjunto de hiper-rect�ngulos
// Entradas: un conjunto de hiper-rect�ngulos en un vector
// Retorno: retorna el m�nimo hiper-rect�ngulo que encierra a esos hiper-rect�ngulos
template<unsigned int Dim>
HyperRect<Dim> MBR(std::vector<HyperRect<Dim>>& Rectangles) {
	Interval MBR_Intervals[Dim];
	// primero escogemos unos como nuestros m�nimos
	for (int i = 0; i < Dim; ++i) {
		MBR_Intervals[i] = Rectangles[0].Ivals[i];
	}
	size_t size_set = Rectangles.size();
	for (size_t i = 0; i < size_set; ++i) {
		for (size_t j = 0; j < Dim; ++j) {
			MBR_Intervals[j].low = std::min(MBR_Intervals[j].low, Rectangles[i].Ivals[j].low);
			MBR_Intervals[j].upp = std::max(MBR_Intervals[j].upp, Rectangles[i].Ivals[j].upp);
		}
	}

	return HyperRect<Dim>(MBR_Intervals);
}



#endif //SHAPES_HPP_