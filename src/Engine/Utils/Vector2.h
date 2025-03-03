#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2 {
private:
	float x_;
	float y_;

public:
	/// @~english
	/// @brief \c Vector2 's default constructor.
	/// @remark Values X and Y are assigned to \c 0.0f .
	/// @~spanish
	/// @brief Constructora por defecto de la clase \c Vector2 .
	/// @remark Los valores de X e Y se asignan a \c 0.0f .
	Vector2();

	/// @~english
	/// @brief Parameter based constructor.
	/// @param x X component.
	/// @param y Y component.
	/// @~spanish
	/// @brief Constructora a partir de parámetros.
	/// @param x Componente en X.
	/// @param y Componente en Y.
	Vector2(float x, float y);

	/// @~spanish
	/// @brief Copy constructor.
	/// @param v Vector to copy.
	/// @~spanish
	/// @brief Constructora a partir de otro vector.
	/// @param v Vector a copiar.
	Vector2(const Vector2& v);

	/// @~english
	/// @brief Uniform vector constructor.
	/// @remark The same value will be assigned to both components.
	/// @param e X and Y components.
	/// @~spanish
	/// @brief Constructor de vector uniforme.
	/// @remark Se asignará el mismo valor a ambas componentes.
	/// @param e Componente X e Y.
	explicit Vector2(float e);

	/// @~english
	/// @brief \c Vector2 's default destructor.
	/// @~spanish
	/// @brief Destructora por defecto de la clase \c Vector2 .
	~Vector2();

	/// @~english
	/// @brief Sums two vectors.
	/// @param v Vector to sum.
	/// @return Resultant vector.
	/// @~spanish
	/// @brief Suma dos vectores.
	/// @param v Vector a sumar.
	/// @return El vector resultante.
	Vector2 operator+(const Vector2& v) const;

	/// @~english
	/// @brief Subtract a vector to another.
	/// @param v Vector to subtract.
	/// @return Resultant vector.
	/// @~spanish
	/// @brief Resta dos vectores.
	/// @param v Vector a restar.
	/// @return El vector resultante.
	Vector2 operator-(const Vector2& v) const;

	/// @~english
	/// @brief Multiplies a vector by a scalar.
	/// @param e Scalar to multiply by.
	/// @return Resultant vector.
	/// @~spanish
	/// @brief Multiplica un vector por un escalar.
	/// @param e Escalar por el que multiplicar.
	/// @return El vector resultante.
	Vector2 operator*(float e) const;

	/// @~english
	/// @brief Divides a vector by a scalar.
	/// @param e Scalar to divide by.
	/// @return Resultant vector.
	/// @~spanish
	/// @brief Divide un vector por un escalar.
	/// @param e Escalar por el que dividir.
	/// @return El vector resultante.
	Vector2 operator/(float e) const;

	/// @~english
	/// @brief Divides a vector's components by another vector's components.
	/// @param v Vector to divide by.
	/// @return Resultant vector.
	/// @~spanish
	/// @brief Divide un vector entre las componentes de otro.
	/// @param v Vector por el que dividir.
	/// @return El vector resultante.
	Vector2 operator/(Vector2 const& v) const;

	/// @~english
	/// @brief Component product (Hadamard).
	/// @param v Vector to multiply by.
	/// @return Resultant vector.
	/// @~spanish
	/// @brief Producto por componentes (Hadamard).
	/// @param v Vector por el que multiplicar.
	/// @return El vector resultante.
	Vector2 operator*(const Vector2& v) const;

	/// @~english
	/// @brief Two vector equality. Compares component by component if they are the same.
	/// @param v Vector to compare.
	/// @return Comparison result.
	/// @~spanish
	/// @brief Igualdad de dos vectores. Compara componente a componente si son iguales.
	/// @param v Vector a comparar.
	/// @return Resultado de la comprobación.
	bool operator==(const Vector2& v) const;

	/// @~english
	/// @brief Two vector inequality.
	/// @param v Vector to compare.
	/// @return Comparison result.
	/// @~spanish
	/// @brief Desigualdad de dos vectores.
	/// @param v Vector a comparar.
	/// @return Resultado de la comprobación.
	bool operator!=(const Vector2& v) const;

	/// @~english
	/// @brief Sums two vectors.
	/// @param v Vector to sum.
	/// @return Updated vector.
	/// @~spanish
	/// @brief Suma de dos vectores.
	/// @param v Vector a sumar.
	/// @return El vector actualizado.
	Vector2 operator+=(const Vector2& v);

	/// @~spanish
	/// @brief Component product (Hadamard).
	/// @param v Vector to multiply by.
	/// @return Updated vector.
	/// @~spanish
	/// @brief Producto de componentes (Hadamard).
	/// @param v Vector por el que multiplicar.
	/// @return El vector actualizado.
	Vector2 operator*=(const Vector2& v);

	/// @~english
	/// @brief Multiplies a vector by a scalar.
	/// @param e Scalar to multiply by.
	/// @return Updated vector.
	/// @~spanish
	/// @brief Multiplica un vector por un escalar.
	/// @param e Escalar por el que multiplicar.
	/// @return El vector actualizado.
	Vector2 operator*=(float e);

	/// @~english
	/// @brief Calculates the vector's magnitude.
	/// @return Floating point value representing the vector's magnitude.
	/// @~spanish
	/// @brief Calcula el módulo de un vector.
	/// @return Valor en coma flotante representando el módulo del vector.
	float magnitude() const;

	/// @~english
	/// @brief Normalizes a vector.
	/// @remark This vector is modified to have a value \c 1.0f of magnitude.
	/// @return Normalized vector.
	/// @~spanish
	/// @brief Normaliza un vector.
	/// @remark Este vector se modificará para tener un módulo de valor |c 1.0f .
	/// @return Vector normalizado.
	Vector2& normalize();

	/// @~english
	/// @brief Returns a normalized copy of the vector.
	/// @remark A copy of this vector will be created with a magnitude of \c 1.0f .
	/// @return Normalized copy of the vector.
	/// @~spanish
	/// @brief Devuelve una copia normalizada del vector.
	/// @remark Se creará una copia de este vector con módulo \c 1.0f .
	/// @return Copia normalizada del vector.
	Vector2 normalized() const;

	/// @~english
	/// @brief Returns the X coordinate value.
	/// @return X coordinate.
	/// @~spanish
	/// @brief Devuelve el valor de la coordenada X.
	/// @return La coordenada X.
	float getX() const;

	/// @~english
	/// @brief Returns the Y coordinate value.
	/// @return Y coordinate.
	/// @~spanish
	/// @brief Devuelve el valor de la coordenada Y.
	/// @return La coordenada Y.
	float getY() const;

	/// @~english
	/// @brief Sets the X coordinate value.
	/// @param x New X value.
	/// @~spanish
	/// @brief Asigna el valor de la coordenada X.
	/// @param x Nuevo valor de X.
	void setX(float x);

	/// @~english
	/// @brief Sets the Y coordinate value.
	/// @param y New Y value.
	/// @~spanish
	/// @brief Asigna el valor de la coordenada Y.
	/// @param y Nuevo valor de Y.
	void setY(float y);

	/// @~english
	/// @brief Sets a vector based off of another vector.
	/// @param v Vector to be based off of.
	/// @~spanish
	/// @brief Asigna un vector a través de otro vector.
	/// @param v Vector a asignar.
	void set(const Vector2& v);

	/// @~english
	/// @brief Sets a vector based off of values for each coordinate.
	/// @param x New X value.
	/// @param y New Y value.
	/// @~spanish
	/// @brief Asigna un vector a través de valores para cada coordenada.
	/// @param x Nuevo valor de X.
	/// @param y Nuevo valor de Y.
	void set(float x, float y);

	/// (0, 0)
	static const Vector2 ZERO;
	/// (1, 0)
	static const Vector2 RIGHT;
	/// (-1, 0)
	static const Vector2 LEFT;
	/// (0, 1)
	static const Vector2 UP;
	/// (0, -1)
	static const Vector2 DOWN;
	/// (1, 1)
	static const Vector2 UNIT;
};

#endif