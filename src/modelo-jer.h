// Nombre: David, Apellidos: González Durán, Titulación: GIADE
// Email: davgondur2910@correo.ugr.es, DNI: 32071015E
// *********************************************************************

#ifndef MODELO_JER_HPP
#define MODELO_JER_HPP

#include "grafo-escena.h"

class GrafoEstrellaX : public NodoGrafoEscena 
{
   protected:
      Matriz4f * pm_rot_alpha = nullptr;

   public:
      GrafoEstrellaX( const unsigned n, const float alpha = 0.0 ) ;
      void fijarAlpha( const float n_alpha );
      unsigned leerNumParametros() const;
      void actualizarEstadoParametro( const unsigned iParam, const float t_sec );
} ;


class GrafoCubos : public NodoGrafoEscena
{
   protected:
      Matriz4f * pm_rot_alpha = nullptr;
   
   public:
      GrafoCubos();
      void fijarAlpha( const float n_alpha );
      unsigned leerNumParametros() const;
      void actualizarEstadoParametro( const unsigned iParam, const float t_sec );

} ;



class Coche : public NodoGrafoEscena
{
   protected:
      Matriz4f * pm_rot_coche = nullptr;
      Matriz4f * pm_rot_rueda = nullptr;
      float Trueda = 2.0;
      float Tcoche = 5.0;
   
   public:
      Coche();
      void fijarRotacionRueda( const float n_alpha );
      void fijarRotacionCoche( const float n_alpha );
      unsigned leerNumParametros() const;
      void actualizarEstadoParametro( const unsigned iParam, const float t_sec );

} ;


class NodoCubo24 : public NodoGrafoEscena
{
   protected:

   public:
      NodoCubo24();
      
};


class NodoDado : public NodoGrafoEscena
{
   protected:

   public:
      NodoDado();
      
};


class NodoDiscoP4 : public NodoGrafoEscena
{
   protected:

   public:
      NodoDiscoP4();
      
};


class NodoEsferaP4 : public NodoGrafoEscena
{
   protected:

   public:
      NodoEsferaP4();
      
};


class GrafoEsferasP5 : public NodoGrafoEscena
{
   protected:

   public:
      GrafoEsferasP5();
};


#endif // MODELO_JER_HPP

// *********************************************************************
