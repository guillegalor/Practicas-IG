// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>          // usar std::vector

#include "Objeto3D.hpp"   // declaración de 'Objeto3D'

// ---------------------------------------------------------------------
// Funciones auxiliares

// Función que devuelve el centro de la caja englobante de un vector de vértices
Tupla3f calcularCentroCajaEnglobante(std::vector<Tupla3f> puntos);

// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D
{

    protected:
        bool vbo_creado;

        // Tablas
        std::vector<Tupla3f> tabla_verts;   // Vértices
        std::vector<Tupla3i> tabla_caras;   // Caras
        std::vector<Tupla3f> nor_ver;       // Normales Vértices
        std::vector<Tupla3f> nor_tri;       // Normales Caras
        std::vector<Tupla3f> col_ver;       // Colores Vértices
        std::vector<Tupla3f> col_tri;       // Colores Caras
        std::vector<Tupla2f> tabla_text;    // Texturas

        // IDs de los VBO de vertices y caras respectivamente
        GLuint id_vbo_ver, id_vbo_tri, id_vbo_col_ver, id_vbo_tex, id_vbo_nor_ver;

        // Tamaño en bytes de las tablas
        unsigned tam_ver, tam_tri, tam_tex;

        // calculo de las normales de esta malla
        void calcular_normales();

        // crear VBO de caras y VBO de vertices
        void crearVBOs();
        // visualizar con 'draw elements', en modo inmediato
        void visualizarDE_MI (ContextoVis & cv);
        // visualizar con 'draw elements', en modo diferido (con VBOS)
        void visualizarDE_VBOs (ContextoVis & cv);

        void visualizarDE_NT (ContextoVis & cv); // vis. con normales y cc.tt.
        void visualizarVBOs_NT (ContextoVis & cv); // vis. normales y cc.tt. en VBOs

        void calcularCentroOC();

    public:
        // crea una malla vacía (nombre: "malla indexada nueva vacía")
        MallaInd() ;
        // crea una malla vacía con un nombre concreto:
        MallaInd( const std::string & nombreIni );
        // visualizar el objeto con OpenGL
        virtual void visualizarGL( ContextoVis & cv ) ;
        // fijar un color para la figura
        void fijarColorNodo(const Tupla3f& color);
} ;
// ---------------------------------------------------------------------

class Cubo : public MallaInd
{
    public:
        Cubo ();
};
// ---------------------------------------------------------------------

class Tetraedro : public MallaInd
{
    public:
        Tetraedro ();
};

//-----------------------------------------------------------------------
// EJERCICIOS

// Examen Prácticas Año Pasado
class Disco : public MallaInd
{
    public:
        Disco(unsigned nper);
        void invertirCoordText(bool s, bool t);
};
#endif
