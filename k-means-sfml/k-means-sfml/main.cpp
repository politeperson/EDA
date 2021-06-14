// SFML
#include <SFML/Graphics.hpp>
// Librerías propias
#include "KMeans.hpp"
#include "gen_and_read_dataset.hpp"
#include "k-means-visualization.hpp"

#define _USE_MATH_DEFINES

// C++
#include <math.h>

#define DIMENSION 2

void generar_puntos_de_prueba(std::vector<Point<DIMENSION>>& points)
{
    std::cout << "Escoge la opcion de conjunto de prueba que desees visualizar: \n";
    std::cout << "1) Caja simple\n";
    std::cout << "2) Circulos simples\n";
    std::cout << "3) Rectangulos simples\n";
    int opcion;
    float square_range = 200;
    std::cin >> opcion;
    Point<DIMENSION> tmp_point;

	if (opcion == 1)
	{
        int n_random_points;
        std::cout << "Ingresa la cantidad de puntos aleatorios que quieres generar (solo positivos): ";
        std::cin >> n_random_points;
		std::cout << "generando puntos aleatorios...\n";
		std::cout << "rango en x = [-200, 200]\n";
		std::cout << "rango en y = [-200, 200]\n";
		for (int i = 0; i < n_random_points; ++i)
		{
            tmp_point.setP(std::array<float, DIMENSION>({ getRandomFloat(-square_range, square_range), 
                                                          getRandomFloat(-square_range, square_range) }));
			points.push_back(tmp_point);
		}
    }
    else if (opcion == 2)
    {
        int n_circulos;
        float density;
        bool uniform;
        do
        {
            std::cout << "Ingresa la cantidad de circulos que quieres generar (solo positivos): ";
            std::cin >> n_circulos;
            std::cout << "Ingresa la cantidad puntos (densidad) aleatorios en cada circulo rango[0,1]: ";
            std::cin >> density;
        } while (density < 0 || density > 1 || n_circulos <= 0);
        
        std::cout << "Deseas circulos variados o uniformes (0: variado, 1: uniforme): ";
        std::cin >> uniform;
        
        std::cout << "Generando circulos espera...\n";
        // por defecto el radio de los círculos es 20, si y solo si son uniformes
        float radio = 20, area;
        Point<DIMENSION> center; // un centro para los círculos
        for(int i = 0; i < n_circulos; ++i)
        {
            // un centro temporal
            center.setP(std::array<float, DIMENSION>({ getRandomFloat(-square_range, square_range),
                                                       getRandomFloat(-square_range, square_range) }));
            
            if (!uniform) radio = getRandomFloat(10, 20); // un radio aleatorio entre 10 y 20
            for (int j = 0; j < static_cast<int>(M_PI * radio * radio * density); ++j)
            {
                bool is_inside_circle;
                do
                {
                    tmp_point.setP(std::array<float, DIMENSION>(
                        { getRandomFloat(center.p[0] - radio, center.p[0] + radio) ,
                          getRandomFloat(center.p[1] - radio, center.p[1] + radio) }));
                    is_inside_circle = tmp_point.distance(center) <= radio;
                } while (!is_inside_circle);
                points.push_back(tmp_point);
            }
        }
    }
    else {
        int n_rectangulos;
        float density;
        bool uniform;
        do
        {
            std::cout << "Ingresa la cantidad de rectangulos que quieres generar (solo positivos): ";
            std::cin >> n_rectangulos;
            std::cout << "Ingresa la cantidad puntos (densidad) aleatorios en cada rectangulo rango[0,1]:";
            std::cin >> density;
            
        } while (density < 0 || density > 1 || n_rectangulos <= 0);
        std::cout << "Deseas rectangulos variados o uniformes (0: variado, 1: uniforme): ";
        std::cin >> uniform;
        int largo = 10, ancho = 10;
        if (uniform) {
            do
            {
                std::cout << "ingresa el largo(coordenada x) rango[10, 50]: ";
                std::cin >> largo;
                std::cout << "ingresa el ancho(coordenada y) rango[10, 50]: ";
                std::cin >> ancho;
            } while (largo < 10 || largo > 50 || ancho < 10 || ancho > 50);
        }
        
        std::cout << "Generando rectangulos espera...\n";
        // por defecto el radio de los círculos es 20, si y solo si son uniformes
        float radio = 20, area;
        Point<DIMENSION> center; // el origen de cada cuadrado, su centro
        for (int i = 0; i < n_rectangulos; ++i)
        {
            center.setP(std::array<float, DIMENSION>({ getRandomFloat(-square_range, square_range),
                                                       getRandomFloat(-square_range, square_range) }));
            if (!uniform) {
                largo = getRandomInteger(10, 50); // rango [10, 50] para ambos
                ancho = getRandomInteger(10, 50);
            }
            for (int j = 0; j < static_cast<int>((float)(largo * ancho) * density); ++j)
            {
				tmp_point.setP(std::array<float, DIMENSION>(
					{ getRandomFloat(center.p[0] - (largo / 2.f), center.p[0] + (largo / 2.f)) ,
					  getRandomFloat(center.p[1] - (ancho / 2.f), center.p[1] + (ancho / 2.f)) }));
				points.push_back(tmp_point);
            }
        }
    }
}

int main()
{
    int data_set_size;
    char option;
    std::vector<Point<DIMENSION>> points;
    std::cout << "el clustering funciona para puntos multidimensionales, pero los ejemplos\n";
    std::cout << "se encuentran en R^2\n\n";

    std::cout << "Desea generar el dataset de NY(n) o los de prueba(p)?: ";
    std::cin >> option;
    if(option == 'n')
    {
        do
        {
            std::cout << "Ingrese la cantidad de puntos a generar, rango permitido [1,500000]: ";
            
            std::cin >> data_set_size;
            generate_data_set(false, data_set_size); //primero generé un documento de 100000 de puntos
            read_points_data(data_set_size, points); // ahora leo 100000 puntos
        } while (data_set_size <= 0 || data_set_size > 500000);
    }
    else
        generar_puntos_de_prueba(points);

    int n_clusters, iterations;
    do
    {
        std::cout << "Elija la cantidad de clusters solo positivos (max: "<< max_num_of_cluster << "): ";
        std::cin >> n_clusters;
        std::cout << "Elija la cantidad de iteraciones para el algoritmo K-means (solo positivos): ";
        std::cin >> iterations;

    } while (n_clusters <= 0 || n_clusters > max_num_of_cluster || iterations <= 0);
    

    KM<DIMENSION> NY(points, n_clusters);
    
    std::vector<Point<DIMENSION>>init_pos_clusters = NY.getCentroids();

    NY.KMeans(iterations); // cuando se llega a n iteraciones, el algoritmo se detiene

    std::vector<sf::CircleShape> k_means_centroids;
    std::vector<sf::CircleShape> k_means_shapes = get_k_means_shapes(NY, k_means_centroids);
    
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "K-means Visualization");
    window.setFramerateLimit(60);
    
    // sf::View view(sf::FloatRect(-50, -50, 300, 200));
    sf::View view(sf::Vector2f(0.f, 0.f), sf::Vector2f(400.f, 300.f));

    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    
    sf::Vertex x_coord[] =
    {
        sf::Vertex(sf::Vector2f(-500, 0)),
        sf::Vertex(sf::Vector2f(500, 0))
    };

    sf::Vertex y_coord[] =
    {
        sf::Vertex(sf::Vector2f(0, -500)),
        sf::Vertex(sf::Vector2f(0, 500))
    };

    // run the program as long as the window is open
    while (window.isOpen())
    {
        
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseWheelMoved) 
                view.zoom(1.f + event.mouseWheel.delta * 0.05f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            view.move(view.getSize().x / 100.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            view.move(-view.getSize().x / 100.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            view.move(0.f, -view.getSize().y / 100.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            view.move(0.f, view.getSize().y / 100.f);


        // clear the window with black color
        window.clear(background);

        // dibujamos los puntos del data set
        // show_points(points, window, view);
        // show_points(init_pos_clusters, window, view);
        show_k_means_results(n_clusters, k_means_shapes, k_means_centroids, window, view);
        
        // dibujamos las coordenadas x e y
        window.draw(x_coord, 2, sf::Lines);
        window.draw(y_coord, 2, sf::Lines);

        // configurando la vista
        window.setView( view );

        // end the current frame
        window.display();
    }
    

	return 0;
}