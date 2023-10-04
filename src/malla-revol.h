// Nombre: David, Apellidos: González Durán, Titulación: GIADE
// Email: davgondur2910@correo.ugr.es, DNI: 32071015E
// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase MallaRevol
// **
// *********************************************************************

#ifndef IG_MALLAREVOL_HPP
#define IG_MALLAREVOL_HPP

#include <vector>          // usar std::vector
#include <string>

#include "malla-ind.h"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para mallas indexadas obtenidas a partir de la revolución de un perfil

class MallaRevol : public MallaInd
{
   private:

   std::vector<Tupla3f> calcularNormales
   (
      const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
      const unsigned num_copias                // número de copias del perfil
   );

   std::vector<float> calcularDistTextura
   (
      const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
      const unsigned num_copias                // número de copias del perfil
   );


   protected: //

   MallaRevol() {} // solo usable desde clases derivadas con constructores especificos

   // Método que crea las tablas de vértices, triángulos, normales y cc.de.tt.
   // a partir de un perfil y el número de copias que queremos de dicho perfil.
   void inicializar
   (
      const std::vector<Tupla3f> & perfil,     // tabla de vértices del perfil original
      const unsigned               num_copias  // número de copias del perfil
   ) ;
} ;
// --------------------------------------------------------------------- lauvivaldi@hotmail.com


class MallaRevolPLY : public MallaRevol
{
   public:
   MallaRevolPLY( const std::string & nombre_arch,
                  const unsigned nperfiles ) ;
} ;

// --------------------------------------------------------------------

class Cilindro : public MallaRevol
{
   public:
      Cilindro(
         const int num_verts_per,  //número de vértices del perfil original (m)
         const unsigned nperfiles  //número de perfiles (n)
      );
};

// --------------------------------------------------------------------

class Cono : public MallaRevol
{
   public:
      Cono(
         const int num_verts_per,  //número de vértices del perfil original (m)
         const unsigned nperfiles  //número de perfiles (n)
      );
};

// --------------------------------------------------------------------

class Esfera : public MallaRevol
{
   public:
      Esfera(
         const int num_verts_per,  //número de vértices del perfil original (m)
         const unsigned nperfiles  //número de perfiles (n)
      );
};

// --------------------------------------------------------------------

class MallaBarrido : public MallaInd
{
   private:

   protected: 

   MallaBarrido() {} 

   void inicializar
   (
      const std::vector<Tupla3f> & perfil,     
      const unsigned               num_copias 
   ) ;
} ;

// --------------------------------------------------------------------

class SemiCilindroBarrido : public MallaBarrido
{
   public:
      SemiCilindroBarrido(
         const int n,  
         const int m  
      );
} ;

// --------------------------------------------------------------------

#endif
