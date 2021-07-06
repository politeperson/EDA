Prerequisitos:
- Instalar cmake (https://cmake.org/download/).
- Descrompimir el archivo .zip en un directorio.
- Si eres usuario windows descargar Visual Studio Community (https://visualstudio.microsoft.com/es/downloads/).
Pasos para la compilación y ejecución del código del programa:
1. Generar la carpeta build: dentro del directorio general ejecutaremos lo siguiente.
   * Para usuarios Windows
     $ cmake -S . -B build -G "Visual Studio 16 2019"
   * Para usuarios Linux
     $ cmake -S . -B build -G "Unix Makefiles"
2. Generar el ejecutable: ahora entramos a la carpeta build y seguimos los siguientes pasos.
   * Para usuarios Windows 
     Abrir el archivo "String Matching with suffix-tree.sln", presiona las teclas (Ctrl+Shift+B), esto compilará el programa,
     también puedes probar con la tecla F5 para compilar y ejecutar (pero te dará error si no tienes el conjunto de datos dentro
     de la carpeta build).
   * Para usuarios Linux (tipear en la terminal)
     $ make
3. Descargar el conjunto de datos: Colocar dentro de la carpeta build el documento que podrá descargarlo en (https://drive.google.com/file/d/1Bb9Jqn16zbLK69xlGaljnsok23Vutuil/view?usp=sharing)
4. Ejecutar el programa: El ejecutable ya fue generado, por lo que podremos ejecutarlo de la siguiente manera.
   * Para usuarios Windows
     Dentro de Visual Studio Community presiona la tecla F5, esto compilará y ejecutará el programa dentro del IDE.
   * Para Usuarios Linux (tipear en la terminal)
     $ ./string_matching
Instrucciones de Uso:
1. El programa es muy intuitivo, primero le pedirá la cantidad de documentos que desea cargar.
   Advertencia: Lamentablemente el programa ocupa mucha memoria con 20000 documentos, trate de nos obrepasar este límite.
2. Luego el programa mostrará los detalles del tiempo que demora en cargar (solo espere hasta que el programa le pida algo).
3. Por último el programa le solicitará el ingreso de una cadena relacionada a búsquedas de papers académicos.
   ingrese por ejemplo "quantum mechanics".
4. Una vez ingresada la consulta, el programa le mostrará un ranking de resultados ordenados de mayor a menor por un score
   (TF-IDF) de mayor similitud con la búsqueda, en el cual le señalará el puntaje del documento, su link en arxiv y el link
   del documento ".pdf".
5. Escriba EXIT para salir del programa.
