// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica2.hpp"
#include "Objeto3D.hpp"
#include "MallaPLY.hpp"
#include "MallaRevol.hpp"

using namespace std ;

static unsigned objetoActivo2 = 0 ; // objeto activo: malla ply (0), malla revol (1)
static constexpr int numObjetos2 = 2 ;

static Objeto3D * objetos2[numObjetos2] = { nullptr, nullptr };

// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P2_Inicializar(  )
{
    cout << "Creando objetos de la práctica 2 .... " << flush ;

    objetos2[0] = new MallaPLY("../plys/beethoven.ply");
    objetos2[1] = new Cilindroide(0.75, 20, 20, true, true, false);

    //testear color
    objetos2[0]->fijarColorNodo({0,1,1});

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

bool P2_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
    if ( toupper(tecla) != 'O')
        return false ;

    objetoActivo2 = (objetoActivo2+1) % numObjetos2 ;
    cout << "práctica 2: nuevo objeto activo es: " << objetoActivo2 ;
    if ( objetos2[objetoActivo2] != nullptr )
        cout << " (" << objetos2[objetoActivo2]->leerNombre() << ")." << endl ;
    else
        cout << " (objeto no creado)" << endl ;
    return true ;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 1  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P2_DibujarObjetos( ContextoVis & cv )
{
    objetos2[objetoActivo2]->visualizarGL(cv);
}
