// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tuplas
#include "practicas.hpp"
#include "practica3.hpp"
#include "practica4.hpp"

using namespace std ;

// DONE: práctica 4: declaración de variables de la práctica 4 (static)
// ....
static unsigned objetoActivo4 = 0 ; // objeto activo: muneco(0)
static constexpr int numObjetos4 = 4 ;
static NodoGrafoEscena* objetos4[numObjetos4] = {nullptr, nullptr, nullptr};
MaterialCambiante* m = nullptr;
FuenteLuzPosicional* f = nullptr;

ColFuentesLuz p4_fuentes;

int angulo_activo;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 4 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P4_Inicializar(  )
{
    cout << "Creando objetos de la práctica 4 .... " << flush ;

    m = new MaterialCambiante(
            {0.314, 0.784, 0.471},
            0.0,
            0.6,
            0.4,
            10,
            {1.0,1.0,1.0},
            0.0,
            1.0,
            0.0,
            1.0
            );

    f = new FuenteLuzPosicional({10,-10,10}, {0, 1, 0, 1});

    /* p4_fuentes.insertar(new FuenteLuzDireccional(-10, 30, Tupla4f{1, 1, 1, 1})); */
    /* p4_fuentes.insertar(new FuenteLuzPosicional({0, 20, 0}, Tupla4f{0.4, 0.4, 0.4, 1})); */
    p4_fuentes.insertar(new FuenteLuzDireccional(0, 0, {1, 1, 1, 1}));
    /* p4_fuentes.insertar(f); */
    objetos4[0] = new EscenaP4();
    objetos4[1] = new Moneda();
    objetos4[2] = new Test(m);
    objetos4[3] = new CuboColores;

    cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 4 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P4_FGE_PulsarTeclaCaracter( unsigned char tecla) {
    bool res = false  ;
    int key = -1;

    switch ( toupper( tecla ) ) {
        // Teclas ejercicios
        case 'E':
            m->aument();
            res = true;
            break;
        case 'R':
            m->reverse();
            res = true;
            break;
        case 'Z':
            f->posicion[1] += 0.5;
            res = true;
            break;

        // Teclas originales práctica
        case 'G' :
            angulo_activo  = ( angulo_activo + 1 ) % 2;
            break ;

        case '>' :
            key = angulo_activo == 0 ? GLFW_KEY_UP : GLFW_KEY_RIGHT;
            break ;

        case '<' :
            key = angulo_activo == 0 ? GLFW_KEY_DOWN : GLFW_KEY_LEFT;
            break ;

        case 'O' :
            objetoActivo4 = (objetoActivo4+1) % numObjetos4 ;
            cout << "práctica 3: nuevo objeto activo es: " << objetoActivo4 ;
            if ( objetos4[objetoActivo4] != nullptr )
                cout << " (" << objetos4[objetoActivo4]->leerNombre() << ")." << endl ;
            else
                cout << " (objeto no creado)" << endl ;

            res = true;

            break ;

        default :
            break ;
    }
    if (key != -1) {
        res = p4_fuentes.ptrFuente(0)->gestionarEventoTeclaEspecial(key);
    }

    return res ;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 4  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P4_DibujarObjetos( ContextoVis & cv )
{
    glEnable(GL_LIGHTING);

    // Si modo de iluminación activado
    p4_fuentes.activar(cv.modoVis);

    objetos4[objetoActivo4]->visualizarGL(cv);

    glDisable(GL_LIGHTING);
}
