// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp)
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include "MallaInd.hpp"   // declaración de 'ContextoVis'

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
   // COMPLETAR: en la práctica 2: calculo de las normales de la malla
   // .......

}
// -----------------------------------------------------------------------------
void MallaInd::crearVBOs()
{
    if (id_vbo_ver == 0){
        tam_ver = sizeof(float)*3L*tabla_verts.size();
        // crear VBO vértices
        id_vbo_ver = VBO_Crear(GL_ARRAY_BUFFER, tam_ver, tabla_verts.data());
    }

    if (id_vbo_tri == 0){
        tam_tri = sizeof(unsigned)*3L*tabla_caras.size();
        // crear VBO caras
        id_vbo_tri = VBO_Crear( GL_ELEMENT_ARRAY_BUFFER, tam_tri, tabla_caras.data());
    }
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
   crearVBOs();
    
   glBindBuffer (GL_ARRAY_BUFFER, id_vbo_ver);
   glVertexPointer (3, GL_FLOAT, 0, 0);
   glBindBuffer (GL_ARRAY_BUFFER, 0);
   glEnableClientState (GL_VERTEX_ARRAY);

   glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, id_vbo_tri);
   glDrawElements (GL_TRIANGLES, 3L*tabla_caras.size(), GL_UNSIGNED_INT, NULL);
   glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

   glDisableClientState (GL_VERTEX_ARRAY);
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

        tabla_caras = {
                Tupla3i (0,1,2),
                Tupla3i (0,1,3),
                Tupla3i (0,2,3),
                Tupla3i (1,2,3)
        };

}

Tetraedro::Tetraedro ()
: Tetraedro (Tupla3f(0,0,0), 1)
{}
// *****************************************************************************
