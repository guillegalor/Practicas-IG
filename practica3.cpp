// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica3.hpp"

using namespace std ;

static unsigned objetoActivo3 = 0 ; // objeto activo: muneco(0)
static constexpr int numObjetos3 = 1 ;
static bool animaciones = false;
static unsigned parametroActivo = 0;

static NodoGrafoEscenaParam* objetos3[numObjetos3] = {nullptr};

// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P3_Inicializar(  )
{
    cout << "Creando objetos de la práctica 3 .... " << flush ;

    objetos3[0] = new Muneco();

    cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 1 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P3_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
    bool res = false  ; // valor devuelto (true si se ha procesado tecla)

    switch ( toupper( tecla ) )
    {
        case 'O' :
            objetoActivo3 = (objetoActivo3+1) % numObjetos3 ;
            cout << "práctica 3: nuevo objeto activo es: " << objetoActivo3 ;
            if ( objetos3[objetoActivo3] != nullptr )
                cout << " (" << objetos3[objetoActivo3]->leerNombre() << ")." << endl ;
            else
                cout << " (objeto no creado)" << endl ;

            res = true;

            break ;

        case 'A' :
            animaciones = !animaciones;

            if (animaciones)
                FijarFuncDesocupado(FGE_Desocupado);

            std::cout << (animaciones
                ? "Activando "
                : "Desactivando ")
            << "animaciones" << std::endl;

            res = true;

            break ;

        case 'G' :
            parametroActivo = (++parametroActivo)%(objetos3[objetoActivo3]->numParametros());

            std::cout << "Parametro activo: " << parametroActivo << std::endl;
            break ;

        case 'R' :
            for (int i = 0; i < objetos3[objetoActivo3]->numParametros(); ++i){
                objetos3[objetoActivo3]->leerPtrParametro(i)->reset();
            }

            animaciones = false;

            std::cout << "Reseteando parámetros y desactivando animaciones" << std::endl;

            res = true;

            break ;

        case '>' :
            if(animaciones)
                objetos3[objetoActivo3]->leerPtrParametro(parametroActivo)->acelerar();
            else
                objetos3[objetoActivo3]->leerPtrParametro(parametroActivo)->incrementar();

            res = true;

            break ;

        case '<' :
            if(animaciones)
                objetos3[objetoActivo3]->leerPtrParametro(parametroActivo)->decelerar();
            else
                objetos3[objetoActivo3]->leerPtrParametro(parametroActivo)->decrementar();

            res = true;

            break ;

        default :
            break ;

    }
    return res ;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 3  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P3_DibujarObjetos( ContextoVis & cv )
{
    objetos3[objetoActivo3]->visualizarGL(cv);
}

//--------------------------------------------------------------------------

bool P3_FGE_Desocupado()
{
    if (!animaciones)
        return false;

    /* std::cout << "Test" << std::endl; */
    objetos3[objetoActivo3]->siguienteCuadro();
    redibujar_ventana = true;

    return true;
}
