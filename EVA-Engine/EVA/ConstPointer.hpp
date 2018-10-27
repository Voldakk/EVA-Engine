#pragma once

namespace EVA
{
	/**
	* \brief Allows the use of a const pointer (the ConstPointer) where the stored pointer's non const methods can still be called
	* \tparam T The class it points to
	*/
	template <class T>
	class ConstPointer
	{
		T** m_Pointer;

	public:

		/**
		* \brief Constructor
		* \param pointer The pointer the ConstPtr is referencing
		*/
		ConstPointer(T** pointer) : m_Pointer(pointer)
		{
		}

		/**
		* \brief Dereferences the stored pointer
		* \return The stored pointer
		*/
		T* operator ->() const
		{
			return *m_Pointer;
		}

		/**
		* \brief Dereferences the stored pointer
		* \return The stored pointer
		*/
		T* operator *() const
		{
			return *m_Pointer;
		}

		/**
		* \brief Returns the stored pointer
		* \return The stored pointer
		*/
		T* Get() const
		{
			return *m_Pointer;
		}

		/**
		* \brief Checks if the stored pointer is not null
		*/
		explicit operator bool() const
		{
			return (m_Pointer != nullptr && *m_Pointer != nullptr);
		}

		/**
		* \brief Compares with another ConstPointer
		* \param other The other ConstPtr
		* \return True if the ConstPointers point to the same pointer
		*/
		bool operator ==(const ConstPointer other) const
		{
			return other.m_Pointer == m_Pointer;
		}

		/**
		* \brief Compares with another ConstPointer
		* \param other
		* \return True if the ConstPointers point to different pointers
		*/
		bool operator !=(const ConstPointer other) const
		{
			return other.m_Pointer != m_Pointer;
		}
	};
}
