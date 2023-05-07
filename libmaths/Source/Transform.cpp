#include "Transform.h"

#include "Vector/Vector4.h"

namespace LibMath
{
	Transform::Transform()
		: m_position(Vector3::zero()), m_rotation(Vector3::zero()),
		m_scale(Vector3::one())
	{
		Transform::onChange();
	}

	Transform::Transform(const Vector3& position, const Vector3& rotation,
		const Vector3& scale)
		: m_position(position), m_rotation(rotation), m_scale(scale)
	{
		Transform::onChange();
	}

	Transform::Transform(const Transform& other)
		: m_position(other.m_position), m_rotation(other.m_rotation),
		m_scale(other.m_scale)
	{
		Transform::onChange();
	}

	Transform::Transform(Transform&& other) noexcept
		: m_position(other.m_position), m_rotation(other.m_rotation),
		m_scale(other.m_scale)
	{
		Transform::onChange();
	}

	Transform& Transform::operator=(const Transform& other)
	{
		if (&other == this)
			return *this;

		m_position = other.m_position;
		m_rotation = other.m_rotation;
		m_scale = other.m_scale;

		onChange();

		return *this;
	}

	Transform& Transform::operator=(Transform&& other) noexcept
	{
		if (&other == this)
			return *this;

		m_position = other.m_position;
		m_rotation = other.m_rotation;
		m_scale = other.m_scale;

		onChange();

		return *this;
	}

	Transform Transform::operator*(const Transform& other) const
	{
		Transform result = *this;
		return result *= other;
	}

	Transform& Transform::operator*=(const Transform& other)
	{
		const Matrix4 newMat = getMatrix() * other.getMatrix();

		m_position = Vector3(newMat(0, 3), newMat(1, 3), newMat(2, 3));
		m_rotation += other.getRotation();
		m_rotation.m_x *= -1.f;
		m_rotation.m_z *= -1.f;
		m_scale *= other.getScale();

		onChange();

		return *this;
	}

	Vector3 Transform::forward() const
	{
		return up().cross(right());
	}

	Vector3 Transform::right() const
	{
		return (m_matrix * Vector4::right()).xyz().normalized();
	}

	Vector3 Transform::up() const
	{
		return (m_matrix * Vector4::up()).xyz().normalized();
	}

	Vector3 Transform::back() const
	{
		return -forward();
	}

	Vector3 Transform::left() const
	{
		return -right();
	}

	Vector3 Transform::down() const
	{
		return -up();
	}

	Vector3 Transform::getPosition() const
	{
		return m_position;
	}

	Vector3 Transform::getRotation() const
	{
		return m_rotation;
	}

	Vector3 Transform::getScale() const
	{
		return m_scale;
	}

	Matrix4x4 Transform::getMatrix() const
	{
		return m_matrix;
	}

	Transform& Transform::setPosition(const Vector3& position)
	{
		m_position = position;

		onChange();

		return *this;
	}

	Transform& Transform::setRotation(const Vector3& rotation)
	{
		m_rotation = rotation;

		onChange();

		return *this;
	}

	Transform& Transform::setScale(const Vector3& scale)
	{
		m_scale = scale;

		onChange();

		return *this;
	}

	Transform& Transform::translate(const Vector3& translation)
	{
		m_position += translation;

		onChange();

		return *this;
	}

	Transform& Transform::rotate(const Vector3& rotation)
	{
		m_rotation += rotation;

		onChange();

		return *this;
	}

	Transform& Transform::scale(const Vector3& scale)
	{
		m_scale *= scale;

		onChange();

		return *this;
	}

	void Transform::onChange()
	{
		m_matrix = Matrix4::translation(m_position)
			* Matrix4::rotation(m_rotation, false)
			* Matrix4::scaling(m_scale);
	}
}
