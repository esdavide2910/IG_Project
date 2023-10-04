
// Nombre: David, Apellidos: González Durán, Titulación: GIADE
// Email: davgondur2910@correo.ugr.es, DNI: 32071015E
#ifndef GAMECUBE_HPP
#define GAMECUBE_HPP

#include "grafo-escena.h"

using namespace tup_mat ;


class GameCube : public NodoGrafoEscena
{
    protected:
        const unsigned NUM_MATRICES_ROT = 17;
        Matriz4f ** pm_rot_alpha;

        Matriz4f * pm_rot_final;

        const unsigned NUM_MATRICES_TRAS = 2; 
        Matriz4f ** pm_tras_dist;

        Matriz4f * pm_escalado;

        const unsigned NUM_CAS_INVISIBLES = 15;
        bool ** visibilidad;
            
    private:
        NodoGrafoEscena * cuadrados();
        NodoGrafoEscena * cubo_movil();

        const float k = 2.0/3.0;
        const float m = 1.0/3.0;

        const float T = 0.25;    // Tiempo de periodo, recomendado 0,25 o 0,5ç

        std::string version;
   
    public:
        GameCube (const std::string version = "Classic");

        void fijarRot     (const unsigned num_T,    const float fase);
        void fijarTrasl   (const unsigned num_T,    const float fase);
        void fijarEsc     (const unsigned num_T,    const float fase);
        void fijarVisib   (const unsigned num_T,    const bool visib);
        unsigned leerNumParametros() const;
        void actualizarEstadoParametro( const unsigned iParam, const float t_sec );
} ;


#endif // GAMECUBE_HPP