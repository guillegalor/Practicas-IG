// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Práctica 1  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica1.hpp"
#include "Objeto3D.hpp"
#include "MallaInd.hpp"
#include "exextr1819.hpp"

static unsigned      objetoActivo       = 0 ; // objeto activo: cubo (0), tetraedro (1), otros....
static constexpr int numObjetos         = 4 ; // numero de objetos creados para esta práctica
static Objeto3D *    objeto[numObjetos] = { nullptr, nullptr, nullptr, nullptr }; // punteros a los objetos

using namespace std ;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P1_Inicializar(  )
{
    cout << "Creando objetos de la práctica 1 .... " << flush ;

    objeto[0] = new PrismaRegP1(5);
    objeto[1] = new PrismaRegP1(6);
    objeto[2] = new Tetraedro();
    objeto[3] = new Cubo();

    //testear color
    objeto[0]->fijarColorNodo({1,0,1});

    cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 1 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el cubo, el tetraedro u otros objetos (cambia el valor de
//    'objeto_activo').
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P1_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
    if ( toupper(tecla) != 'O')
        return false ;

    objetoActivo = (objetoActivo+1) % numObjetos ;
    cout << "práctica 1: nuevo objeto activo es: " << objetoActivo ;
    if ( objeto[objetoActivo] != nullptr )
        cout << " (" << objeto[objetoActivo]->leerNombre() << ")." << endl ;
    else
        cout << " (objeto no creado)" << endl ;
    return true ;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 1  para dibujar los objetos

void P1_DibujarObjetos( ContextoVis & cv )
{
    objeto[objetoActivo]->visualizarGL(cv);
    /* poligonos3_M(6); */
    /* dibujaTriRecursiva(4, -2, 2, -2, 2, 1); */
    /* CilindroViewport(5, 3, 7); */

    /* dibujarCuadradosRecursivo(3, 0); */
    /* cuadradoBlancoFondoAzul(); */
}
