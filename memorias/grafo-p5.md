# INFORMÁTICA GRÁFICA 2022/2023. 

# PRÁCTICA 5. INTERACCIÓN: CÁMARAS Y SELECCIÓN.

Hecha por David González Durán (Grupo 2 de prácticas, doble grado de Ingeniería Informática y ADE)



## 1. Qué se nos pide.

En esta práctica se pide el diagrama PHIGS completo de nuestro modelo, incluyendo transformaciones, materiales e identificadores en cada nodo. 

Además, se nos pide una lista con información de todos los identificadores de selección.



## 2. Diagrama PHIGS.

El diagrama PHIGS de mi proyecto es el siguiente:



<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/PHIGS_GameCube.jpg" alt="PHIGS_GameCube" style="zoom:57%;" />



<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/PHIGS_cuadrados.jpg" alt="PHIGS_cuadrados" style="zoom:100%;" />



<img src="/home/esdavide/Desktop/Coding/IG/IG-practica1/archivos-alumno/images/PHIGS_cubo_movil.jpg" alt="PHIGS_cubo_movil" style="zoom:70%;" />

Toda la implementación del modelo se recoge en los archivos `gamecube.h` y `gamecube.cpp`. 





## 3. Identificadores en el modelo.

Solo he empleado dos identificadores en mi modelo:

- Identificador 1, para el nodo `cuadrados`, con nombre asignado "Plataformas cuadradas", que tiene el centro en el origen (0,0,0) (Nombre e identificador se asignan en las líneas 136 y 137 del .cpp)
- Identificador 2, para el nodo `cubo_movil`, con nombre asignado "Cubo móvil", que tiene centro distinto dependiendo del momento en que pinches el cubo. (Nombre e identificador se asignan en las líneas 78 y 79 del .cpp)
