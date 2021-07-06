# Proyecto final (**Substring Matching**) para el curso de estructuras de datos avanzadas  
## Vista general
La definición del problema y los algoritmos implementados los encuentra en la carpeta `presentations/`, dentro hay dos documentos `presentations/parcial_presentacion_1.pdf` y `presentations/final_presentacion_2.pdf`, en el primero se expone el problema y en el segundo se muestran las implementaciones en C++ de los algoritmos propuestos en la presentación parcial, además de los resultados de las pruebas realizadas en el software.  
## Conjunto de datos  
En la carpeta `PythonProcesses/` están ubicados los programas hechos en python para generar el conjunto de datos limpio: [data_set.txt](https://drive.google.com/file/d/1Bb9Jqn16zbLK69xlGaljnsok23Vutuil/view?usp=sharing), que usamos en la generación de nuestro SuffixTree en C++. El primer conjunto de datos lo encuentra en el siguiente link: [Arxiv Dataset](https://www.kaggle.com/Cornell-University/arxiv).  
## Prerequisitos para compilar y ejecutar el programa
- Instalar [cmake](https://cmake.org/download/).
- Descrompimir el archivo .zip (del proyecto) en un directorio.
- Si eres usuario windows descargar Visual Studio Community [Visual Studio 2019](https://visualstudio.microsoft.com/es/downloads/).
## Pasos para la compilación y ejecución del código del programa
1. Generar la carpeta build: dentro del directorio general ejecutaremos lo siguiente.
   * Para usuarios Windows  
     `$ cmake -S . -B build -G "Visual Studio 16 2019"`
   * Para usuarios Linux  
     `$ cmake -S . -B build -G "Unix Makefiles"`
2. Generar el ejecutable: ahora entramos a la carpeta build y seguimos los siguientes pasos.
   * Para usuarios Windows   
     Abrir el archivo "String Matching with suffix-tree.sln", presiona las teclas (Ctrl+Shift+B), esto compilará el programa,
     también puedes probar con la tecla F5 para compilar y ejecutar (pero te dará error si no tienes el conjunto de datos dentro
     de la carpeta build).
   * Para usuarios Linux (tipear en la terminal)  
     `$ make`
3. Descargar el conjunto de datos: Colocar dentro de la carpeta build el documento que podrá descargarlo en [data_set.txt](https://drive.google.com/file/d/1Bb9Jqn16zbLK69xlGaljnsok23Vutuil/view?usp=sharing)
4. Ejecutar el programa: El ejecutable ya fue generado, por lo que podremos ejecutarlo de la siguiente manera.
   * Para usuarios Windows
     Dentro de Visual Studio Community presiona la tecla F5, esto compilará y ejecutará el programa dentro del IDE.
   * Para Usuarios Linux (tipear en la terminal)  
     `$ ./string_matching`
## Instrucciones de Uso y Advertencias
1. El programa primero le indicará que ingrese la cantidad de documentos que desea cargar, puede cargar hasta 1.7 Millones aproximadamente, pero tan solo con 20000 el programa ocupa alrededor de 4.2 GB, por lo que no recomiendo cargar varios a menos que se tenga una RAM de 512 GB.  
2. Luego el programa mostrará los detalles del tiempo que demora la lectura y la construcción del árbol de sufijos (solo espere hasta que el programa le pida las búsquedas).
3. Por último el programa le solicitará el ingreso de una cadena relacionada a búsquedas de papers académicos.
   ingrese por ejemplo "quantum mechanics".
4. Una vez ingresada la consulta, el programa le mostrará un ranking de resultados ordenados de mayor a menor por un score
   (TF-IDF) de mayor similitud con la búsqueda, en el cual le señalará el puntaje del documento, su link en arxiv y el link
   del documento ".pdf".
5. Escriba **EXIT** para salir del programa.
