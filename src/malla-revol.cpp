// Nombre: David, Apellidos: González Durán, Titulación: GIADE
// Email: davgondur2910@correo.ugr.es, DNI: 32071015E
// *********************************************************************
// **
// ** Informática Gráfica, curso 2019-20
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

#include "ig-aux.h"
#include "lector-ply.h"
#include "malla-revol.h"

using namespace std ;

// *****************************************************************************




// Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
// a partir de un perfil y el número de copias que queremos de dicho perfil.
void MallaRevol::inicializar
(
   const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
   const unsigned               num_copias  // número de copias del perfil
)
{
   // COMPLETAR: Práctica 2: completar: creación de la malla....

   int n = num_copias+1;
   int m = perfil.size();

   vector<Tupla3f> norm = calcularNormales(perfil, num_copias);
   vector<float> dist_t = calcularDistTextura(perfil, num_copias);

   float theta;

   for(int i = 0; i < n; i++){

      theta = (2.0 * M_PI * (float)i) / (n-1);

      for(int j = 0; j < m; j++){

         vertices.push_back
         ( 
            {  cosf(theta) * perfil[j](0) + sinf(theta) * perfil[j](2),       // Componente X
               perfil[j](1),                                                  // Componente Y
               -sinf(theta) * perfil[j](0) + cosf(theta) * perfil[j](2)    }  // Componente Z
         );

         nor_ver.push_back
         (
            {  cosf(theta) * norm[j](0) + sinf(theta) * norm[j](2),     // Componente X
               norm[j](1),                                              // Componente Y
               -sinf(theta) * norm[j](0) + cosf(theta) * norm[j](2)   } // Componente Z
         );

         cc_tt_ver.push_back
         ( 
            {  (float)i/ ((float)(n-2)) ,      // Componente S       // DUDA: Esto lo he dividido entre n-2 en vez de n-1 como indicaba el guión
               1.0 - dist_t[j]            }    // Componente T
         );

      }
   }

   for(int i = 0; i < (n-1); i++)
   {
      for(int j = 0; j < (m-1); j++)
      {
         unsigned int k = i * m + j;
         triangulos.push_back( {k, k+m, k+m+1} );
         triangulos.push_back( {k, k+m+1, k+1} );
      }
   }

}


// -----------------------------------------------------------------------------

vector<Tupla3f> MallaRevol::calcularNormales
(
   const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
   const unsigned               num_copias  // número de copias del perfil
)
{
   int num_vertices = perfil.size();
   int num_aristas = num_vertices-1;

   // ----------- CÁLCULO DE LAS NORMALES EN LAS ARISTAS ------------------------------

   vector<Tupla3f> norm_aristas;

   for(int i = 0; i < num_aristas ; ++i){
      if((perfil[i+1]-perfil[i]).lengthSq() <= std::numeric_limits<float>::min() ) { // Duda: ¿Es correcto?
         norm_aristas.push_back( {0.0, 0.0, 0.0} );
      }
      else {
         Tupla3f norm_arista = (perfil[i+1] - perfil[i]).normalized();
         Tupla3f norm_arista_rotada = { norm_arista(1), -norm_arista(0), norm_arista(2)};
         norm_aristas.push_back( norm_arista_rotada );
      }
   }

   // ---------------------------------------------------------------------------------
   // ----------- ASIGNACIÓN DE LAS NORMALES EN LOS VÉRTICES --------------------------

   vector<Tupla3f> norm_vertices;

   norm_vertices.push_back( norm_aristas[0] );
   for(int i = 1; i < num_aristas; ++i) 
      norm_vertices.push_back( (norm_aristas[i-1]+norm_aristas[i]).normalized() );
   norm_vertices.push_back( norm_aristas[num_aristas-1] );

   // ---------------------------------------------------------------------------------

   return norm_vertices;
}

// -----------------------------------------------------------------------------

vector<float> MallaRevol::calcularDistTextura
(
   const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
   const unsigned num_copias                // número de copias del perfil
)
{
   int num_vertices = perfil.size();
   int num_aristas = num_vertices-1;

   // ----------- CÁLCULO DE LAS DISTANCIAS DE LAS ARISTAS ----------------------------

   float distancia[num_aristas];

   for(int i = 0; i < num_aristas; ++i )
   {
      distancia[i] = sqrtf( powf(perfil[i+1](0)-perfil[i](0), 2) +
                            powf(perfil[i+1](1)-perfil[i](1), 2) + 
                            powf(perfil[i+1](2)-perfil[i](2), 2) );   
   }

   // ---------------------------------------------------------------------------------
   // ----------- CÁLCULO DE LAS DISTANCIAS ESTANDARIZADAS ----------------------------

   vector<float> t;

   t.push_back(0.0f);

   float sumatorio1 = 0.0f;   
   float sumatorio2 = 0.0f;
   for(int j = 0; j <= (num_vertices-2); ++j) sumatorio2 += distancia[j];

   for(int i = 1; i < num_vertices; ++i) 
   {
      sumatorio1 += distancia[i-1];
      t.push_back(sumatorio1/sumatorio2);
   }

   // ---------------------------------------------------------------------------------

   return t;
}



// -----------------------------------------------------------------------------
// constructor, a partir de un archivo PLY

MallaRevolPLY::MallaRevolPLY
(
   const std::string & nombre_arch,
   const unsigned      nperfiles
)
{
   ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));
   // COMPLETAR: práctica 2: crear la malla de revolución
   // Leer los vértice del perfil desde un PLY, después llamar a 'inicializar'
   // ...........................
   vector<Tupla3f> perfil;
   LeerVerticesPLY(nombre_arch, perfil);
   inicializar(perfil, nperfiles);
}


// ****************************************************************************
// Clase 'Cilindro'

Cilindro::Cilindro(const int num_verts_per, const unsigned nperfiles)
{
   vector<Tupla3f> perfil =
      { { 0.0, -1.0, 0.0 },
        { 1.0, -1.0, 0.0 },
        { 1.0, 1.0, 0.0 },
        { 0.0, 1.0, 0.0 },
      };

   inicializar(perfil, nperfiles);
}

// ****************************************************************************
// Clase 'Cono'

Cono::Cono(const int num_verts_per, const unsigned nperfiles)
{
   vector<Tupla3f> perfil =
      { { 0.0, 0.0, 0.0 },
        { 1.0, 0.0, 0.0 },
        { 0.0, 1.0, 0.0 },
      };

   inicializar(perfil, nperfiles);
}

// ****************************************************************************
// Clase 'Esfera'

Esfera::Esfera(const int num_verts_per, const unsigned nperfiles)
{
   vector<Tupla3f> perfil;

   double step = (M_PI  / num_verts_per);
   double x, y, current = 0.0;
   
   for(int i = 0; i <= num_verts_per; i++)
   {
      x = sin(current) * 1.0; //1.0 es el radio
      y = cos(current) * 1.0; //1.0 es el radio

      perfil.push_back( Tupla3f( x, y, 0.0 ) );

      current -= step; 
   }
   
   inicializar(perfil, nperfiles);
}

// ****************************************************************************
// Clase 'MallaBarrido'

void MallaBarrido::inicializar
(
   const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
   const unsigned               num_copias  // número de copias del perfil
)
{
   int n = num_copias;
   int m = perfil.size();

   for(int i = 0; i < n; ++i){
      for(int j = 0; j < m; ++j){
         vertices.push_back( { perfil[j](0), perfil[j](1)+i/5.0, perfil[j](2)  } );
      }
   }

   for(int i = 0; i < (n-1); ++i){

      for(int j = 0; j< (m-1); ++j){

         unsigned int k = i * m + j;
         triangulos.push_back( Tupla3u(k, k+m, k+m+1) );
         triangulos.push_back( Tupla3u(k, k+m+1, k+1) );
      }
   }
}

// ****************************************************************************
// Clase 'SemiCilindroBarrido'

SemiCilindroBarrido::SemiCilindroBarrido(const int n, const int m)
{
   vector<Tupla3f> perfil;

   double step = (M_PI  / m);
   double x, z, current = 0.0;
   
   for(int i = 0; i <= m; i++)
   {
      x = sin(current) * 1.0; //1.0 es el radio
      z = cos(current) * 1.0; //1.0 es el radio

      perfil.push_back( Tupla3f( x, 0.0, z ) );

      current += step; 
   }
   
   inicializar(perfil, n);
}

