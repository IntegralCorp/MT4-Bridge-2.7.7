#pragma once

template <typename T> 
void atomicStore(std::atomic<T>& a, T val) {
	T prevVal;
	int cnt = 0;
	do {
		if (cnt > 1000) {
			std::this_thread::yield();
			cnt = 0;
		}
		prevVal = a.load(std::memory_order_acquire);
		cnt++;
	} while (!a.compare_exchange_weak(prevVal, val));
}


template <typename T> 
T atomicLoad(const std::atomic<T>& a) {
	return a.load(std::memory_order_acquire);
}


