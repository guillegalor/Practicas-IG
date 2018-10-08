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
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D
{

    protected:
        // Tablas para modo inmediato
        std::vector<Tupla3f> tabla_verts;
        std::vector<Tupla3i> tabla_caras;

        // IDs de los VBO de vertices y caras respectivamente
        GLuint id_vbo_ver, id_vbo_tri;

        // Tamaño en bytes de las tablas
        unsigned tam_ver, tam_tri;

        // Tabla de normales y de colores
        std::vector<Tupla3f> nor_tri;
        std::vector<Tupla3f> nor_ver;
        std::vector<Tupla3f> col_tri;
        std::vector<Tupla3f> col_ver;

        // calculo de las normales de esta malla
        void calcular_normales();

        // crear VBO de caras y VBO de vertices
        void crearVBOs();
        // visualizar con 'draw elements', en modo inmediato
        void visualizarDE_MI( ContextoVis & cv );
        // visualizar con 'draw elements', en modo diferido (con VBOS)
        void visualizarDE_VBOs( ContextoVis & cv );

    public:
        // crea una malla vacía (nombre: "malla indexada nueva vacía")
        MallaInd() ;
        // crea una malla vacía con un nombre concreto:
        MallaInd( const std::string & nombreIni );
        // visualizar el objeto con OpenGL
        virtual void visualizarGL( ContextoVis & cv ) ;
} ;
// ---------------------------------------------------------------------

class Cubo : public MallaInd
{
    public:
        Cubo ();
        Cubo (Tupla3f main_corner, float edge_size);
};
// ---------------------------------------------------------------------

class Tetraedro : public MallaInd
{
    public:
        Tetraedro ();
        Tetraedro (Tupla3f center, float edge_size);
};

#endif
