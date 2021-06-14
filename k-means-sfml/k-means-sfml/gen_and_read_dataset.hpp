#pragma once
#ifndef GENERATE_DATA_SETS_HPP_
#define GENERATE_DATA_SETS_HPP_
#include <fstream>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <iostream>
#include <algorithm>
#include <set>
#include "KMeans.hpp"


std::vector<std::string> split(std::string& s, std::string delim)
{
	if (s.empty()) 
	{
		std::cout << "el string pasado está vacío :(\n";
		return std::vector<std::string>();
	}
	std::vector<std::string> w;
	size_t start = 0U, end = s.find(delim);
	while (end != std::string::npos)
	{
		w.push_back(s.substr(start, end - start));
		start = end + delim.length();
		end = s.find(delim, start);
	}
	w.push_back(s.substr(start, end));
	return w;
}

// retorna verdaddero si existe falso de lo contrario
bool fileExists(const std::string& filename)
{
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1)
	{
		return true;
	}
	return false;
}


// lee el archivo: yellow_tripdata_2009-12.csv
// y genera un archivo de texto conteniendo todos los campos Start_Lon Start_Lat
// de cada una de las entradas
void generate_data_set(bool all, int n) 
{
	std::ifstream data_set_reader("yellow_tripdata_2009-12.csv");
	if (!data_set_reader.is_open()) 
	{
		std::cout << "No se pudo abrir el archivo yellow_tripdata_2009-12.csv\n";
		std::cout << "revisa tu directorio o descarga el dataset en: https://s3.amazonaws.com/nyc-tlc/trip+data/yellow_tripdata_2009-12.csv\n";
		return;
	}

	std::string line;
	std::string data_set_path = all? "yellowtrip_depured_data_set.txt" 
									  : "yellowtrip_depured_data_set_" + std::to_string(n) + ".txt";
	
	if(fileExists(data_set_path))
	{
		// el archivo ya existe, simplemente retorno
		return;
	}
	std::ofstream data_set_writer(data_set_path);
	std::vector<std::string> data_vec;

	if(all)
	{
		std::getline(data_set_reader, line);
		std::getline(data_set_reader, line);

		while (std::getline(data_set_reader, line))
		{
			data_vec = split(line, ",");
			if (!data_vec.empty())
			{
				data_set_writer << data_vec[5] << " " << data_vec[6] << "\n";
			}
		}
	}
	else
	{
		std::getline(data_set_reader, line);
		std::getline(data_set_reader, line);

		for (int i = 0; i < n; ++i)
		{
			std::getline(data_set_reader, line);
			data_vec = split(line, ",");
			if (!data_vec.empty()) {
				data_set_writer << data_vec[5] << " " << data_vec[6] << "\n";
			}
		}
	}

	data_set_reader.close();
	data_set_writer.close();
}


template<int dim>
void read_points_data(int n, std::vector<Point<dim>>& data_points)
{
	data_points.clear();

	std::string path_data_set = "yellowtrip_depured_data_set_" + std::to_string(n) + ".txt";
	std::ifstream data_set(path_data_set);
	if (!data_set.is_open())
	{
		std::cout << "no se encontró el archivo " << path_data_set << std::endl;
		return;
	}
	// longitud y latitud 
	float longi, latti;
	
	// usamos un set para no repetir puntos en el conjunto de datos, esto es ideal
	// para eliminar información inecesaria y para ahorrar memoria
	std::set<std::array<float, dim>> non_copy_set;
	while (data_set >> longi >> latti) 
	{
		//if(longi <= -74.0625 && latti > 0)
			non_copy_set.insert({ longi, latti });
	}

	for (auto non_copied_point : non_copy_set) 
		data_points.push_back(Point<dim>(non_copied_point));

	std::cout << "cleaned data_size: " << data_points.size() << std::endl;
}



#endif // GENERATE_DATA_SETS_HPP_ 
