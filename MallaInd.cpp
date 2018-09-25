// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include "MallaInd.hpp"   // declaración de 'ContextoVis'

// *****************************************************************************
// funciones auxiliares

// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre("malla indexada, anónima");

}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;

}
// -----------------------------------------------------------------------------
// calcula las dos tablas de normales

void MallaInd::calcular_normales()
{
   // COMPLETAR: en la práctica 2: calculo de las normales de la malla
   // .......

}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI( ContextoVis & cv )
{
   glEnableClientState( GL_VERTEX_ARRAY );
   glVertexPointer( 3, GL_FLOAT, 0, tabla_verts.data() );
   // Hace falta multiplicar tabla_caras*3 porque es un vector de 3-uplas
   glDrawElements( GL_TRIANGLES, tabla_caras.size()*3, GL_UNSIGNED_INT, tabla_caras.data() );
   glDisableClientState( GL_VERTEX_ARRAY );
}

// ----------------------------------------------------------------------------
void MallaInd::visualizarDE_VBOs( ContextoVis & cv )
{
   // COMPLETAR: práctica 1: visualizar en modo diferido,
   //                        usando VBOs (Vertex Buffer Objects)
   // ..........

}

// -----------------------------------------------------------------------------

void MallaInd::visualizarGL( ContextoVis & cv )
{
        // Establecer modo indicado en el contexto
        GLenum mode;
        switch (cv.modoVis){
                case modoSolido:
                        mode = GL_FILL;
                        break;
                case modoPuntos:
                        mode = GL_POINT;
                        break;
                case modoAlambre:
                        mode = GL_LINE;
                        break;
        }

        glPolygonMode (GL_FRONT_AND_BACK, mode);

        if (cv.usarVBOs)
                visualizarDE_VBOs (cv);
        else
                visualizarDE_MI (cv);

}
// *****************************************************************************

// *****************************************************************************

Cubo::Cubo(Tupla3f main_corner, float edge_size)
:  MallaInd ("malla cubo")
{
   // Inicialización de vertices del cubo
   unsigned i,j,k;

   for (i = 0; i < 2; ++i) {
       for (j = 0; j < 2; ++j) {
           for (k = 0; k < 2; ++k) {
               tabla_verts.push_back (Tupla3f (
                                          main_corner(0) + i*edge_size,
                                          main_corner(1) + j*edge_size,
                                          main_corner(2) + k*edge_size)
               );
           }
       }
   }

   // Inicialización de caras del cubo
   tabla_caras = {
                        Tupla3i (0,1,3),
                        Tupla3i (0,2,3),
                        Tupla3i (0,1,5),
                        Tupla3i (0,4,5),
                        Tupla3i (0,2,6),
                        Tupla3i (0,4,6),
                        Tupla3i (7,1,3),
                        Tupla3i (7,2,3),
                        Tupla3i (7,1,5),
                        Tupla3i (7,4,5),
                        Tupla3i (7,2,6),
                        Tupla3i (7,4,6),
   };

}

Cubo::Cubo()
:  Cubo (Tupla3f(0,0,0), 1)
{}
// *****************************************************************************

Tetraedro::Tetraedro(Tupla3f center, float edge_size)
:  MallaInd( "malla tetraedro")
{
        tabla_verts = {
                Tupla3f (center(0)+ 1*edge_size/2, center(1), center(2) - 1/sqrt(2)*edge_size/2),
                Tupla3f (center(0)- 1*edge_size/2, center(1), center(2) - 1/sqrt(2)*edge_size/2),
                Tupla3f (center(0), center(1)+ 1*edge_size/2, center(2) + 1/sqrt(2)*edge_size/2),
                Tupla3f (center(0), center(1)- 1*edge_size/2, center(2) + 1/sqrt(2)*edge_size/2)
        };

        for (auto v : tabla_verts)
                std::cout << v << std::endl;

        tabla_caras = {
                Tupla3i (0,1,2),
                Tupla3i (0,1,3),
                Tupla3i (0,2,3),
                Tupla3i (1,2,3)
        };

        for (auto v : tabla_caras)
                std::cout << v << std::endl;

}

Tetraedro::Tetraedro ()
: Tetraedro (Tupla3f(0,0,0), 1)
{}
// *****************************************************************************
