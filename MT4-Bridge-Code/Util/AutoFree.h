#pragma once

#include <functional>
#include <iostream>

class CAutoFree
{
public:
	CAutoFree(std::function<void()>& freeFunc) : oFreeFunc(freeFunc)
	{

	}

	~CAutoFree()
	{
		oFreeFunc();
	}

private:
	std::function<void()>&   oFreeFunc;

	CAutoFree();
	CAutoFree(const CAutoFree& ol);
	CAutoFree& operator =(const CAutoFree& ol);
};