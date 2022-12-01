#include"MakePackToBuffer.h"

	PackToBuffer::PackToBuffer(const int buffSize_)
	{
		m_Size = buffSize_;
		m_Buffer = new char[m_Size];
	}

	void PackToBuffer::SetBuffer(char* buf, const int size)
	{
		m_Size = size;
		m_Buffer = new char[size];
		memcpy_s(m_Buffer, size, buf, size);
	}

	bool PackToBuffer::CheckWriteBoundary(int offsetSize_)
	{
		if (m_WritePtr + offsetSize_ >= m_Size) {
			return false;
		}
		return true;
	}

	bool PackToBuffer::CheckReadBoundary(int offsetSize_)
	{
		if (m_ReadPtr + offsetSize_ >= m_Size) {
			return false;
		}
		return true;
	}

	void PackToBuffer::Serialize(const std::string& in)
	{
		int size = in.size();
		WRITE(size);

		for (int i = 0; i < size; i++) {
			WRITE(in[i]);
		}
	}

	void PackToBuffer::DeSerialize(std::string* out)
	{
		int size = 0;
		READ(&size);

		char* pTemp = new char[size + 1];
		for (int i = 0; i < size; i++) {
			READ(&pTemp[i]);
		}
		pTemp[size] = '\0';
		*out = pTemp;
		pTemp = nullptr;
	}

	PackToBuffer& PackToBuffer::operator<<(const int& in)
	{
		WRITE(in);
		return *this;
	}


	PackToBuffer& PackToBuffer::operator<<(const float& in)
	{
		WRITE(in);
		return *this;
	}

	PackToBuffer& PackToBuffer::operator<<(const double& in)
	{
		WRITE(in);
		return *this;
	}

	PackToBuffer& PackToBuffer::operator<<(const bool& in)
	{
		WRITE(in);
		return *this;
	}

	PackToBuffer& PackToBuffer::operator<<(const std::string& in)
	{
		Serialize(in);
		return *this;
	}

	PackToBuffer& PackToBuffer::operator>>(int* out)
	{
		READ(out);
		return *this;
	}

	PackToBuffer& PackToBuffer::operator>>(float* out)
	{
		READ(out);
		return *this;
	}

	PackToBuffer& PackToBuffer::operator>>(double* out)
	{
		READ(out);
		return *this;
	}

	PackToBuffer& PackToBuffer::operator>>(bool* out)
	{
		READ(out);
		return *this;
	}

	PackToBuffer& PackToBuffer::operator>>(std::string* out)
	{
		DeSerialize(out);
		return *this;
	}