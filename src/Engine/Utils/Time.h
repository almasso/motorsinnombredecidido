#ifndef TIME_H
#define TIME_H

class TimeManager;
struct Time {
private:
	friend TimeManager;

	static double deltaTime_;
	static double time_;
public:
	/// @brief Tiempo en segundos que ha pasado desde la anterior actualizacion
	static constexpr double const& deltaTime = deltaTime_;
	/// @brief Tiempo en segundos que ha pasado desde el comienzo de la ejecucion
	static constexpr double const& time = time_;
	/// @brief Tiempo en segundos cada el que se evaluan las operaciones fisicas
	static constexpr double fixedDeltaTime = 0.02;
};

#endif