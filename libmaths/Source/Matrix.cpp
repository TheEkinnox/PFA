#include "Matrix.h"

#include <stdexcept>

#include "Arithmetic.h"
#include "Trigonometry.h"
#include "Angle.h"

#include "Vector/Vector3.h"

using namespace LibMath::Exceptions;

namespace LibMath
{
	Matrix2x2::Matrix2x2() : Matrix(2, 2)
	{
	}

	Matrix2x2::Matrix2x2(const float scalar) : Matrix(2, 2, scalar)
	{
	}

	Matrix2x2::Matrix2x2(const Matrix& other) : Matrix(other)
	{
		if (other.getRowCount() != 2 || other.getColumnCount() != 2)
			throw IncompatibleMatrix();
	}

	Matrix2x2::Matrix2x2(Matrix&& other) : Matrix(other)
	{
		if (other.getRowCount() != 2 || other.getColumnCount() != 2)
			throw IncompatibleMatrix();
	}

	Matrix2x3::Matrix2x3() : Matrix(2, 3)
	{
	}

	Matrix2x3::Matrix2x3(const float scalar) : Matrix(2, 3, scalar)
	{
	}

	Matrix2x3::Matrix2x3(const Matrix& other) : Matrix(other)
	{
		if (other.getRowCount() != 2 || other.getColumnCount() != 3)
			throw IncompatibleMatrix();
	}

	Matrix2x3::Matrix2x3(Matrix&& other) : Matrix(other)
	{
		if (other.getRowCount() != 2 || other.getColumnCount() != 3)
			throw IncompatibleMatrix();
	}

	Matrix2x4::Matrix2x4() : Matrix(2, 4)
	{
	}

	Matrix2x4::Matrix2x4(const float scalar) : Matrix(2, 4, scalar)
	{
	}

	Matrix2x4::Matrix2x4(const Matrix& other) : Matrix(other)
	{
		if (other.getRowCount() != 2 || other.getColumnCount() != 4)
			throw IncompatibleMatrix();
	}

	Matrix2x4::Matrix2x4(Matrix&& other) : Matrix(other)
	{
		if (other.getRowCount() != 2 || other.getColumnCount() != 4)
			throw IncompatibleMatrix();
	}

	Matrix3x2::Matrix3x2() : Matrix(3, 2)
	{
	}

	Matrix3x2::Matrix3x2(const float scalar) : Matrix(3, 2, scalar)
	{
	}

	Matrix3x2::Matrix3x2(const Matrix& other) : Matrix(other)
	{
		if (other.getRowCount() != 3 || other.getColumnCount() != 2)
			throw IncompatibleMatrix();
	}

	Matrix3x2::Matrix3x2(Matrix&& other) : Matrix(other)
	{
		if (other.getRowCount() != 3 || other.getColumnCount() != 2)
			throw IncompatibleMatrix();
	}

	Matrix3x3::Matrix3x3() : Matrix(3, 3)
	{
	}

	Matrix3x3::Matrix3x3(const float scalar) : Matrix(3, 3, scalar)
	{
	}

	Matrix3x3::Matrix3x3(const Matrix& other) : Matrix(other)
	{
		if (other.getRowCount() != 3 || other.getColumnCount() != 3)
			throw IncompatibleMatrix();
	}

	Matrix3x3::Matrix3x3(Matrix&& other) : Matrix(other)
	{
		if (other.getRowCount() != 3 || other.getColumnCount() != 3)
			throw IncompatibleMatrix();
	}

	Matrix3x4::Matrix3x4() : Matrix(3, 4)
	{
	}

	Matrix3x4::Matrix3x4(const float scalar) : Matrix(3, 4, scalar)
	{
	}

	Matrix3x4::Matrix3x4(const Matrix& other) : Matrix(other)
	{
		if (other.getRowCount() != 3 || other.getColumnCount() != 4)
			throw IncompatibleMatrix();
	}

	Matrix3x4::Matrix3x4(Matrix&& other) : Matrix(other)
	{
		if (other.getRowCount() != 3 || other.getColumnCount() != 4)
			throw IncompatibleMatrix();
	}

	Matrix4x2::Matrix4x2() : Matrix(4, 2)
	{
	}

	Matrix4x2::Matrix4x2(const float scalar) : Matrix(4, 2, scalar)
	{
	}

	Matrix4x2::Matrix4x2(const Matrix& other) : Matrix(other)
	{
		if (other.getRowCount() != 4 || other.getColumnCount() != 2)
			throw IncompatibleMatrix();
	}

	Matrix4x2::Matrix4x2(Matrix&& other) : Matrix(other)
	{
		if (other.getRowCount() != 4 || other.getColumnCount() != 2)
			throw IncompatibleMatrix();
	}

	Matrix4x3::Matrix4x3() : Matrix(4, 3)
	{
	}

	Matrix4x3::Matrix4x3(const float scalar) : Matrix(4, 3, scalar)
	{
	}

	Matrix4x3::Matrix4x3(const Matrix& other) : Matrix(other)
	{
		if (other.getRowCount() != 4 || other.getColumnCount() != 3)
			throw IncompatibleMatrix();
	}

	Matrix4x3::Matrix4x3(Matrix&& other) : Matrix(other)
	{
		if (other.getRowCount() != 4 || other.getColumnCount() != 3)
			throw IncompatibleMatrix();
	}

	Matrix4x4::Matrix4x4() : Matrix(4, 4)
	{
	}

	Matrix4x4::Matrix4x4(const float scalar) : Matrix(4, 4, scalar)
	{
	}

	Matrix4x4::Matrix4x4(const Matrix& other) : Matrix(other)
	{
		if (other.getRowCount() != 4 || other.getColumnCount() != 4)
			throw IncompatibleMatrix();
	}

	Matrix4x4::Matrix4x4(Matrix&& other) : Matrix(other)
	{
		if (other.getRowCount() != 4 || other.getColumnCount() != 4)
			throw IncompatibleMatrix();
	}

	Matrix4x4 Matrix4x4::translation(const float x, const float y, const float z)
	{
		Matrix4x4 translationMatrix(1.f);

		translationMatrix(0, 3) = x;
		translationMatrix(1, 3) = y;
		translationMatrix(2, 3) = z;

		return translationMatrix;
	}

	Matrix4x4 Matrix4x4::translation(const Vector3& translation)
	{
		return Matrix4x4::translation(translation.m_x, translation.m_y, translation.m_z);
	}

	Matrix4x4 Matrix4x4::scaling(const float x, const float y, const float z)
	{
		Matrix4x4 scalingMatrix;

		scalingMatrix(0, 0) = x;
		scalingMatrix(1, 1) = y;
		scalingMatrix(2, 2) = z;
		scalingMatrix(3, 3) = 1.f;

		return scalingMatrix;
	}

	Matrix4x4 Matrix4x4::scaling(const Vector3& scale)
	{
		return scaling(scale.m_x, scale.m_y, scale.m_z);
	}

	Matrix4x4 Matrix4x4::rotation(const Radian& angle, const Vector3& axis)
	{
		const Vector3 dir = axis.normalized();
		const float cos = LibMath::cos(angle);
		const float sin = LibMath::sin(angle);

		Matrix4x4 mat;

		mat(0, 0) = cos + dir.m_x * dir.m_x * (1.f - cos);
		mat(0, 1) = dir.m_x * dir.m_y * (1 - cos) - dir.m_z * sin;
		mat(0, 2) = dir.m_x * dir.m_z * (1 - cos) + dir.m_y * sin;
		mat(0, 3) = 0.f;

		mat(1, 0) = dir.m_y * dir.m_x * (1 - cos) + dir.m_z * sin;
		mat(1, 1) = cos + dir.m_y * dir.m_y * (1.f - cos);
		mat(1, 2) = dir.m_y * dir.m_z * (1 - cos) - dir.m_x * sin;
		mat(1, 3) = 0.f;

		mat(2, 0) = dir.m_z * dir.m_x * (1 - cos) - dir.m_y * sin;
		mat(2, 1) = dir.m_z * dir.m_y * (1 - cos) + dir.m_x * sin;
		mat(2, 2) = cos + dir.m_z * dir.m_z * (1.f - cos);
		mat(2, 3) = 0.f;

		mat(3, 0) = 0.f;
		mat(3, 1) = 0.f;
		mat(3, 2) = 0.f;
		mat(3, 3) = 1.f;

		return mat;
	}

	Matrix4x4 Matrix4x4::rotation(const Radian& yaw, const Radian& pitch, const Radian& roll)
	{
		const float cosYaw = cos(yaw);
		const float sinYaw = sin(yaw);

		const float cosPitch = cos(pitch);
		const float sinPitch = sin(pitch);

		const float cosRoll = cos(roll);
		const float sinRoll = sin(roll);

		Matrix4x4 rotationMat;

		rotationMat(0, 0) = cosYaw * cosRoll + sinYaw * sinPitch * sinRoll;
		rotationMat(0, 1) = -cosYaw * sinRoll + sinYaw * sinPitch * cosRoll;
		rotationMat(0, 2) = sinYaw * cosPitch;
		rotationMat(0, 3) = 0.f;

		rotationMat(1, 0) = sinRoll * cosPitch;
		rotationMat(1, 1) = cosRoll * cosPitch;
		rotationMat(1, 2) = -sinPitch;
		rotationMat(1, 3) = 0.f;

		rotationMat(2, 0) = -sinYaw * cosRoll + cosYaw * sinPitch * sinRoll;
		rotationMat(2, 1) = sinRoll * sinYaw + cosYaw * sinPitch * cosRoll;
		rotationMat(2, 2) = cosYaw * cosPitch;
		rotationMat(2, 3) = 0.f;

		rotationMat(3, 0) = 0.f;
		rotationMat(3, 1) = 0.f;
		rotationMat(3, 2) = 0.f;
		rotationMat(3, 3) = 1.f;

		return rotationMat;
	}

	Matrix4x4 Matrix4x4::rotation(const Vector3& angles, const bool isRadian)
	{
		if (isRadian)
			return rotation(Radian(angles.m_y), Radian(angles.m_x), Radian(angles.m_z));

		return rotation(Degree(angles.m_y), Degree(angles.m_x), Degree(angles.m_z));
	}

	Matrix4x4 Matrix4x4::rotationEuler(const Radian& xAngle, const Radian& yAngle, const Radian& zAngle)
	{
		return rotation(zAngle, xAngle, yAngle);
	}

	Matrix4x4 Matrix4x4::rotationEuler(const Vector3& angles, const bool isRadian)
	{
		if (isRadian)
			return rotationEuler(Radian(angles.m_x), Radian(angles.m_y), Radian(angles.m_z));

		return rotationEuler(Degree(angles.m_x), Degree(angles.m_y), Degree(angles.m_z));
	}

	// Adapted from https://gist.github.com/kevinmoran/b45980723e53edeb8a5a43c49f134724
	Matrix4x4 Matrix4x4::rotationFromTo(const Vector3& from, const Vector3& to)
	{
		const auto& fromDir = from.normalized();
		const auto& toDir = to.normalized();

		if (toDir == fromDir)
			return Matrix4x4(1.f);

		if (toDir == -fromDir)
			return scaling(-1.f, -1.f, -1.f);

		const Vector3 axis = from.cross(to);

		const float cosA = from.dot(to);
		const float k = 1.0f / (1.0f + cosA);

		Matrix4x4 rotationMat;

		rotationMat(0, 0) = (axis.m_x * axis.m_x * k) + cosA;
		rotationMat(0, 1) = (axis.m_y * axis.m_x * k) - axis.m_z;
		rotationMat(0, 2) = (axis.m_z * axis.m_x * k) + axis.m_y;
		rotationMat(0, 3) = 0.f;

		rotationMat(1, 0) = (axis.m_x * axis.m_y * k) + axis.m_z;
		rotationMat(1, 1) = (axis.m_y * axis.m_y * k) + cosA;
		rotationMat(1, 2) = (axis.m_z * axis.m_y * k) - axis.m_x;
		rotationMat(1, 3) = 0.f;

		rotationMat(2, 0) = (axis.m_x * axis.m_z * k) - axis.m_y;
		rotationMat(2, 1) = (axis.m_y * axis.m_z * k) + axis.m_x;
		rotationMat(2, 2) = (axis.m_z * axis.m_z * k) + cosA;
		rotationMat(2, 3) = 0.f;

		rotationMat(3, 0) = 0.f;
		rotationMat(3, 1) = 0.f;
		rotationMat(3, 2) = 0.f;
		rotationMat(3, 3) = 1.f;

		return rotationMat;
	}

	Matrix4x4 Matrix4x4::orthographicProjection(const float left, const float right,
		const float bottom, const float top, const float near, const float far)
	{
		Matrix4x4 mat;

		mat(0, 0) = 2.f / (right - left);
		mat(0, 3) = (right + left) / (left - right);

		mat(1, 1) = 2.f / (top - bottom);
		mat(1, 3) = (top + bottom) / (bottom - top);

		mat(2, 2) = 2.f / (near - far);
		mat(2, 3) = (far + near) / (near - far);

		mat(3, 3) = 1.f;

		return mat;
	}

	Matrix4x4 Matrix4x4::perspectiveProjection(const Radian& fovY, const float aspect,
		const float near, const float far)
	{
		const float tanHalfFovY = tan(fovY * .5f);

		Matrix4x4 mat;

		mat(0, 0) = 1.f / (aspect * tanHalfFovY);
		mat(1, 1) = 1.f / tanHalfFovY;
		mat(2, 2) = (far + near) / (near - far);
		mat(2, 3) = (2.f * far * near) / (near - far);
		mat(3, 2) = -1.f;

		return mat;
	}

	Matrix4x4 Matrix4x4::lookAt(const Vector3& eye, const Vector3& center, const Vector3& up)
	{
		const Vector3 f = (center - eye).normalized();
		const Vector3 s = f.cross(up).normalized();
		const Vector3 u = s.cross(f);

		Matrix4x4 mat;

		mat(0, 0) = s.m_x;
		mat(0, 1) = s.m_y;
		mat(0, 2) = s.m_z;
		mat(0, 3) = -s.dot(eye);

		mat(1, 0) = u.m_x;
		mat(1, 1) = u.m_y;
		mat(1, 2) = u.m_z;
		mat(1, 3) = -u.dot(eye);

		mat(2, 0) = -f.m_x;
		mat(2, 1) = -f.m_y;
		mat(2, 2) = -f.m_z;
		mat(2, 3) = f.dot(eye);

		mat(3, 3) = 1.f;

		return mat;
	}

	Matrix::Matrix(const length_t rows, const length_t columns)
	{
		if (rows == 0 || columns == 0)
			throw std::invalid_argument("Invalid matrix size");

		m_rows = rows;
		m_columns = columns;

		const size_t size = static_cast<size_t>(m_rows) * m_columns;

		m_values = new float[size]();

		// Builds a matrix filled with zeros
		for (size_t i = 0; i < size; i++)
			m_values[i] = 0;
	}

	Matrix::Matrix(const length_t rows, const length_t columns, const float scalar)
	{
		if (rows == 0 || columns == 0)
			throw std::invalid_argument("Invalid matrix size");

		m_rows = rows;
		m_columns = columns;

		const size_t size = static_cast<size_t>(m_rows) * m_columns;

		m_values = new float[size]();

		// Builds a diagonal matrix with the given scalar
		for (length_t row = 0; row < m_rows; row++)
			for (length_t col = 0; col < m_columns; col++)
				(*this)(row, col) = row == col ? scalar : 0;
	}

	Matrix::length_t Matrix::getIndex(const length_t row, const length_t column) const
	{
		if (row < 0 || row >= m_rows || column < 0 || column >= m_columns)
			throw std::out_of_range("Index out of range");

		return row * m_columns + column;
	}

	float* Matrix::getArray()
	{
		return m_values;
	}

	const float* Matrix::getArray() const
	{
		return m_values;
	}

	Matrix::~Matrix()
	{
		delete[] m_values;
	}

	Matrix::Matrix(const Matrix& other)
	{
		const size_t size = static_cast<size_t>(other.m_rows) * other.m_columns;

		m_values = new float[size]();

		for (size_t i = 0; i < size; i++)
			m_values[i] = other[i];

		m_columns = other.m_columns;
		m_rows = other.m_rows;
	}

	Matrix::Matrix(Matrix&& other) noexcept
	{
		m_columns = other.m_columns;
		m_rows = other.m_rows;
		m_values = other.m_values;

		other.m_columns = other.m_rows = 0;
		other.m_values = nullptr;
	}

	Matrix& Matrix::operator=(const Matrix& other)
	{
		if (this == &other)
			return *this;

		delete[] m_values;

		const length_t size = other.m_columns * other.m_rows;

		m_values = new float[size]();

		for (length_t i = 0; i < size; i++)
			m_values[i] = other[i];

		m_columns = other.m_columns;
		m_rows = other.m_rows;

		return *this;
	}

	Matrix& Matrix::operator=(Matrix&& other) noexcept
	{
		if (this == &other)
			return *this;

		delete[] m_values;

		m_columns = other.m_columns;
		m_rows = other.m_rows;
		m_values = other.m_values;

		other.m_columns = other.m_rows = 0;
		other.m_values = nullptr;

		return *this;
	}

	float Matrix::operator[](const size_t index) const
	{
		const size_t size = static_cast<size_t>(m_rows) * m_columns;

		if (index < 0 || index >= size)
			throw std::out_of_range("Index out of range");

		return m_values[index];
	}

	float& Matrix::operator[](const size_t index)
	{
		const size_t size = static_cast<size_t>(m_rows) * m_columns;

		if (index < 0 || index >= size)
			throw std::out_of_range("Index out of range");

		return m_values[index];
	}

	float Matrix::operator()(const length_t row, const length_t column) const
	{
		return (*this)[getIndex(row, column)];
	}

	float& Matrix::operator()(const length_t row, const length_t column)
	{
		return (*this)[getIndex(row, column)];
	}

	Matrix& Matrix::operator+=(const Matrix& other)
	{
		if (other.m_columns != m_columns || other.m_rows != m_rows)
			throw IncompatibleMatrix();

		const size_t size = static_cast<size_t>(m_rows) * m_columns;

		for (size_t i = 0; i < size; i++)
			m_values[i] += other[i];

		return *this;
	}

	Matrix& Matrix::operator-=(const Matrix& other)
	{
		if (other.m_columns != m_columns || other.m_rows != m_rows)
			throw IncompatibleMatrix();

		const size_t size = static_cast<size_t>(m_rows) * m_columns;

		for (size_t i = 0; i < size; i++)
			m_values[i] -= other[i];

		return *this;
	}

	Matrix& Matrix::operator*=(const Matrix& other)
	{
		return (*this = *this * other);
	}

	Matrix& Matrix::operator/=(const Matrix& other)
	{
		return (*this = *this / other);
	}

	Matrix& Matrix::operator+=(const float scalar)
	{
		const size_t size = static_cast<size_t>(m_rows) * m_columns;

		for (size_t i = 0; i < size; i++)
			m_values[i] += scalar;

		return *this;
	}

	Matrix& Matrix::operator-=(const float scalar)
	{
		const size_t size = static_cast<size_t>(m_rows) * m_columns;

		for (size_t i = 0; i < size; i++)
			m_values[i] -= scalar;

		return *this;
	}

	Matrix& Matrix::operator*=(const float scalar)
	{
		const size_t size = static_cast<size_t>(m_rows) * m_columns;

		for (size_t i = 0; i < size; i++)
			m_values[i] *= scalar;

		return *this;
	}

	Matrix& Matrix::operator/=(const float scalar)
	{
		const size_t size = static_cast<size_t>(m_rows) * m_columns;

		for (size_t i = 0; i < size; i++)
			m_values[i] /= scalar;

		return *this;
	}

	Matrix Matrix::operator+(const Matrix& other) const
	{
		Matrix mat = *this;
		return mat += other;
	}

	Matrix Matrix::operator-(const Matrix& other) const
	{
		Matrix mat = *this;
		return mat -= other;
	}

	Matrix Matrix::operator*(const Matrix& other) const
	{
		if (other.m_rows != m_columns)
			throw IncompatibleMatrix();

		Matrix result(m_rows, other.m_columns);

		for (length_t otherCol = 0; otherCol < other.m_columns; otherCol++)
		{
			for (length_t row = 0; row < m_rows; row++)
			{
				float scalar = 0;

				for (length_t col = 0; col < m_columns; col++)
					scalar += (*this)(row, col) * other(col, otherCol);

				result(row, otherCol) = scalar;
			}
		}

		return result;
	}

	Matrix Matrix::operator/(const Matrix& other) const
	{
		return *this * other.inverse();
	}

	Matrix Matrix::operator+(const float scalar) const
	{
		Matrix mat = *this;
		return mat += scalar;
	}

	Matrix Matrix::operator-(const float scalar) const
	{
		Matrix mat = *this;
		return mat -= scalar;
	}

	Matrix Matrix::operator*(const float scalar) const
	{
		Matrix mat = *this;
		return mat *= scalar;
	}

	Matrix Matrix::operator/(const float scalar) const
	{
		Matrix mat = *this;
		return mat /= scalar;
	}

	Matrix Matrix::operator-() const
	{
		return *this * -1;
	}

	bool Matrix::operator==(const Matrix& other) const
	{
		if (other.m_rows != m_rows || other.m_columns != m_columns)
			return false;

		const size_t size = static_cast<size_t>(m_rows) * m_columns;

		for (size_t i = 0; i < size; i++)
			if (!floatEquals(m_values[i], other[i]))
				return false;

		return true;
	}

	bool Matrix::operator!=(const Matrix& other) const
	{
		return !(*this == other);
	}

	bool Matrix::isIdentity() const
	{
		if (m_rows != m_columns)
			return false;

		return *this == Matrix(m_rows, m_columns, 1.f);
	}

	Matrix::length_t Matrix::getRowCount() const
	{
		return m_rows;
	}

	Matrix::length_t Matrix::getColumnCount() const
	{
		return m_columns;
	}

	float Matrix::cofactor(const length_t row, const length_t column) const
	{
		if (m_rows != m_columns)
			throw NonSquareMatrix();

		if (m_rows == 1)
			return m_values[0];

		// The multiplier is (-1)^(i+j) so 1 when i + j is pair and -1 otherwise
		const float multiplier = (row + column) % 2 == 0 ? 1.f : -1.f;

		return multiplier * minor(row, column).determinant();
	}

	Matrix Matrix::minor(const length_t row, const length_t column) const
	{
		Matrix minor(m_rows - 1, m_columns - 1);
		length_t minorLine = 0;

		for (length_t i = 0; i < m_rows; i++)
		{
			if (i == row)
				continue;

			length_t minorCol = 0;

			for (length_t j = 0; j < m_columns; j++)
			{
				if (j == column)
					continue;

				minor(minorLine, minorCol++) = (*this)(i, j);
			}

			minorLine++;
		}

		return minor;
	}

	float Matrix::determinant2x2() const
	{
		// 0 1
		// 2 3
		return (*this)[0] * (*this)[3] - (*this)[1] * (*this)[2];
	}

	float Matrix::determinant3x3() const
	{
		// 0 1 2
		// 3 4 5
		// 6 7 8
		const float positive = (*this)[0] * (*this)[4] * (*this)[8] +
			(*this)[1] * (*this)[5] * (*this)[6] +
			(*this)[2] * (*this)[3] * (*this)[7];

		const float negative = (*this)[2] * (*this)[4] * (*this)[6] +
			(*this)[1] * (*this)[3] * (*this)[8] +
			(*this)[0] * (*this)[5] * (*this)[7];

		return positive - negative;
	}

	float Matrix::determinant() const
	{
		if (m_rows != m_columns)
			throw NonSquareMatrix();

		if (m_rows == 1)
			return m_values[0];

		if (m_rows == 2)
			return determinant2x2();

		if (m_rows == 3)
			return determinant3x3();

		float determinant = 0;

		for (length_t col = 0; col < m_columns; col++)
			determinant += m_values[col] * cofactor(0, col);

		return determinant;
	}

	Matrix Matrix::transposed() const
	{
		if (m_rows != m_columns)
			throw NonSquareMatrix();

		Matrix transposed(m_columns, m_rows);

		for (length_t i = 0; i < m_rows; i++)
			for (length_t j = 0; j < m_columns; j++)
				transposed(j, i) = (*this)(i, j);

		return transposed;
	}

	Matrix Matrix::coMatrix() const
	{
		if (m_rows != m_columns)
			throw NonSquareMatrix();

		Matrix coMatrix(m_rows, m_columns);

		for (length_t row = 0; row < m_rows; row++)
			for (length_t col = 0; col < m_columns; col++)
				coMatrix(row, col) = cofactor(row, col);

		return coMatrix;
	}

	Matrix Matrix::adjugate() const
	{
		return coMatrix().transposed();
	}

	Matrix Matrix::inverse() const
	{
		if (m_rows != m_columns)
			throw NonSquareMatrix();

		const float det = determinant();

		if (det == 0.f)
			throw NonInvertibleMatrix();

		return adjugate() / det;
	}
}
