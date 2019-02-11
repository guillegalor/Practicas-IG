// Examen de prácticas IG (GIM, 18-19, conv.extraordinaria, 11-feb 19)
// Implementación de métodos de clases.
//
// Nombre: Guillermo Galindo Ortuño
// DNI: 25619527b
// e-mail ugr: guillegalor@ugr.es

#include "exextr1819.hpp"

// #############################################################################
// P1: prisma regular sin normales

PrismaRegP1::PrismaRegP1( const int n )
{
    float alpha = 360.0/n;

    for (int i = 0; i < n; ++i) {
            tabla_verts.push_back(MAT_Rotacion(alpha*i, 0, 1, 0)*Tupla3f({1,0,0}));
    }

    Tupla3f p;
    for (int i = 0; i < n; ++i) {
            p = tabla_verts[i];
            p(Y) = 1;
            tabla_verts.push_back(p);
    }

    tabla_verts.push_back({0,0,0});
    tabla_verts.push_back({0,1,0});
    int ind_c_inf = tabla_verts.size()-2;
    int ind_c_sup = tabla_verts.size()-1;

    int j;
    for (int i = 0; i < n; ++i){
        tabla_caras.push_back({i, (i+1)%n, i+n});
        tabla_caras.push_back({i+n, (i+1)%n, (i+1)%n +n});

        tabla_caras.push_back({i, ind_c_inf, (i+1)%n});
        tabla_caras.push_back({i+n, (i+1)%n +n, ind_c_sup});
    }
}

// #############################################################################
// P2: prisma regular con normales

PrismaRegP2::PrismaRegP2( const int n )
{
    float alpha = 360.0/n;

    Tupla3f p, p1;
    for (int i = 0; i < n; ++i) {
            p = MAT_Rotacion(alpha*i, 0, 1, 0)*Tupla3f({1,0,0});
            p1 = MAT_Rotacion(alpha*(i+1), 0, 1, 0)*Tupla3f({1,0,0});

            tabla_verts.push_back(p);
            tabla_verts.push_back(p1);
            tabla_verts.push_back({p(X), 1, p(Z)});

            tabla_verts.push_back({p(X), 1, p(Z)});
            tabla_verts.push_back(p1);
            tabla_verts.push_back({p1(X), 1, p1(Z)});
    }

    tabla_verts.push_back({0,0,0});
    tabla_verts.push_back({0,1,0});
    int ind_c_inf = tabla_verts.size()-2;
    int ind_c_sup = tabla_verts.size()-1;

    for (int i = 0; i < n; ++i){
        tabla_caras.push_back({6*i, 6*i+1, 6*i+2});
        tabla_caras.push_back({6*i+3, 6*i+4, 6*i+5});

        tabla_caras.push_back({6*i, ind_c_inf, 6*i+1});
        tabla_caras.push_back({6*i+3, 6*i+5, ind_c_sup});
    }

    calcular_normales();
}

// #############################################################################
// P3: rejilla de prismas,

PrismasP3::PrismasP3( const int nf, const int nc )
{
    PrismaRegP2* instancias[nf];
    std::vector<Prisma*> prismas;

    Matriz4f m = MAT_Traslacion(0,0,3);
    Matriz4f nueva_fila = MAT_Traslacion(3, 0, -3*nc);

    agregar(MAT_Escalado(1.0/2, 2, 1.0/2));

    for (int i = 0; i < nf; ++i) {
        instancias[i] = new PrismaRegP2(i+3);
    }

    for (int i = 0; i < nf*nc; ++i){
        prismas.push_back(new Prisma(instancias[i/nc]));
    }

    for (int i = 0; i < nf; ++i) {
        for (int j = 0; j < nc; ++j) {
            prismas[i*nc+j]->ponerIdentificador(1+i*nc+j);
            agregar(prismas[i*nc+j]);
            agregar(m);
        }
        agregar(nueva_fila);
    }

    for (int i = 0; i< nc*nf; ++i){
        parametros.push_back(
                Parametro(
                    "Rotacion Prima",
                    prismas[i]->getArticulacion(),
                    [=] (float v) {return MAT_Rotacion(v, 0, 1, 0);},
                    false,
                    0,
                    10,
                    0.1
                    )
        );
    }
}

PrismasP3::Prisma::Prisma(PrismaRegP2* p){
    articulacion = agregar(MAT_Ident());
    agregar(p);
}

Matriz4f* PrismasP3::Prisma::getArticulacion(){
    return leerPtrMatriz(articulacion);
}

// #############################################################################
// P4: rejilla de prismas con material texturado.

TexturaPrisma::TexturaPrisma()
: Textura("../imgs/text-madera-examen.jpg"){
    modo_gen_ct = mgct_coords_objeto;

    coefs_s[0] = coefs_s[3] = 1.0/2;
    coefs_s[1] = coefs_s[2] = 0;

    coefs_t[0] = coefs_t[1] = 0;
    coefs_t[2] = coefs_t[3] = 1.0/2;
}
// -----------------------------------------------------------------------------

MatPrismas::MatPrismas()
: Material(new TexturaPrisma(), 0.2, 0.6, 0.2, 5){
    ponerNombre("MaterialMaderaPrismas");
}
// -----------------------------------------------------------------------------

PrismasP4::PrismasP4( const int nf, const int nc )
{
    agregar(new MatPrismas);
    agregar(new PrismasP3(nf, nc));
}

// #############################################################################
// P5: rejilla de prismas , preparada para selección.

PrismasP5::PrismasP5( const int nf, const int nc )
{
    agregar(new PrismasP4(nf, nc));
}

// #############################################################################
