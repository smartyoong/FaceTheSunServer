#pragma once
#include "Packet.h"
class PackToBuffer
{
public:
	PackToBuffer(const int BufferSize);
	void SetBuffer(char* buf, const int size);
	PackToBuffer(const PackToBuffer& copy)
	{
		this->SetBuffer(copy.m_Buffer, copy.m_Size);
	}
	void operator =(const PackToBuffer& copy) {
		this->SetBuffer(copy.m_Buffer, copy.m_Size);
	}

	int GetBufferSize()const { return m_Size; }
	int GetReadDataSize()const { return m_ReadPtr; }
	int GetWriteDataSize()const { return m_WritePtr; }
	const char* GetBuffer() { return m_Buffer; }
public:
	// string , vector .. 더 필요한 STL들은 알아서 집어넣길
	void Serialize(const std::string& in);
	template<typename T>
	void Serialize(const std::vector<T>& in);
	template<typename T>
	void Serialize(const T& in);
	template<typename T>
	void DeSerialize(T* out);
	template<typename T>
	void DeSerialize(std::vector<T>* out);
	void DeSerialize(std::string* out);
	void Serialize(const RoomInfo& in);
	void DeSerialize(RoomInfo* out);
	void Serialize(std::vector<RoomInfo>& in); // 특수화
	void DeSerialize(std::vector<RoomInfo>* out); // 특수화
public:
	// operator 사용할 것인가?
	PackToBuffer& operator << (const int& in);
	PackToBuffer& operator << (const float& in);
	PackToBuffer& operator << (const double& in);
	PackToBuffer& operator << (const bool& in);
	template <typename T>
	PackToBuffer& operator << (const std::vector<T>& in);
	PackToBuffer& operator << (const std::string& in);

	PackToBuffer& operator >> (int* out);
	PackToBuffer& operator >> (float* out);
	PackToBuffer& operator >> (double* out);
	PackToBuffer& operator >> (bool* out);
	template <typename T>
	PackToBuffer& operator >> (std::vector<T>* out);
	PackToBuffer& operator >> (std::string* out);
private:
	// 버퍼가 가득찼을 때 false 반환
	bool CheckWriteBoundary(int offsetSize_);
	bool CheckReadBoundary(int offsetSize_);
	// 메모리 단일 복사 과정 
	template <typename T>
	void WRITE(const T& data);
	template <typename T>
	void READ(T* pData);
private:
	int m_Size = 0;
	int m_ReadPtr = 0;
	int m_WritePtr = 0;
	char* m_Buffer = nullptr;
};

template<typename T>
inline void PackToBuffer::Serialize(const std::vector<T>& in)
{
	size_t size = in.size();
	WRITE(size);
	for (int i = 0; i < size; i++) {
		WRITE(in[i]);
	}
}

template<typename T>
inline void PackToBuffer::Serialize(const T& in)
{
	WRITE(in);
}

template<typename T>
inline void PackToBuffer::DeSerialize(T* out)
{
	READ(out);
}

template<typename T>
inline void PackToBuffer::DeSerialize(std::vector<T>* out)
{
	int size = 0;
	READ(&size);

	for (int i = 0; i < size; i++) {
		T data;
		READ(&data);
		out->push_back(data);
	}
}

template<typename T>
inline PackToBuffer& PackToBuffer::operator<<(const std::vector<T>& in)
{
	Serialize(in);
	return *this;
}

template<typename T>
inline PackToBuffer& PackToBuffer::operator>>(std::vector<T>* out)
{
	DeSerialize(out);
	return *this;
}

template<typename T>
inline void PackToBuffer::WRITE(const T& data)
{
	int dataSize = sizeof(T);
	if (CheckWriteBoundary(dataSize)) {
		memcpy_s((void* const)(m_Buffer + m_WritePtr), sizeof(m_Buffer), (const void*)(&data),
			dataSize);
		m_WritePtr += dataSize;
	}
}

template<typename T>
inline void PackToBuffer::READ(T* pData)
{
	int dataSize = sizeof(T);
	if (CheckReadBoundary(dataSize)) {
		memcpy_s((void* const)(pData), dataSize, (const void* const)(m_Buffer + m_ReadPtr), dataSize);
		m_ReadPtr += dataSize;
	}
}