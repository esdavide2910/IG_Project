# INFORMÁTICA GRÁFICA 2022/2023. 

# PRÁCTICA 4. MATERIALES, FUENTES DE LUZ Y TEXTURAS.

Hecha por David González Durán (Grupo 2 de prácticas, doble grado de Ingeniería Informática y ADE)



## 1. Qué se nos pide.

En esta práctica se pide que añadamos el nuevo tipo de entrada `Material` a nuestro proyecto. Debemos incluir, al menos:

- Un objeto con texturas generadas a partir de su tabla de texturas.

- Un objeto con texturas generadas automáticamente.

- Un objeto con color plano. 

  

Además, se deberán cubrir materiales de distintos tipos:

- Materiales difusos.
- Materiales pseudo-especulares.

Donde cada material se asociará con un identificador. 



Para hacer todo esto ampliaré mi diseño de la práctica 3, creando dos versiones del mismo:

- Una versión clásica, de la intro de la GameCube, en la que todos los materiales serán metálicos y, por tanto, primará la componente pseudo-especular, y de un mismo color plano.
- Una versión cubo de Rubik, en la que el cubo móvil es un cubo de Rubik, y las plataformas cuadradas tienen una textura de madera, teniendo ambos materiales mayor componente difusa que pseudo-especular.



Cabe señalar que para esta práctica he cambiado el objeto `Cubo` por `Cubo24`, para que la iluminación sea correcta, y he generado las plataformas cuadradas a partir de este `Cubo24`, escalándolo por 0.1 en el eje Y. Ahora más tarde, explicaré el nuevo objeto `CuboRubik` que he creado, cuya única diferencia con el objeto `Cubo24` radicará en las coordenadas de textura de los vértices.





## 2. Fuentes de iluminación.

Para este modelo, he cambiado las fuentes de luz para que fueran 2 fuentes de luz blanca (una con mayor y otra con menor intensidad).

```c++
Col3Fuentes::Col3Fuentes()
{
   insertar( new FuenteLuz( 20.0, 20.0,  Tupla3f {  0.7, 0.7, 0.7} ) ); 
   insertar( new FuenteLuz( 230.0, 40.0,  Tupla3f {  1.0, 1.0, 1.0} ) ); 
}
```



En las siguientes imágenes podemos ver con esferas en qué direcciones están las fuentes de luz. Por supuesto, este objeto solo lo he añadido a modo ejemplificativo, y no forma parte del modelo.

<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/iluminación1.png" alt="iluminación1" style="zoom:80%;" />



<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/iluminación2.png" alt="iluminación2" style="zoom:67%;" />





## 3. GameCube vers. clásica.

El modelo "GameCube versión clásica" tiene el siguiente diagrama PHIGS:



<img src="/home/esdavide/Downloads/Telegram Desktop/photo_2022-12-31_16-04-27.jpg" alt="photo_2022-12-31_16-04-27" style="zoom:50%;" />

donde `MaterMetálico` es un material sin textura, con coeficientes:

-  Ambiental: $k_a =0.5$ 
-  Difuso: $k_b=0.5$
-  Pseudo-especular: $k_s=0.7$

y exponente de brillo $e=100$.



El resultado es el siguiente:

<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/modeloGCversClassic.png" alt="modeloGCversClassic" style="zoom:110%;" />





<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/modeloGCversClassic2.png" alt="modeloGCversClassic2" style="zoom:80%;" />





## 4. GameCube vers. Rubik.

El modelo "GameCube versión Rubik" tiene el siguiente diagrama PHIGS:



<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/photo_2022-12-31_16-03-41.jpg" alt="photo_2022-12-31_16-03-41" style="zoom:50%;" />

donde `MaterMadera` es un material con textura de madera, con coeficientes:

-  Ambiental: $k_a =0.4$ 
- Difuso: $k_b=0.7$
- Pseudo-especular: $k_s=0.05$

y exponente de brillo $e=100$. Las coordenadas de textura de `cuadrados`, que será el único nodo afectado por el material, se generarán automáticamente en modo coordenadas de objeto; para esto hemos usado los vectores de coeficientes $s=(0.5, 0.5, 0.0, 0.0)$ y $t=(0.5, 0.5, 0.0, 0.0)$. La imagen utilizada en la textura es "textura-lamas-madera.jpg":

<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/textura-lamas-madera.jpg" alt="textura-lamas-madera" style="zoom:50%;" />

Ppr otro lado,`MaterRubik` es un material con textura de un cubo de Rubik (la imagen es "rubik.jpg") con coeficientes:

* Ambiental: $k_a=0.4$
* Difuso: $k_b=0.85$
* Pseudo-especular: $k_s=0.3$

y exponente de brillo $e=100$. En este caso sí se utilizará la tabla de coordenadas de textura del cubo para aplicar la textura. Para ello, he creado un nuevo cubo `MallaInd`, denominado `CuboRubik`, con las coordenadas de textura que aceptan el siguiente tipo de imagen, de forma que se cubren todas las caras del cubo:

<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/rubik.jpg" alt="rubik" style="zoom:90%;" />



```c++
CuboRubik::CuboRubik()
: MallaInd("CuboRubik")
{
   vertices =
      {  
         // CARA L
         { -1.0, -1.0, -1.0 }, { -1.0, -1.0, +1.0 }, { -1.0, +1.0, -1.0 },{ -1.0, +1.0, +1.0 },
         // CARA B
         { +1.0, -1.0, -1.0 }, { -1.0, -1.0, -1.0 }, { +1.0, +1.0, -1.0 }, { -1.0, +1.0, -1.0 }, 
         // CARA D
         { +1.0, -1.0, +1.0 }, { -1.0, -1.0, +1.0 }, { +1.0, -1.0, -1.0 }, { -1.0, -1.0, -1.0 }, 
         // CARA R
         { +1.0, -1.0, -1.0 }, { +1.0, -1.0, +1.0 }, { +1.0, +1.0, -1.0 }, { +1.0, +1.0, +1.0 }, 
         // CARA F
         { +1.0, -1.0, +1.0 }, { -1.0, -1.0, +1.0 }, { +1.0, +1.0, +1.0 }, { -1.0, +1.0, +1.0 }, 
         // CARA U
         { +1.0, +1.0, +1.0 }, { -1.0, +1.0, +1.0 }, { +1.0, +1.0, -1.0 }, { -1.0, +1.0, -1.0 }, 
      } ;
   
   for( int i=0; i<6 ; ++i ) {
      triangulos.push_back( {i*4+2, i*4+1, i*4  } );
      triangulos.push_back( {i*4+1, i*4+2, i*4+3} );
   }

   for(int i=0; i<4; ++i) nor_ver.push_back( {-1.0, 0.0, 0.0} );
   for(int i=0; i<4; ++i) nor_ver.push_back( {0.0, 0.0, -1.0} );
   for(int i=0; i<4; ++i) nor_ver.push_back( {0.0, -1.0, 0.0} );

   for(int i=0; i<4; ++i) nor_ver.push_back( {1.0, 0.0, 0.0} );
   for(int i=0; i<4; ++i) nor_ver.push_back( {0.0, 0.0, 1.0} );
   for(int i=0; i<4; ++i) nor_ver.push_back( {0.0, 1.0, 0.0} );

   // Cara L
   cc_tt_ver.push_back({0.0, 2.0/3.0});         cc_tt_ver.push_back({0.25, 2.0/3.0});
   cc_tt_ver.push_back({0.0, 1.0/3.0});         cc_tt_ver.push_back({0.25, 1.0/3.0});             

   // Cara B   
   cc_tt_ver.push_back({0.75, 2.0/3.0});     cc_tt_ver.push_back({1.0, 2.0/3.0}); 
   cc_tt_ver.push_back({0.75, 1.0/3.0});     cc_tt_ver.push_back({1.0, 1.0/3.0});   

   // Cara D   
   cc_tt_ver.push_back({0.5, 2.0/3.0});      cc_tt_ver.push_back({0.25, 2.0/3.0});     
   cc_tt_ver.push_back({0.5, 1.0});          cc_tt_ver.push_back({0.25, 1.0});  

   // Cara R    
   cc_tt_ver.push_back({0.75, 2.0/3.0});     cc_tt_ver.push_back({0.5, 2.0/3.0});  
   cc_tt_ver.push_back({0.75, 1.0/3.0});     cc_tt_ver.push_back({0.5, 1.0/3.0});  
   
   // Cara F    
   cc_tt_ver.push_back({0.5, 2.0/3.0});      cc_tt_ver.push_back({0.25, 2.0/3.0});  
   cc_tt_ver.push_back({0.5, 1.0/3.0});      cc_tt_ver.push_back({0.25, 1.0/3.0}); 
   
   // Cara U
   cc_tt_ver.push_back({0.5, 1.0/3.0});      cc_tt_ver.push_back({0.25, 1.0/3.0}); 
   cc_tt_ver.push_back({0.5, 0.0});          cc_tt_ver.push_back({0.25, 0.0});   
}

```



El resultado es el siguiente: 

![modeloGCversRubik](/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/modeloGCversRubik.png)





## 5. Diagrama de PHIGS.

<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/PHIGS_GameCube.jpg" alt="PHIGS_GameCube" style="zoom:50%;" />



<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/PHIGS_cuadrados.jpg" alt="PHIGS_cuadrados" style="zoom:100%;" />



<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/PHIGS_cubo_movil.jpg" alt="PHIGS_cubo_movil" style="zoom:70%;" />
