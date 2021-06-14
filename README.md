# Curso de Estructuras de datos avanzadas (EDA 2021-01)  
## vectors-distance  
Tarea para obtener una matriz de distancias  
## PR-QuadTree  
Estructura de datos para visualizar un mapa de calor, al momento de ejecutar cmake para contruir la carpeta build se debe mover los archivos: **DataSet.tsv**, **HeadMap.txt**, **TaleMap.txt**, a la carpeta build, esto generará un documento **LeafMap.html** en donde la visualización estará, también encontrará un archivo con la ruta **PR-quadtree/source/LeafMap.html**, que es un ejemplo de un mapa de calor con un PR-quadtree de profundidad 9.   
## R-Tree  
Estructura de datos R-Tree, almacena cuadrados, aún incompleto (falta función delete).  
## K-Means (librería SFML)  
Algoritmo de agrupación(clustering) k-means para datos multidimensionales, pero para la visualización solo debe usar puntos en R<sup>2</sup>, el conjunto de datos se encuentra en [yellow_tripdata dataset](**https://s3.amazonaws.com/nyc-tlc/trip+data/yellow_tripdata_2009-12.csv**), solo se usaron los campos **_Start_Lon_** y **_Start_Lat_**.  
Los parámetros principales fueron:  
1. **n_clusters:** la cantidad de centroides para el algoritmo.  
2. **iterations:** la cantidad de iteraciones en el algoritmo.  

Las funciones principales se encuentran en la clase **KM**, en particular revise los métodos:
* **_KM():_** El constructor de la clase, donde genero los **C** centroides pasados como parámetros.  
* **_void adjustCentroids():_** Ajusta cada uno de los centroides de la clase, primero revisa las asignaciones de los puntos a cada cluster y después actualiza los centroides al promedio de las entradas de cada una de las dimensiones definidas.  
* **_void assignCentroids():_** Asigna todos los puntos a un determinado cluster.  
***
### Ejemplos  
Son tres los ejemplos de prueba y el dataset NY, cuyos resultados se ve en la carpeta `k-means-sfml/results/`, primero mostramos los ejemplos de prueba:  
**caja simple 5K puntos con 5 clusters**
![caja simple 5K puntos con 5 clusters logs](https://raw.githubusercontent.com/politeperson/EDA/main/k-means-sfml/results/example_5Kp_5C_log.PNG)
![caja simple 5K puntos con 5 clusters](https://raw.githubusercontent.com/politeperson/EDA/main/k-means-sfml/results/example_5Kp_5C_visual.PNG)
**círculos simples (10) con 5 clusters**
![circulo simple 10 con 5 clusters](https://raw.githubusercontent.com/politeperson/EDA/main/k-means-sfml/results/Circles_10_5C_log.PNG)
![circulo simple 10 con 5 clusters](https://raw.githubusercontent.com/politeperson/EDA/main/k-means-sfml/results/Circles_10_5C_visual.PNG)
**cuadrados simples (5) con 5 clusters**
![cuadrado simple 5 con 5 clusters](https://raw.githubusercontent.com/politeperson/EDA/main/k-means-sfml/results/Squares_5_5C_log.PNG)
![cuadrado simple 5 con 5 clusters](https://raw.githubusercontent.com/politeperson/EDA/main/k-means-sfml/results/Squares_5_5C_visual.PNG)
**NY dataset 100K puntos con 7 clusters**
En el gráfico no se aprecia bien la agrupación ya que no se hizo el acercamiento debido.  
![NY con 100K 7 clusters](https://raw.githubusercontent.com/politeperson/EDA/main/k-means-sfml/results/NY_data_set_100K_with_7_clusters.PNG)
Para ver mas conjuntos de datos revise la carpeta `k-means-sfml/results/`.