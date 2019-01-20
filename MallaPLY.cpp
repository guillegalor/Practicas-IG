// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Implementación de la clase 'MallaPLY'
// **
// *********************************************************************

#include <cassert>
#include <aux.hpp>
#include <tuplasg.hpp>
#include <file_ply_stl.hpp>
#include "MallaPLY.hpp"

using namespace std ;

// *****************************************************************************

MallaPLY::MallaPLY( const std::string & nombre_arch )
{
    ponerNombre(string("malla leída del archivo '") + nombre_arch + "'" );

    vector<float> verts;
    vector<int> caras;

    ply::read(nombre_arch.c_str(), verts, caras);

    // Inicializar tabla de vertices
    for (int i = 0; i < verts.size(); i += 3) {
        tabla_verts.push_back({verts[i], verts[i+1], verts[i+2]});
    }

    // Inicializar tabla de caras
    for (int i = 0; i < caras.size(); i += 3) {
        tabla_caras.push_back({caras[i], caras[i+1], caras[i+2]});
    }

    // calcular la tabla de normales
    //calcular_normales();
}

// *****************************************************************************
