#ifndef TIME_H
#define TIME_H

class TimeManager;

/// @~english
/// @brief Static structure containing the updated time values
/// @~spanish
/// @brief Estructura estática que contiene los valores de tiempo actualizados
struct Time {
private:
	friend TimeManager;

	static float _deltaTime;
	static float _time;
public:
	/// @~english
	/// @brief Time in seconds passed since last update
	/// @~spanish
	/// @brief Tiempo en segundos que ha pasado desde la anterior actualización
	static constexpr float const& deltaTime = _deltaTime;

	/// @~english
	/// @brief Time in seconds passed since the start of the program
	/// @~spanish
	/// @brief Tiempo en segundos que ha pasado desde el comienzo del programa
	static constexpr float const& time = _time;

	/// @~english
	/// @brief Time in seconds between every physic's operations' update
	/// @~spanish
	/// @brief Tiempo en segundos cada el que se evalúan las operaciones físicas
	static constexpr float fixedDeltaTime = 0.02;
};

#endif