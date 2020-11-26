#pragma once

#include "SharedPointer.h"

#include <map>

namespace SharedPointerData {
	static std::map<void*, uint64_t> pointers;

	static bool PointerExists(void* pointer) {
		if (!pointer) return false;
		auto itr = pointers.find(pointer);
		return itr != pointers.end();
	}

	static void CreateCount(void* pointer) {
		if (!pointer) return;
		auto itr = pointers.find(pointer);
		if (itr != pointers.end()) return;
		pointers.insert({ pointer, 0 });
	}

	static void IncCount(void* pointer) {
		if (!pointer) return;
		auto itr = pointers.find(pointer);
		if (itr != pointers.end()) itr->second++;
	}

	static bool DecCount(void* pointer) {
		if (!pointer) return false;
		auto itr = pointers.find(pointer);
		if (itr != pointers.end()) {
			itr->second--;
			bool ret = itr->second == 0;
			if (ret) pointers.erase(itr);
			return ret;
		}
		return false;
	}
}

template <typename T, uint64_t Count>
SharedPointer<T, Count>::SharedPointer() {
	this->ptr = new T[Count];
	SharedPointerData::CreateCount(this->ptr);
	SharedPointerData::IncCount(this->ptr);
}

template <typename T, uint64_t Count>
SharedPointer<T, Count>::SharedPointer(T ptr[Count]) {
	this->ptr = ptr;
	SharedPointerData::CreateCount(this->ptr);
	SharedPointerData::IncCount(this->ptr);
}

template <typename T, uint64_t Count>
SharedPointer<T, Count>::SharedPointer(const SharedPointer<T, Count>& copy) {
	this->ptr = copy.ptr;
	SharedPointerData::IncCount(this->ptr);
}

template <typename T, uint64_t Count>
SharedPointer<T, Count>::~SharedPointer() {
	if (SharedPointerData::DecCount(this->ptr)) delete[] this->ptr;
}

template <typename T, uint64_t Count>
T* SharedPointer<T, Count>::operator[](uint64_t index) {
	if (!SharedPointerData::PointerExists(this->ptr)) {
		this->ptr = nullptr;
		return nullptr;
	}
	return this->ptr[index];
}

template <typename T, uint64_t Count>
const T* SharedPointer<T, Count>::operator[](uint64_t index) const {
	if (!SharedPointerData::PointerExists(this->ptr)) {
		this->ptr = nullptr;
		return nullptr;
	}
	return this->ptr[index];
}

template <typename T>
template <typename... Args>
SharedPointer<T, 1>::SharedPointer(Args... args) {
	this->ptr = new T(args...);
	SharedPointerData::CreateCount(this->ptr);
	SharedPointerData::IncCount(this->ptr);
}

template <typename T>
SharedPointer<T, 1>::SharedPointer(T* ptr) {
	this->ptr = ptr;
	SharedPointerData::CreateCount(this->ptr);
	SharedPointerData::IncCount(this->ptr);
}

template <typename T>
SharedPointer<T, 1>::SharedPointer(const SharedPointer<T, 1>& copy) {
	this->ptr = copy.ptr;
	SharedPointerData::IncCount(this->ptr);
}

template <typename T>
SharedPointer<T, 1>::~SharedPointer() {
	if (SharedPointerData::DecCount(this->ptr)) delete this->ptr;
}

template <typename T>
T* SharedPointer<T, 1>::operator->() {
	if (!SharedPointerData::PointerExists(this->ptr)) this->ptr = nullptr;
	return this->ptr;
}

template <typename T>
const T* SharedPointer<T, 1>::operator->() const {
	if (!SharedPointerData::PointerExists(this->ptr)) this->ptr = nullptr;
	return this->ptr;
}