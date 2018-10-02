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
MallaRevol::MallaRevol( const std::string & nombre)
{
        ponerNombre( nombre );
}

MallaRevol::MallaRevol( const std::string & nombre_arch,
                        const unsigned nperfiles,
                        const bool     crear_tapas,
                        const bool     cerrar_malla  )
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
   crearMallaRevol(  perfil_original, false, false);

   // calcular la tabla de normales
   // calcular_normales();

}

void MallaRevol::crearMallaRevol(
                const std::vector<Tupla3f> & perfil_original,
                const bool crear_tapas,
                const bool crear_malla)
{

        // Añadir todos los vertices a la tabla de vértices
        for (int i = 0; i < nper; ++i) {
               for (auto vert : perfil_original){
                       tabla_verts.push_back (MAT_Rotacion((float)(360*i)/nper, 0, 1, 0)*vert);
               }
        }

        int vert_index;
        int vert_index_nxt_per;

        // Añadir caras a la tabla de vértices
        for (int i = 0; i < nper; ++i) {
                for (int j = 0; j < perfil_original.size()-1; ++j){
                        vert_index = j+i*perfil_original.size();
                        vert_index_nxt_per = (vert_index + perfil_original.size()) % (nper*perfil_original.size());

                        tabla_caras.push_back({vert_index, vert_index+1, vert_index_nxt_per});
                        tabla_caras.push_back({vert_index+1, vert_index_nxt_per, vert_index_nxt_per+1});
                }
        }

}

// *****************************************************************************
