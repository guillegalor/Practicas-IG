// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp)
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include "MallaInd.hpp"   // declaración de 'ContextoVis'
#include "matrices-tr.hpp"

// *****************************************************************************
// funciones auxiliares

GLuint VBO_Crear (GLuint tipo, GLuint tamanio, GLvoid *puntero)
{
    assert (tipo == GL_ARRAY_BUFFER || tipo == GL_ELEMENT_ARRAY_BUFFER);
    GLuint id_vbo;
    glGenBuffers (1, &id_vbo);
    glBindBuffer (tipo,id_vbo);
    glBufferData (tipo, tamanio, puntero, GL_STATIC_DRAW);

    glBindBuffer (tipo, 0);
    return id_vbo;
}

Tupla3f calcularCentroCajaEnglobante(std::vector<Tupla3f> puntos){
    Tupla3f maximos = puntos[0],
            minimos = puntos[0];

    for( auto p : puntos){
        maximos[0] = std::max(p[0], maximos[0]);
        maximos[1] = std::max(p[1], maximos[1]);
        maximos[2] = std::max(p[2], maximos[2]);

        minimos[0] = std::min(p[0], minimos[0]);
        minimos[1] = std::min(p[1], minimos[1]);
        minimos[2] = std::min(p[2], minimos[2]);

    }

    return (maximos + minimos) / 2.0;
}

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd(): MallaInd("malla indexada, anónima")
{}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
    ponerNombre(nombreIni) ;
    id_vbo_ver = id_vbo_tri = 0;

}
// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcular_normales()
{
    for (auto i: tabla_verts){
        nor_ver.push_back({0, 0, 0});
    }

    for (auto cara: tabla_caras){
        Tupla3f a, b, n;
        int v0, v1, v2;
        v0 = cara(0);
        v1 = cara(1);
        v2 = cara(2);

        // Calcular normales de las caras ((v1-v0)x(v2-vo))||(v1-v0)x(v2-v0)||
        a = tabla_verts[v1] - tabla_verts[v0];
        b = tabla_verts[v2] - tabla_verts[v0];

        n = a.cross(b).normalized();

        // Calcular normales de los vértices sum(normales adyancentes a tu vértice)||sum(normales adyancentes a tu vértice)||
        nor_ver[v0] = (nor_ver[v0] + n);
        nor_ver[v1] = (nor_ver[v1] + n);
        nor_ver[v2] = (nor_ver[v2] + n);

        nor_tri.push_back(n);
    }

    for (auto& normal : nor_ver){
        normal = normal.normalized();
    }

}
// -----------------------------------------------------------------------------
void MallaInd::crearVBOs()
{
    tam_ver = sizeof(float)*3L*tabla_verts.size();
    tam_tri = sizeof(unsigned)*3L*tabla_caras.size();
    tam_tex = sizeof(float)*2L*tabla_text.size();

    if (id_vbo_ver == 0)
        // crear VBO vértices
        id_vbo_ver = VBO_Crear (GL_ARRAY_BUFFER, tam_ver, tabla_verts.data());

    if (id_vbo_tri == 0)
        // crear VBO caras
        id_vbo_tri = VBO_Crear (GL_ELEMENT_ARRAY_BUFFER, tam_tri, tabla_caras.data());

    // crear VBO con los colores de los vértices
    if ( col_ver.size() > 0 )
        id_vbo_col_ver = VBO_Crear (GL_ARRAY_BUFFER, tam_ver, col_ver.data() );

    // crear VBO con las normales de los vertices
    if (nor_ver.size() > 0)
        id_vbo_nor_ver = VBO_Crear (GL_ARRAY_BUFFER, tam_ver, nor_ver.data());

    // crear VBO con las coordenadas de textura
    if (tabla_text.size() > 0)
        id_vbo_tex = VBO_Crear (GL_ARRAY_BUFFER, tam_tex, tabla_text.data());

}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI( ContextoVis & cv ){

    if( col_ver.size() > 0 && ! cv.modoSeleccionFBO){ // si hay colores de v. disponibles:
        std::cout << "mandando colores" << std::endl;
        glEnableClientState( GL_COLOR_ARRAY );
        // habilitar colores
        glColorPointer( 3, GL_FLOAT, 0, col_ver.data() ); // fija puntero a colores
    }

    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, tabla_verts.data() );

    // Hace falta multiplicar tabla_caras*3 porque es un vector de 3-uplas
    glDrawElements( GL_TRIANGLES, tabla_caras.size()*3L, GL_UNSIGNED_INT, tabla_caras.data() );

    // deshabilitar arrays
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );

    if (cv.modoSeleccionFBO)
        std::cout << "Saliendo del visualizar modo seleccion" << std::endl;
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_NT (ContextoVis & cv) // visu. con normales y cc.tt.
{
    glVertexPointer (3, GL_FLOAT, 0, tabla_verts.data());
    glTexCoordPointer (2, GL_FLOAT, 0, tabla_text.data());
    glNormalPointer (GL_FLOAT, 0, nor_ver.data() );

    glEnableClientState (GL_VERTEX_ARRAY );
    glEnableClientState (GL_NORMAL_ARRAY );
    glEnableClientState (GL_TEXTURE_COORD_ARRAY );

    glDrawElements (GL_TRIANGLES, tabla_caras.size()*3, GL_UNSIGNED_INT, tabla_caras.data());

    glDisableClientState (GL_VERTEX_ARRAY );
    glDisableClientState (GL_NORMAL_ARRAY );
    glDisableClientState (GL_TEXTURE_COORD_ARRAY );
}

// ----------------------------------------------------------------------------
void MallaInd::visualizarDE_VBOs( ContextoVis & cv )
{
    crearVBOs();

    if(col_ver.size() > 0){
        glBindBuffer (GL_ARRAY_BUFFER, id_vbo_col_ver); //Activar el vbo con los colores de los vertices
        glColorPointer (3, GL_FLOAT, 0, 0);             //Formato y obset de colores
        glEnableClientState (GL_COLOR_ARRAY);           //Activa el uso de colores de vertices
    }

    glBindBuffer (GL_ARRAY_BUFFER, id_vbo_ver);
    glVertexPointer (3, GL_FLOAT, 0, 0);
    glBindBuffer (GL_ARRAY_BUFFER, 0);
    glEnableClientState (GL_VERTEX_ARRAY);

    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri);
    glDrawElements (GL_TRIANGLES, 3L*tabla_caras.size(), GL_UNSIGNED_INT, NULL);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableClientState (GL_VERTEX_ARRAY);
    glDisableClientState (GL_COLOR_ARRAY);
}

// -----------------------------------------------------------------------------
void MallaInd::visualizarVBOs_NT (ContextoVis & cv) // vis. normales y cc.tt. en VBOs
{
    crearVBOs();

    // activar VBO de coordenadas de normales
    glBindBuffer( GL_ARRAY_BUFFER, id_vbo_nor_ver );
    glNormalPointer( GL_FLOAT, 0, 0 );

    // (0 == offset en vbo)
    glEnableClientState( GL_NORMAL_ARRAY );

    // activar VBO de coordenadas de textura
    glBindBuffer( GL_ARRAY_BUFFER, id_vbo_tex );
    glTexCoordPointer( 2, GL_FLOAT, 0, 0 ); // (0 == offset en vbo)
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    // visualizar (el mismo método ya visto)
    visualizarDE_VBOs(cv);

    // desactivar punteros a tablas
    glDisableClientState( GL_NORMAL_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}

// -----------------------------------------------------------------------------
void MallaInd::visualizarGL( ContextoVis & cv )
{
    // Establecer modo indicado en el contexto
    GLenum mode;

    // Establecer sombra
    GLenum shade;

    // Practica 5: Parámetros para modo selección
    if (cv.modoSeleccionFBO){
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glShadeModel(GL_FLAT);

        visualizarDE_MI (cv);

        return;
    }

    else
        switch (cv.modoVis){
            default:
            case modoSolido:
                mode = GL_FILL;
                break;
            case modoPuntos:
                mode = GL_POINT;
                break;
            case modoAlambre:
                mode = GL_LINE;
                break;
            case modoIluminacionPlano:
                mode = GL_FILL;
                shade = GL_FLAT;
                break;
            case modoIluminacionSuave:
                mode = GL_FILL;
                shade = GL_SMOOTH;
                break;
        }

    glPolygonMode (GL_FRONT_AND_BACK, mode);

    if (cv.modoVis < 3)
        if (cv.usarVBOs)
            visualizarDE_VBOs (cv);
        else
            visualizarDE_MI (cv);
    else {
        glShadeModel(shade);

        if (cv.usarVBOs)
            visualizarVBOs_NT (cv);
        else
            visualizarDE_NT(cv);
    }
}

// -----------------------------------------------------------------------------
void MallaInd::fijarColorNodo(const Tupla3f& color){
    for (int i = 0; i < tabla_verts.size(); ++i) {
        col_ver.push_back(color);
    }
}

// -----------------------------------------------------------------------------
void MallaInd::calcularCentroOC(){
    Tupla3f centro = calcularCentroCajaEnglobante(tabla_verts);

    ponerCentroOC(centro);

    centro_calculado = true;
}

// -----------------------------------------------------------------------------
// *****************************************************************************

Cubo::Cubo()
    :  MallaInd ("malla cubo")
{
    // Inicialización de vertices del cubo
    unsigned i,j,k;

    for (i = 0; i < 2; ++i) {
        for (j = 0; j < 2; ++j) {
            for (k = 0; k < 2; ++k) {
                tabla_verts.push_back (Tupla3f (
                            i,
                            j,
                            k)
                        );
            }
        }
    }

    // Inicialización de caras del cubo
    tabla_caras = {
        Tupla3i (0,1,3),
        Tupla3i (2,0,3),
        Tupla3i (1,0,5),
        Tupla3i (0,4,5),
        Tupla3i (0,2,6),
        Tupla3i (4,0,6),
        Tupla3i (1,7,3),
        Tupla3i (7,2,3),
        Tupla3i (7,1,5),
        Tupla3i (7,5,4),
        Tupla3i (7,6,2),
        Tupla3i (7,4,6),
    };
}

// *****************************************************************************

Tetraedro::Tetraedro()
    :  MallaInd( "malla tetraedro")
{
    tabla_verts = {
        Tupla3f ((float)1/2, 0, (float)-1/sqrt(2)/2),
        Tupla3f ((float)-1/2, 0,(float)-1/sqrt(2)/2),
        Tupla3f (0, (float)1/2, (float)1/sqrt(2)/2),
        Tupla3f (0, (float)-1/2, (float)1/sqrt(2)/2)
    };

    tabla_caras = {
        Tupla3i (0,1,2),
        Tupla3i (1,0,3),
        Tupla3i (0,2,3),
        Tupla3i (1,3,2)
    };
}

// *****************************************************************************

//-----------------------------------------------------------------------
// EJERCICIOS

// Examen Prácticas Año Pasado
Disco::Disco(unsigned nper){
    tabla_verts.push_back({0, 0, 0});
    tabla_text.push_back({0.5, 0.5});
    nor_ver.push_back ({0, 1, 0});

    Tupla3f vert;

    for (int i = 0; i < nper; ++i) {
        vert = MAT_Rotacion((float)(360*i)/(nper-1), 0, 1, 0)* Tupla3f({0,0,1});
        tabla_verts.push_back (vert);

        // Crear texturas
        tabla_text.push_back ({(vert[0]+1)/2, (vert[2]+1)/2});
        // Crear normales
        nor_ver.push_back ({0, 1, 0});
    }

    // Crear caras
    for (int i = 0; i < nper-1; ++i) {
        tabla_caras.push_back ({i+1, i+2, 0});
    }

}

void Disco::invertirCoordText(bool s, bool t){
    for (auto& text : tabla_text){
        text[0] = (s ? 1 - text[0] : text[0]);
        text[1] = (t ? 1 - text[1] : text[1]);
    }
}

CaraArriba::CaraArriba(){
    // Añadimos vértices
    tabla_verts.push_back({0,1,0});
    tabla_verts.push_back({0,1,1});
    tabla_verts.push_back({1,1,0});
    tabla_verts.push_back({1,1,1});

    // Añadimos caras
    tabla_caras.push_back({2, 0, 3});
    tabla_caras.push_back({3, 0, 1});

    // Añadimos coordenadas de textura
    tabla_text.push_back({0.5, (float)1/3});
    tabla_text.push_back({0.5, (float)2/3});
    tabla_text.push_back({0.75, (float)1/3});
    tabla_text.push_back({0.75, (float)2/3});

    // Calcular normales
    calcular_normales();
}

CaraAbajo::CaraAbajo(){
    // Añadimos vértices
    tabla_verts.push_back({0,0,0});
    tabla_verts.push_back({0,0,1});
    tabla_verts.push_back({1,0,0});
    tabla_verts.push_back({1,0,1});

    // Añadimos caras
    tabla_caras.push_back({0, 2, 3});
    tabla_caras.push_back({0, 3, 1});

    // Añadimos coordenadas de textura
    tabla_text.push_back({0.25, (float)1/3});
    tabla_text.push_back({0.25, (float)2/3});
    tabla_text.push_back({0, (float)1/3});
    tabla_text.push_back({0, (float)2/3});

    // Calcular normales
    calcular_normales();
}

CaraIzq::CaraIzq(){
    // Añadimos vértices
    tabla_verts.push_back({0,0,0});
    tabla_verts.push_back({0,0,1});
    tabla_verts.push_back({0,1,0});
    tabla_verts.push_back({0,1,1});

    // Añadimos caras
    tabla_caras.push_back({0, 1, 3});
    tabla_caras.push_back({0, 3, 2});

    // Añadimos coordenadas de textura
    tabla_text.push_back({0.25, (float)1/3});
    tabla_text.push_back({0.25, (float)2/3});
    tabla_text.push_back({0.5, (float)1/3});
    tabla_text.push_back({0.5, (float)2/3});

    // Calcular normales
    calcular_normales();
}

CaraDer::CaraDer(){
    // Añadimos vértices
    tabla_verts.push_back({1,0,0});
    tabla_verts.push_back({1,0,1});
    tabla_verts.push_back({1,1,0});
    tabla_verts.push_back({1,1,1});

    // Añadimos caras
    tabla_caras.push_back({1, 0, 3});
    tabla_caras.push_back({3, 0, 2});

    // Añadimos coordenadas de textura
    tabla_text.push_back({1, (float)1/3});
    tabla_text.push_back({1, (float)2/3});
    tabla_text.push_back({0.75, (float)1/3});
    tabla_text.push_back({0.75, (float)2/3});

    // Calcular normales
    calcular_normales();
}

CaraFrente::CaraFrente(){
    // Añadimos vértices
    tabla_verts.push_back({0,0,1});
    tabla_verts.push_back({0,1,1});
    tabla_verts.push_back({1,0,1});
    tabla_verts.push_back({1,1,1});

    // Añadimos caras
    tabla_caras.push_back({0, 2, 3});
    tabla_caras.push_back({0, 3, 1});

    // Añadimos coordenadas de textura
    tabla_text.push_back({0.5, 1});
    tabla_text.push_back({0.5, (float)2/3});
    tabla_text.push_back({0.75, 1});
    tabla_text.push_back({0.75, (float)2/3});

    // Calcular normales
    calcular_normales();
}

CaraAtras::CaraAtras(){
    // Añadimos vértices
    tabla_verts.push_back({0,0,0});
    tabla_verts.push_back({0,1,0});
    tabla_verts.push_back({1,0,0});
    tabla_verts.push_back({1,1,0});

    // Añadimos caras
    tabla_caras.push_back({2, 0, 3});
    tabla_caras.push_back({3, 0, 1});

    // Añadimos coordenadas de textura
    tabla_text.push_back({0.5, 0});
    tabla_text.push_back({0.5, (float)1/3});
    tabla_text.push_back({0.75, 0});
    tabla_text.push_back({0.75, (float)1/3});

    // Calcular normales
    calcular_normales();
}
