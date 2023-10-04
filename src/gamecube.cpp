// Nombre: David, Apellidos: González Durán, Titulación: GIADE
// Email: davgondur2910@correo.ugr.es, DNI: 32071015E

#include "gamecube.h"
#include "malla-ind.h"
#include "malla-revol.h"

using namespace std;

// ···································································································
// ······························ Clase 'GameCube' ···················································
// ···································································································

GameCube::GameCube (const string vers)
{
    version = vers;

    NodoGrafoEscena * cuadrados = this->cuadrados();
    NodoGrafoEscena * cubo_movil = this->cubo_movil();

    this->ponerColor({0.265625, 0.230469, 0.429688});

    this->agregar( MAT_Escalado(2.0, 2.0, 2.0) );

    if (version == "Rubik") 
        this->agregar( new Material( new TexturaXY("textura-lamas-madera.jpg"), 0.4, 0.7, 0.05, 100.0) );
    else 
        this->agregar( new Material( 0.5, 0.5, 0.7, 100.0) );

    this->agregar( cuadrados );

    if (version == "Rubik")
        this->agregar( new Material( new Textura("rubik.jpg"), 0.4, 0.85, 0.3, 100.0) );

    this->agregar( cubo_movil );
}


NodoGrafoEscena * GameCube::cubo_movil()
{
    pm_rot_alpha = new Matriz4f *[NUM_MATRICES_ROT];
    pm_tras_dist = new Matriz4f *[NUM_MATRICES_TRAS];

    NodoGrafoEscena * cubo = new NodoGrafoEscena();
    

    //······ Traslación del cubo al final de la animación ····························································
    unsigned ind_tras[NUM_MATRICES_TRAS]; 
    for (int i = NUM_MATRICES_TRAS-1; i >= 0; --i) ind_tras[i] = cubo->agregar( MAT_Traslacion({0.0, 0.0, 0.0}) );
    //················································································································ 

    //······ Rotaciones del cubo sobre los cuadrados ·································································
    unsigned ind_rot[NUM_MATRICES_ROT]; 
    for (int i = NUM_MATRICES_ROT-1; i >= 0; --i) ind_rot[i] = cubo->agregar( MAT_Rotacion(0.0, {1.0, 0.0, 0.0}) ); 
    //················································································································  

    cubo->agregar( MAT_Traslacion({0, 2*k, -k}) );

    //······ Escalado del cubo al final de la animación ······························································
    unsigned ind_esc = cubo->agregar( MAT_Escalado(1.0, 1.0, 1.0) );
    //················································································································  

    //······ Rotación de 180º final ··················································································
    unsigned ind_rot_final = cubo->agregar(MAT_Rotacion(0.0, {1.0, 0.0, 0.0}));
    //················································································································

    cubo->agregar( MAT_Escalado(m, m, m) );
    cubo->agregar( new CuboRubik() );

    // ····· Guardado de puunteros a matriz ··········································································
    pm_rot_final = cubo->leerPtrMatriz(ind_rot_final);
    pm_escalado = cubo->leerPtrMatriz(ind_esc);
    for (unsigned i = 0; i < NUM_MATRICES_ROT; ++i) pm_rot_alpha[i] = cubo->leerPtrMatriz(ind_rot[i]);
    for (unsigned i = 0; i < NUM_MATRICES_TRAS; ++i) pm_tras_dist[i] = cubo->leerPtrMatriz(ind_tras[i]);
    // ···············································································································

    // ····· Asignación de nombre e identificador ····································································
    cubo->ponerNombre("Cubo móvil");
    cubo->ponerIdentificador(2);
    // ···············································································································

    return cubo;
}


NodoGrafoEscena * GameCube::cuadrados()
{
    visibilidad = new bool *[NUM_CAS_INVISIBLES];
    for(unsigned i = 0; i < NUM_CAS_INVISIBLES; ++i) visibilidad[i] = new bool (false);

    NodoGrafoEscena * cuadradoXZ = new NodoGrafoEscena();
        cuadradoXZ->agregar( MAT_Escalado(m, m, m) );
        cuadradoXZ->agregar( MAT_Traslacion({0.0, -0.1, 0.0}) );
        cuadradoXZ->agregar( MAT_Escalado(1.0, 0.1, 1.0) );
        cuadradoXZ->agregar( new Cubo24() );

    NodoGrafoEscena * cuadradoXY = new NodoGrafoEscena();
        cuadradoXY->agregar( MAT_Rotacion(90.0, {1.0, 0.0, 0.0}) );
        cuadradoXY->agregar(cuadradoXZ);

    NodoGrafoEscena * cuadradoYZ = new NodoGrafoEscena();
        cuadradoYZ->agregar( MAT_Rotacion(90.0, {0.0, 1.0, 0.0}) );
        cuadradoYZ->agregar(cuadradoXY);

    NodoGrafoEscena * res = new NodoGrafoEscena();
        res->agregar( visibilidad[0] );     res->agregar( MAT_Traslacion({0, k+m, -k}) );   res->agregar( cuadradoXZ ); 
        res->agregar( visibilidad[1] );     res->agregar( MAT_Traslacion({-k, 0, 0}) );     res->agregar( cuadradoXZ ); 
        res->agregar( visibilidad[2] );     res->agregar( MAT_Traslacion({0, 0, k}) );      res->agregar( cuadradoXZ );
        res->agregar( visibilidad[3] );     res->agregar( MAT_Traslacion({0, 0, k}) );      res->agregar( cuadradoXZ );

        res->agregar( visibilidad[4] );     res->agregar( MAT_Traslacion({0, -m, m}) );     res->agregar( cuadradoXY );
        res->agregar( visibilidad[5] );     res->agregar( MAT_Traslacion({0, -k, 0}) );     res->agregar( cuadradoXY );
        res->agregar( visibilidad[6] );     res->agregar( MAT_Traslacion({0, -k, 0}) );     res->agregar( cuadradoXY );
        res->agregar( visibilidad[7] );     res->agregar( MAT_Traslacion({k, 0, 0}) );      res->agregar( cuadradoXY );
        res->agregar( visibilidad[8] );     res->agregar( MAT_Traslacion({k, 0, 0}) );      res->agregar( cuadradoXY );

        res->agregar( visibilidad[9] );     res->agregar( MAT_Traslacion({m, 0, -m}) );     res->agregar( cuadradoYZ );
        res->agregar( visibilidad[10] );    res->agregar( MAT_Traslacion({0, 0, -k}) );     res->agregar( cuadradoYZ );
        res->agregar( visibilidad[11] );    res->agregar( MAT_Traslacion({0, 0, -k}) );     res->agregar( cuadradoYZ );
        res->agregar( visibilidad[12] );    res->agregar( MAT_Traslacion({0, k, 0}) );      res->agregar( cuadradoYZ );
        res->agregar( visibilidad[13] );    res->agregar( MAT_Traslacion({0, k, 0}) );      res->agregar( cuadradoYZ );

        res->agregar( visibilidad[14] );    

        if(version == "Rubik") { // Opción 1: Cubo rubik
            res->agregar( MAT_Traslacion({0, 0, 0.75*k}) );     
            res->agregar( MAT_Escalado(1.0, 1.0, 0.5));     
            res->agregar( cuadradoYZ );
        }
        else {  // Opción 2: GameCube 
            res->agregar( MAT_Traslacion({0, 0, k}) );          
            res->agregar( cuadradoYZ );
        }

        // ·····  Asignación de nombre e identificador ···································································
        res->ponerNombre("Plataformas cuadradas");
        res->ponerIdentificador(1);
        // ···············································································································

    return res;
}


void GameCube::fijarRot (const unsigned num_T, const float fase )
{
   const float percentage = fase / T;
   const float n_alpha = 90.0 * percentage;

   switch(num_T)
   {
      case 0:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {-m, k+m, -k},   {0.0, 0.0, 1.0} );     break;
      case 1:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {-k, k+m, -m},   {1.0, 0.0, 0.0} );     break;
      case 2:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {-k, k+m, +m},   {1.0, 0.0, 0.0} );     break;
      case 3:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {-k, k+m, k+m},  {1.0, 0.0, 0.0} );     break;
      case 4:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {-k, k+m, k+m},  {1.0, 0.0, 0.0} );     break;
      case 5:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {-k, m, k+m},    {1.0, 0.0, 0.0} );     break;
      case 6:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {-k, -m, k+m},   {1.0, 0.0, 0.0} );     break;
      case 7:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {-m, -k, k+m},   {0.0, 1.0, 0.0} );     break;
      case 8:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {m, -k, k+m},    {0.0, 1.0, 0.0} );     break;
      case 9:     *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {k+m, -k, k+m},  {0.0, 1.0, 0.0} );     break;
      case 10:    *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {k+m, -k, k+m},  {0.0, 1.0, 0.0} );     break;
      case 11:    *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {k+m, -k, m},    {0.0, 1.0, 0.0} );     break;
      case 12:    *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {k+m, -k, -m},   {0.0, 1.0, 0.0} );     break;
      case 13:    *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {k+m, -m, -k},   {0.0, 0.0, 1.0} );     break;
      case 14:    *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {k+m, m, -k},    {0.0, 0.0, 1.0} );     break;
      case 15:    *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {k+m, k, -m},    {0.0, -1.0, 0.0});     break;
      case 16:    *pm_rot_alpha[num_T] = MAT_Rotacion( n_alpha,  {k+m, k, m},     {0.0, -1.0, 0.0});     break;

      case 17:    *pm_rot_final        = MAT_Rotacion( 180.0*percentage,          {0.0, 0.0, 1.0});      break;

      default:    break;
   }
}


void GameCube::fijarTrasl(const unsigned num_T, const float fase)
{
    Tupla3f v;

    switch(num_T)
    {
        case 17:  
            v = (Tupla3f{-1.0*k, 2.0*k, 0}) * fase / T;     //a = {2.0*k, k, k}; b = {k, 3.0*k, k}; v = (b-a) * fase / T;
            *pm_tras_dist[0] = MAT_Traslacion(v);
            break;

        case 18:
            v = (Tupla3f{-0.5*m, -2.0*k-0.5*m, -0.5*m}) * fase / T;  //a = {k+1.5*m, 3.0*k+1.5*m, k+1.5*m}; b = {k+m, k+m, k+m}; v = (b-a) * fase / T;
            *pm_tras_dist[1] = MAT_Traslacion(v);
            break;
        
        default: break;
    }
}


void GameCube::fijarEsc(const unsigned num_T, const float fase)
{
    if(num_T == 17){
        float factor = 1.0 + (0.5 * fase / T);
        *pm_escalado = MAT_Escalado(factor, factor, factor);
    }
}


void GameCube::fijarVisib(const unsigned num_T, const bool visib)
{
    if       (num_T <= 3)   *visibilidad[num_T] = visib;
    else if  (num_T <= 9)   *visibilidad[num_T-1] = visib;
    else if  (num_T <= 16)  *visibilidad[num_T-2] = visib;
}


unsigned GameCube::leerNumParametros() const
{
    return 1;
}


void GameCube::actualizarEstadoParametro ( const unsigned iParam, const float t_sec )
{
    assert(iParam < leerNumParametros()); //Al ser iParam unsigned siempre es >= 0, por eso no se comprueba

    unsigned T_actual = trunc(t_sec/T); // T_actual es el número de periodo en la animación
    float fase = fmod(t_sec, T);  // fase es el tiempo actual transcurrido dentro del periodo

    if(T_actual>0) // Terminamos transformaciones de fases anteriores, por si alguna no la he hecho
    {   
        fijarRot   ( T_actual-1, T );
        fijarTrasl  ( T_actual-1, T );
        fijarEsc    ( T_actual-1, T );
    }

    // Realizamos transformaciones del actual periodo
    fijarRot    ( T_actual, fase );
    fijarTrasl  ( T_actual, fase );
    fijarEsc    ( T_actual, fase );

    // Visibilizamos elementos que ahora sí se deben ver
    fijarVisib  ( T_actual, true ); 
}
