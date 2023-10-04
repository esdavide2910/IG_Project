// Nombre: David, Apellidos: González Durán, Titulación: GIADE
// Email: davgondur2910@correo.ugr.es, DNI: 32071015E
#include "latapeones.h"
#include "malla-ind.h"
#include "malla-revol.h"

using namespace std;

LataPeones::LataPeones()
{
    NodoGrafoEscena * lata = new NodoGrafoEscena();
        lata->agregar( new Material( 0.5, 0.8, 0.7, 50.0) );
        lata->agregar( MAT_Escalado(2.2, 2.2, 2.2) );
        lata->agregar( new MallaRevolPLY("lata-psup.ply", 30) );
        lata->agregar( new MallaRevolPLY("lata-pinf.ply", 30) );
        lata->agregar( new Material( new Textura("lata-coke.jpg"), 0.5, 0.8, 0.7, 50.0) );
        lata->agregar( new MallaRevolPLY("lata-pcue.ply", 30) );

    NodoGrafoEscena * peon = new NodoGrafoEscena();
        peon->agregar( MAT_Traslacion({0.0, 0.57, 0.0}) );
        peon->agregar( MAT_Escalado(0.4, 0.4, 0.4) );
        peon->agregar( new MallaRevolPLY("peon.ply", 30) );

    NodoGrafoEscena * peon_madera = new NodoGrafoEscena();
        peon_madera->agregar( new Material( new TexturaXY("text-madera.jpg"), 0.5, 0.8, 0.8, 50.0 ));
        peon_madera->agregar( peon );

    NodoGrafoEscena * peon_blanco = new NodoGrafoEscena();
        peon_blanco->agregar( new Material(0.5, 0.8, 0.1, 50.0) );
        peon_blanco->agregar( peon );

    NodoGrafoEscena * peon_negro = new NodoGrafoEscena();
        peon_negro->agregar( new Material(0.1, 0.1, 0.8, 50.0) );
        peon_negro->agregar( peon );

    this->agregar( lata );
    this->agregar( MAT_Traslacion({0.0, 0.0, 1.5}) );
    this->agregar( peon_madera );
    this->agregar( MAT_Traslacion({1.2, 0.0, 0.0}) );
    this->agregar( peon_blanco );
    this->agregar( MAT_Traslacion({1.2, 0.0, 0.0}) );
    this->agregar( peon_negro );
}




VariasLatasPeones::VariasLatasPeones()
{
    NodoGrafoEscena * lata_CocaCola = new NodoGrafoEscena();
        lata_CocaCola->agregar( new Material( 0.5, 0.8, 0.7, 50.0) );
        lata_CocaCola->agregar( MAT_Escalado(2.2, 2.2, 2.2) );
        lata_CocaCola->agregar( new MallaRevolPLY("lata-psup.ply", 30) );
        lata_CocaCola->agregar( new MallaRevolPLY("lata-pinf.ply", 30) );
        lata_CocaCola->agregar( new Material( new Textura("lata-coke.jpg"), 0.5, 0.8, 0.7, 50.0) );
        lata_CocaCola->agregar( new MallaRevolPLY("lata-pcue.ply", 30) );
        lata_CocaCola->ponerNombre("Lata de Coca-Cola");
        lata_CocaCola->ponerIdentificador(1);

    NodoGrafoEscena * lata_Pepsi = new NodoGrafoEscena();
        lata_Pepsi->agregar( new Material( 0.5, 0.8, 0.7, 50.0) );
        lata_Pepsi->agregar( MAT_Escalado(2.2, 2.2, 2.2) );
        lata_Pepsi->agregar( new MallaRevolPLY("lata-psup.ply", 30) );
        lata_Pepsi->agregar( new MallaRevolPLY("lata-pinf.ply", 30) );
        lata_Pepsi->agregar( new Material( new Textura("lata-pepsi.jpg"), 0.5, 0.8, 0.7, 50.0) );
        lata_Pepsi->agregar( new MallaRevolPLY("lata-pcue.ply", 30) );
        lata_Pepsi->ponerNombre("Lata de Pepsi");
        lata_Pepsi->ponerIdentificador(2);

    NodoGrafoEscena * lata_UGR = new NodoGrafoEscena();
        lata_UGR->agregar( new Material( 0.5, 0.8, 0.7, 50.0) );
        lata_UGR->agregar( MAT_Escalado(2.2, 2.2, 2.2) );
        lata_UGR->agregar( new MallaRevolPLY("lata-psup.ply", 30) );
        lata_UGR->agregar( new MallaRevolPLY("lata-pinf.ply", 30) );
        lata_UGR->agregar( new Material( new Textura("window-icon.jpg"), 0.5, 0.8, 0.7, 50.0) );
        lata_UGR->agregar( new MallaRevolPLY("lata-pcue.ply", 30) );
        lata_UGR->ponerNombre("Lata de la UGR");
        lata_UGR->ponerIdentificador(3);

    NodoGrafoEscena * peon = new NodoGrafoEscena();
        peon->agregar( MAT_Traslacion({0.0, 0.57, 0.0}) );
        peon->agregar( MAT_Escalado(0.4, 0.4, 0.4) );
        peon->agregar( new MallaRevolPLY("peon.ply", 30) );
        peon->ponerNombre("Peón");

    NodoGrafoEscena * peon_madera = new NodoGrafoEscena();
        peon_madera->agregar( new Material( new TexturaXZ("text-madera.jpg"), 0.5, 0.8, 0.8, 50.0 ));
        peon_madera->agregar( peon );
        peon_madera->ponerNombre("Peón de madera");
        peon_madera->ponerIdentificador(4);

    NodoGrafoEscena * peon_blanco = new NodoGrafoEscena();
        peon_blanco->agregar( new Material(0.5, 0.8, 0.1, 50.0) );
        peon_blanco->agregar( peon );
        peon_blanco->ponerNombre("Peón blanco");
        peon_blanco->ponerIdentificador(5);

    NodoGrafoEscena * peon_negro = new NodoGrafoEscena();
        peon_negro->agregar( new Material(0.1, 0.1, 0.8, 50.0) );
        peon_negro->agregar( peon );
        peon_negro->ponerNombre("Peón negro");
        peon_negro->ponerIdentificador(6);

    this->agregar( lata_CocaCola );
    this->agregar( MAT_Traslacion( {2.0, 0.0, 0.0}) );
    this->agregar( lata_Pepsi);
    this->agregar( MAT_Traslacion( {2.0, 0.0, 0.0}) );
    this->agregar( lata_UGR );
    this->agregar( MAT_Traslacion( {-4.0, 0.0, 1.5}) );
    this->agregar( peon_madera );
    this->agregar( MAT_Traslacion( {1.0, 0.0, 0.0}) );
    this->agregar( peon_blanco );
    this->agregar( MAT_Traslacion( {1.0, 0.0, 0.0}) );
    this->agregar( peon_negro );
}