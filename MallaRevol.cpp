// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaRevol'
// **
// *********************************************************************

const double PI = 3.1415926535897;

#include <cmath>
#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include <matrices-tr.hpp>
#include "MallaRevol.hpp"

using namespace std ;

// *****************************************************************************
// Funciones auxiliares

float norm(Tupla3f v){
    return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
};

// *****************************************************************************
MallaRevol::MallaRevol( const std::string & nombre)
{
    ponerNombre( nombre );
}

MallaRevol::MallaRevol( const std::string & nombre_arch,
        const unsigned nperfiles,
        const bool crear_tapas,
        const bool cerrar_malla,
        const bool crear_texturas)
{
    ponerNombre( std::string("malla por revolución del perfil en '"+ nombre_arch + "'" ));

    vector<float> verts;
    vector<Tupla3f> perfil_original;

    ply::read_vertices(nombre_arch.c_str(), verts);

    // Inicializar perfil original
    for (int i = 0; i < verts.size(); i += 3) {
        perfil_original.push_back({verts[i], verts[i+1], verts[i+2]});
    }

    // Inicializar número de perfiles
    nper = nperfiles;

    // Rellena las tablas
    crearMallaRevol(perfil_original, crear_tapas, cerrar_malla, crear_texturas);

    // calcular la tabla de normales
    calcular_normales();
}

void MallaRevol::crearMallaRevol(
        const std::vector<Tupla3f> & perfil_original,
        const bool crear_tapas,
        const bool cerrar_malla,
        const bool crear_texturas)
{
    unsigned n_vert_per = perfil_original.size();
    //Si se crean las texturas, en cada entrada se almacena la longitud del perfil {vértice inicial, ..., vértice i-ésimo}
    std::vector<float> distancias_perfil;

    if (crear_texturas){
        distancias_perfil.push_back(0.0);
        for (int i = 1; i < n_vert_per; ++i){
            distancias_perfil.push_back(distancias_perfil[i-1] + norm(perfil_original[i] - perfil_original[i-1]));
        }
    }

    // Añadir todos los vertices a la tabla de vértices
    for (int i = 0; i < nper; ++i) {
        unsigned j = 0;
        for (auto vert : perfil_original){
            if (cerrar_malla)
                tabla_verts.push_back (MAT_Rotacion((float)(360*i)/nper, 0, 1, 0)*vert);
            else{
                tabla_verts.push_back (MAT_Rotacion((float)(360*i)/(nper-1), 0, 1, 0)*vert);
                if (crear_texturas){
                    float s, t;

                    s = (float)i/(float)(nper-1);
                    t = distancias_perfil[j] /distancias_perfil[n_vert_per-1];
                    tabla_text.push_back ({s,1-t});
                }
            }
            ++j;
        }
    }

    if (crear_tapas){
        tabla_verts.push_back( {0, perfil_original[0](1), 0} );
        tabla_verts.push_back( {0, perfil_original[n_vert_per-1](1), 0} );
    }

    int vert_index;
    int vert_index_nxt_per;

    int ncaras = (cerrar_malla ? nper : nper-1);
    // Añadir caras a la tabla de vértices
    for (int i = 0; i < ncaras; ++i) {
        for (int j = 0; j < n_vert_per-1; ++j){
            vert_index = j+i*n_vert_per;

            if (cerrar_malla)
                vert_index_nxt_per = (vert_index + n_vert_per) % (nper * n_vert_per);
            else
                vert_index_nxt_per = vert_index + n_vert_per;

            tabla_caras.push_back({vert_index+1, vert_index, vert_index_nxt_per});
            tabla_caras.push_back({vert_index+1, vert_index_nxt_per, vert_index_nxt_per+1});
        }
    }

    // Poner tapas
    if (crear_tapas){
        int tapa0_ver_index = tabla_verts.size()-2;
        int tapa1_ver_index = tabla_verts.size()-1;

        int vert_index_bot, vert_index_top;
        int vert_index_nxt_per_bot, vert_index_nxt_per_top;

        for (int i = 0; i < ncaras; ++i) {
            vert_index_bot = i*n_vert_per;
            vert_index_top = n_vert_per-1 + i*n_vert_per;

            if (cerrar_malla){
                vert_index_nxt_per_bot = (vert_index_bot + n_vert_per) % (nper * n_vert_per);
                vert_index_nxt_per_top = (vert_index_top + n_vert_per) % (nper * n_vert_per);
            }
            else{
                vert_index_nxt_per_bot = vert_index_bot + n_vert_per;
                vert_index_nxt_per_top = vert_index_top + n_vert_per;
            }

            tabla_caras.push_back ({vert_index_nxt_per_bot, vert_index_bot, tapa0_ver_index});
            tabla_caras.push_back ({vert_index_top, vert_index_nxt_per_top, tapa1_ver_index});
        }
    }

}

// *****************************************************************************
// Constructor para el cilindro

Cilindro::Cilindro( const unsigned num_verts_per,
        const unsigned nperfiles,
        const bool crear_tapas,
        const bool cerrar_malla,
        const bool crear_texturas)
    : MallaRevol ("Cilindro")
{
    std::vector<Tupla3f> perfil_original;

    for (int i = 0; i < num_verts_per; ++i) {
        perfil_original.push_back({1,(float) i/(num_verts_per-1), 0});
    }

    nper = nperfiles;

    crearMallaRevol (perfil_original, crear_tapas, crear_tapas, crear_texturas);
    calcular_normales();
}

// *****************************************************************************
// Constructor para el cilindro

Cilindroide::Cilindroide(
        const float rad_base_sup,
        const unsigned num_verts_per,
        const unsigned nperfiles,
        const bool crear_tapas,
        const bool cerrar_malla,
        const bool crear_texturas)
    : MallaRevol ("Cilindroide")
{
    std::vector<Tupla3f> perfil_original;

    for (int i = 0; i < num_verts_per; ++i) {
        perfil_original.push_back({1 - (1-rad_base_sup)*i/(num_verts_per-1) ,(float) i/(num_verts_per-1), 0});
    }

    nper = nperfiles;

    crearMallaRevol (perfil_original, crear_tapas, crear_tapas, crear_texturas);
}
// *****************************************************************************
//
// Constructor para el cono
Cono::Cono( const unsigned num_verts_per,
        const unsigned nperfiles,
        const bool crear_tapas,
        const bool cerrar_malla,
        const bool crear_texturas)
    : MallaRevol ("Cono")
{
    std::vector<Tupla3f> perfil_original;

    for (int i = 0; i < num_verts_per; ++i) {
        perfil_original.push_back({1-(float) i/(num_verts_per-1),(float) i/(num_verts_per-1), 0});
    }

    nper = nperfiles;

    crearMallaRevol (perfil_original, crear_tapas, crear_tapas, crear_texturas);
}

// *****************************************************************************
// Constructor para la esfera

Esfera::Esfera( const unsigned num_verts_per,
        const unsigned nperfiles,
        const bool crear_tapas,
        const bool cerrar_malla,
        const bool crear_texturas)
    : MallaRevol ("Cono")
{
    std::vector<Tupla3f> perfil_original;

    float x, y, z;
    for (int i = 0; i < num_verts_per; ++i) {
        z = 0;
        y = -1 + (float)2*(i+1)/(num_verts_per+1);
        x = sqrt(1 - y*y);

        perfil_original.push_back({x, y, z});
    }

    nper = nperfiles;

    crearMallaRevol (perfil_original, crear_tapas, crear_tapas, crear_texturas);
    calcular_normales();
}
