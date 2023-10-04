// // Nombre: David, Apellidos: González Durán, Titulación: GIADE
// // Email: davgondur2910@correo.ugr.es, DNI: 32071015E
// // *********************************************************************
// // **
// // ** Informática Gráfica, curso 2019-20
// // ** Implementación de la clase 'MallaRevol'
// // **
// // *********************************************************************

// #include "ig-aux.h"
// #include "lector-ply.h"
// #include "malla-prof.h"

// using namespace std ;


// void MallaPerfiles::inicializar
// (
//     const vector<vector<Tupla3f>> & perfiles  // tabla de perfiles
// )
// {
//     int n = perfiles.size();

//     int m = perfiles[0].size();

//     for(int i=1; i<n; ++i) {
//         assert(perfiles[i].size() == m);
//     }


//     float theta;

//     for(int i = 0; i < n; i++){

//         theta = (2.0 * M_PI * (float)i) / (n - 2);

//         for(int j = 0; j < m; j++){

//             vertices.push_back( perfiles.at(i).at(j) );

//         }
//     }

//     for(int i = 0; i < (n-1); i++)
//     {
//         for(int j = 0; j< (m-1); j++)
//         {
//             unsigned int k = i * m + j;
//             triangulos.push_back( {k, k+m, k+m+1} );
//             triangulos.push_back( {k, k+m+1, k+1} );
//         }
//     }
// }


// void calcularPerfiles(
//     const vector<vector<Tupla3f>> & perfiles,
//     const vector<Tupla3f> (*func)(int x, int z),  // tabla de perfiles
//     const unsigned              num_perfiles   // número de perfiles
// )
// {
//     int n = num_perfiles; // Número de perfiles

//     int m = num_perfiles; // Número de evaluaciones (en vertical) en cada perfil

//     for(int i = 0; i < n; ++i)
//     {
//         float p = i/(float)n;

//         for(int j = 0; j < m; ++j)
//         {
//             float q = j/(float)m;

//             perfiles[i][j].push_back()
//         }
//     }
// };