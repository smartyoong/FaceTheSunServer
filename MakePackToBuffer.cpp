#include"MakePackToBuffer.h"
#include<iostream>
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

	void PackToBuffer::Serialize(const RoomInfo& in)
	{
		Serialize(in.RoomName);
		Serialize(in.HostName);
		Serialize(in.CurrentPlayer);
		Serialize(in.CanJoin);
	}

	void PackToBuffer::DeSerialize(RoomInfo* out)
	{
		DeSerialize(&out->RoomName);
		DeSerialize(&out->HostName);
		DeSerialize(&out->CurrentPlayer);
		DeSerialize(&out->CanJoin);
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
	/*방 목록을 전송하기 위한 벡터 특수화 기본 자료형이 아니면 벡터가 에러가 나서,,,*/
	void PackToBuffer::Serialize(std::vector<RoomInfo>& in)
	{
		size_t size = in.size();
		WRITE(size);
		for (int i = 0; i < size; i++) {
			Serialize(in[i]);
		}
	}

	void PackToBuffer::DeSerialize(std::vector<RoomInfo>* out)
	{
		int size = 0;
		READ(&size);

		for (int i = 0; i < size; i++) {
			RoomInfo data;
			DeSerialize(&data);
			out->push_back(data);
		}
	}