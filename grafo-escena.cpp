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
    *matriz = pMatriz;
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
    for (auto& param: parametros)
        param.siguiente_cuadro();
}

//------------------------------------------------------------------------------

Grua::Grua(){
    int i;
    agregar(new Palo1);

    //Palo horizontal
    agregar(MAT_Traslacion(21,0,0));
    agregar(MAT_Traslacion(0,126,0));
    agregar(MAT_Rotacion(90,0,0,1));
    //Matriz parametro prueba
    i = agregar(MAT_Ident());
    agregar(new Palo2);

    //Parámetro de prueba
    Parametro param(
            "parametro de prueba",
            leerPtrMatriz(i),
            [=] (float v) {return MAT_Rotacion(v, 1, 0, 0);},
            false,
            0.0,
            10.0,
            4
            );

    parametros.push_back(param);
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

Muneco::Muneco(){
    Cilindro* c = new Cilindro(
            2,
            20,
            true,
            true
            );
    Esfera* e = new Esfera(
            20,
            20,
            false,
            true
            );

    int i_rot_principal = agregar(MAT_Ident());
    agregar(MAT_Escalado(1,2,1));

    // Primer cilindro del cuerpo
    agregar(c);

    agregar(MAT_Traslacion(0,1,0));

    // Esfera de articulación y rotación
    agregar(e);
    int i_rotacion1 = agregar(MAT_Ident());

    // Segundo cilindro del cuerpo
    agregar(c);

    agregar(MAT_Traslacion(0,1,0));

    // Esfera de articulación y rotación
    agregar(e);
    int i_rotacion2 = agregar(MAT_Ident());

    // Tercer cilindro del cuerpo
    agregar(c);

    agregar(MAT_Traslacion(0,1,0));

    Brazo1* br1 = new Brazo1;
    Brazo2* br2 = new Brazo2;
    agregar(br1);
    agregar(br2);

    // Esfera de articulación y rotación
    agregar(e);
    int i_rotacion3 = agregar(MAT_Ident());

    // Cilindro cabeza
    agregar(c);

    //Rotación de todo el muneco
    Parametro rot_principal(
            "Rotacion de todo el objeto",
            leerPtrMatriz(i_rot_principal),
            [=] (float v) {return MAT_Rotacion(v, 0, 1, 0);},
            false,
            0,
            10.0,
            0
            );

    //Rotación 1
    Parametro rotacion1(
            "Rotacion 1",
            leerPtrMatriz(i_rotacion1),
            [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
            true,
            0,
            10,
            0.1
            );

    //Rotación 2
    Parametro rotacion2(
            "Rotacion 2",
            leerPtrMatriz(i_rotacion2),
            [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
            true,
            0,
            10.0,
            0.1
            );

    //Rotación 3
    Parametro rotacion3(
            "Rotacion 3",
            leerPtrMatriz(i_rotacion3),
            [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
            true,
            0,
            10.0,
            0.1
            );

    //Rotación hombro brazo 1
    Parametro rotacion_hombro1(
            "Rotacion hombro brazo 1",
            br1->getArticulacionHombro(),
            [=] (float v) {return MAT_Rotacion(v, 0, 1, 0);},
            true,
            0,
            30,
            0.1
            );

    //Rotación hombro brazo 2
    Parametro rotacion_hombro2(
            "Rotacion hombro brazo 2",
            br2->getArticulacionHombro(),
            [=] (float v) {return MAT_Rotacion(v, 0, 1, 0);},
            true,
            0,
            50,
            0.1
            );

    // Rotación codo brazo 1
    Parametro rotacion_codo1(
            "Rotacion codo brazo 1",
            br1->getArticulacionCodo(),
            [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
            true,
            -10,
            90,
            0.1
            );

    //Rotación codo brazo 2
    Parametro rotacion_codo2(
            "Rotacion codo brazo 2",
            br2->getArticulacionCodo(),
            [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
            true,
            -20,
            80,
            0.1
            );

    /* parametros.push_back(rot_principal); */
    parametros.push_back(rotacion1);
    parametros.push_back(rotacion2);
    parametros.push_back(rotacion3);
    parametros.push_back(rotacion_hombro1);
    parametros.push_back(rotacion_hombro2);
    parametros.push_back(rotacion_codo1);
    parametros.push_back(rotacion_codo2);
}

Muneco::Brazo1::Brazo1(){
    Cilindro* c = new Cilindro(2, 20, true, true);
    Esfera* e = new Esfera(20, 20, false, true);

    agregar(MAT_Traslacion(-1, -0.5, 0));
    indice_hombro = agregar(MAT_Ident());   // Articulación hombro
    agregar(MAT_Rotacion(75, 0, 0, 1));
    agregar(MAT_Escalado(0.25, 1, 0.25));
    agregar(e);
    agregar(c);
    agregar(MAT_Traslacion(0, 1, 0));
    agregar(c);
    agregar(MAT_Traslacion(0, 1, 0));
    indice_codo = agregar(MAT_Ident());     // Articulación codo
    agregar(e);
    agregar(c);
    agregar(MAT_Traslacion(0, 1, 0));
    agregar(c);
    agregar(MAT_Traslacion(0, 1, 0));
    agregar(c);
}

// Igual que el brazo1 solo que simetrico respecto del plano YOZ
Muneco::Brazo2::Brazo2(){
    Cilindro* c = new Cilindro(2, 20, true, true);
    Esfera* e = new Esfera(20, 20, false, true);

    agregar(MAT_Escalado(-1,1,1));          // Simetría
    agregar(MAT_Traslacion(-1, -0.5, 0));
    indice_hombro = agregar(MAT_Ident());   // Articulación hombro
    agregar(MAT_Rotacion(75, 0, 0, 1));
    agregar(MAT_Escalado(0.25, 1, 0.25));
    agregar(e);
    agregar(c);
    agregar(MAT_Traslacion(0, 1, 0));
    agregar(c);
    agregar(MAT_Traslacion(0, 1, 0));
    indice_codo = agregar(MAT_Ident());     // Articulación codo
    agregar(e);
    agregar(c);
    agregar(MAT_Traslacion(0, 1, 0));
    agregar(c);
    agregar(MAT_Traslacion(0, 1, 0));
    agregar(c);
}

Matriz4f* Muneco::Brazo1::getArticulacionHombro(){
    return leerPtrMatriz(indice_hombro);
}

Matriz4f* Muneco::Brazo1::getArticulacionCodo(){
    return leerPtrMatriz(indice_codo);
}

Matriz4f* Muneco::Brazo2::getArticulacionHombro(){
    return leerPtrMatriz(indice_hombro);
}

Matriz4f* Muneco::Brazo2::getArticulacionCodo(){
    return leerPtrMatriz(indice_codo);
}
