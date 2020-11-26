#pragma once

#include <stdint.h>

template <typename T, uint64_t Count>
struct SharedPointer {
public:
	SharedPointer();
	SharedPointer(T ptr[Count]);
	SharedPointer(const SharedPointer<T, Count>& copy);
	~SharedPointer();

	T* operator[](uint64_t index);
	const T* operator[](uint64_t index) const;

private:
	T* ptr;
};

template <typename T>
struct SharedPointer<T, 1> {
public:
	template <typename... Args>
	SharedPointer(Args... args);
	SharedPointer(T* ptr);
	SharedPointer(const SharedPointer<T, 1>& copy);
	~SharedPointer();

	T* operator->();
	const T* operator->() const;

private:
	T* ptr;
};

template <typename T>
struct SharedPointer<T, 0> {};

#include "SharedPointerImpl.h"