# Curso de Estructuras de datos avanzadas (EDA 2021-01)  
## Proyecto-Final  
Entre en la carpeta `Proyecto-Final`, ahí encontrará las indicaciones para su uso y su descripción.  
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
# Lista Persistente  
El trabajo consistió en hacer una lista persistente que permite la inserción de nodos con un nombre de campo y su valor, cada nodo presenta una tabla de datos: **[\<field\>:\<value\>]** y una de modificaciones con el formato **V(\<version\>) [\<field\>:\<value\>]**, además de una tabla de punteros a sus siguientes nodos con el formato **V(\<version\> : \<nextPtr\>)**, cada vez que inserto un nuevo nodo en la lista la versión se actualiza.  
Las funciones importantes son **_insert(nodePtr, field, value, newPtr=nullptr)_** y **_find(version,field)_**.  
### Función insert  
#### descripción:
Actualiza el valor de algún campo de los datos de un nodo con la versión actual de la lista, en caso que se desee modificar el puntero siguiente, pasamos como parámetro el nodo siguiente, también actualiza la versión de la lista(++version_list) después de realizada la inserción.  
#### parámetros:
* *_nodePtr_* es un nodo de la lista, donde se insertará el nuevo registro.  
* *_field_* es el nombre del campo registrado en la tabla de datos del nodo *_nodePtr_*, en caso de que este sea **\"NEXT_PTR\"** significa que modificaremos su puntero siguiente.  
* *_value_* es el nuevo valor del campo modificado.  
* *_newPtr_* es el nuevo nodo apuntado por *_nodePtr_* en su tabla de punteros siguientes, solo se debe llenar cuando *_field_*=**\"NEXT_PTR\"**.  
#### resultados:  
La lista se actualiza con el nuevo dato y aumenta internamente la versión.  
### Función find  
#### descripción:
Busca el valor del campo *_field_* en la versión *_version_* y lo imprime.  
#### parámetros:
* *_version_* es la versión específica del campo que estamos buscando.  
* *_field_* es el nombre del campo el cual deseamos encontrar, si tuviese el valor **\"NEXT_PTR\"** buscaremos el puntero siguiente, por lo que imprimiremos los datos de un nodo.  
#### resultados:  
Imprime el valor del campo en la versión especificada.  
## Uso del programa  
Dentro de la carpeta `PersistentList` ejecute el comando `cmake -S . -B build`, luego entre a la carpeta build y dependiendo de su compilador de c++ compile y ejecute el programa, el programa main.cpp presenta un ejemplo predefinido que puede verlo como imagen en la carpeta `images/abstract_example.svg`, los datos de entrada del ejemplo se encuentran en el documento `source/inputs.txt`.  
