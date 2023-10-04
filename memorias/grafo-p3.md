# INFORMÁTICA GRÁFICA 2022/2023. 

# PRÁCTICA 3. MODELOS JERÁRQUICOS.

Hecha por David González Durán (Grupo 2 de prácticas, doble grado de Ingeniería Informática y ADE)



## 1. Mi diseño original: intro de GameCube.

Mi diseño original trata de copiar la animación de la intro de GameCube - salvo obviamente el texto que le acompaña -. La intro está a continuación (pincha en la imagen para redireccionar al vídeo en YouTube):

[![INTRO DE GAMECUBE](https://img.youtube.com/vi/CpmYW-gCSy4/0.jpg)](https://youtu.be/CpmYW-gCSy4)

Para esta tercera práctica todavía no introduciré iluminación ni materiales, de eso ya nos ocuparemos en las siguientes prácticas. 





## 2. Analizando la composición.

Primero, conviene diferenciar los objetos que podrían formar la animación:

1. **Cubo de referencia**: Este cubo no es visible, ni siquiera vamos a introducirlo en el grafo, pero se intuye en el vídeo, ya que tanto los cuadrados de la superficie como el cubo móvil están sobre su superficie y rotan sobre ella. A partir de este cubo de 3x3x3 haremos cálculos para disponer los cuadrados de la superficie y el cubo móvil, por lo que será importante conocer sus medidas. 

   El cubo que usaremos de referencia tendrá centro de gravedad en el origen $(0,0,0)$, cada arista es paralela con un solo eje de coordenadas, y mide 2 unidades, de forma que los vértices son los que se muestran en la imagen (además del $(-1,-1,-1)$ que está por detrás del cubo): 

   <img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/photo_2022-11-19_11-41-54.jpg" alt="photo_2022-11-19_11-41-54" style="zoom:75%;" />

   

   <img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/photo_2022-11-19_12-16-40.jpg" alt="photo_2022-11-19_12-16-40" style="zoom:75%;" />

   

2. **Cuadrados de la superficie**: Estos cuadrados van apareciendo poco a poco a medida que el cubo móvil pisa la superficie sobre la que se dispondrán. Pero ahora mismo no tenemos ninguna herramientas para gestionar la **invisibilidad** temporal de los cuadrados, por lo que tendremos que crear una, eso lo veremos en el [Apartado 3](#3. Adecuación del código para introducir invisibilidad.).

   

3. **Cubo móvil**: Este cubo hace una serie de giros sobre la superficie del cubo de referencia. Podemos diferenciar varias transformaciones que realiza:

   * **Giros**, sobre distintos ejes en diferentes momentos. Para facilitarnos un poco la introducción de giros sobre un eje, cambiaremos un elemento de `tup_mat.h`, eso lo veremos en el [Apartado 4](#4. Facilitación del código para introducir giros sobre un eje.).
   * **Traslación**, realizada casi al final de la animación, cuando el cubo se traslada a un punto que está por encima del cubo general y después, vuelve a bajar.
   * **Escalado**, también al final de la animación; a medida que el cubo se traslada a este punto superior se hace más grande, y mantiene ese tamaño para cuando vuelve a bajar. 





## 3. Adecuación del código para introducir entradas de invisibilidad.

Para permitir que los cuadrados empiecen en invisibilidad y a medida que pase el tiempo se vuelvan visibles, podríamos añadir un booleano en los atributos de la clase `objeto3d` para indicar si un objeto es invisible o no, y cuando lo sea, simplemente no visualizarlo, pero con esta pobre solución perderíamos la versatilidad que nos proporcionan la clase `grafo-escena` y sus nodos, por lo que probablemente sea mejor idea expandir las funcionalidades de esta. 

Para todo esto, nos serviremos de un nuevo tipo de nodo: el **nodo de visibilidad**. A continuación, se muestra los cambios introducidos en el proyecto para implementar este nodo de visibilidad. 

1. El primer cambio que realizamos es introducir en el `enum` de `TipoEntNGE` , en el archivo `grafo-escena.h`, el nuevo tipo de nodo `visibilidad`. 

   ```c++
   // tipo enumerado con los tipos de entradas del nodo del grafo de escena
   
   enum class TipoEntNGE { objeto, transformacion, material, visibilidad, noInicializado };  
   // AÑADIDO: nuevo tipo de dato visibilidad
   ```



2. También tendremos que añadir el puntero a booleano en los atributos de la clase `EntradaNGE`, además de crear un constructor de objetos para esta clase que acepte un puntero a booleano en el argumento.

   ```c++
   // Entrada del nodo del Grafo de Escena
   
   struct EntradaNGE
   {
      TipoEntNGE tipo = TipoEntNGE::noInicializado ;   // objeto, transformacion, material
      union
      {  Objeto3D * objeto = nullptr ;  // ptr. a un objeto (no propietario)
         Matriz4f * matriz ;  // ptr. a matriz 4x4 transf. (propietario)
         Material * material ; // ptr. a material (no propietario)
         bool * visibilidad; // ptr. a booleano  
         // AÑADIDO: booleano de visibilidad
      } ;
      // constructores (uno por tipo)
      EntradaNGE() = delete; //prohibe constructor sin parámetros
      EntradaNGE( Objeto3D * pObjeto ) ;      // (copia solo puntero)
      EntradaNGE( const Matriz4f & pMatriz ); // (crea copia en el heap)
      EntradaNGE( Material * pMaterial ) ;    // (copia solo puntero)
      EntradaNGE( bool * visibilidad ) ; // (copia solo puntero)
      // AÑADIDO: constructor de entrada con el puntero a booleano
      ~EntradaNGE() ;
   } ;
   ```

   La definición de este constructor — en `grafo-escena.cpp` — nuevo sería:

   ```c++
   // Constructor para entrada de tipo "visibilidad"
   
   EntradaNGE::EntradaNGE( bool * vsb )
   {
      assert( vsb != NULL );
      tipo        = TipoEntNGE::visibilidad ;
      visibilidad = vsb;
   }
   // AÑADIDO: definición del constructor nuevo de EntradaNGE
   ```

   

3. Sobrecargar el método `agregar()` de la clase `NodoGrafoEscena`, también en `grafo-escena.h` para que permita pasar un puntero a booleano en el argumento.

   ```c++
   class NodoGrafoEscena : public Objeto3D
   {
      ...
      // construir una entrada y añadirla (al final)
      unsigned agregar( Objeto3D * pObjeto ); // objeto (copia solo puntero)
      unsigned agregar( const Matriz4f & pMatriz ); // matriz (copia objeto)
      unsigned agregar( Material * pMaterial ); // material (copia solo puntero)
      unsigned agregar( bool * visibilidad ); // booleano (copia solo puntero)
      // AÑADIDO: sobrecarga de agregar() para que agregue EntradaNGE construida con puntero a booleano
      ...
   };
   ```

   La definición del método `agregar` sobrecargado nuevo — en `grafo-escena.cpp` — sería:

   ```c++
   // visibilidad (copia solo puntero)
   unsigned NodoGrafoEscena::agregar( bool * visibilidad )
   {
      return agregar( EntradaNGE( visibilidad ) );
   }
   // AÑADIDO: definición del método agregar() para que agregue EntradaNGE construida con puntero a booleano
   ```

   

4. Por último, lo más importante, añadir la forma en la que se evaluarán los nodos para realizar la visualización: 

   ```c++
   // Visualiza usando OpenGL
   
   void NodoGrafoEscena::visualizarGL( ContextoVis & cv )
   {
      // comprobar que hay un cauce en 'cv' 
      assert( cv.cauce != nullptr );
   
      const Tupla4f color_previo = leerFijarColVertsCauce( cv );
   
      bool visibilidad = true;
      // AÑADIDO: booleano para evaluar la visibilidad en cada iteración
   
      cv.cauce->pushMM();
   
      for( unsigned i = 0; i < entradas.size(); ++i)
      {
         switch( entradas[i].tipo )
         {
            case TipoEntNGE::objeto :
               entradas[i].objeto->visualizarGL( cv );
               break;
            case TipoEntNGE::transformacion :
               cv.cauce->compMM( *(entradas[i].matriz) );
               break;
            case TipoEntNGE::visibilidad :
               visibilidad = *(entradas[i].visibilidad);
               break;
            // AÑADIDO: el booleano visibilidad copia el valor del booleano visibilidad de la entrada leída
         }
         if(visibilidad == false) break;
      	  // AÑADIDO: si el booleano visibilidad tiene valor false, se sale del bucle for
      }
   
      // Se recupera el color guardado anteriormente.
      cv.cauce->fijarColor(color_previo);
   
      cv.cauce->popMM();
   }
   ```

   Con esta definición, conseguimos que cuando se lea una entrada con el booleano `visibilidad` en false, no se sigan leyendo las siguientes entradas del nodo actual, y, por tanto, no se visualicen. 

   También introducimos esta definición en `VisualizarGeomGL()`:

   ```c++
   // visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...void 
   
   NodoGrafoEscena::visualizarGeomGL( ContextoVis & cv )
   {
      // comprobar que hay un cauce en 'cv' 
      assert( cv.cauce != nullptr );
   
      cv.cauce->pushMM();
   
      bool visibilidad = true;
   
      // Se guarda el color en uso actual y se cambia por otro.
   
      for( unsigned i = 0; i < entradas.size(); ++i)
      {
         switch( entradas[i].tipo )
         {
            case TipoEntNGE::objeto :
               entradas[i].objeto->visualizarGeomGL( cv );
               break;
            case TipoEntNGE::transformacion :
               cv.cauce->compMM( *(entradas[i].matriz) );
               break;
            case TipoEntNGE::visibilidad :
               visibilidad = *(entradas[i].visibilidad);
               break;
         }
         if(visibilidad == false) break;
      }
   
      // Se recupera el color guardado anteriormente.
   
      cv.cauce->popMM();
   
   }
   ```

Ya con todo esto, podemos implementar los cuadrados de la superficie, de forma que vayan apareciendo poco a poco.





## 4. Facilitación del código para introducir giros sobre una recta paralela a un eje.

El cubo no presenta un solo giro sobre una misma recta, sino que presenta muchos giros distintos sobre distintas rectas:

<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/photo_2022-11-19_13-01-14.jpg" alt="photo_2022-11-19_13-01-14" style="zoom:80%;" />



Para cada giro sobre una recta definida por un vector $\vec{v}$ y un punto contenido en ella $\dot{p}$ hay que:

- Trasladar el cubo para que limite con la recta que pasa por el origen $O(0,0,0)$ y tiene vector de dirección $\vec{v}$. Para esto, hay que trasladar la figura del punto $\dot{p}$ al punto origen $\dot{O}$. Esto requiere de una matriz de traslación ( `MAT_Traslacion(-punto)` ).
- Hacer rotar el cubo sore la recta que pasa por el origen $O(0,0,0)$ y tiene vector de dirección $\vec{v}$. Esto requiere de una matriz de rotación ( `MAT_Rotacion(vector)` ).
- Trasladas de vuelta el cubo a su sitio, del punto origen $\dot{O}$ al punto $\dot{p}$. Y de nuevo haría falta la matriz de traslación ( `MAT_Traslacion(punto)` ).



Para facilitar la lectura y escritura de este código he sobrecargado el método `MAT_Rotacion` de `tup_mat.h`, de forma que acepte el punto $\dot{p}$ y el vector $\vec{v}$ correspondientes a la recta sobre la que quiere rotar y realice estos tres pasos que hemos contado en uno solo:

```c++
// AÑADIDO : Matriz de rotación sobre una recta paralela definida por un punto y el vector que pasa por él.

inline Matriz4f MAT_Rotacion( const float ang_gra, const Tupla3f & punto, const Tupla3f & vect)
{
   return (MAT_Traslacion(punto) * MAT_Rotacion(ang_gra, vect) * MAT_Traslacion(-punto));
}
```





## 5. Grafo escena de mi diseño.

### 5.1. Declaración de la clase GameCube.

La declaración de la clase GameCube es la siguiente: 

```c++
class GameCube : public NodoGrafoEscena
{
   protected:
      const unsigned NUM_MATRICES_ROT = 17;
      Matriz4f ** pm_rot_alpha;

      const unsigned NUM_MATRICES_TRAS = 2; 
      Matriz4f ** pm_tras_dist;

      Matriz4f * pm_escalado;

      const unsigned NUM_CAS_INVISIBLES = 14;
      bool ** visibilidad;
         
   private:
      NodoGrafoEscena * cuadrados();
      NodoGrafoEscena * cubo();

      const float k = 2.0/3.0;
      const float m = 1.0/3.0;

      const float T = 0.25;    // Tiempo de periodo, recomendado 0,25 o 0,5
   
   public:
      GameCube();
      void fijarRot     (const unsigned num_T,    const float fase);
      void fijarTrasl   (const unsigned num_T,    const float fase);
      void fijarEsc     (const unsigned num_T,    const float fase);
      void fijarVisib   (const unsigned num_T,    const bool visib);
      unsigned leerNumParametros() const;
      void actualizarEstadoParametro( const unsigned iParam, const float t_sec );

} ;
```

En los próximos apartados explicaremos mejor para qué sirve cada atributo y método.





### 5.2. Sincronización de las animaciones.

La sincronización de las animaciones depende de la constante flotante `T`, que es el periodo. Para el valor del periodo utilizaremos de referencia el tiempo en segundos que tardará el cubo móvil en hacer un giro de 90º sobre una de sus aristas. 

En el método `actualizarEstadoParametro()` tendremos las variables:

- `T_actual`: Esta indicará el número de periodo actual en la animación, es decir, cuántas veces ha pasado el tiempo de periodo. Se calcula como el cociente entre el tiempo actual transcurrido y el tiempo de periodo (`t_sec / T`).
-  `fase`: Esta indicará el tiempo transcurrido en el periodo actual. Se calcula como el resto entre el tiempo actual transcurrido y el tiempo de periodo (`t_sec % T`).

El código del método queda así:

```c++
void GameCube::actualizarEstadoParametro ( const unsigned iParam, const float t_sec )
{
   assert(iParam < leerNumParametros()); //Al ser iParam unsigned siempre es >= 0, por eso no se comprueba

   unsigned T_actual = trunc(t_sec/T); // T_actual es el número de periodo en la animación
   float fase = fmod(t_sec, T);  // fase es el tiempo actual transcurrido dentro del periodo

   if(T_actual>0) //Terminamos transformaciones de fases anteriores, por si alguna no la he hecho
   {   
      fijarGiro   ( T_actual-1, T );
      fijarTrasl  ( T_actual-1, T );
      fijarEsc    ( T_actual-1, T );
   }
   
   //Realizamos transformaciones del actual periodo.
   fijarGiro   ( T_actual, fase );
   fijarTrasl  ( T_actual, fase );
   fijarEsc    ( T_actual, fase );
    
   // Visibilizamos elementos que ahora sí se deben ver
   fijarVisib  ( T_actual, true );
}
```

De esta forma, el `T_actual` permitirá a los métodos saber qué matriz (o matrices) deberán modificar, y `fase/T` les permitirá conocer el porcentaje de completación de la transformación en ese periodo. Por ello mismo, le pasa a los métodos modificadores de matrices el valor de `fase`. Veremos estos más a fondo en el [Apartado 5.5](#5.5. Resto de métodos de GameCube.).

Por último, en `leerNumParametros()` solo se devuelve `1`, al ser realmente el único parámetro el del periodo (`T`), a partir del cual se calcula todo.

```c++
unsigned GameCube::leerNumParametros() const
{
   return 1;
}
```





### 5.3. Creación de los cuadrados de la superficie.

Para la creación de los cuadrados en la superficie lo primero que deberemos tener claro es dónde deben ir cada uno de los cuadrados que dispondremos y hacia dónde irán orientados.

<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/photo_2022-11-19_20-27-53.jpg" alt="photo_2022-11-19_20-27-53" style="zoom:40%;" />

Lo primero que hacemos es crear un cuadrado para cada orientación:

- Los cuadrados de la cara superior (en rojo en la imagen superior) del cubo tienen superficie paralela a los ejes X y Z, haciendo un total de 4 cuadrados.
- Los cuadrados de la cara izquierda inferior (en verde en la imagen superior) del cubo tienen superficie paralela a los ejes X e Y, haciendo un total de 5 cuadrados.
- Los cuadrados de la cara derecha inferior (en azul en la imagen superior) del cubo tienen superficie paralela a los ejes Y y Z, haciendo un total de 6 cuadrados.



A parte de esto, para hacer que los cuadrados empiecen siendo invisibles y vayan haciéndose visibles poco a poco, implementaremos un vector de booleanos, de 15 posiciones, ya que hay 15 casillas, y los inicialezaremos todos con el valor `false`. 

```c++
class GameCube : public NodoGrafoEscena
{
   protected:
      ...
      const unsigned NUM_CAS_INVISIBLES = 15;
      bool ** visibilidad;
```

Deberá existir una entrada de visibilidad antes de que cada cuadrado.



De esta forma, el grafo PHIGS sería el siguiente:

![photo_2022-11-19_21-54-22](/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/photo_2022-11-19_21-54-22.jpg)



El método `cuadrados()` de la clase `GameCube` permite ensamblar esta composición:

```c++
NodoGrafoEscena * GameCube::cuadrados()
{
   visibilidad = new bool *[NUM_CAS_INVISIBLES];
   for(unsigned i = 0; i < NUM_CAS_INVISIBLES; ++i) visibilidad[i] = new bool (false);

   NodoGrafoEscena * cuadradoXZ = new NodoGrafoEscena();
      cuadradoXZ->agregar( MAT_Escalado(m, m, m) );
      cuadradoXZ->agregar( new CuadradoProfundidad() );

   NodoGrafoEscena * cuadradoXY = new NodoGrafoEscena();
      cuadradoXY->agregar( MAT_Rotacion(90.0, {1.0, 0.0, 0.0}) );
      cuadradoXY->agregar(cuadradoXZ);

   NodoGrafoEscena * cuadradoYZ = new NodoGrafoEscena();
      cuadradoYZ->agregar( MAT_Rotacion(90.0, {0.0, 0.0, 1.0}) );
      cuadradoYZ->agregar(cuadradoXZ);

   NodoGrafoEscena * res = new NodoGrafoEscena();
      res->agregar( visibilidad[0] );     res->agregar( MAT_Traslacion({0, k+m, -k}) );   res->agregar( cuadradoXZ ); 
      res->agregar( visibilidad[1] );     res->agregar( MAT_Traslacion({-k, 0, 0}) );     res->agregar( cuadradoXZ ); 
      res->agregar( visibilidad[2] );     res->agregar( MAT_Traslacion({0, 0, k}) );      res->agregar( cuadradoXZ );
      res->agregar( visibilidad[3] );     res->agregar( MAT_Traslacion({0, 0, k}) );      res->agregar( cuadradoXZ );

      res->agregar( visibilidad[4] );     res->agregar( MAT_Traslacion({0, -m, m}) );     res->agregar( scuadradoXY );
      res->agregar( visibilidad[5] );     res->agregar( MAT_Traslacion({0, -k, 0}) );     res->agregar( cuadradoXY );
      res->agregar( visibilidad[6] );     res->agregar( MAT_Traslacion({0, -k, 0}) );     res->agregar( cuadradoXY );
      res->agregar( visibilidad[7] );     res->agregar( MAT_Traslacion({k, 0, 0}) );      res->agregar( cuadradoXY );
      res->agregar( visibilidad[8] );     res->agregar( MAT_Traslacion({k, 0, 0}) );      res->agregar( cuadradoXY );

      res->agregar( visibilidad[9] );     res->agregar( MAT_Traslacion({m, 0, -m}) );     res->agregar( cuadradoYZ );
      res->agregar( visibilidad[10] );    res->agregar( MAT_Traslacion({0, 0, -k}) );     res->agregar( cuadradoYZ );
      res->agregar( visibilidad[11] );    res->agregar( MAT_Traslacion({0, 0, -k}) );     res->agregar( cuadradoYZ );
      res->agregar( visibilidad[12] );    res->agregar( MAT_Traslacion({0, k, 0}) );      res->agregar( cuadradoYZ );
      res->agregar( visibilidad[13] );    res->agregar( MAT_Traslacion({0, k, 0}) );      res->agregar( cuadradoYZ );
      res->agregar( visibilidad[14] );    res->agregar( MAT_Traslacion({0, 0, k}) );      res->agregar( cuadradoYZ );

   return res;
}
```



Por último, durante la ejecución de la animación lo único que cambia es la visibilidad de los cuadrados, el método que realiza esto es `fijarVisib`, que en cada periodo visibiliza un cuadrado, salvo en los periodos 4 y 10 , ya que en estos el cubo móvil debe girar 180º, en vez de 90º:

```c++
void GameCube::fijarVisib(const unsigned num_T, const bool visib)
{
   if       (num_T <= 3)   *visibilidad[num_T]   = visib;
   else if  (num_T <= 9)   *visibilidad[num_T-1] = visib;
   else if  (num_T <= 16)  *visibilidad[num_T-2] = visib;
}
```





### 5.4. Creación del cubo móvil.

Para la creación del cubo y su animación, primero deberemos establecerlo en su posición inicial: 

<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/photo_2022-11-19_22-47-22.jpg" alt="photo_2022-11-19_22-47-22" style="zoom:33%;" />

Las transformaciones que sufre el cubo, como ya describimos antes, son de rotación, traslación y escalado:

- Hay 17 rotaciones diferentes (realmente 15, pero por facilitación de implementación serán 17) del cubo en la animación, por lo que harán falta 17 matrices de rotación. 
- Hay 3 traslaciones diferentes, de las cuales una es es la traslación inicial y las otras dos son las que forman parte de la animación, por lo que harán falta 2 matrices de traslación.
- Se usa una sola transformación de escalado en la animación, por lo que solo hace falta un matriz de escalado.



```c++
class GameCube : public NodoGrafoEscena
{
   protected:
	  const unsigned NUM_MATRICES_ROT = 17;
      Matriz4f ** pm_rot_alpha;

      const unsigned NUM_MATRICES_TRAS = 2; 
      Matriz4f ** pm_tras_dist;

      Matriz4f * pm_escalado;
      ...
```









De esta forma, el diagrama de PHIGS quedaría de la siguiente forma:

<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/photo_2022-11-19_22-26-55.jpg" alt="photo_2022-11-19_22-26-55" style="zoom:70%;" />

NOTA: La traslación $\{0, 2k, -k\}$ es la traslación del cubo a su posición de inicio.



El método `cubo_movil()` de la clase `GameCube` permite ensamblar esta composición:

```c++
NodoGrafoEscena * GameCube::cubo_movil()
{
   pm_rot_alpha = new Matriz4f *[NUM_MATRICES_ROT];
   pm_tras_dist = new Matriz4f *[NUM_MATRICES_TRAS];

   NodoGrafoEscena * cubo = new NodoGrafoEscena();

   unsigned ind_tras[NUM_MATRICES_TRAS]; 
   for (unsigned i = NUM_MATRICES_TRAS-1; i >= 0; --i) ind_tras[i] = cubo->agregar( MAT_Traslacion({0.0, 0.0, 0.0}) );

   unsigned ind_rot[NUM_MATRICES_ROT]; 
   for (unsigned i = NUM_MATRICES_ROT-1; i >= 0; --i) ind_rot[i] = cubo->agregar( MAT_Rotacion(0.0, {1.0, 0.0, 0.0}) );   

   cubo->agregar( MAT_Traslacion({0, 2*k, -k}) );
   unsigned ind_esc = cubo->agregar( MAT_Escalado(1.0, 1.0, 1.0) );
   cubo->agregar( MAT_Escalado(m, m, m) );
   cubo->agregar( new Cubo() );

   for (unsigned i = 0; i < NUM_MATRICES_ROT; ++i) pm_rot_alpha[i] = cubo->leerPtrMatriz(ind_rot[i]);
   for (unsigned i = 0; i < NUM_MATRICES_TRAS; ++i) pm_tras_dist[i] = cubo->leerPtrMatriz(ind_tras[i]);
   pm_escalado = cubo->leerPtrMatriz(ind_esc);

   return cubo;
}
```



Durante la ejecución de la animación, deben cambiar las matrices de rotación, las de traslación y la de escalado. Estos se hará mediante los métodos `fijarRot()`, `fijarTrasl()` y `fijarEsc()`, que estarán diseñados como indicamos antes en el [Apartado 5.2.](#5.2. Sincronización de las animaciones.), de forma que usan el número de periodo para identificar la matriz a modificar y la fase para calcular el porcentaje de completación de cada transformación:

```c++
void GameCube::fijarRot (const unsigned num_T, const float fase)
{
   const float n_alpha = 90.0 * fase / T;

   switch(num_T)
   {
      case 0:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {-m, k+m, -k},   {0.0, 0.0, 1.0} );     break;
      case 1:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {-k, k+m, -m},   {1.0, 0.0, 0.0} );     break;
      case 2:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {-k, k+m, +m},   {1.0, 0.0, 0.0} );     break;
      case 3:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {-k, k+m, k+m},  {1.0, 0.0, 0.0} );     break;
      case 4:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {-k, k+m, k+m},  {1.0, 0.0, 0.0} );     break;
      case 5:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {-k, m, k+m},    {1.0, 0.0, 0.0} );     break;
      case 6:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {-k, -m, k+m},   {1.0, 0.0, 0.0} );     break;
      case 7:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {-m, -k, k+m},   {0.0, 1.0, 0.0} );     break;
      case 8:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {m, -k, k+m},    {0.0, 1.0, 0.0} );     break;
      case 9:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {k+m, -k, k+m},  {0.0, 1.0, 0.0} );     break;
      case 10:    *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {k+m, -k, k+m},  {0.0, 1.0, 0.0} );     break;
      case 11:    *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {k+m, -k, m},    {0.0, 1.0, 0.0} );     break;
      case 12:    *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {k+m, -k, -m},   {0.0, 1.0, 0.0} );     break;
      case 13:    *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {k+m, -m, -k},   {0.0, 0.0, 1.0} );     break;
      case 14:    *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {k+m, m, -k},    {0.0, 0.0, 1.0} );     break;
      case 15:    *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {k+m, k, -m},    {0.0, -1.0, 0.0});     break;
      case 16:    *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {k+m, k, m},     {0.0, -1.0, 0.0});     break;
      default:    break;
   }
}


void GameCube::fijarTrasl(const unsigned num_T, const float fase)
{
   Tupla3f v;

   switch(num_T)
   {
      case 17:  
         v = (Tupla3f{-1.0*k, 2.0*k, 0}) * fase / T;
         *pm_tras_dist[0] = MAT_Traslacion(v);
         break;

      case 18:
         v = (Tupla3f{-0.5*m, -2.0*k-0.5*m, -0.5*m}) * fase / T;  
        *pm_tras_dist[1] = MAT_Traslacion(v);
         break;
      
      default: break;
   }
}


void GameCube::fijarEsc(const unsigned num_T, const float fase)
{
   if(num_T == 17){
      float factor = 1.0 + (0.5 * fase / T);
      *pm_escalado = MAT_Escalado(factor, factor, factor);
   }
}
```





### 5.5 Constructor de GameCube.

Finalmente, el ensamblaje de todo esto quedará en el siguiente grafo.

<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/photo_2022-11-19_23-02-18.jpg" alt="photo_2022-11-19_23-02-18" style="zoom:40%;" />

donde `cuadrados` son los cuadrados de la superficie del [Apartado 5.3.](#5.3. Creación de los cuadrados de la superficie.), y `cubos_movil` es el cubo móvil del [Apartado 5.4.](#5.4. Creación del cubo móvil.s).

```c++
GameCube::GameCube()
{
   NodoGrafoEscena * cuadrados = this->cuadrados();
   NodoGrafoEscena * cubo_movil = this->cubo_movil();

   this->ponerColor({0.167451, 0.1444706, 0.27529});
   this->agregar( MAT_Escalado(0.5, 0.5, 0.5) );
   this->agregar( cuadrados );
   this->agregar( cubo_movil );
}
```

