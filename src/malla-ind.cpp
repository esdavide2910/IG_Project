// Nombre: David, Apellidos: González Durán, Titulación: GIADE
// Email: davgondur2910@correo.ugr.es, DNI: 32071015E
// *********************************************************************
// **
// ** Informática Gráfica, curso 2020-21
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#include "ig-aux.h"
#include "malla-ind.h"   // declaración de 'ContextoVis'
#include "lector-ply.h"

// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // nombre por defecto
   ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
}

//-----------------------------------------------------------------------------
// calcula la tabla de normales de triángulos una sola vez, si no estaba calculada

void MallaInd::calcularNormalesTriangulos()
{

   // si ya está creada la tabla de normales de triángulos, no es necesario volver a crearla
   const unsigned nt = triangulos.size() ;
   assert( 1 <= nt );
   if ( 0 < nor_tri.size() )
   {
      assert( nt == nor_tri.size() );
      return ;
   }

   // COMPLETAR: Práctica 4: creación de la tabla de normales de triángulos
   // ....
   for(std::vector<Tupla3u>::const_iterator it = triangulos.cbegin(); it != triangulos.cend(); ++it)
   {
      Tupla3f  p = vertices[(*it)(0)],     
               q = vertices[(*it)(1)],     
               r = vertices[(*it)(2)];

      Tupla3f a = q-p,     
              b = r-p;

      Tupla3f m_c = a.cross(b);

      Tupla3f n_c = (m_c.lengthSq() == 0.0) ? Tupla3f{0.0, 0.0, 0.0} : m_c.normalized();

      nor_tri.push_back(n_c);
   }
}  


// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcularNormales()
{
   // COMPLETAR: en la práctica 4: calculo de las normales de la malla
   // se debe invocar en primer lugar 'calcularNormalesTriangulos'
   // .......
   calcularNormalesTriangulos();

   std::vector<Tupla3f> sum_ver;
   sum_ver.resize(vertices.size(), {0.0, 0.0, 0.0});

   // Falta por hacer versión de menor orden

   int size_triangulos = triangulos.size();
   for(int i = 0; i < size_triangulos; ++i)
   {
      sum_ver[triangulos[i](0)] = sum_ver[triangulos[i](0)] + nor_tri[i];
      sum_ver[triangulos[i](1)] = sum_ver[triangulos[i](1)] + nor_tri[i];
      sum_ver[triangulos[i](2)] = sum_ver[triangulos[i](2)] + nor_tri[i];
   }

   assert( nor_ver.size() == 0 );

   for(std::vector<Tupla3f>::const_iterator it = sum_ver.cbegin(); it != sum_ver.cend(); ++it)
   {
      nor_ver.push_back( ((*it).lengthSq() == 0.0) ? Tupla3f{0.0, 0.0, 0.0} : (*it).normalized() );
   }
}

// -----------------------------------------------------------------------------
// calcula los bordes de la figura

void MallaInd::calcularVerticesBordesCentro()
{
   Tupla2f rango_X = Tupla2f(vertices[0](0), vertices[0](0));
   Tupla2f rango_Y = Tupla2f(vertices[0](1), vertices[0](1));
   Tupla2f rango_Z = Tupla2f(vertices[0](2), vertices[0](2));

   int size = vertices.size();
   for(int i = 1; i < size; ++i)
   {
      if (vertices[i](0) < rango_X[0])       rango_X[0] = vertices[i](0);
      else if (vertices[i](0) > rango_X[1])  rango_X[1] = vertices[i](0);

      if (vertices[i](1) < rango_Y[0])       rango_Y[0] = vertices[i](1);
      else if (vertices[i](1) > rango_Y[1])  rango_Y[1] = vertices[i](1);

      if (vertices[i](2) < rango_Z[0])       rango_Z[0] = vertices[i](2);
      else if (vertices[i](2) > rango_Z[1])  rango_Z[1] = vertices[i](2);
   }

   std::cout << "Rango X = [" << rango_X[0] << "," << rango_X[1] << "]" << std::endl;
   std::cout << "Rango Y = [" << rango_Y[0] << "," << rango_Y[1] << "]" << std::endl;
   std::cout << "Rango Z = [" << rango_Z[0] << "," << rango_Z[1] << "]" << std::endl;

   vertices_borde.clear();

   vertices_borde.push_back(Tupla3f(rango_X[0], rango_Y[0], rango_Z[0]));
   vertices_borde.push_back(Tupla3f(rango_X[0], rango_Y[0], rango_Z[1]));
   vertices_borde.push_back(Tupla3f(rango_X[0], rango_Y[1], rango_Z[0]));
   vertices_borde.push_back(Tupla3f(rango_X[0], rango_Y[1], rango_Z[1]));

   vertices_borde.push_back(Tupla3f(rango_X[1], rango_Y[0], rango_Z[0]));
   vertices_borde.push_back(Tupla3f(rango_X[1], rango_Y[0], rango_Z[1]));
   vertices_borde.push_back(Tupla3f(rango_X[1], rango_Y[1], rango_Z[0]));
   vertices_borde.push_back(Tupla3f(rango_X[1], rango_Y[1], rango_Z[1]));

   ponerCentroOC( { (rango_X[0]+rango_X[1])/2.0f,
                    (rango_Y[0]+rango_Y[1])/2.0f,
                    (rango_Z[0]+rango_Z[1])/2.0f  } );

   centro_calculado = true;
}


//AÑADIDO POR MÍ
void MallaInd::getVerticesBordes(std::vector<Tupla3f> & verts)
{
   std::cout << "Calculando vertices de bordes de objeto " << leerNombre() << std::endl;
   if(!centro_calculado) calcularVerticesBordesCentro();

   for(std::vector<Tupla3f>::const_iterator it = vertices_borde.cbegin(); it != vertices_borde.cend(); ++it)
   {
      verts.push_back((*it));
   }
}


// --------------------------------------------------------------------------------------------

void MallaInd::visualizarGL( ContextoVis & cv )
{
   using namespace std ;
   assert( cv.cauce != nullptr );
   CError();

   if ( triangulos.size() == 0 || vertices.size() == 0 )
   {  cout << "advertencia: intentando dibujar malla vacía '" << leerNombre() << "'" << endl << flush ;
      return ;
   }

   if ( cv.visualizando_normales )
   {  visualizarNormales( cv );
      return ;
   }

   // guardar el color previamente fijado y fijar el color del objeto actual
   const Tupla4f color_previo = leerFijarColVertsCauce( cv );

   // COMPLETAR: práctica 1: si el nombre del VAO es 0, crear el VAO con sus VBOs:
   //   * en primer lugar se crea y activa el VAO , con 'CrearVAO'.
   //   * después se añade el VBO con la tabla de coordenadas de posición, 'CrearVBOAtrib'.
   //   * se añade el VBO con la tabla de índices (la tabla de triángulos), con 'CrearVBOInd'.
   //   * finalmente se añaden al VAO los VBOs con tablas de atributos (opcionaes) que haya, con 'CrearVBOAtrib'.
   // si el VAO ya está creado, (nombre_vao > 0), activarlo, con 'glBindVertexArray'
   //
   //  hay que tener en cuenta que el nombre del VAO y los nombres de los VBOs deben quedar en las correspondientes 
   //  variables de instancia. Estos nombres son los devueltos por las 
   //  funciones 'CrearVAO', 'CrearVBOAtrib' y 'CrearVBOInd'.
   //
   if ( nombre_vao == 0 )
   {
      nombre_vao = CrearVAO();
      nombre_vbo_pos = CrearVBOAtrib( ind_atrib_posiciones, vertices );
      if( !triangulos.empty() ) 
         nombre_vbo_tri = CrearVBOInd( triangulos );
      if( !col_ver.empty() )   
         nombre_vbo_col = CrearVBOAtrib( ind_atrib_colores, col_ver );
      if( !nor_ver.empty() )   
         nombre_vbo_nor = CrearVBOAtrib( ind_atrib_normales, nor_ver );
      if( !cc_tt_ver.empty() ) 
         nombre_vbo_cct = CrearVBOAtrib( ind_atrib_coord_text, cc_tt_ver );
   }
   else 
      glBindVertexArray( nombre_vao );

   // COMPLETAR: práctica 1: visualizar con 'glDrawElements' y desactivar VAO. //DUDA: ¿Es correcto?
   glDrawElements(GL_TRIANGLES, triangulos.size() * 3, GL_UNSIGNED_INT, 0);
   glBindVertexArray( 0 ); 

   // restaurar el color previamente fijado
   cv.cauce->fijarColor( color_previo );
}


// -----------------------------------------------------------------------------
// visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...
// (se supone que el estado de OpenGL está fijado antes de esta llamada de alguna forma adecuada)

void MallaInd::visualizarGeomGL( ContextoVis & cv )
{
   // COMPLETAR: práctica 1: asegurarnos de que el VAO de geometría está creado y activado //DUDA: ¿Es correcto?
   if ( nombre_vao_geom == 0 )
   {
      nombre_vao_geom = CrearVAO(); 
      glBindBuffer( GL_ARRAY_BUFFER, nombre_vbo_pos );
      glVertexAttribPointer( ind_atrib_posiciones, 3, GL_FLOAT, GL_FALSE, 0, 0 );
      glEnableVertexAttribArray( ind_atrib_posiciones );
      glBindBuffer( GL_ARRAY_BUFFER, 0 );
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, nombre_vbo_tri );
   }
   else 
      glBindVertexArray( nombre_vao_geom );

   // COMPLETAR: práctica 1. Visualizar la malla y desactivar VAO.
   glDrawElements(GL_TRIANGLES, triangulos.size() * 3, GL_UNSIGNED_INT, 0 );
   glBindVertexArray( 0 );

   // AÑADIDO POR MÍ: FUNCIONALIDAD DE BORDE ----------------------------------------------------------------------
   if( nombre_vao_borde == 0 )
   {
      nombre_vao_borde = CrearVAO();
      calcularVerticesBordesCentro();
      nombre_vbo_cubo_borde = CrearVBOAtrib( ind_atrib_posiciones , vertices_borde );
      nombre_vbo_tri_borde = CrearVBOInd( lineas_borde );
   }
   else 
      glBindVertexArray( nombre_vao_borde );

   glDrawElements(GL_LINES, lineas_borde.size(), GL_UNSIGNED_INT, 0);
   glBindVertexArray( 0 );
   
   //--------------------------------------------------------------------------------------------------------------
}


// -----------------------------------------------------------------------------
// visualizar las normales en cada vértice

void MallaInd::visualizarNormales( ContextoVis & cv )
{
   using namespace std ;

   if ( nor_ver.size() == 0 )
   {
      cout << "Advertencia: intentando dibujar normales de una malla que no tiene tabla (" << leerNombre() << ")." << endl ;
      return ;
   }  
   if ( nombre_vao_normales == 0 )
   {  
      for( unsigned i = 0 ; i < vertices.size() ; i++ )
      {  
         segmentos_normales.push_back( vertices[i] );
         segmentos_normales.push_back( vertices[i]+ 0.35f*(nor_ver[i]) );
      }
      nombre_vao_normales = CrearVAO();
      CrearVBOAtrib( ind_atrib_posiciones, segmentos_normales );   
   }
   else 
      glBindVertexArray( nombre_vao_normales );

   cv.cauce->fijarColor( 1.0, 0.5, 0.2 ); // fijar el color (rojo), con el VAO activado.
   glDrawArrays( GL_LINES, 0, segmentos_normales.size() );
   glBindVertexArray( 0 );
   CError();
}

// ****************************************************************************
// Clase 'MallaPLY'

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
   ponerNombre( std::string("malla leída del archivo '") + nombre_arch + "'" );

   // COMPLETAR: práctica 2: leer archivo PLY e inicializar la malla
   // ..........................

   LeerPLY(nombre_arch, vertices, triangulos);


   // COMPLETAR: práctica 4: invocar  a 'calcularNormales' para el cálculo de normales
   // .................
   calcularNormales();

}

// ****************************************************************************
// Clase 'Cubo' completo

Cubo::Cubo()
:  MallaInd( "cubo 8 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

}

// ****************************************************************************
// Clase 'CuboTejado'

CuboTejado::CuboTejado()
:  MallaInd( "cubo 8 vértices" )
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
         { +0.0, +2.0, +0.0 }, // 8
      } ;

   for( Tupla3f & v : vertices )
      v = 2.0f*v +Tupla3f({0.0,2.0,0.0});



   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         //{2,3,7}, {2,7,6}, // Y+ (+2)   // quito cara superior
         {2,3,8}, {3,7,8}, {7,6,8}, {6,2,8}, // añado tejado 

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;

}

// ****************************************************************************
// Clase 'CuboColores' completo

CuboColores::CuboColores()
:  MallaInd( "cubo 8 vértices con colores" )
{
   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7
      } ;


   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6}, // Z-
         {1,5,7}, {1,7,3}  // Z+ (+1)
      } ;


   col_ver =
      {  { 0.0, 0.0, 0.0 }, // 0
         { 0.0, 0.0, 1.0 }, // 1
         { 0.0, 1.0, 0.0 }, // 2
         { 0.0, 1.0, 1.0 }, // 3
         { 1.0, 0.0, 0.0 }, // 4
         { 1.0, 0.0, 1.0 }, // 5
         { 1.0, 1.0, 0.0 }, // 6
         { 1.0, 1.0, 1.0 }, // 7
      } ;
}

// ****************************************************************************
// Clase 'Tetraedro' completo

Tetraedro::Tetraedro()
:  MallaInd( "poliedro con cuatro caras, seis aristas y cuatro vértices" )
{
   vertices =
      {  { -1.0, -1.0, -0.7 }, // 0
         { -0.7, -0.5, +1.0 }, // 1
         { -0.5, +0.8, +0.6 }, // 2
         { +1.0, +1.0, +0.8 }, // 3
      } ;

   triangulos =
      {  {0,1,2}, {0,1,3},
         {0,2,3}, {1,2,3}
      } ;

   col_ver =
      { { 0.0, 0.5, 0.25}, 
        { 0.0, 0.5, 0.25}, 
        { 0.0, 0.5, 0.25}, 
        { 0.0, 0.5, 0.25}, 
      };

}

// ****************************************************************************
// Clase 'EstrellaZ'

EstrellaZ::EstrellaZ(int n, float r1, float r2, Tupla3f origen)
: MallaInd("estrella plana de n vértices")
{
   vertices = {{0,0,0}};

   addNCirclePoints(n, r1, 0.0, vertices);
   addNCirclePoints(n, r2, 0.5, vertices);

   if(origen(0)!=0.0 or origen(1)!=0.0 or origen(2)!=0.0)
   {
      displacePoints(vertices, origen);
   }

   triangulos.push_back( {0, 1, 2*n} );
   triangulos.push_back( {0, 1, 1+n} );

   for(int i = 2; i <= n; ++i)
   {
      triangulos.push_back({0, i, i+n });
      triangulos.push_back({0, i, i+n-1 });
   }

   col_ver = { {1.0, 1.0, 1.0} };

   for(int i=1; i <= (2*n); ++i)
   {
      col_ver.push_back(
         Tupla3f(vertices[i](0)/r1, vertices[i](1)/r1, vertices[i](2)/r1)
      );
   }
}

// ****************************************************************************
// Clase 'EstrellaZProfundidad'

EstrellaZProfundidad::EstrellaZProfundidad(int n, float r1, float r2, float profundidad, Tupla3f origen)
: MallaInd("estrella plana de n vértices")
{
   vertices = { {0,0,-profundidad}, {0,0,profundidad} };

   addNCirclePoints(n, r1, 0.0, vertices);
   addNCirclePoints(n, r2, 0.5, vertices);

   // Generación de color

   col_ver = { {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0} };

   for(int i=2; i <= (2*n+1); ++i)
   {
      col_ver.push_back(Tupla3f(vertices[i](0)/r1, vertices[i](1)/r1, vertices[i](2)/r1));
   }

   //Desplazamiento de vértices, si procede

   if(origen(0)!=0.0 or origen(1)!=0.0 or origen(2)!=0.0)
   {
      displacePoints(vertices, origen);
   }

   //Cálculo de índices

   triangulos.push_back( {0, 2, 2*n+1} );
   triangulos.push_back( {0, 2, n+2} );
   triangulos.push_back( {1, 2, 2*n+1} );
   triangulos.push_back( {1, 2, n+2} );

   for(int i = 3; i <= (n+1); ++i)
   {
      triangulos.push_back({0, i, i+n });
      triangulos.push_back({0, i, i+n-1 });
      triangulos.push_back({1, i, i+n});
      triangulos.push_back({1, i, i+n-1});
   }

   
}

// ****************************************************************************
// Clase 'CasaX'

CasaX::CasaX(Tupla3f escala, Tupla3f origen)
: MallaInd("Casa")
{
   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7

         { -1.0, +2.0, +0.0 }, // 8
         { +1.0, +2.0, +0.0 }, // 9
      } ;


   for( Tupla3f & v : vertices )
   {
      v = Tupla3f(v(0)*escala(0), v(1)*escala(1), v(2)*escala(2)) + origen;
   }

   triangulos =
      {  {0,1,3}, {0,3,2}, // X-
         {4,7,5}, {4,6,7}, // X+ (+4)

        // {0,5,1}, {0,4,5}, // Y-
        // {2,3,7}, {2,7,6}, // Y+ (+2)

         {0,6,4}, {0,2,6},  // Z-
         {1,5,7}, {1,7,3},  // Z+ (+1)

         {8,2,3}, {7,9,6},
         {8,9,3}, {8,9,6},
         {9,3,7}, {8,2,6},


      } ;
}

// ****************************************************************************
// Clase 'RejillaY'

RejillaY::RejillaY(unsigned n, unsigned m)
: MallaInd("RejillaY")
{
   assert(n>1); assert(m>1);

   float size = 1.0;

   float r = 0.0;
   
   for(unsigned i = 0; i <= n; ++i){

      float c = 0.0;

      for(unsigned j = 0; j <= m; ++j){

         vertices.push_back( Tupla3f(r, 0.0, c) );
         col_ver.push_back( Tupla3f(r, 0.0, c) );
         
         c += size/m;

      }

      r += size/n;

   }

   for(unsigned i = 0; i < n; ++i){
      for(unsigned j = 0; j < m; ++j){
         triangulos.push_back( Tupla3u( i*(m+1)+j, i*(m+1)+j+1, i*(m+1)+m+j+2 ) );
         triangulos.push_back( Tupla3u( i*(m+1)+j, i*(m+1)+m+j+2, i*(m+1)+m+j+1 ) );
      }
   }
}

// ****************************************************************************
// Clase 'MallaTorre'

MallaTorre::MallaTorre(unsigned n)
: MallaInd("MallaTorre")
{
   assert(n>=1);

   float y = 0.0;
   float step = 0.5;

   for(unsigned j=0; j <= n; j++, y+=step)
   {
      vertices.push_back( Tupla3f(0.0, y, 1.0) ); 
      vertices.push_back( Tupla3f(0.0, y, 0.0) );
      vertices.push_back( Tupla3f(1.0, y, 0.0) );
      vertices.push_back( Tupla3f(1.0, y, 1.0) );
   }

   
   for(unsigned i = 0; i < n; i++)
   {
      for(unsigned j = 0; j < 3; ++j)
      {
         triangulos.push_back( Tupla3u(4*i+j, 4*i+j+1, 4*i+j+5) );
         triangulos.push_back( Tupla3u(4*i+j, 4*i+j+5, 4*i+j+4) );
      }
      triangulos.push_back( Tupla3u(4*i+3,   4*i, 4*i+4) );
      triangulos.push_back( Tupla3u(4*i+3, 4*i+4, 4*i+7) );
   }   

}

// ****************************************************************************
// Clase 'Cuadrado'

Cuadrado::Cuadrado()
: MallaInd("Cuadrado")
{
   vertices.push_back( {-1.0, 0.0, 1.0} );
   vertices.push_back( {1.0, 0.0, 1.0} );
   vertices.push_back( {1.0, 0.0, -1.0} );
   vertices.push_back( {-1.0, 0.0, -1.0} );

   triangulos.push_back( {0, 1, 2} );
   triangulos.push_back( {0, 2, 3} );

}


// ****************************************************************************
// Clase 'Cuadrado'

CuadradoProfundidad::CuadradoProfundidad()
: MallaInd("Cuadrado con profundidad")
{
   vertices.push_back( {-1.0, 0.00, 1.0} );
   vertices.push_back( {1.0, 0.00, 1.0} );
   vertices.push_back( {1.0, 0.00, -1.0} );
   vertices.push_back( {-1.0, 0.00, -1.0} );

   vertices.push_back( {-1.0, -0.05, 1.0} );
   vertices.push_back( {1.0, -0.05, 1.0} );
   vertices.push_back( {1.0, -0.05, -1.0} );
   vertices.push_back( {-1.0, -0.05, -1.0} );

   triangulos.push_back( {0, 1, 2} );
   triangulos.push_back( {0, 2, 3} );
   triangulos.push_back( {4, 5, 6} );
   triangulos.push_back( {4, 6, 7} );

   triangulos.push_back( {0, 4, 5} );
   triangulos.push_back( {0, 5, 1} );
   triangulos.push_back( {1, 5, 6} );
   triangulos.push_back( {1, 6, 2} );
   triangulos.push_back( {2, 6, 7} );
   triangulos.push_back( {2, 7, 3} );
   triangulos.push_back( {3, 7, 4} );
   triangulos.push_back( {3, 4, 0} );

   calcularNormales();
}


// ****************************************************************************
// Clase 'Cuadrado con esquinas redondas'

CuadradoEsquinasRedondas::CuadradoEsquinasRedondas()
: MallaInd("CuadradoEsquinasRedondas")
{
   vertices.push_back({0.0, 0.0, 0.0});

   vertices.push_back({0.5, 0.0, 0.95});
   vertices.push_back({0.675, 0.0, 0.92});
   vertices.push_back({0.85, 0.0, 0.85});
   vertices.push_back({0.92, 0.0, 0.675});
   vertices.push_back({0.95, 0.0, 0.5});

   triangulos.push_back( {0,1,2} );
   triangulos.push_back( {0,2,3} );
   triangulos.push_back( {0,3,4} );
   triangulos.push_back( {0,4,5} );

   vertices.push_back({0.95, 0.0, -0.5});
   vertices.push_back({0.92, 0.0, -0.675});
   vertices.push_back({0.85, 0.0, -0.85});
   vertices.push_back({0.675, 0.0, -0.92});
   vertices.push_back({0.5, 0.0, -0.95});

   triangulos.push_back( {0,16,17} );
   triangulos.push_back( {0,17,18} );
   triangulos.push_back( {0,18,19} );
   triangulos.push_back( {0,19,20} );

   vertices.push_back({-0.5, 0.0, -0.95});
   vertices.push_back({-0.675, 0.0, -0.92});
   vertices.push_back({-0.85, 0.0, -0.85});
   vertices.push_back({-0.92, 0.0, -0.675});
   vertices.push_back({-0.95, 0.0, -0.5});

   triangulos.push_back( {0,11,12} );
   triangulos.push_back( {0,12,13} );
   triangulos.push_back( {0,13,14} );
   triangulos.push_back( {0,14,15} );

   vertices.push_back({-0.95, 0.0, 0.5});
   vertices.push_back({-0.92, 0.0, 0.675});
   vertices.push_back({-0.85, 0.0, 0.85});
   vertices.push_back({-0.675, 0.0, 0.92});
   vertices.push_back({-0.5, 0.0, 0.95});

   triangulos.push_back( {0,6,7} );
   triangulos.push_back( {0,7,8} );
   triangulos.push_back( {0,8,9} );
   triangulos.push_back( {0,9,10} );

   triangulos.push_back( {0,5,6} );
   triangulos.push_back( {0,10,11});
   triangulos.push_back( {0,15,16});
   triangulos.push_back( {0,20,1});

}

// ****************************************************************************
// Clase 'CuboPuntas'

CuboPuntas::CuboPuntas()
: MallaInd("CuboPuntas")
{

   vertices =
      {  { -1.0, -1.0, -1.0 }, // 0
         { -1.0, -1.0, +1.0 }, // 1
         { -1.0, +1.0, -1.0 }, // 2
         { -1.0, +1.0, +1.0 }, // 3
         { +1.0, -1.0, -1.0 }, // 4
         { +1.0, -1.0, +1.0 }, // 5
         { +1.0, +1.0, -1.0 }, // 6
         { +1.0, +1.0, +1.0 }, // 7

         //NUEVO:
         { +6.0, 0.0 , 0.0  }, //8 
         { -6.0, 0.0 , 0.0  }, //9
         {  0.0, 0.0 , +6.0 }, //10
         {  0.0, 0.0 , -6.0 }, //11
      } ;



   triangulos =
      {  //{0,1,3}, {0,3,2}, // X-
         //{4,7,5}, {4,6,7}, // X+ (+4)

         {0,5,1}, {0,4,5}, // Y-
         {2,3,7}, {2,7,6}, // Y+ (+2)

         //{0,6,4}, {0,2,6}, // Z-
         //{1,5,7}, {1,7,3}  // Z+ (+1)

         //NUEVO:
         {5,7,8}, {7,6,8}, {6,4,8}, {4,5,8}, 
         
         {1,3,9}, {3,2,9}, {2,0,9}, {0,1,9},

         {5,7,10}, {7,3,10}, {3,1,10}, {1,5,10},

         {0,2,11}, {2,6,11}, {6,4,11}, {4,0,11},
      } ;

   for(int i=0; i<vertices.size(); ++i){
      col_ver.push_back( { vertices[i](0) / 12.0 + 0.5,
                           (vertices[i](0) / 12.0 + 0.5)/2 + (vertices[i](2) / 12.0 + 0.5)/2, 
                           vertices[i](1) * 0.5 + 0.5});
   }

}

// ****************************************************************************
// Clase 'Cubo24'

Cubo24::Cubo24()
: MallaInd("Cubo24")
{
   vertices =
      {  // CARA 0
         { -1.0, -1.0, -1.0 }, // 0 // 0
         { -1.0, -1.0, +1.0 }, // 1 // 1
         { -1.0, +1.0, -1.0 }, // 2 // 2
         { -1.0, +1.0, +1.0 }, // 3 // 3

         // CARA 1
         { +1.0, -1.0, -1.0 }, // 4 // 4
         { -1.0, -1.0, -1.0 }, // 0 // 5
         { +1.0, +1.0, -1.0 }, // 6 // 6
         { -1.0, +1.0, -1.0 }, // 2 // 7

         // CARA 2
         { +1.0, -1.0, +1.0 }, // 5 // 8
         { -1.0, -1.0, +1.0 }, // 1 // 9
         { +1.0, -1.0, -1.0 }, // 4 // 10
         { -1.0, -1.0, -1.0 }, // 0 // 11

         // CARA 3
         { +1.0, -1.0, -1.0 }, // 4 // 12
         { +1.0, -1.0, +1.0 }, // 5 // 13
         { +1.0, +1.0, -1.0 }, // 6 // 14
         { +1.0, +1.0, +1.0 }, // 7 // 15

         // CARA 4
         { +1.0, -1.0, +1.0 }, // 5 // 16
         { -1.0, -1.0, +1.0 }, // 1 // 17
         { +1.0, +1.0, +1.0 }, // 7 // 18
         { -1.0, +1.0, +1.0 }, // 3 // 19

         // CARA 5
         { +1.0, +1.0, +1.0 }, // 7 // 20
         { -1.0, +1.0, +1.0 }, // 3 // 21
         { +1.0, +1.0, -1.0 }, // 6 // 22
         { -1.0, +1.0, -1.0 }, // 2 // 23
      } ;
   

   for( int i=0; i<6 ; ++i ) {
      triangulos.push_back( {i*4+2, i*4+1, i*4} );
      triangulos.push_back( {i*4+1, i*4+2, i*4+3} );
      cc_tt_ver.push_back( {1.0, 1.0} );
      cc_tt_ver.push_back( {0.0, 1.0} );
      cc_tt_ver.push_back( {1.0, 0.0} );
      cc_tt_ver.push_back( {0.0, 0.0} );
   }

   for(int i=0; i<4; ++i) nor_ver.push_back( {-1.0, 0.0, 0.0} );
   for(int i=0; i<4; ++i) nor_ver.push_back( {0.0, 0.0, -1.0} );
   for(int i=0; i<4; ++i) nor_ver.push_back( {0.0, -1.0, 0.0} );

   for(int i=0; i<4; ++i) nor_ver.push_back( {1.0, 0.0, 0.0} );
   for(int i=0; i<4; ++i) nor_ver.push_back( {0.0, 0.0, 1.0} );
   for(int i=0; i<4; ++i) nor_ver.push_back( {0.0, 1.0, 0.0} );
}


// ****************************************************************************
// Clase 'CuboRubik'

CuboRubik::CuboRubik()
: MallaInd("CuboRubik")
{
   vertices =
      {  // CARA 0 -> L
         { -1.0, -1.0, -1.0 }, // 0 // 0
         { -1.0, -1.0, +1.0 }, // 1 // 1
         { -1.0, +1.0, -1.0 }, // 2 // 2
         { -1.0, +1.0, +1.0 }, // 3 // 3

         // CARA 1 ->B
         { +1.0, -1.0, -1.0 }, // 4 // 4
         { -1.0, -1.0, -1.0 }, // 0 // 5
         { +1.0, +1.0, -1.0 }, // 6 // 6
         { -1.0, +1.0, -1.0 }, // 2 // 7

         // CARA 2 -> D
         { +1.0, -1.0, +1.0 }, // 5 // 8
         { -1.0, -1.0, +1.0 }, // 1 // 9
         { +1.0, -1.0, -1.0 }, // 4 // 10
         { -1.0, -1.0, -1.0 }, // 0 // 11

         // CARA 3 -> R
         { +1.0, -1.0, -1.0 }, // 4 // 12
         { +1.0, -1.0, +1.0 }, // 5 // 13
         { +1.0, +1.0, -1.0 }, // 6 // 14
         { +1.0, +1.0, +1.0 }, // 7 // 15

         // CARA 4 -> F
         { +1.0, -1.0, +1.0 }, // 5 // 16
         { -1.0, -1.0, +1.0 }, // 1 // 17
         { +1.0, +1.0, +1.0 }, // 7 // 18
         { -1.0, +1.0, +1.0 }, // 3 // 19

         // CARA 5 -> U
         { +1.0, +1.0, +1.0 }, // 7 // 20
         { -1.0, +1.0, +1.0 }, // 3 // 21
         { +1.0, +1.0, -1.0 }, // 6 // 22
         { -1.0, +1.0, -1.0 }, // 2 // 23 
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

// ****************************************************************************
// Clase 'MallaDiscoP4'

MallaDiscoP4::MallaDiscoP4()
: MallaInd("MallaDiscoP4")
{
   ponerColor({1.0, 1.0, 1.0});
   const unsigned ni = 23, nj = 31 ;

   for( unsigned i= 0 ; i < ni ; i++ )
   for( unsigned j= 0 ; j < nj ; j++ )
   {
      const float
      fi = float(i)/float(ni-1),
      fj = float(j)/float(nj-1),
      ai = 2.0*M_PI*fi,
      x  = fj * cos( ai ),
      y  = fj * sin( ai ),
      z  = 0.0 ;
      vertices.push_back({ x, y, z });
      // cc_tt_ver.push_back({ (x+1)/2.0, (y+1)/2.0 });
      cc_tt_ver.push_back({ fi, fj });
   }
   for( unsigned i= 0 ; i < ni-1 ; i++ )
   for( unsigned j= 0 ; j < nj-1 ; j++ )
   {
      triangulos.push_back({ i*nj+j, i*nj+(j+1), (i+1)*nj+(j+1) });
      triangulos.push_back({ i*nj+j, (i+1)*nj+(j+1), (i+1)*nj+j });
   }

   calcularNormales();
}


// ****************************************************************************
// Clase 'CuboRubik'

Dado::Dado()
: MallaInd("Dado")
{
   vertices = {  
    	   // CARA 6
         { 0.0, 0.0, 0.0 }, 	{ 0.0, 0.0, 1.0 }, 		{ 0.0, 1.0, 0.0 }, 		{ 0.0, 1.0, 1.0 }, 

         // // CARA 3
    	   { 0.0, 0.0, 1.0 },	{ 1.0, 0.0, 1.0 }, 		{ 0.0, 1.0, 1.0 },	 	{ 1.0, 1.0, 1.0 },
    
    	   // // CARA 1
    	   { 1.0, 0.0, 1.0 }, 	{ 1.0, 0.0, 0.0 }, 		{ 1.0, 1.0, 1.0 }, 		{ 1.0, 1.0, 0.0 },
    	
    	   // // CARA 4
    	   { 1.0, 0.0, 0.0 }, 	{ 0.0, 0.0, 0.0 },		{ 1.0, 1.0, 0.0 }, 		{ 0.0, 1.0, 0.0 },
    
    	   // // CARA 5
    	   { 1.0, 1.0, 1.0 }, 	{ 1.0, 1.0, 0.0 },		{ 0.0, 1.0, 1.0 },		{ 0.0, 1.0, 0.0 },
    
    	   // // CARA 2
    	   { 0.0, 0.0, 1.0 }, 	{ 0.0, 0.0, 0.0 }, 		{ 1.0, 0.0, 1.0 }, 		{ 1.0, 0.0, 0.0 }, 
    
      } ;

   cc_tt_ver = {
		// CARA 6  
      {0.0, 2.0/3.0}, {0.25, 2.0/3.0}, {0.0, 1.0/3.0}, {0.25, 1.0/3.0},  
    
    	// // CARA 3 
      {0.25, 2.0/3.0}, {0.5, 2.0/3.0}, {0.25, 1.0/3.0}, {0.5, 1.0/3.0},
    
    	// // CARA 1
      {0.5, 2.0/3.0}, {0.75, 2.0/3.0}, {0.5, 1.0/3.0}, {0.75, 1.0/3.0},
    
    	// // CARA 4
      {0.75, 2.0/3.0}, {1.0, 2.0/3.0}, {0.75, 1.0/3.0}, {1.0, 1.0/3.0},
    
    	// // CARA 5
      {0.5, 1.0/3.0}, {0.75, 1.0/3.0}, {0.5, 0.0}, {0.75, 0.0},
    
    	// // CARA 2
      {0.5, 1.0}, {0.75, 1.0}, {0.5, 2.0/3.0}, {0.75, 2.0/3.0},
      
    
	} ;
   

   for( int i=0; i<6 ; ++i ) {
      triangulos.push_back( {i*4+2, i*4+1, i*4  } );
      triangulos.push_back( {i*4+1, i*4+2, i*4+3} );
   }

   for(int i=0; i<4; ++i) nor_ver.push_back( {-1.0, 0.0, 0.0} );
   for(int i=0; i<4; ++i) nor_ver.push_back( {0.0, 0.0, 1.0} );
   for(int i=0; i<4; ++i) nor_ver.push_back( {1.0, 0.0, 0.0} );

   for(int i=0; i<4; ++i) nor_ver.push_back( {0.0, 0.0, -1.0} );
   for(int i=0; i<4; ++i) nor_ver.push_back( {0.0, 1.0, 0.0} );
   for(int i=0; i<4; ++i) nor_ver.push_back( {0.0, -1.0, 0.0} ); 
    
}

// -----------------------------------------------------------------------------------------------

MallaEXP4::MallaEXP4()
: MallaInd("MallaEXP4")
{
   vertices =
      {  
         // Cara 1
         {  0.0,  1.0,  0.0 }, {  1.0,  0.0,  0.0 }, {  0.0,  0.0,  0.0 }, // Vértices 0, 1 y 2

         // Cara 2
         {  0.0,  0.0,  0.0 }, {  1.0,  0.0,  0.0 }, {  0.0,  0.0,  1.0 }, // Vértices 0, 1 y 3

         // Cara 3 
         {  0.0,  0.0,  1.0 }, {  0.0,  1.0,  0.0 }, {  0.0,  0.0,  0.0 }, // Vértices 0, 2 y 3

         // Cara 4
         {  1.0,  0.0,  0.0 }, {  0.0,  1.0,  0.0 }, {  0.0,  0.0,  1.0 }, // Vértices 1, 2 y 3
      } ;

   triangulos =
      {  {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {9, 10, 11}, } ;

   cc_tt_ver = {
		// CARA 1
      {0.5, 0.0}, {0.0, 1.0}, {1.0, 1.0},  
    
    	// CARA 2
      {0.5, 0.0}, {0.0, 1.0}, {1.0, 1.0}, 
    
    	// CARA 3
      {0.5, 0.0}, {0.0, 1.0}, {1.0, 1.0}, 
    
    	// CARA 4
      {0.5, 0.0}, {0.0, 1.0}, {1.0, 1.0}, 
	} ;

   calcularNormales();
}
