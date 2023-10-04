// Nombre: David, Apellidos: González Durán, Titulación: GIADE
// Email: davgondur2910@correo.ugr.es, DNI: 32071015E

#include "ig-aux.h"
#include "practicas.h"
#include "camara.h"
#include "fbo.h"
#include "seleccion.h"
#include "grafo-escena.h"

// framebuffer object usado para selección
static Framebuffer * fbo = nullptr ;

// ---------------------------------------------------------------------

Framebuffer * FBOActual()
{
   return fbo ;
}

// ---------------------------------------------------------------------
// fijar el color actual en el cauce usando un identificador entero

void FijarColVertsIdent( Cauce & cauce, const int ident )  // 0 ≤ ident < 2^24
{
   // COMPLETAR: práctica 5: fijar color actual de OpenGL usando 'ident' (con 'cauce.fijarColor')
   
   const unsigned char
      byteR = ( ident            ) % 0x100U,
      byteG = ( ident /   0x100U ) % 0x100U,
      byteB = ( ident / 0x10000U ) % 0x100U;

   cauce.fijarColor( float(byteR)/255.0f, float(byteG)/255.0f, float(byteB)/255.0f);
}

// ----------------------------------------------------------------------------------
// leer un identificador entero codificado en el color de un pixel en el
// framebuffer activo actualmente

int LeerIdentEnPixel( int xpix, int ypix )
{
   // COMPLETAR: práctica 5: leer el identificador codificado en el color del pixel (x,y)
   // .....(sustituir el 'return 0' por lo que corresponda)

   unsigned char bytes[3]; 
   // leer los 3 bytes del frame-buffer
   glReadPixels( xpix, ypix, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *)bytes);
   // reconstruir el identificador y devolverlo
   return bytes[0] + ( 0x100U * bytes[1]) + (0x10000U * bytes[2]);
}

// -------------------------------------------------------------------------------
// Función principal de selección, se llama al hacer click con el botón izquierdo
//
// (x,y)       = posición donde se ha hecho click en coordenadas del sistema de ventanas (enteras)
// objeto_raiz = objeto raiz (no subobjeto de otro) que se está visualizando en la ventana
// cv_dib      = contexto de visualización usado para dibujar el objeto
//
// devuelve: true si se ha seleccionado algún objeto, false si no

bool Seleccion( int x, int y, Escena * escena, ContextoVis & cv_dib )
{
   using namespace std ;
   assert( escena != nullptr );

   Cauce * cauce = cv_dib.cauce;

   // COMPLETAR: práctica 5:
   // Visualizar escena en modo selección y leer el color del pixel en (x,y)
   // Se deben de dar estos pasos:

   // cout << "Seleccion( x == " << x << ", y == " << y << ", obj.raíz ==  " << objeto_raiz.leerNombre() << " )" << endl ;

   // 1. Crear (si es necesario) y activar el framebuffer object (fbo) de selección

   if(fbo == nullptr)
      fbo = new Framebuffer(cv_dib.ventana_tam_x, cv_dib.ventana_tam_y);


   // 2. crear un 'ContextoVis' apropiado, en ese objeto:
   //    * activar modo selecion, desactivar iluminación, poner modo relleno
   //    * usar el mismo cauce que en 'cv_dib'
   //    * fijar el tamaño de la ventana igual que en 'cv_dib'
   
   ContextoVis * cv_nuevo = new ContextoVis();

   cv_nuevo->modo_seleccion = true;
   cv_nuevo->iluminacion = false;
   cv_nuevo->modo_visu = ModosVisu::relleno;

   cv_nuevo->cauce = cauce;

   cv_nuevo->ventana_tam_x = cv_dib.ventana_tam_x;
   cv_nuevo->ventana_tam_y = cv_dib.ventana_tam_y;

   FijarColVertsIdent(*cauce, 0);


   // 3. Activar fbo, cauce y viewport. Configurar cauce (modo solido relleno, sin ilum.
   //    ni texturas). Limpiar el FBO (color de fondo: 0)
   fbo->activar(cv_dib.ventana_tam_x, cv_dib.ventana_tam_y);
   cauce->activar();
   glViewport(0, 0, cv_dib.ventana_tam_x, cv_dib.ventana_tam_y);

   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   cauce->fijarEvalMIL(false);
   cauce->fijarEvalText(false);
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
   
   // 4. Activar la cámara (se debe leer de la escena con 'camaraActual')
   escena->camaraActual()->activar(*cauce);

   // 5. Visualizar el objeto raiz actual (se debe leer de la escena con 'objetoActual()')
   //    la visualización se hace en modo selección.
   escena->objetoActual()->visualizarGL(*cv_nuevo);

   // 6. Leer el color del pixel (usar 'LeerIdentEnPixel')
   // (hay que hacerlo mientras está activado el framebuffer de selección)
   int id_pixel = LeerIdentEnPixel(x, y);

   cout << "Identificador de pixel seleccionado: " << id_pixel << endl;

   // 7. Desactivar el framebuffer de selección
   fbo->desactivar();

   // 8. Si el identificador del pixel es 0, imprimir mensaje y terminar (devolver 'false')
   if(id_pixel == 0){
      cout << "No se ha seleccionado ningún objeto." << endl;
      return false;
   } 

   // 9. Buscar el objeto en el objeto_raiz (puede ser un grafo de escena)
   //    e informar del nombre del mismo (si no se encuentra, indicarlo)
   //   (usar 'buscarObjeto')
   // .....
   Objeto3D * objeto_seleccionado;
   Tupla3f centro;

   if( escena->objetoActual()->buscarObjeto(id_pixel, MAT_Ident(), &objeto_seleccionado, centro))
   {
      EsferaEXP5 * pesf = dynamic_cast<EsferaEXP5 *> (objeto_seleccionado);

      if(pesf != nullptr) {
         pesf->des_seleccionar();
         cout << "Se ha seleccionado la esfera " << pesf << "." << endl;
      }

      cout << "Se ha seleccionado el objeto " << objeto_seleccionado->leerNombre() <<
              ", con identificador " << objeto_seleccionado->leerIdentificador() <<
              ", de centro (" << centro(0) << ", " << centro(1) << ", " << centro(2) <<  ")." << endl;
      // escena->camaraActual()->mirarHacia(centro);
   }
   else
   {
      cout << "No se ha podido seleccionar el objeto con identificador " << id_pixel << ".";
   }
   
   

   // al final devolvemos 'true', ya que hemos encontrado un objeto
   return true ;
}
