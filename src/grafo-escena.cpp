// Nombre: David, Apellidos: González Durán, Titulación: GIADE
// Email: davgondur2910@correo.ugr.es, DNI: 32071015E
// *********************************************************************
// **
// ** Gestión de una grafo de escena (implementación)
// ** Copyright (C) 2016 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#include "ig-aux.h"
#include "grafo-escena.h"
#include "malla-ind.h"
#include "malla-revol.h"

using namespace std ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != NULL );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const Matriz4f & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new Matriz4f() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != NULL );
   tipo     = TipoEntNGE::material ;
   material = pMaterial ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "visibilidad"

EntradaNGE::EntradaNGE( bool * vsb )
{
   assert( vsb != NULL );
   tipo        = TipoEntNGE::visibilidad ;
   visibilidad = vsb;
}
// AÑADIDO: definición del constructor nuevo de EntradaNGE

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
   /**  no funciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL( ContextoVis & cv )
{
   // comprobar que hay un cauce en 'cv' 
   assert( cv.cauce != nullptr );

   const Tupla4f color_previo = leerFijarColVertsCauce( cv );

   // COMPLETAR: práctica 3: recorrer las entradas y visualizar cada nodo.
   // ........
   cv.cauce->pushMM();

   Material * material_previo = cv.iluminacion ? cv.material_act : nullptr;

   bool visibilidad = true;
   // AÑADIDO: booleano para evaluar la visibilidad en cada iteración

   for( unsigned i = 0; i < entradas.size(); ++i)
   {
      switch( entradas[i].tipo )
      {
         case TipoEntNGE::objeto :
            if(entradas[i].objeto->tieneColor()) cv.cauce->fijarColor(entradas[i].objeto->leerColor());
            entradas[i].objeto->visualizarGL( cv );
            break;
         case TipoEntNGE::transformacion :
            cv.cauce->compMM( *(entradas[i].matriz) );
            break;
         case TipoEntNGE::material :
            if( cv.iluminacion ) 
               entradas[i].material->activar( cv );
            break;
         case TipoEntNGE::visibilidad :
            visibilidad = *(entradas[i].visibilidad);
            break;
         // AÑADIDO: el booleano visibilidad copia el valor del booleano visibilidad de la entrada leída
      }
      if( !visibilidad ) break;
   	// AÑADIDO: si el booleano visibilidad tiene valor false, se sale del bucle for
   }

   if ( material_previo != nullptr )
      material_previo->activar( cv );

   // Se recupera el color guardado anteriormente.
   cv.cauce->fijarColor(color_previo);

   cv.cauce->popMM();

   // COMPLETAR: práctica 4: en la práctica 4, si 'cv.iluminacion' es 'true',
   // se deben de gestionar los materiales:
   //   1. guardar puntero al material activo al inicio (está en cv.material_act)
   //   2. si una entrada es de tipo material, activarlo y actualizar 'cv.material_act'
   //   3. al finalizar, restaurar el material activo al inicio (si es distinto del actual)

}




// *****************************************************************************
// visualizar pura y simplemente la geometría, sin colores, normales, coord. text. etc...

void NodoGrafoEscena::visualizarGeomGL( ContextoVis & cv )
{
   // comprobar que hay un cauce en 'cv' 
   assert( cv.cauce != nullptr );
  

   // COMPLETAR: práctica 3
   //
   // Este método hace un recorrido de las entradas del nodo, parecido a 'visualizarGL', teniendo 
   // en cuenta estos puntos:
   //
   // - usar push/pop de la matriz de modelado al inicio/fin (al igual que en visualizatGL)
   // - recorrer las entradas, llamando recursivamente a 'visualizarGeomGL' en los nodos u objetos hijos
   // - ignorar el color o identificador del nodo (se supone que el color ya está prefijado antes de la llamada)
   // - ignorar las entradas de tipo material, y la gestión de materiales (se usa sin iluminación)

   // .......

   cv.cauce->pushMM();

   bool visibilidad = true;
   // AÑADIDO: booleano para evaluar la visibilidad en cada iteración

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
         // AÑADIDO: el booleano visibilidad copia el valor del booleano visibilidad de la entrada leída
      }
      if(visibilidad == false) break;
   	// AÑADIDO: si el booleano visibilidad tiene valor false, se sale del bucle for
   }

   // Se recupera el color guardado anteriormente.

   cv.cauce->popMM();

}



// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena()
{

}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica (de cualqiuer tipo de entrada)

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
   // COMPLETAR: práctica 3: agregar la entrada al nodo, devolver índice de la entrada agregada
   // ........
   entradas.push_back(entrada);

   return entradas.size()-1;

}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const Matriz4f & pMatriz )
{
   return agregar( EntradaNGE( pMatriz ) );
}

// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial )
{
   return agregar( EntradaNGE( pMaterial ) );
}

// ---------------------------------------------------------------------
// visibilidad (copia solo puntero)
unsigned NodoGrafoEscena::agregar( bool * visibilidad )
{
   return agregar( EntradaNGE( visibilidad ) );
}
// AÑADIDO: definición del método agregar() para que agregue EntradaNGE construida con puntero a booleano

// devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
   // COMPLETAR: práctica 3: devolver puntero la matriz en ese índice
   //   (debe de dar error y abortar si no hay una matriz en esa entrada)
   // ........(sustituir 'return nullptr' por lo que corresponda)
   assert( indice < entradas.size() );
   assert( entradas[indice].tipo == TipoEntNGE::transformacion );
   assert( entradas[indice].matriz != nullptr);

   return entradas[indice].matriz;
}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{
   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)

   if(centro_calculado) return;

   //********************************************************************************** */

   std::vector<Tupla3f> vertices_borde;

   getVerticesBordes( vertices_borde );
   
   Tupla2f rango_X = Tupla2f(vertices_borde[0](0), vertices_borde[0](0)),
           rango_Y = Tupla2f(vertices_borde[0](1), vertices_borde[0](1)),
           rango_Z = Tupla2f(vertices_borde[0](2), vertices_borde[0](2));

   int size = vertices_borde.size();
   for(int i = 1; i < size; ++i)
   {
      cout << "VERTICE " << vertices_borde[i] << endl;

      if (vertices_borde[i](0) < rango_X[0])       rango_X[0] = vertices_borde[i](0);
      else if (vertices_borde[i](0) > rango_X[1])  rango_X[1] = vertices_borde[i](0);

      if (vertices_borde[i](1) < rango_Y[0])       rango_Y[0] = vertices_borde[i](1);
      else if (vertices_borde[i](1) > rango_Y[1])  rango_Y[1] = vertices_borde[i](1);

      if (vertices_borde[i](2) < rango_Z[0])       rango_Z[0] = vertices_borde[i](2);
      else if (vertices_borde[i](2) > rango_Z[1])  rango_Z[1] = vertices_borde[i](2);
   }

   cout << "Rango X = [" << rango_X[0] << "," << rango_X[1] << "]" << endl;
   cout << "Rango Y = [" << rango_Y[0] << "," << rango_Y[1] << "]" << endl;
   cout << "Rango Z = [" << rango_Z[0] << "," << rango_Z[1] << "]" << endl;

   //********************************************************************************** */
   ponerCentroOC( {  (rango_X[0]+rango_X[1])/2.0f,
                     (rango_Y[0]+rango_Y[1])/2.0f,
                     (rango_Z[0]+rango_Z[1])/2.0f  } );

   centro_calculado = true;
}


// -----------------------------------------------------------------------------

void NodoGrafoEscena::getVerticesBordes(std::vector<Tupla3f> & vertices_borde)
{
   std::cout << "Calculando vertices del borde de " << leerNombre() << endl;

   Matriz4f matriz = MAT_Ident();

   for(std::vector<EntradaNGE>::const_iterator it_ent = entradas.cbegin(); it_ent != entradas.cend(); ++it_ent)
   {
      if((*it_ent).tipo == TipoEntNGE::transformacion)
      {
         matriz = matriz * (*(*it_ent).matriz);
      }
      else if((*it_ent).tipo == TipoEntNGE::objeto)
      {
         std::vector<Tupla3f> vertices_borde_aux;
         (*it_ent).objeto->getVerticesBordes(vertices_borde_aux);

         for(std::vector<Tupla3f>::const_iterator it_ver = vertices_borde_aux.cbegin(); it_ver != vertices_borde_aux.cend(); ++it_ver)
         {
            vertices_borde.push_back(matriz * (*it_ver));
         }
      }
   }
}


// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
   const int         ident_busc, // identificador a buscar
   const Matriz4f &  mmodelado,  // matriz de modelado
   Objeto3D       ** objeto,     // (salida) puntero al puntero al objeto
   Tupla3f &         centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
   assert( 0 < ident_busc );

   Matriz4f mmodelado_actual = mmodelado;
   // Matriz4f mmodelado_actual = MAT_Ident();


   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // Se deben de dar estos pasos:

   // 1. calcula el centro del objeto, (solo la primera vez)

   if(!centro_calculado) this->calcularCentroOC();


   // 2. si el identificador del nodo es el que se busca, ya está (terminar)

   if(ident_busc == leerIdentificador())
   {
      *objeto = this;                       
      centro_wc = mmodelado_actual * leerCentroOC();
      return true;
   }

   // 3. El nodo no es el buscado: buscar recursivamente en los hijos
   //    (si alguna llamada para un sub-árbol lo encuentra, terminar y devolver 'true')

   else 
   {
      for( std::vector<EntradaNGE>::const_iterator it = entradas.cbegin(); it != entradas.cend(); ++it)
      {
         if((*it).tipo == TipoEntNGE::transformacion)
         {
            mmodelado_actual = mmodelado_actual * (*(*it).matriz);
         }
         else if((*it).tipo == TipoEntNGE::objeto)
         {
            if( (*it).objeto->buscarObjeto(ident_busc, mmodelado_actual, objeto, centro_wc) )
               return true;
               
         }
      }
   }


   // ni este nodo ni ningún hijo es el buscado: terminar
   return false ;
}


//·············· NODOEXP4 ·························

NodoEXP4::NodoEXP4()
{
   agregar( new Material( new Textura("textura-exp4-v2.jpg"), 0.5, 0.5, 0.5, 80.0) );
   agregar( new MallaEXP4() );
}

EsferaEXP5::EsferaEXP5(unsigned n)
{
   agregar( MAT_Traslacion({1.2, 0.0, 0.0}));
   agregar( MAT_Escalado(M_PI/n, M_PI/n, M_PI/n) );
   agregar( new Esfera(20,20) );
}

void EsferaEXP5::des_seleccionar(){
   seleccionado = !seleccionado;

   if (seleccionado) ponerColor({1.0, 0.0, 0.0});
   else              ponerColor({1.0, 1.0, 1.0});
}

AnilloEXP5::AnilloEXP5(unsigned n)
{
   float step = 360/n;

   for(int i=1; i<=n; ++i)
   {
      agregar(MAT_Rotacion(step, {0.0, 1.0, 0.0}));

      NodoGrafoEscena * esfera = new NodoGrafoEscena();
      
         esfera->ponerIdentificador(i);
         esfera->ponerNombre("Esfera " + i);
         esfera->agregar( new EsferaEXP5(n));
      
      agregar(esfera);
   }
}





