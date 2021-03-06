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
#include "materiales.hpp"

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

    // Guardamos el material activo anterior
    cv.pilaMateriales.push();

    for (unsigned i = 0; i < entradas.size(); i++)
        switch (entradas[i].tipo) {
            case TipoEntNGE::objeto:
                if (cv.modoSeleccionFBO && leerIdentificador() >= 0) {
                    FijarColorIdent(leerIdentificador());
                }
                entradas[i].objeto->visualizarGL (cv);
                break;
            case TipoEntNGE::transformacion:
                glMatrixMode (GL_MODELVIEW);
                glMultMatrixf (*(entradas[i].matriz));
                break;
            case TipoEntNGE::material:
                if(!cv.modoSeleccionFBO)
                    cv.pilaMateriales.activarMaterial(entradas[i].material);
                break;

            default:
                std::cout << "Tipo no conocido" << std::endl;
        }

    // Restauramos el material activo anterior y lo activamos (pop lo hace todo)
    cv.pilaMateriales.pop();

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
    for(auto e : entradas){
        if(e.tipo == TipoEntNGE::objeto)
            e.objeto->fijarColorNodo(nuevo_color);
    }
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

    // DONE: práctica 5: calcular y guardar el centro del nodo
    //    en coordenadas de objeto (hay que hacerlo recursivamente)
    //   (si el centro ya ha sido calculado, no volver a hacerlo)
    // ........

    Matriz4f cumulative_transformation_m = MAT_Ident();
    std::vector<Tupla3f> centros;

    if (!centro_calculado){
        for (auto entrada : entradas){
            switch(entrada.tipo) {
                case TipoEntNGE::objeto:
                    centros.push_back(cumulative_transformation_m * entrada.objeto->leerCentroOC());
                    break;
                case TipoEntNGE::transformacion:
                    cumulative_transformation_m = cumulative_transformation_m * (*entrada.matriz);
                    break;
                default:
                    break;
            }
        }
    }

    ponerCentroOC(calcularCentroCajaEnglobante(centros));

    centro_calculado = true;
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
    // DONE: práctica 5: buscar un sub-objeto con un identificador
    // ........

    if (ident_busc == leerIdentificador()){
        *objeto = this;
        centro_wc = mmodelado*leerCentroOC();
        return true;
    }

    Matriz4f nueva_mmodelado = mmodelado;

    // TEST
    unsigned counter = 0;

    for (auto &entrada: entradas){
        switch(entrada.tipo) {
            case TipoEntNGE::objeto:
                if (entrada.objeto->buscarObjeto(ident_busc, nueva_mmodelado, objeto, centro_wc)){
                    return true;
                }
                break;
            case TipoEntNGE::transformacion:
                nueva_mmodelado = nueva_mmodelado * (*entrada.matriz);
                break;
            default:
                break;
        }
    }

    return false;

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
    Tupla3f color = {(float)229/255, (float)235/255, (float)25/255};
    Cilindro* c = new Cilindro(
            2,
            20,
            true,
            true,
            false
            );
    Esfera* e = new Esfera(
            20,
            20,
            false,
            true,
            false
            );
    Cola* cola = new Cola();
    Brazo* br1 = new Brazo;
    Brazo* br2 = new Brazo;
    Ojo* ojo = new Ojo();

    //Fijar Color
    c->fijarColorNodo(color);
    e->fijarColorNodo(color);
    cola->fijarColorNodo(color);
    br1->fijarColorNodo(color);
    br2->fijarColorNodo(color);

    /* int i_rot_principal = agregar(MAT_Ident()); */
    agregar(MAT_Escalado(1,2,1));

    // Primer cilindro del cuerpo
    agregar(c);

    agregar(MAT_Traslacion(0,1,0));

    // Cola
    agregar(MAT_Escalado(1, 1.0/2, 1));
    agregar(cola);
    agregar(MAT_Escalado(1, 2, 1));

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

    agregar(MAT_Traslacion(0, 1.5, 0));
    // Brazo izq
    agregar(br1);

    // Brazo der
    agregar(MAT_Escalado(-1, 1, 1));
    agregar(br2);
    agregar(MAT_Escalado(-1, 1, 1));
    agregar(MAT_Traslacion(0, -0.5, 0));

    // Esfera de articulación y rotación
    agregar(e);
    int i_rotacion3 = agregar(MAT_Ident());

    // Cilindro cabeza
    agregar(c);

    // Ojos
    agregar(MAT_Traslacion(-0.3, 0.7, 1));
    agregar(MAT_Escalado(1, 0.5, 1));
    agregar(ojo);
    agregar(MAT_Traslacion(0.6, 0, 0));
    agregar(ojo);
    agregar(MAT_Escalado(1, 2, 1));
    agregar(MAT_Traslacion(-0.3, -0.7, -1));

    //Rotación 1
    Parametro rotacion1(
            "Rotacion primera articulación del cuerpo",
            leerPtrMatriz(i_rotacion1),
            [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
            true,
            0,
            10,
            0.1
            );

    //Rotación 2
    Parametro rotacion2(
            "Rotacion segunda articulación del cuerpo",
            leerPtrMatriz(i_rotacion2),
            [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
            true,
            0,
            10.0,
            0.1
            );

    //Rotación 3
    Parametro rotacion3(
            "Rotacion tercera articulación del cuerpo",
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

    for (int i = 0; i < cola->numArticulaciones(); ++i) {
        Parametro movimiento_cola(
                "Movimiento " + std::to_string(i) + "a articulación de la cola",
                cola->getArticulacion(i),
                [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
                true,
                0,
                45,
                0.1
                );
        parametros.push_back(movimiento_cola);
    }

    /* parametros.push_back(rot_principal); */
    parametros.push_back(rotacion1);
    parametros.push_back(rotacion2);
    parametros.push_back(rotacion3);
    parametros.push_back(rotacion_hombro1);
    parametros.push_back(rotacion_hombro2);
    parametros.push_back(rotacion_codo1);
    parametros.push_back(rotacion_codo2);
}

Muneco::Brazo::Brazo(){
    Cilindro* c = new Cilindro(2, 20, true, true, false);
    Esfera* e = new Esfera(20, 20, false, true, false);

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

Matriz4f* Muneco::Brazo::getArticulacionHombro(){
    return leerPtrMatriz(indice_hombro);
}

Matriz4f* Muneco::Brazo::getArticulacionCodo(){
    return leerPtrMatriz(indice_codo);
}

Muneco::Cola::Cola(){
    Cilindroide* c = new Cilindroide(0.8, 2, 20, true, true, false);
    Esfera* e = new Esfera(20, 20, true, true, false);
    Matriz4f escalado = MAT_Escalado(0.8, 0.8, 0.8);

    agregar(MAT_Rotacion(-90, 1, 0, 0));

    /* agregar(MAT_Escalado(0.6, 1,0.6)); */
    agregar(c);

    agregar(MAT_Traslacion(0, 1, 0));
    agregar(escalado);
    ind.push_back(agregar(MAT_Ident()));
    agregar(e);
    agregar(c);

    agregar(MAT_Traslacion(0, 1, 0));
    agregar(escalado);
    ind.push_back(agregar(MAT_Ident()));
    agregar(e);
    agregar(c);

    agregar(MAT_Traslacion(0, 1, 0));
    agregar(escalado);
    ind.push_back(agregar(MAT_Ident()));
    agregar(e);
    agregar(c);

    agregar(MAT_Traslacion(0, 1, 0));
    agregar(escalado);
    ind.push_back(agregar(MAT_Ident()));
    agregar(e);
    agregar(c);

    agregar(MAT_Traslacion(0, 1, 0));
    agregar(escalado);
    ind.push_back(agregar(MAT_Ident()));
    agregar(e);
    agregar(c);

    agregar(MAT_Traslacion(0, 1, 0));
    agregar(escalado);
    ind.push_back(agregar(MAT_Ident()));
    agregar(e);
    agregar(c);

    agregar(MAT_Traslacion(0, 1, 0));
    agregar(escalado);
    ind.push_back(agregar(MAT_Ident()));
    agregar(e);
    agregar(c);

    agregar(MAT_Traslacion(0, 1, 0));
    agregar(escalado);
    agregar(e);
}

Matriz4f* Muneco::Cola::getArticulacion(int i){
    return leerPtrMatriz(ind[i]);
}

unsigned Muneco::Cola::numArticulaciones(){
    return ind.size();
}

Muneco::Ojo::Ojo(){
    Esfera* e1 = new Esfera(20, 20, true, true, false);    // Esfera de fuera
    Esfera* e2 = new Esfera(20, 20, true, true, false);    // Pupila
    e1->fijarColorNodo({1,1,1});
    e2->fijarColorNodo({0,0,0});

    agregar(MAT_Escalado(0.2, 0.2, 0.15));

    agregar(e1);
    agregar(MAT_Traslacion(-0.2, -0.2, 0.3));
    agregar(MAT_Escalado(0.7, 0.7, 1));
    agregar(e2);
}

Lata::Lata(){
    ponerIdentificador(1);
    ponerNombre("Lata");
    agregar(new TapaArriba());
    agregar(new Cuerpo());
    agregar(new TapaAbajo());
}

Lata::Cuerpo::Cuerpo(){
    ponerIdentificador(-1);
    ponerNombre("Cuerpo lata");
    agregar(new MaterialLata());
    agregar(new MallaRevol("../plys/lata-pcue.ply", 50, false ,false, true));
}

Lata::TapaArriba::TapaArriba(){
    ponerIdentificador(-1);
    ponerNombre("Tapa arriba lata");
    agregar(new MaterialTapasLata());
    agregar(new MallaRevol("../plys/lata-psup.ply", 50, true, false, true));
}

Lata::TapaAbajo::TapaAbajo(){
    ponerIdentificador(-1);
    ponerNombre("Tapa abajo lata");
    agregar(new MaterialTapasLata());
    agregar(new MallaRevol("../plys/lata-pinf.ply", 50, true, false, true));
}

PeonMadera::PeonMadera(){
    ponerIdentificador(2);
    ponerNombre("Peon madera");
    agregar(new MaterialPeonMadera());
    agregar(new MallaRevol("../plys/peon.ply", 50, true, false, true));
}

PeonBlanco::PeonBlanco(){
    ponerIdentificador(3);
    ponerNombre("Peon blanco");
    agregar(new MaterialPeonBlanco());
    agregar(new MallaRevol("../plys/peon.ply", 50, true, false, true));
}

PeonNegro::PeonNegro(){
    ponerIdentificador(4);
    ponerNombre("Peon negro");
    agregar(new MaterialPeonNegro());
    agregar(new MallaRevol("../plys/peon.ply", 50, true, false, true));
}

EscenaP4::EscenaP4(){
    agregar(new Lata());

    agregar(MAT_Escalado((float) 1/7,(float) 1/7,(float) 1/7));
    agregar(MAT_Traslacion(0, 1, 12));
    agregar(new Dado);
    agregar(MAT_Traslacion(0, -1,-12));
    agregar(MAT_Escalado((float) 7,(float) 7,(float) 7));

    agregar(MAT_Traslacion(0, 0.25, 1));

    agregar(MAT_Escalado((float) 1/5,(float) 1/5,(float) 1/5));
    agregar(new PeonMadera);
    agregar(MAT_Traslacion(3, 0, 0));
    agregar(new PeonBlanco);
    agregar(MAT_Traslacion(3, 0, 0));
    agregar(new PeonNegro);
}

//---------------------------------------------------------------------------------
// EJERCICIOS RELACIÓN
// 33-.

void tronco(){
    std::vector<Tupla3f> vertices;
    std::vector<Tupla3i> caras;

    std::vector<int> linea1;
    std::vector<int> linea2;
    std::vector<int> linea3;

    // Inicialización de vértices
    vertices.push_back({0, 0, 0});
    vertices.push_back({1, 0, 0});
    vertices.push_back({1, 1, 0});
    vertices.push_back({2, 2, 0});
    vertices.push_back({1.5, 2.5, 0});
    vertices.push_back({0.5, 1.5, 0});
    vertices.push_back({0, 3, 0});
    vertices.push_back({-0.5, 3, 0});
    vertices.push_back({0, 1.5, 0});

    caras.push_back({0, 1, 2});
    caras.push_back({0, 2, 8});
    caras.push_back({2, 5, 8});
    caras.push_back({2, 4, 5});
    caras.push_back({2, 3, 4});
    caras.push_back({5, 6, 8});
    caras.push_back({6, 7, 8});

    linea1.push_back(1);
    linea1.push_back(2);
    linea1.push_back(3);

    linea2.push_back(4);
    linea2.push_back(5);
    linea2.push_back(6);

    linea3.push_back(7);
    linea3.push_back(8);
    linea3.push_back(0);

    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

    glEnableClientState( GL_VERTEX_ARRAY ); // habilitar array de vértices
    glVertexPointer( 3, GL_FLOAT, 0, vertices.data() ); // establecer dirección y estructura

    glColor3f(0.5, 0.5, 1); // Azul clarito(?)

    glDrawElements( GL_TRIANGLES, caras.size()*3L, GL_UNSIGNED_INT, caras.data() );

    glColor3f(0.0, 0.0, 1); // Azul
    glLineWidth(3.0);
    glDrawElements( GL_LINE_STRIP, linea1.size(), GL_UNSIGNED_INT, linea1.data() );
    glDrawElements( GL_LINE_STRIP, linea2.size(), GL_UNSIGNED_INT, linea2.data() );
    glDrawElements( GL_LINE_STRIP, linea3.size(), GL_UNSIGNED_INT, linea3.data() );

    glDisableClientState( GL_VERTEX_ARRAY );

}

void arbol(unsigned niveles){
    if (niveles > 0){
        tronco();

        glPushMatrix();
        glTranslatef(1.5, 2.5, 0);
        glRotatef(-45.0, 0.0, 0.0, 1.0);
        glScalef(sqrt(0.5), sqrt(0.5), sqrt(0.5));
        arbol(niveles-1);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.5, 3, 0);
        glScalef(0.5, 0.5, 0.5);
        arbol(niveles-1);
        glPopMatrix();
    }
}

//Examen Grado
Test::Test(Material* m){
    agregar(m);
    /* agregar(new MaterialEsmeralda); */
    agregar(new Esfera(20, 20, true, false, true));
}

// Examen Prácticas Año Pasado
Moneda::Moneda(){
    agregar(new CaraSuperior);
    agregar(new Borde);
    agregar(new CaraInferior);
}

CaraSuperior::CaraSuperior(){
    agregar(MAT_Traslacion(0.0, 0.05, 0));
    agregar(new MaterialCaras());
    agregar(new Disco(50));
}

CaraInferior::CaraInferior(){
    agregar(MAT_Rotacion(180, 0, 0, 1));
    agregar(new MaterialCaras());
    Disco * d = new Disco(50);
    d->invertirCoordText(true, false);
    agregar(d);
}

Borde::Borde(){
    agregar(MAT_Escalado(1, 0.05, 1));
    agregar(new MaterialBorde());
    agregar(new Cilindro(2, 50, false, false, true));
}

// Examen fotocopia
CuboColores::CuboColores(){
    agregar(new MaterialDado());
    agregar(new CaraArriba());
    agregar(new CaraAbajo());
    agregar(new CaraIzq());
    agregar(new CaraDer());
    agregar(new CaraFrente());
    agregar(new CaraAtras());

}

// EXAMEN P4

Dado::Dado(){
    ponerIdentificador(5);
    agregar(new MaterialDadoP4);
    agregar(new DadoP4());
}

// Examen Prácticas
Arbusto::Rama::Rama(){
    agregar(MAT_Ident());

    agregar(new Esfera(20, 20, true, true, false));
    agregar(MAT_Rotacion(-90, 0, 0, 1));
    agregar(MAT_Escalado(0.5, 4, 0.5));
    agregar(new Cilindro(2, 20, true, true, false));
    agregar(MAT_Escalado(2, 1.0/4, 2));
    agregar(MAT_Traslacion(0, 4, 0));
    agregar(new Esfera(20, 20, true, true, false));
}

Matriz4f* Arbusto::Rama::getArticulacion(){
    return leerPtrMatriz(0);
}

Arbusto::Rama1::Rama1(){
    agregar(MAT_Ident());
    articulaciones[0] = leerPtrMatriz(0);
    agregar(new Rama());

    agregar(MAT_Traslacion(4, 0, 0));
    Rama* r1 = new Rama();
    Rama* r2 = new Rama();
    articulaciones[1] = r1->getArticulacion();
    articulaciones[2] = r2->getArticulacion();
    agregar(r1);
    agregar(r2);
}

Matriz4f* Arbusto::Rama1::getArticulacion(unsigned i){
    return articulaciones[i];
}

Arbusto::Arbusto(){
    int i_rot_total;
    Rama1* r11 = new Rama1();
    Rama1* r12 = new Rama1();

    i_rot_total = agregar(MAT_Ident());
    agregar(new Rama());
    agregar(MAT_Traslacion(4, 0, 0));
    agregar(r11);
    agregar(r12);

    Parametro rot1(
            "Rotación total",
            leerPtrMatriz(i_rot_total),
            [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
            true,
            22,
            22,
            0.1
    );

    Parametro rot2(
            "Rotación total",
            r11->getArticulacion(0),
            [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
            true,
            22,
            22,
            0.1
    );

    Parametro rot3(
            "Rotación total",
            r11->getArticulacion(1),
            [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
            true,
            22,
            22,
            0.1
    );

    Parametro rot4(
            "Rotación total",
            r11->getArticulacion(2),
            [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
            true,
            67,
            22,
            0.1
    );

    Parametro rot5(
            "Rotación total",
            r12->getArticulacion(0),
            [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
            true,
            67,
            22,
            0.1
    );

    Parametro rot6(
            "Rotación total",
            r12->getArticulacion(1),
            [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
            true,
            22,
            22,
            0.1
    );

    Parametro rot7(
            "Rotación total",
            r12->getArticulacion(2),
            [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
            true,
            67,
            22,
            0.1
    );

    parametros.push_back(rot1);
    parametros.push_back(rot2);
    parametros.push_back(rot3);
    parametros.push_back(rot4);
    parametros.push_back(rot5);
    parametros.push_back(rot6);
    parametros.push_back(rot7);
}

CamaraRotante::CamaraRotante(){
    unsigned indices[4];

    indices[0] = agregar(MAT_Ident());
    agregar(MAT_Traslacion(5, 0 , 0));

    agregar(MAT_Escalado(0.3, 2.5, 0.3));
    agregar(new Cilindro(2, 20, true, true, true));
    agregar(MAT_Escalado(1.0/0.3, 1.0/2.5, 1.0/0.3));

    agregar(MAT_Traslacion(0, 2.5, 0));
    indices[1] = agregar(MAT_Ident());
    indices[2] = agregar(MAT_Ident());
    agregar(new Objetivo());

    Parametro traslacion_objetivo(
            "Traslación objetivo",
            leerPtrMatriz(indices[1]),
            [=] (float v) {return MAT_Traslacion(0, v, 0);},
            true,
            0,
            0.5,
            0.1
    );

    Parametro rotacion_objetivo(
            "Rotación objetivo",
            leerPtrMatriz(indices[2]),
            [=] (float v) {return MAT_Rotacion(v, 0, 0, 1);},
            true,
            0,
            30,
            0.1
    );

    parametros.push_back(traslacion_objetivo);
    parametros.push_back(rotacion_objetivo);
};

CamaraRotante::Objetivo::Objetivo(){
    agregar(MAT_Escalado(1.5, 1, 1));
    agregar(new Cubo());
    agregar(MAT_Escalado(1.0/1.5, 1, 1));

    agregar(MAT_Traslacion(-1.25, 0, 0));
    agregar(MAT_Escalado(1.5, 0.25, 0.25));
    agregar(MAT_Rotacion(-90, 0, 0, 1));
    agregar(new Cono(2, 20, true, true, true));
}
