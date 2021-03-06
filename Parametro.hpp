#include <string>
#include <functional>
#include <matrizg.hpp>

#ifndef GRADO_LIBERTAD_HPP
#define GRADO_LIBERTAD_HPP

class Objeto3D ;

// tipo usado para las funciones que calculan una matriz a partir de un flotante
typedef std::function< Matriz4f( float )> TFuncionCMF  ;

// -----------------------------------------------------------------------------

class Parametro
{
    private:
        const std::string descripcion;
        const bool acotado;
        /* Función de cada parámetro que permite calcular la matriz
         * a partir de un valor real (valor_actual)
         */
        const TFuncionCMF fun_calculo_matriz;
        const float
            c, // valor inicial (y también central para grados acotados)
            s, // semiamplitud (si acotado), o factor de escala (si no acotado)
            f; // si acotado: frecuencia (ciclospor unidad del valor normalizado)
        Matriz4f* const ptr_mat;
        float
            valor_norm, // valor actual, normalizado, no acotado (crece desde 0)(Es la "p")
            velocidad; //velocidad actual (se suma al valor_norm)

        static constexpr float
            valor_incremento = 0.1;
        static constexpr float
            valor_incremento_aceleracion = 0.1;

    public:
        // crear grado de libertad no acotado
        Parametro
            (  const std::string &  p_descripcion,
               Matriz4f *           p_ptr_mat,
               TFuncionCMF          p_fun_calculo_matriz,
               bool                 p_acotado,
               float                p_c,
               float                p_s,
               float                p_f
            );

        void  actualizar_matriz(); //actualiza la matriz a la que apunta el puntero
        void  siguiente_cuadro();   // actualizar valor y matriz al siguiente frame
        void  reset();        // vuelve al estado inicial
        void  incrementar();  // incrementar el valor
        void  decrementar() ; // decrementar el valor
        void  acelerar();     // acelerar (aumentar velocidad normalizada)
        void  decelerar();    // decelerar (disminuir la velocidad normalizada)
        float leer_valor_actual(); // devuelve el valor actual (escalado, no normalizado)(Es la "q")
        float leer_velocidad_actual();    // devuelve velocidad o frecuencia actual
        std::string
            leer_descripcion();
};

#endif
