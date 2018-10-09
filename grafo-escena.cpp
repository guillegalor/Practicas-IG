// *********************************************************************
// **
// ** Gestión de una grafo de escena (implementación)
// ** Copyright (C) 2016 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#include "aux.hpp"
#include "matrices-tr.hpp"
#include "shaders.hpp"
#include "grafo-escena.hpp"

using namespace std ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
    assert( pObjeto != NULL );
    tipo   = TipoEntNGE::objeto ;
    objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const Matriz4f & pMatriz )
{
    tipo    = TipoEntNGE::transformacion ;
    matriz  = new Matriz4f() ; // matriz en el heap, puntero propietario
    *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
    assert( pMaterial != NULL );
    tipo     = TipoEntNGE::material ;
    material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE()
{
    /**  no fnciona debido a que se hacen copias (duplicados) de punteros
      if ( tipo == TipoEntNGE::transformacion )
      {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
      }
     * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL( ContextoVis & cv )
{
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix ();

    for (unsigned i = 0; i < entradas.size(); i++)
        switch (entradas[i].tipo) {
            case TipoEntNGE::objeto:
                entradas[i].objeto->visualizarGL (cv);
                break;
            case TipoEntNGE::transformacion:
                glMatrixMode (GL_MODELVIEW);
                glMultMatrixf (*(entradas[i].matriz));
                break;
            default:
                std::cout << "Tipo no conocido" << std::endl;
        }

    glMatrixMode (GL_MODELVIEW);
    glPopMatrix();
}
// -----------------------------------------------------------------------------

NodoGrafoEscena::NodoGrafoEscena()
{
    ponerNombre("Nodo escena anónimo");
}
// -----------------------------------------------------------------------------

void NodoGrafoEscena::fijarColorNodo( const Tupla3f & nuevo_color )
{
    // COMPLETAR: práctica 3: asignarle un color plano al nodo, distinto del padre
    // ........
}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
    entradas.push_back(entrada);
    return entradas.size()-1;
}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
    return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const Matriz4f & pMatriz )
{
    return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial )
{
    return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f* NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
    assert(entradas[indice].tipo == TipoEntNGE::transformacion);

    return entradas[indice].matriz;

}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC()
{

    // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
    //    en coordenadas de objeto (hay que hacerlo recursivamente)
    //   (si el centro ya ha sido calculado, no volver a hacerlo)
    // ........

}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
    const int         ident_busc, // identificador a buscar
    const Matriz4f &  mmodelado,  // matriz de modelado
    Objeto3D       ** objeto,     // (salida) puntero al puntero al objeto
    Tupla3f &         centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
    // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
    // ........

}

// *****************************************************************************
// Nodo del grafo de escena, con una lista añadida de parámetros
// *****************************************************************************

// -----------------------------------------------------------------------------
// devuelve el numero de grados de libertad
int NodoGrafoEscenaParam::numParametros()
{
    return parametros.size();
}
// -----------------------------------------------------------------------------

// devuelve un puntero al i-ésimo grado de libertad
Parametro * NodoGrafoEscenaParam::leerPtrParametro( unsigned i )
{
    return &parametros[i];
}
// -----------------------------------------------------------------------------

void NodoGrafoEscenaParam::siguienteCuadro()
{
    // COMPLETAR: práctica 3: actualizar todos los parámetros al siguiente cuadro
    // ........

}

//------------------------------------------------------------------------------

Grua::Grua(){
    agregar(new Palo1);

    //Palo horizontal
    glPushMatrix ();
    agregar(MAT_Traslacion(21,0,0));
    agregar(MAT_Traslacion(0,126,0));
    agregar(MAT_Rotacion(90,0,0,1));
    agregar(new Palo2);
    glPopMatrix ();

    glPushMatrix ();

}

Grua::Palo2::Palo2() : Palo1(){
    agregar (new Trozo2);
}

Grua::Palo2::Trozo2::Trozo2(){
    agregar (MAT_Escalado(1,7,1));
    agregar (new Cubo);
    agregar (MAT_Traslacion(0,0,6));
    agregar (new Cubo);

    agregar (MAT_Traslacion(0,0,-6));
    agregar (MAT_Escalado(1,(float)1/7,7));
    agregar (MAT_Traslacion(0,6,0));
    agregar (new Cubo);
}

Grua::Palo1::Palo1(){
    Trozo1* t1 = new Trozo1;
    Matriz4f m = MAT_Traslacion(0,7,0);

    for (int i = 0; i < 20; ++i) {
        agregar(t1);
        agregar(m);
    }
}

Grua::Palo1::Trozo1::Trozo1(){
    agregar(new Pilares);
    agregar(new Tapa1);
    agregar(new Tapa2);
}

Grua::Palo1::Trozo1::Pilares::Pilares(){
    Matriz4f m = MAT_Escalado(1,7,1);
    EntradaNGE mat = EntradaNGE(m);

    Cubo* c = new Cubo();
    EntradaNGE cub = EntradaNGE(c);

    agregar(mat);
    agregar(cub);

    agregar(MAT_Traslacion(6,0,0));
    agregar(cub);
    agregar(MAT_Traslacion(0,0,6));
    agregar(cub);
    agregar(MAT_Traslacion(-6,0,0));
    agregar(cub);

}

Grua::Palo1::Trozo1::Tapa1::Tapa1(){
    Cubo* c = new Cubo();

    agregar(MAT_Traslacion(0,0,1));
    agregar(MAT_Escalado(1,1,5));
    agregar(c);
    agregar(MAT_Traslacion(6,0,0));
    agregar(c);
    agregar(MAT_Traslacion(0,6,0));
    agregar(c);
    agregar(MAT_Traslacion(-6,0,0));
    agregar(c);

}

Grua::Palo1::Trozo1::Tapa2::Tapa2(){
    Cubo* c = new Cubo();

    agregar(MAT_Traslacion(1,0,0));
    agregar(MAT_Escalado(5,1,1));
    agregar(c);
    agregar(MAT_Traslacion(0,0,6));
    agregar(c);
    agregar(MAT_Traslacion(0,6,0));
    agregar(c);
    agregar(MAT_Traslacion(0,0,-6));
    agregar(c);

}
