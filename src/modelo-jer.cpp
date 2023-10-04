// Nombre: David, Apellidos: González Durán, Titulación: GIADE
// Email: davgondur2910@correo.ugr.es, DNI: 32071015E

#include "modelo-jer.h"
#include "malla-ind.h"
#include "malla-revol.h"

using namespace std;

// ············································································
// ·························· Clase 'GrafoEstrellaX' ··························
// ············································································

GrafoEstrellaX::GrafoEstrellaX ( const unsigned n, const float alpha )
{
   NodoGrafoEscena * scene_Star = new NodoGrafoEscena();
   scene_Star->agregar( MAT_Escalado(1.3, 1.3, 1.3) );
   scene_Star->agregar( new EstrellaZProfundidad(n) );

   NodoGrafoEscena * scene_Cone = new NodoGrafoEscena();
   scene_Cone->agregar( MAT_Traslacion({0.0, 1.3, 0.0}) );
   scene_Cone->agregar( MAT_Escalado( 0.14, 0.15, 0.14) );
   scene_Cone->ponerColor( {0.0, 128.0, 128.0} );
   scene_Cone->agregar( new Cono(20,20) );

   unsigned ind1 = this->agregar( MAT_Rotacion( alpha, {0.0, 0.0, 1.0}) ); 

   this->agregar(scene_Star);
   
   for( unsigned i = 0; i < n; ++i )
   {
      this->agregar( scene_Cone );
      this->agregar( MAT_Rotacion(360/n, {0.0, 0.0, 1.0}) );
   }

   pm_rot_alpha = this->leerPtrMatriz( ind1 );

}

void GrafoEstrellaX::fijarAlpha
( const float n_alpha )
{
   *pm_rot_alpha = MAT_Rotacion( n_alpha, {0.0, 0.0, 1.0} );
}

unsigned GrafoEstrellaX::leerNumParametros() const
{
   return 1;
}

void GrafoEstrellaX::actualizarEstadoParametro
( const unsigned iParam, const float t_sec )
{
   assert(iParam < leerNumParametros()); //Al ser iParam unsigned siempre es >= 0, por eso no se comprueba

   switch(iParam)
   {
      case 0:
         fijarAlpha(2.5*t_sec);
         break;
   }
}


// ············································································
// ···························· Clase 'GrafoCubos' ····························
// ············································································

GrafoCubos::GrafoCubos()
{
   NodoGrafoEscena * cubo_central = new NodoGrafoEscena();

         NodoGrafoEscena * cara_XZ_inf = new NodoGrafoEscena(); 
         cara_XZ_inf->agregar( MAT_Traslacion({ -0.5, -0.5, -0.5}) );
         cara_XZ_inf->agregar( new RejillaY(10,10) );

      cubo_central->agregar(cara_XZ_inf);

      cubo_central->agregar( MAT_Rotacion(90, {0.0, 0.0, 1.0}) );
      cubo_central->agregar(cara_XZ_inf);

      cubo_central->agregar( MAT_Rotacion(90, {0.0, 0.0, 1.0}) );
      cubo_central->agregar(cara_XZ_inf);

      cubo_central->agregar( MAT_Rotacion(90, {0.0, 0.0, 1.0}) );
      cubo_central->agregar(cara_XZ_inf);

      cubo_central->agregar( MAT_Rotacion(90, {1.0, 0.0, 0.0}) );
      cubo_central->agregar(cara_XZ_inf);

      cubo_central->agregar( MAT_Rotacion(180, {1.0, 0.0, 0.0}) );
      cubo_central->agregar(cara_XZ_inf);

   

   NodoGrafoEscena * cubos_laterales = new NodoGrafoEscena();

         NodoGrafoEscena * cubo_XZ_inf = new NodoGrafoEscena();

         unsigned ind1 = cubo_XZ_inf->agregar( MAT_Rotacion(0, {0.0, 1.0, 0.0}) ); 
         cubo_XZ_inf->agregar( MAT_Traslacion({0.0, 0.75, 0.0}) );
         cubo_XZ_inf->agregar( MAT_Escalado(0.15, 0.25, 0.15) );
         cubo_XZ_inf->agregar( new CuboColores() );

      cubos_laterales->agregar( cubo_XZ_inf);

      cubos_laterales->agregar( MAT_Rotacion(90, {0.0, 0.0, 1.0}) );
      cubos_laterales->agregar( cubo_XZ_inf);

      cubos_laterales->agregar( MAT_Rotacion(90, {0.0, 0.0, 1.0}) );
      cubos_laterales->agregar( cubo_XZ_inf);

      cubos_laterales->agregar( MAT_Rotacion(90, {0.0, 0.0, 1.0}) );
      cubos_laterales->agregar( cubo_XZ_inf);

      cubos_laterales->agregar( MAT_Rotacion(90, {1.0, 0.0, 0.0}) );
      cubos_laterales->agregar( cubo_XZ_inf);

      cubos_laterales->agregar( MAT_Rotacion(180, {1.0, 0.0, 0.0}) );
      cubos_laterales->agregar( cubo_XZ_inf);


   this->agregar(cubo_central);
   this->agregar(cubos_laterales);

   pm_rot_alpha = cubo_XZ_inf->leerPtrMatriz( ind1 );

}

void GrafoCubos::fijarAlpha
( const float n_alpha )
{
   *pm_rot_alpha = MAT_Rotacion( n_alpha, {0.0, 1.0, 0.0} );
}

unsigned GrafoCubos::leerNumParametros() const
{
   return 1;
}

void GrafoCubos::actualizarEstadoParametro
( const unsigned iParam, const float t_sec )
{
   assert(iParam < leerNumParametros()); //Al ser iParam unsigned siempre es >= 0, por eso no se comprueba

   switch(iParam)
   {
      case 0:
         fijarAlpha(5*t_sec);
         break;
   }
}




// ············································································
// ······························ Clase 'Coche' ····························
// ············································································


Coche::Coche()
{
   NodoGrafoEscena * caja = new NodoGrafoEscena();

      NodoGrafoEscena * cajasup = new NodoGrafoEscena();

         cajasup->agregar( MAT_Escalado(0.9, 0.6, 1.25) );
         cajasup->agregar( new Cubo() );

      NodoGrafoEscena * cajainf = new NodoGrafoEscena();

         cajainf->agregar( MAT_Escalado(1.1, 0.7, 2.0) );
         cajainf->agregar( new Cubo() );

      caja->agregar(MAT_Traslacion({0.0,-0.7, 0.0}));
      caja->agregar(cajainf);
      caja->agregar(MAT_Traslacion({0.0, 1.3, 0.0}));
      caja->agregar(cajasup);


   NodoGrafoEscena * rueda = new NodoGrafoEscena();

      NodoGrafoEscena * cilindro = new NodoGrafoEscena();

         cilindro->agregar( MAT_Rotacion(90.0, {0.0, 0.0, 1.0}) );
         cilindro->agregar(MAT_Traslacion({0.0, -0.5, 0.0}));
         cilindro->agregar(new SemiCilindroBarrido(7,15) );
         cilindro->agregar(MAT_Rotacion(180.0, {0.0, 1.0, 0.0}));      
         cilindro->agregar(new SemiCilindroBarrido(7,15) );

      unsigned ind_rot_rueda = rueda->agregar(MAT_Rotacion(0.0, {1.0, 0.0, 0.0}));
      rueda->agregar(MAT_Escalado(0.5,0.5,0.5));
      rueda->agregar( cilindro );
      rueda->agregar(MAT_Escalado(0.5, 0.7, 0.7));
      rueda->agregar( new Cubo() );


   NodoGrafoEscena * ruedas_izq = new NodoGrafoEscena();

      ruedas_izq->agregar(MAT_Traslacion({1.45,-1.2,1.2}));
      ruedas_izq->agregar(rueda);
      ruedas_izq->agregar(MAT_Traslacion({0.0,0.0,-2.4}));
      ruedas_izq->agregar(rueda);

   NodoGrafoEscena * ruedas_der = new NodoGrafoEscena();

      ruedas_der->agregar(MAT_Traslacion({-1.45,-1.2,1.2}));
      ruedas_der->agregar(rueda);
      ruedas_der->agregar(MAT_Traslacion({0.0,0.0,-2.4}));
      ruedas_der->agregar(rueda);

   
   unsigned ind_rot_coche = this->agregar(MAT_Rotacion(1.0, {0.0, 1.0, 0.0}));
   this->agregar(MAT_Traslacion({-5.0, 0.0, 0.0}));
   this->agregar(caja);
   this->agregar(ruedas_izq);
   this->agregar(ruedas_der);

   pm_rot_coche = leerPtrMatriz(ind_rot_coche);
   pm_rot_rueda = rueda->leerPtrMatriz(ind_rot_rueda);
}

void Coche::fijarRotacionRueda( const float n_alpha )
{
   float rot = n_alpha * 360.0;
   *pm_rot_rueda = MAT_Rotacion(rot, {1.0, 0.0, 0.0});
}

void Coche::fijarRotacionCoche( const float n_alpha )
{
   float rot = n_alpha * 360.0;
   *pm_rot_coche = MAT_Rotacion(rot, {0.0, 1.0, 0.0});
}

unsigned Coche::leerNumParametros() const
{
   return 2;
}

void Coche::actualizarEstadoParametro( const unsigned iParam, const float t_sec )
{
   assert(iParam < leerNumParametros()); 

   switch(iParam)
   {
      case 0:
         fijarRotacionCoche(t_sec/Tcoche);
         break;

      case 1:
         fijarRotacionRueda(t_sec/Trueda);
         break;
   }
}


// ············································································
// ···························· Clase 'NodoCubo24' ····························
// ············································································

NodoCubo24::NodoCubo24()
{
   this->agregar( new Material(new Textura("window-icon.jpg"), 0.5, 0.7, 0.7, 50.0) );
   this->agregar( new Cubo24() );
}

// ············································································
// ···························· Clase 'NodoDiscoP4' ····························
// ············································································

NodoDiscoP4::NodoDiscoP4()
{
   ponerNombre("Nodo ejercicio adicional práctica 4, examen 27 enero");
   agregar( new Material( new Textura("cuadricula.jpg"), 0.5, 0.7, 0.7, 50.0) );
   agregar( new MallaDiscoP4() );
}

// ············································································
// ···························· Clase 'NodoEsferaP4' ····························
// ············································································

NodoEsferaP4::NodoEsferaP4()
{
   agregar( new Material( 0.5, 0.5, 1.0, 50.0) );
   agregar( new Esfera(50,50) );
}

// ············································································
// ·························· Clase 'GrafoEsferasP5' ··························
// ············································································

GrafoEsferasP5::GrafoEsferasP5()
{
   const unsigned n_filas_esferas   = 8,
                  n_esferas_x_fila  = 5;

   const float e = 0.4/n_esferas_x_fila;

   agregar( MAT_Escalado(e, e, e) );

   for(unsigned i = 0; i < n_filas_esferas; ++i)
   {
      NodoGrafoEscena * fila_esferas = new NodoGrafoEscena();

      for(unsigned j = 0; j < n_esferas_x_fila; ++j)
      {
         NodoGrafoEscena * esfera = new NodoGrafoEscena();

         esfera->ponerIdentificador(i * n_esferas_x_fila + j + 1); 
         // Hay que poner +1, ya que el identificador 0 ya está cogido

         esfera->ponerNombre("esfera número " + to_string(j+1) + " de la fila número " + to_string(i+1));

         esfera->agregar( new Esfera(40,40) );

         fila_esferas->agregar( MAT_Traslacion( {2.2, 0.0, 0.0} ) );

         fila_esferas->agregar( esfera );
      }
      agregar(fila_esferas);
      agregar( MAT_Traslacion( {0.0, 0.0, 5.0} ) );
   }   

}

// ············································································
// ····························· Clase 'NodoDado' ·····························
// ············································································

NodoDado::NodoDado()
{
   agregar( new Material( new Textura("rubik2.jpg"), 0.5, 0.5, 0.5, 100.0 ) );
   agregar( new Dado() );
}

