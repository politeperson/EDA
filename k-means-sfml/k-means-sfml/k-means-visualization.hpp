#pragma once
#ifndef K_MEANS_VISUALIZATION_HPP_
#define K_MEANS_VISUALIZATION_HPP_
#include <SFML/Graphics.hpp>
#include "KMeans.hpp"
#include <iostream>
const int max_num_of_cluster = 13;

const sf::Color background = sf::Color::Black;

sf::Color clusters_colors[max_num_of_cluster] = { sf::Color(255,255,255),
                                                 sf::Color(255,0,0),
                                                 sf::Color(0,255,0),
                                                 sf::Color(0,0,255),
                                                 sf::Color(255, 153, 51),
                                                 sf::Color(204, 51, 255),
                                                 sf::Color(0, 204, 102),
                                                 sf::Color(102, 153, 0),
                                                 sf::Color(255, 255, 0),
                                                 sf::Color(153, 51, 0),
                                                 sf::Color(102, 102, 153),
                                                 sf::Color(255, 51, 153),
                                                 sf::Color(0, 255, 255), };


// los puntos contienne dimensión 2
template<int dim>
void show_points(std::vector<Point<dim>>& points, sf::RenderWindow& win, sf::View& view)
{
    if (points.empty()) {
        std::cout << "no hay puntos recibidos :(\n";
        return;
    }
    if (dim != 2)
    {
        std::cout << "solo se aceptan puntos en R^2\n";
        return;
    }
    sf::CircleShape drawPoint;
    float radius_points = abs(view.getSize().x + view.getSize().y) / 1000;

    //std::vector<sf::CircleShape> drawablePoints;
    size_t n_points = points.size();
    for (size_t i = 0; i < n_points; ++i)
    {
        //drawablePoints.push_back();
        // graficamos por longitud y latitud
        drawPoint.setRadius(radius_points);
        drawPoint.setPosition(points[i].p[0], points[i].p[1]);
        drawPoint.setFillColor(sf::Color(0, 255, 0));
        win.draw(drawPoint);
    }
}

template<int dim>
std::vector<sf::CircleShape> get_k_means_shapes(const KM<dim>& kmeansAlgo, std::vector<sf::CircleShape>& k_means_centroids)
{
    if (kmeansAlgo.getPoints().empty()) {
        std::cout << "no hay conjunto de datos que graficar :(\n";
        return std::vector<sf::CircleShape>(); // una lista de figuras vacías
    }
    if (dim != 2)
    {
        std::cout << "solo se aceptan puntos en R^2\n";
        return std::vector<sf::CircleShape>(); // una lista de figuras vacías
    }

    int C = kmeansAlgo.getC();
    std::vector<sf::CircleShape> drawable_shapes;
    std::vector<int> assigns = kmeansAlgo.getAssignments();
    std::vector<Point<dim>> points = kmeansAlgo.getPoints();
    std::vector<Point<dim>> centroids = kmeansAlgo.getCentroids();

    // primero añadimos los centroides
    for (int i = 0; i < C; ++i)
    {
        //std::cout << centroids[i].p[0] << ", " << centroids[i].p[1] << std::endl;
        // triangulos
        sf::CircleShape tmp_triangle(0.1f, 3);
        tmp_triangle.setFillColor(background);
        tmp_triangle.setOutlineThickness(1);
        tmp_triangle.setOutlineColor(clusters_colors[i % max_num_of_cluster]);
        tmp_triangle.setPosition(centroids[i].p[0], centroids[i].p[1]);

        k_means_centroids.push_back(tmp_triangle);
    }


    for (size_t i = 0; i < points.size(); ++i)
    {
        sf::CircleShape tmp_circle(0.05f);
        //std::cout << assigns[i] << std::endl;
        tmp_circle.setFillColor(clusters_colors[assigns[i]  % max_num_of_cluster]);
        tmp_circle.setPosition(points[i].p[0], points[i].p[1]);
        drawable_shapes.push_back(tmp_circle);
    }

    return drawable_shapes;
}

void show_k_means_results(int C, std::vector<sf::CircleShape>& data_set_points, std::vector<sf::CircleShape>& centroids,
                           sf::RenderWindow& win, sf::View& view)
{
    float resized_radius = abs(view.getSize().x + view.getSize().y) / 1000;
    // graficando los centroides
    for (int i = 0; i < C; ++i)
    {
        centroids[i].setRadius(resized_radius * 2);
        centroids[i].setOutlineThickness(resized_radius / 2);

        win.draw(centroids[i]);
    }
    
    size_t n_points = data_set_points.size();
    for (sf::CircleShape& set_point : data_set_points)
    {
        //drawablePoints.push_back();
        // graficamos por longitud y latitud
        set_point.setRadius(resized_radius);
        win.draw(set_point);
    }/**/
}







#endif // !K_MEANS_VISUALIZATION_HPP_
