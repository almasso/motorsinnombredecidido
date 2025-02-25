#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <chrono>

/// @~english
/// @brief Manages and updates the static \c Time structure
/// @~spanish
/// @brief Gestiona y actualiza la estructura estática \c Time
class TimeManager {
private:
	std::chrono::system_clock::time_point previous;

public:
	/// @~english
	/// @brief Initializes the time count of the program
	/// @~spanish
	/// @brief Inicializa el conteo del tiempo del programa
	void init();
	/// @~english
	/// @brief Updates the time count of the program and the time passed since the last update
	/// @~spanish
	/// @brief Actualiza el conteo del tiempo del programa y el tiempo pasado desde la última actualización
	void update();
};

#endif
