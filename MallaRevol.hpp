// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase MallaRevol
// **
// *********************************************************************

#ifndef IG_MALLAREVOL_HPP
#define IG_MALLAREVOL_HPP

#include <vector>          // usar std::vector
#include <string>

#include "MallaInd.hpp"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaRevol : public MallaInd{
    protected:
        void crearMallaRevol(
                const std::vector<Tupla3f> & perfil_original,
                const bool crear_tapas,
                const bool cerrar_malla,
                const bool crear_texturas);
        unsigned
            nper , // numero de perfiles
            nvp  ; // numero de vertices por perfil

    public:
        // crea una malla de revolucion
        MallaRevol( const std::string & nombre);
        // crea una malla de revolucion tomando el perfil de un archivo
        MallaRevol( const std::string & nombre_arch,
                const unsigned nperfiles,
                const bool crear_tapas,
                const bool cerrar_malla,
                const bool crear_texturas);

} ;

class Cilindro : public MallaRevol
{
    public:
        Cilindro(   const unsigned num_verts_per,
                const unsigned nperfiles,
                const bool crear_tapas,
                const bool cerrar_malla,
                const bool crear_texturas);
};

class Cilindroide : public MallaRevol
{
    public:
        Cilindroide( const float rad_base_sup,
                    const unsigned num_verts_per,
                    const unsigned nperfiles,
                    const bool crear_tapas,
                    const bool cerrar_malla,
                    const bool crear_texturas);
};

class Esfera : public MallaRevol
{
    public:
        Esfera( const unsigned num_verts_per,
                const unsigned nperfiles,
                const bool crear_tapas,
                const bool cerrar_malla,
                const bool crear_texturas);
};

class Cono : public MallaRevol
{
    public:
        Cono(   const unsigned num_verts_per,
                const unsigned nperfiles,
                const bool crear_tapas,
                const bool cerrar_malla,
                const bool crear_texturas);
};

// *****************************************************************************
// Parcial 1 Prácticas
class MallaRevolRegular : public MallaRevol
{
    public:
        MallaRevolRegular(
        const unsigned nperfiles,
        const float r1,
        const float r2,
        const float nlados);
};

#endif
