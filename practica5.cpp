// *********************************************************************
// **
// ** Informática Gráfica
// ** Práctica 5  (implementaciones)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica5.hpp"
#include "grafo-escena.cpp"
#include "CamaraInter.hpp"

using namespace std ;

// TODO: práctica 5: declarar variables de la práctica 5 (static)
//    (escena, viewport, el vector de camaras, y las variables
//      que sirven para gestionar el modo arrastrar)
// ......

// desplazamiento
static constexpr int desp = 1;

// escena (igual que en la p4)
static unsigned objetoActivo5 = 0 ; // objeto activo: muneco(0)
static constexpr int numObjetos5 = 1 ;
static NodoGrafoEscena* objetos5[numObjetos5] = {nullptr};

ColFuentesLuz p5_fuentes;

// cámara activa
static unsigned camaraActiva5 = 0;
static constexpr int numCamaras5 = 2 ;
// vector de cámaras
static CamaraInteractiva* camaras[numCamaras5];

// x e y iniciales al comenzar el modo arrastrar
static int x_ini, y_ini;

// viewport actual (se recalcula al inicializar y al fijar las matrices)
Viewport viewport ;
// true si se está en modo arrastrar, false si no
static bool modo_arrastrar = false ;

// ---------------------------------------------------------------------

void P5_Inicializar(  int vp_ancho, int vp_alto )
{
    cout << "Creando objetos de la práctica 5 .... " << flush ;
    // TODO: práctica 5: inicializar las variables de la práctica 5 (incluyendo el viewport)
    // .......

    p5_fuentes.insertar(new FuenteLuzPosicional({0, 0, -15}, Tupla4f{0.4, 0.4, 0.4, 1}));
    objetos5[0] = new EscenaP4();

    cout << "hecho." << endl << flush ;
}
// ---------------------------------------------------------------------

void P5_FijarMVPOpenGL( int vp_ancho, int vp_alto )
{
    // DONE: práctica 5: actualizar viewport, actualizar y activar la camara actual
    //     (en base a las dimensiones del viewport)
    // .......

    // Actualizar viewport
    viewport.actualizar(vp_ancho, vp_alto);

    // Actualizar y recalcular viewfrustum de la cámara activa
    camaras[camaraActiva5]->ratio_yx_vp = viewport.ratio_yx;
    camaras[camaraActiva5]->calcularViewfrustum();

    // Activar cámara activa
    camaras[camaraActiva5]->activar();
}
// ---------------------------------------------------------------------

void P5_DibujarObjetos( ContextoVis & cv )
{

    // DONE: práctica 5: activar las fuentes de luz y visualizar la escena
    //      (se supone que la camara actual ya está activada)
    // .......

    glEnable(GL_LIGHTING);

    // Si modo de iluminación activado
    p5_fuentes.activar(cv.modoVis);

    objetos5[objetoActivo5]->visualizarGL(cv);

    glDisable(GL_LIGHTING);

}

// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaCaracter(  unsigned char tecla )
{

    bool result = true ;

    switch ( toupper( tecla ) )
    {
        case 'C':
            // DONE: práctica 5: activar siguiente camara
            // .....

            camaraActiva5 = (++camaraActiva5) % numCamaras5;
            camaras[camaraActiva5]->activar();

            break ;

        case 'V':
            // DONE: práctica 5: conmutar la cámara actual entre modo examinar y el modo primera persona
            // .....

            camaras[camaraActiva5]->examinar = !camaras[camaraActiva5]->examinar;

            break ;

        case '-':
            // DONE: práctica 5: desplazamiento en Z de la cámara actual (positivo) (desplaZ)
            // .....

            camaras[camaraActiva5]->desplaZ(desp);

            break;

        case '+':
            // DONE: práctica 5: desplazamiento en Z de la cámara actual (negativo) (desplaZ)
            // .....

            camaras[camaraActiva5]->desplaZ(-desp);

            break;

        default:
            result = false ;
            break ;
    }
    return result ;
}
// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaEspecial(  int tecla  )
{

    bool result = true ;

    switch ( tecla )
    {
        case GLFW_KEY_LEFT:
            // DONE: práctica 5: desplazamiento/rotacion hacia la izquierda (moverHV)
            // .....

            camaras[camaraActiva5]->moverHV(-desp, 0);

            break;
        case GLFW_KEY_RIGHT:
            // DONE: práctica 5: desplazamiento/rotación hacia la derecha (moverHV)
            // .....

            camaras[camaraActiva5]->moverHV(desp, 0);

            break;
        case GLFW_KEY_UP:
            // DONE: práctica 5: desplazamiento/rotación hacia arriba (moverHV)
            // .....

            camaras[camaraActiva5]->moverHV(0, desp);

            break;
        case GLFW_KEY_DOWN:
            // DONE: práctica 5: desplazamiento/rotación hacia abajo (moverHV)
            // .....

            camaras[camaraActiva5]->moverHV(0, -desp);

            break;
        default:
            result = false ;
            break ;
    }

    return result ;
}
// ---------------------------------------------------------------------
// se llama al hacer click con el botón izquierdo

void P5_ClickIzquierdo( int x, int y )
{

    // TODO: práctica 5: visualizar escena en modo selección y leer el color del pixel en (x,y)

    // 1. crear un 'contextovis' apropiado
    // .....

    // 2. visualizar en modo selección (sobre el backbuffer)
    // ....

    // 3. leer el color del pixel, si es 0 no se hace nada
    // .....

    // 4. buscar el objeto en el grafo de escena e informar del mismo
    // .....

}
// ---------------------------------------------------------------------
// se llama al mover el botón en modo arrastrar

void P5_InicioModoArrastrar( int x, int y )
{
    // DONE: práctica 5: activar bool de modo arrastrar, registrar (x,y) de inicio del modo arrastrar
    // .....

    modo_arrastrar = true;

    x_ini = x;
    y_ini = y;

}
// ---------------------------------------------------------------------
// se llama al subir el botón derecho del ratón

void P5_FinModoArrastrar()
{
    // DONE: práctica 5: desactivar bool del modo arrastrar
    // .....

    modo_arrastrar = false;
}
// ---------------------------------------------------------------------
// se llama durante el modo arrastrar

void P5_RatonArrastradoHasta( int x, int y )
{
    // DONE: práctica 5: calcular desplazamiento desde inicio de modo arrastrar, actualizar la camara (moverHV)
    // .....
    // NOTE: en el guión dice que actualice x_ini y y_ini, no estoy seguro

    camaras[camaraActiva5]->moverHV(x-x_ini, y-y_ini);
}
// ---------------------------------------------------------------------
// pulsar/levantar botón del ratón, específico de la práctica 5

bool P5_FGE_ClickRaton( int boton, int estado, int x, int y )
{
    //cout << "P5_FGE_ClickRaton" << endl ;
    if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_LEFT  )
        P5_ClickIzquierdo( x, viewport.alto-y );
    else if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_RIGHT )
        P5_InicioModoArrastrar( x, viewport.alto-y );
    else if ( estado == GLFW_RELEASE && boton == GLFW_MOUSE_BUTTON_RIGHT )
        P5_FinModoArrastrar();
    else
        return false ;

    return true ;
}
// ---------------------------------------------------------------------
bool P5_FGE_RatonMovidoPulsado( int x, int y )
{
    if ( modo_arrastrar )
    {
        P5_RatonArrastradoHasta( x, viewport.alto-y );
        return true ;
    }
    else
        return false ;
}
// ---------------------------------------------------------------------

bool P5_FGE_Scroll( int direction )
{
    // DONE: práctica 5: acercar/alejar la camara (desplaZ)
    // .....

    camaras[camaraActiva5]->desplaZ(direction * desp);

    return true ;
}
// ---------------------------------------------------------------------

void FijarColorIdent( const int ident )  // 0 ≤ ident < 2^24
{
    // DONE: práctica 5: fijar color actual de OpenGL usando 'ident' (glColor3ub)
    // .....

    const unsigned char
        byteR = (ident) % 0x100U,               // rojo = byte menos significativo
        byteG = (ident / 0x100U ) % 0x100U,     // verde = byte intermedio
        byteB = (ident / 0x10000U ) % 0x100U;   // azul = byte más significativo

    glColor3ub( byteR, byteG, byteB );      // cambio de color en OpenGL.
}
//---------------

int LeerIdentEnPixel( int xpix, int ypix )
{
    // DONE: práctica 5: leer el identificador codificado en el color del pixel (x,y)

    unsigned char bytes[3] ; // para guardar los tres bytes

    // leer los 3 bytes del frame-buffer
    glReadPixels( xpix,ypix, 1,1, GL_RGB,GL_UNSIGNED_BYTE, (void *)bytes);   // .....

    // reconstruir el identificador y devolverlo:
    return bytes[0] +  (0x100U*bytes[1]) + (0x10000U*bytes[2]);
}
//---------------
