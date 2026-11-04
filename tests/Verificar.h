#pragma once

#include <exception>
#include <iostream>
#include <string>

// Mini framework de pruebas, para no agregar una dependencia como GoogleTest.
//
// No usamos assert(): cuando se compila en modo Release se define NDEBUG y
// assert desaparece, así que las pruebas pasarían siempre sin revisar nada.
namespace pruebas
{

// "inline" (C++17) permite definir la variable en un header incluido por
// varios .cpp sin que el linker se queje de definiciones duplicadas.
inline int totalVerificaciones = 0;
inline int totalFallas = 0;

inline void verificar(bool condicion, const std::string& descripcion)
{
    ++totalVerificaciones;
    if (!condicion)
    {
        ++totalFallas;
        std::cerr << "  FALLA: " << descripcion << '\n';
    }
}

// Pasa si la función lanza exactamente el tipo de excepción esperado.
template <typename ExcepcionEsperada, typename Funcion>
void verificarLanza(Funcion funcion, const std::string& descripcion)
{
    ++totalVerificaciones;
    try
    {
        funcion();
    }
    catch (const ExcepcionEsperada&)
    {
        return;
    }
    catch (const std::exception& otra)
    {
        ++totalFallas;
        std::cerr << "  FALLA: " << descripcion << " (lanzó otra excepción: " << otra.what()
                  << ")\n";
        return;
    }
    ++totalFallas;
    std::cerr << "  FALLA: " << descripcion << " (no lanzó ninguna excepción)\n";
}

} // namespace pruebas
