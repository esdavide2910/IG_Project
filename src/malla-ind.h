// Nombre: David, Apellidos: González Durán, Titulación: GIADE
// Email: davgondur2910@correo.ugr.es, DNI: 32071015E
// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>       // usar std::vector
#include "objeto3d.h"   // declaración de 'Objeto3D'

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D
{
   
   protected:
      // COMPLETAR: incluir aquí las variables y métodos privados que sean
      // necesarios para una malla indexada
      // ......
      GLenum nombre_vao_geom = 0; // nombre del VAO de geometría
      // AÑADIDO: FUNCIONALIDAD DE BORDE -----------------------------------------------------------------------------
      GLenum nombre_vao_borde = 0;
      //--------------------------------------------------------------------------------------------------------------

      std::vector<Tupla3f> vertices ;
      std::vector<Tupla3u> triangulos ;
      // AÑADIDO: FUNCIONALIDAD DE BORDE -----------------------------------------------------------------------------
      std::vector<Tupla3f> vertices_borde ;
      std::vector<unsigned> lineas_borde = { 0,1,  0,2,  0,4,  1,3,  1,5,  2,3,  2,6,  3,7,  4,5,  4,6,  5,7,  6,7 };
      //--------------------------------------------------------------------------------------------------------------
      std::vector<Tupla3f> segmentos_normales ;   // dos tuplas por cada segmento (extremos)
      GLenum nombre_vao_normales = 0 ;            // nombre del VAO con los segmentos de normales.


      std::vector<Tupla3f> col_ver ;  // colores de los vértices
      std::vector<Tupla3f> nor_ver ;  // normales de vértices
      std::vector<Tupla3f> nor_tri ;  // normales de triangulos
      std::vector<Tupla2f> cc_tt_ver ; // coordenadas de textura de los vértices

      // nombres de VAO y VBOs
      GLenum nombre_vao     = 0 , // nombre del VAO con la secuencia de vértices
             nombre_vbo_pos = 0 , // VBO de posiciones (>0) una vez creado el VAO
             nombre_vbo_tri = 0 , // nombre del VBO de triángulos, >0
             nombre_vbo_col = 0 , // nombre del VBO de colores, si hay colores, si no 0 
             nombre_vbo_nor = 0 , // nombre del VBO de normales, si hay normales, si no 0 
             nombre_vbo_cct = 0 ; // nombre del VBO de coords de text., si hay, si no 0
             
      // AÑADIDO POR MÍ: FUNCIONALIDAD DE BORDE ----------------------------------------------------------------------
      GLenum nombre_vbo_cubo_borde = 0,
             nombre_vbo_tri_borde  = 0;
      //--------------------------------------------------------------------------------------------------------------

      // normales de triángulos y vértices
      void calcularNormales();

      // calculo de las normales de triángulos (solo si no están creadas ya)
      void calcularNormalesTriangulos() ;

      // AÑADIDO POR MÍ: FUNCIONALIDAD DE BORDE ----------------------------------------------------------------------
      void calcularVerticesBordesCentro();
      //--------------------------------------------------------------------------------------------------------------

      bool centro_calculado = false;

   public:
      // crea una malla vacía (nombre: "malla indexada nueva vacía")
      MallaInd() ;

      // crea una malla vacía con un nombre concreto:
      MallaInd( const std::string & nombreIni );

      // visualizar el objeto con OpenGL
      virtual void visualizarGL( ContextoVis & cv ) ;

      // visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...
      // (se supone que el estado de OpenGL está fijado antes de esta llamada de alguna forma adecuada)
      virtual void visualizarGeomGL( ContextoVis & cv ) ;

      // visualizar las normales en cada vértice
      virtual void visualizarNormales( ContextoVis & cv );

      //AÑADIDO POR MÍ
      virtual void getVerticesBordes(std::vector<Tupla3f> & vertices_borde);
} ;
// ---------------------------------------------------------------------
// Clase para mallas obtenidas de un archivo 'ply'
// es un tipo de malla indexada que define un nuevo constructor
// que recibe el nombre del archivo ply como parámetro

class MallaPLY : public MallaInd
{
   public:
      MallaPLY( const std::string & nombre_arch ) ;
} ;


// ---------------------------------------------------------------------

class Cubo : public MallaInd
{
   public:
      Cubo();
};

// ---------------------------------------------------------------------

class CuboTejado : public MallaInd
{
   public:
      CuboTejado();
};

// --------------------------------------------------------------------

class CuboColores : public MallaInd
{
   public:
      CuboColores();
};

// --------------------------------------------------------------------

class Tetraedro : public MallaInd
{
   public:
      Tetraedro();
};

// --------------------------------------------------------------------

class EstrellaZ : public MallaInd
{
   public:
      EstrellaZ(int n, float r1 = 1.0, float r2 = 0.6, 
                Tupla3f origen = Tupla3f(0.0, 0.0, 0.0));
};

// --------------------------------------------------------------------

class EstrellaZProfundidad : public MallaInd
{
   public:
      EstrellaZProfundidad(int n, float r1 = 1.0, float r2 = 0.6, 
                           float profundidad = 0.3, 
                           Tupla3f origen = Tupla3f(0.0, 0.0, 0.0));
};

// --------------------------------------------------------------------

class CasaX : public MallaInd
{
   public:
      CasaX(Tupla3f escala = Tupla3f(1.0, 1.0, 1.0), 
            Tupla3f origen = Tupla3f(0.0, 0.0, 0.0));
};

// --------------------------------------------------------------------

class RejillaY : public MallaInd
{
   public:
      RejillaY(unsigned n, unsigned m);
};

// --------------------------------------------------------------------

class MallaTorre : public MallaInd 
{
   public:
      MallaTorre(unsigned n);
};

// --------------------------------------------------------------------

class Cuadrado : public MallaInd 
{
   public:
      Cuadrado();
};

// --------------------------------------------------------------------

class CuadradoProfundidad : public MallaInd 
{
   public:
      CuadradoProfundidad();
};

// --------------------------------------------------------------------

class CuadradoEsquinasRedondas : public MallaInd 
{
   public:
      CuadradoEsquinasRedondas();
};

// --------------------------------------------------------------------

class CuboPuntas : public MallaInd 
{
   public:
      CuboPuntas();
};

// --------------------------------------------------------------------

class Cubo24 : public MallaInd 
{
   public:
      Cubo24();
};

// --------------------------------------------------------------------

class CuboRubik : public MallaInd 
{
   public:
      CuboRubik();
};

// --------------------------------------------------------------------

class MallaDiscoP4 : public MallaInd
{
   public:
      MallaDiscoP4();
};

// --------------------------------------------------------------------

class Dado : public MallaInd
{
   public:
      Dado();
};

// --------------------------------------------------------------------

class MallaEXP4 : public MallaInd
{
   public:
      MallaEXP4();
};



#endif
