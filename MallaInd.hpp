// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>          // usar std::vector
// Ejercicios Prácticas

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

        void visualizarDE_MI_Plano(ContextoVis & cv);

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

// Examen fotocopia
class CaraArriba : public MallaInd{
    public:
        CaraArriba();
};

class CaraAbajo : public MallaInd{
    public:
        CaraAbajo();
};

class CaraIzq : public MallaInd{
    public:
        CaraIzq();
};

class CaraDer : public MallaInd{
    public:
        CaraDer();
};

class CaraFrente : public MallaInd{
    public:
        CaraFrente();
};

class CaraAtras : public MallaInd{
    public:
        CaraAtras();
};

//-----------------------------------------------------------------------
// Ejercicio 1 de teoría del examen
void poligonoNLados(int n);

// Ejercicio 2 de teoría del examen
void poligonos3_M(int m);

// Ejercicio 3 de teoría del examen
std::vector<int> calculaAdyacencias(std::vector<Tupla3i> triangulos, int n);

// Ejercicio 4 de teoría del examen
void DibujaTriangulo();
void CilindroViewport(int d, int n, int f);

// Ejercicio 5 Relación
void cuadradoBlancoFondoAzul();

// Ejercicio 32 Relación
void figura_simple(int color);
void dibujarCuadradosRecursivo(int n, int color);

// Ejercicio Examen Grado 2016
void dibujarTriangulo(float xmin, float xmax, float ymin, float ymax, int color);
void dibujaTriRecursiva(unsigned n, float xmin, float xmax, float ymin, float ymax, int color);

// EXAMEN P4
class DadoP4 : public MallaInd{
    public:
        DadoP4();
};

#endif
