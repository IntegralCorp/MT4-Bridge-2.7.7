#pragma once

template<class T>
class CAutoHeapFree
{
public:
	CAutoHeapFree(T& objToFree) : m_FreeObject(objToFree)
	{

	}

	~CAutoHeapFree()
	{
		HEAP_FREE(m_FreeObject);
	}

private:
	T&   m_FreeObject;

	CAutoHeapFree();
	CAutoHeapFree(const CAutoHeapFree<T>& ol);
	CAutoHeapFree<T>& operator =(const CAutoHeapFree<T>& ol);
};