#ifndef RANDOM_H_
#define RANDOM_H_

#include <random>

class Random {
private:
	static std::default_random_engine generator;

public:
	/// @~english
	/// @brief Generates a random floating point number in the interval [0, t).
	/// @param t Interval superior limit.
	/// @return Random floating point number.
	/// @~spanish
	/// @brief Genera un número en coma flotante aleatorio en el intervalo [0, t).
	/// @param t Límite superior del intervalo.
	/// @return Número aleatorio en coma flotante.
	static float generate(float t);

	/// @~english
	/// @brief Generates an integer number in the interval [0, t].
	/// @param t Interval superior limit.
	/// @return Random integer number.
	/// @~spanish
	/// @brief Genera un número entero aleatorio en el intervalo [0, t].
	/// @param t Limite superior del intervalo.
	/// @return Número entero aleatorio.
	static int generate(int t);

	/// @~english
	/// @brief Generates a random floating point number in the interval [a, b).
	/// @param a Interval inferior limit.
	/// @param b Interval superior limit.
	/// @return Random floating point number.
	/// @~spanish
	/// @brief Genera un número en coma flotante aleatorio en el intervalo [a, b).
	/// @param a Limite inferior del intervalo.
	/// @param b Limite superior del intervalo.
	/// @return Número aleatorio en coma flotante.
	static float generateRange(float a, float b);

	/// @~english
	/// @brief Genera un número entero aleatorio en el intervalo [a, b].
	/// @param a Interval inferior limit.
	/// @param b Interval superior limit.
	/// @return Random floating point number.
	/// @~spanish
	/// @brief Genera un número entero aleatorio en el intervalo [a, b].
	/// @param a Limite inferior del intervalo.
	/// @param b Limite superior del intervalo.
	/// @return Número entero aleatorio.
	static int generateRange(int a, int b);
};

#endif