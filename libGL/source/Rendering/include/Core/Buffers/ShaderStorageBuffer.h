#pragma once
#include "Core/EAccessSpecifier.h"
#include "Core/Buffers/Buffer.h"

namespace LibGL::Rendering
{
	class ShaderStorageBuffer : public Buffer
	{
	public:
		ShaderStorageBuffer() = default;
		ShaderStorageBuffer(EAccessSpecifier accessSpecifier);

		/**
		 * \brief Sets the ssbo's binding point
		 */
		void setBindingPoint(uint32_t bindingPoint);

		/**
		 * \brief Binds the ssbo at the given binding point
		 */
		void bind(uint32_t bindingPoint);

		/**
		 * \brief Binds the ssbo to the current binding point
		 */
		void bind() const override;

		/**
		 * \brief Unbinds the ssbo from the current binding point
		 */
		void unbind() const;

		/**
		 * \brief Sends the data block to the buffer
		 * \param data The data block to send
		 * \param blockSize The number of elements in the block
		 */
		void sendBlocks(const void* data, size_t blockSize) const;

	private:
		uint32_t				m_bindingPoint = 0;
	};
}

#include "Core/Buffers/ShaderStorageBuffer.inl"
