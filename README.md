## Proyecto de coloreo de grafos

En este proyecto se realizó una implementación en C para hacer coloreo de grafos y distintos reordenamiento de vertices para disminuir el numero cromático en las sucesivas corridas de greedy.
Los grafos se ingresan por stdin en formato DIMACS.

## Especificacion requerida


**u32:**

Se utilizará el tipo de dato u32 para especificar un entero de 32 bits sin signo.
Todos los enteros sin signo de 32 bits que aparezcan en la implementación deberán usar este tipo de dato.
Los grafos a colorear tendran una lista de lados cuyos vertices seran todos u32.


**GrafoSt:**

Es una estructura, la cual debe contener toda la información sobre el grafo necesaria para correr su implementacion. La
definicion interna de esta estructura es a eleccion de ustedes y debera soportar los metodos que se describiran mas adelante,
mas los metodos que ustedes consideren necesarios para implementar los algoritmos que esten implementando.
Entre los parametros debe haber como mınimo los necesarios para guardar los datos de un grafo (vertices y lados) pero
ademas los necesarios para guardar el coloreo que se tiene hasta ese momento en el grafo y cualquier información requerida
en los algoritmos a implementar.
IMPORTANTE: Como usaremos un algoritmo similar a Greedy, el cual usa un orden de los vertices, en
esta estructura tiene que estar guardado algun orden de los vertices, y como vamos a cambiar ese orden
repetidamente, debe ser algo que pueda ser cambiado.
El coloreo siempre debe cumplir que si es un coloreo con j colores entonces los colores son 1, 2, ..., j.


**Grafo:**


Es un puntero a una estructura de datos GrafoSt.


**Funciones De Construccion/Destruccion del grafo:**


**ConstruccionDelGrafo()**

Prototipo de funcion:

**Grafo ConstruccionDelGrafo();**

La funcion aloca memoria, inicializa lo que haya que inicializar de una estructura GrafoSt ,lee un grafo desde standard
input en el formato indicado abajo, lo carga en la estructura y devuelve un puntero a esta.


**Formato de Entrada**
El formato de entrada sera una variacion de DIMACS, que es un formato estandard para representar grafos, con algunos
cambios.
La descripcion oficial de DIMACS es asi:

• Ninguna linea tiene mas de 80 caracteres. PERO hemos visto archivos DIMACS en la web que NO cumplen esta
especificacion, asi que NO la pediremos.
Su codigo debe poder procesar lineas con una cantidad arbitraria de caracteres.

• Al principio habra cero o mas lineas que empiezan con c las cuales son lineas de comentario y deben ignorarse.

• Luego hay una linea de la forma:
p edge n m
donde n y m son dos enteros. Entre n y m puede haber una cantidad arbitraria de espacios en blancos, e igualmente
luego de m.
El primero numero (n) en teorıa representa el numero de vertices y el segundo (m) el numero de lados, pero hay ejemplos
en la web en donde n es en realidad solo una COTA SUPERIOR del numero de vertices y m una cota superior del
numero de lados. Sin embargo, todos los grafos que nosotros usaremos para testear cumpliran que n sera el
numero de vertices exacto y m el numero de lados exacto, asi que ustedes pueden asumir eso en su programa,
pero si lo testean con algun grafo de la web, pueden tener problemas. De todos modos, en mi pagina hay muchos
ejemplos bien formateados.

• Luego siguen m lineas todas comenzando con e y dos enteros, representando un lado. Es decir, lineas de la forma:
e v w
(Entre “v” y “w”, asi como luego de “w”, puede haber una cantidad arbitraria de espacios en blanco)


• Nunca fijaremos m = 0, es decir, siempre habra al menos un lado. (y por lo tanto, al menos dos vertices).

• En el formato DIMACS no parece estar especificado si hay algun limite para los enteros, pero en nuestro caso los
limitaremos a enteros de 32 bits sin signo.

• Observar que en el ejemplo y en muchos otros casos en la web los vertices son 1,2,...,n, PERO ESO NO SIEMPRE
SERA ASI. 


• El orden de los lados no tiene porque ser en orden ascendente de los vertices.


**DestruccionDelGrafo()**

Prototipo de funcion:


**void DestruccionDelGrafo(Grafo G);**


Destruye G y libera la memoria alocada.

**Funciones para extraer información de datos del grafo**

**NumeroDeVertices()**

Prototipo de funcion:


**u32 NumeroDeVertices(Grafo G);**


Devuelve el numero de vertices de G.


**NumeroDeLados()**

Prototipo de funcion:


**u32 NumeroDeLados(Grafo G);**

Devuelve el numero de lados de G.

**NumeroDeColores()**

Prototipo de funcion:


**u32 NumeroDeColores(Grafo G);**

Devuelve la cantidad de colores usados en el coloreo que tiene en ese momento G.


**Funciones para extraer información de los vertices**


**NombreDelVertice()**

Prototipo de Funcion:

u32 NombreDelVertice(Grafo G, u32 i);
Devuelve el nombre real del vertice numero i en el orden guardado en ese momento en G. (el ındice 0 indica el primer
vertice, el ındice 1 el segundo, etc)

**ColorDelVertice()**

Prototipo de Funcion:


**u32 ColorDelVertice(Grafo G, u32 i);**

Devuelve el color con el que esta coloreado el vertice numero i en el orden guardado en ese momento en G. (el ındice 0
indica el primer vertice, el ındice 1 el segundo, etc)

**GradoDelVertice()**

Prototipo de Funcion:


**u32 GradoDelVertice(Grafo G, u32 i);**

Devuelve el grado del vertice numero i en el orden guardado en ese momento en G. (el ındice 0 indica el primer vertice,
el ındice 1 el segundo, etc)

**ColorJotaesimoVecino()**

Prototipo de funcion:


**u32 ColorJotaesimoVecino(Grafo G, u32 i,u32 j);**

Devuelve el color del vecino numero j (en el orden en que lo tengan guardado uds. en G, con el ındice 0 indicando el
primer vecino, el ındice 1 el segundo, etc)) del vertice numero i en el orden guardado en ese momento en G. (el ındice 0 indica
el primer vertice, el ındice 1 el segundo, etc)
Como en las otras funciones, el orden de LOS VERTICES guardado en G es el orden en que supuestamente vamos a correr
NotSoGreedy, pero el orden DE LOS VECINOS que usen ustedes es irrelevante, lo importante es que de esta forma podemos
iterar sobre los vecinos para realizar tests. (por ejemplo, para saber si el coloreo es o no propio).


**NombreJotaesimoVecino()**

Prototipo de funcion:


**u32 NombreJotaesimoVecino(Grafo G, u32 i,u32 j);**

Devuelve el nombre del vecino numero j (en el orden en que lo tengan guardado uds. en G, con el ındice 0 indicando el
primer vecino, el ındice 1 el segundo, etc)) del vertice numero i en el orden guardado en ese momento en G. (el ındice 0 indica
el primer vertice, el ındice 1 el segundo, etc)

**GradoJotaesimoVecino()**
Podriamos definir una funcion similar a las dos anteriores pero para grado. Sin embargo, no se preveen situaciones de uso asi
que no es requerida.

**Funciones de coloreo**

**NotSoGreedy()**

Prototipo de funcion:


**u32 NotSoGreedy(Grafo G,u32 semilla);**

Corre la variacion de greedy que se indica mas abajo en G con el orden interno que debe estar guardado de alguna forma
dentro de G. Devuelve el numero de colores que se obtiene.
La diferencia con Greedy normal es la siguiente: en Greedy se le da al vertice v el MENOR color posible que no sea un
color de alguno de los vecinos de v ya coloreados. En NotSoGreedy en cambio, cuando se esta por colorear al vertice v se
hace lo siguiente:
1. Si hasta ese momento el algoritmo esta usando b colores, y los colores 1, 2, ..., b son todos colores de algun vecino de v,
entonces a v se le da el color b + 1. (esto es igual que en Greedy)
2. Si hasta ese momento el algoritmo esta usando b colores y no todos los colores 1, 2, ..., b son colores de algun vecino,
entonces se elige al azar ALGUNO de los colores 1, 2, ..., b que no sea color de algun vecino de v, y se colorea v con ese
color.


**Bipartito()**

Prototipo de funcion:


**int Bipartito(Grafo G);**

Si k es el numero de componentes conexas de G, devuelve k si G es bipartito, y −k si no.
Ademas, si es bipartito, colorea G con un coloreo propio de dos colores. Si no es bipartito, debe dejar a G coloreado con
algun coloreo propio. (no necesariamente el mismo que tenıa al principio).

**Funciones de ordenacion**
Estas funciones cambian el orden interno guardado en G que se usa para correr NotSoGreedy.

**OrdenNatural()**

Prototipo de funcion:


**void OrdenNatural(Grafo G);**

Ordena los vertices en orden creciente de sus “nombres” reales. (recordar que el nombre real de un vertice es un u32).


**OrdenWelshPowell()**

Prototipo de funcion:


**void OrdenWelshPowell(Grafo G);**

Ordena los vertices de W de acuerdo con el orden Welsh-Powell, es decir, con los grados en orden no creciente.
Por ejemplo si hacemos un for en i de GradoDelVertice(G,i) luego de haber corrido OrdenWelshPowell(G), deberiamos
obtener algo como 10,9,7,7,7,7,5,4,4,3,3,2,1,1,1,1

**AleatorizarVertices()**

Prototipo de funcion:


**void AleatorizarVertices(Grafo G,u32 semilla);**

Esta funcion ordena pseudoaleatoriamente los vertices de G, usando alguna funcion pseudoaleatoria que dependa determinısticamente
de semilla. La funcion de pseudoaleatoriedad que usen es a criterio suyo, pero debe depender deterministicamente
de la variable semilla. (es decir, correr dos veces esta funcion con semilla=4 pej, debe dar los mismos resultados, pero
si semilla=12, deberıa dar otro resultado).
IMPORTANTE: debe dar el mismo resultado independientemente del coloreo o del orden que tenga en ese momento G. Es decir, si esta funcion es llamada en dos lugares distintos de un programa con la misma
semilla, el orden obtenido debe ser el mismo.
Como dijimos en la funcion NotSoGreedy, pueden usar la funcion de pseudoaleatoriedad que deseen EXCEPTO rand().

**ReordenManteniendoBloqueColores()**

Prototipo de funcion:


**void ReordenManteniendoBloqueColores(Grafo G,u32 x);**

Si G esta coloreado con r colores y V C1 son los vertices coloreados con 1, V C2 los coloreados con 2, etc, entonces esta
funcion ordena los vertices poniendo primero los vertices de V Cj1
, luego los de V Cj2
, etc, donde j1, j2, ..., jr se determinan
a partir de x de la siguiente manera:

• Si x=0, entonces j1 = r, j2 = r − 1,...,jr−1 = 2, jr = 1.

• Si x=1, entonces j1, j2, ... son tales que |V Cj1
| ≤ |V Cj2
| ≤ ... ≤ |V Cjr|

• Si x>1, entonces se usa alguna funcion de pseudoaleatoriedad que dependa determinısticamente de x para generar un
orden aleatorio de los numeros {1, 2, ..., r}, obteniendo un orden j1, j2, ..., jr.
