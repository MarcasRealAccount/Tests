#pragma once

#include "Entity.h"

template <typename T>
constexpr T* Entity::GetChild(uint32_t index) {
	return GetChildImpl<std::is_base_of<Entity, T>::value, T>();
}

template <typename T>
constexpr std::vector<T*> Entity::GetChildren() {
	return GetChildrenImpl<std::is_base_of<Entity, T>::value, T>();
}

template <typename T, typename... Args>
constexpr T* Entity::AddChild(Args... args) {
	return AddChildImpl<std::is_base_of<Entity, T>::value, T, Args...>(args...);
}

template <typename T, typename... Args>
constexpr T* Entity::InsertChild(uint32_t index, Args... args) {
	return InsertChildImpl<std::is_base_of<Entity, T>::value, T, Args...>(args...);
}

template <typename T>
constexpr T* Entity::GetComponent() {
	return GetComponentImpl<std::is_base_of<Component, T>::value, T>();
}

template<typename T>
constexpr std::vector<T*> Entity::GetComponents() {
	return GetComponentsImpl<std::is_base_of<Component, T>::value, T>();
}

template <typename T, typename... Args>
constexpr T* Entity::AddComponent(Args... args) {
	return AddComponentImpl<std::is_base_of<Component, T>::value, T, Args...>(args...);
}

template <typename T>
T* Entity::GetChildImplFalse() {
	return nullptr;
}

template <typename T>
T* Entity::GetChildImplTrue(uint32_t index) {
	if (index < this->children.size()) {
		auto itr = this->children.begin() + index;
		return itr->CastChecked<T>();
	}
	return nullptr;
}

template <typename T>
std::vector<T*> Entity::GetChildrenImplFalse() {
	return {};
}

template <typename T>
std::vector<T*> Entity::GetChildrenImplTrue() {
	std::vector<T*> entities;
	auto itr = this->children.begin();
	while (itr != this->children.end()) {
		if (itr->IsOfType<T>()) entities.push_back(itr->CastChecked<T>());
		itr++;
	}
	return entities;
}

template <typename T>
T* Entity::AddChildImplFalse() {
	return nullptr;
}

template <typename T, typename... Args>
T* Entity::AddChildImplTrue(Args... args) {
	T* t = new T(args...);
	this->children.push_back({ t });
	return t;
}

template <typename T>
T* Entity::InsertChildImplFalse() {
	return nullptr;
}

template <typename T, typename... Args>
T* Entity::InsertChildImplTrue(uint32_t index, Args... args) {
	T* t = new T(args...);
	std::vector<T*>::iterator itr;
	if (index < this->children.size()) {
		itr = this->children.begin() + index;
	} else {
		itr = this->children.end();
	}
	this->children.insert(itr, { t });
	return t;
}

template <typename T>
T* Entity::GetComponentImplFalse() {
	return nullptr;
}

template <typename T>
T* Entity::GetComponentImplTrue() {
	auto itr = this->components.begin();
	while (itr != this->components.end()) {
		if (itr->IsOfType<T>()) return itr->CastChecked<T>();
		itr++;
	}
	return nullptr;
}

template <typename T>
std::vector<T*> Entity::GetComponentsImplFalse() {
	return {};
}

template <typename T>
std::vector<T*> Entity::GetComponentsImplTrue() {
	std::vector<T*> comps;
	auto itr = this->components.begin();
	while (itr != this->components.end()) {
		if (itr->IsOfType<T>()) comps.push_back(itr->CastChecked<T>());
		itr++;
	}
	return comps;
}

template <typename T>
T* Entity::AddComponentImplFalse() {
	return nullptr;
}

template <typename T, typename... Args>
T* Entity::AddComponentImplTrue(Args... args) {
	T* t = new T(args...);
	this->components.push_back({ t });
	return t;
}

template <bool B, typename T>
constexpr T* Entity::GetChildImpl(uint32_t index) {
	if constexpr (B) return GetChildImplTrue<T>(index);
	else return GetChildImplFalse<T>();
}

template <bool B, typename T>
constexpr std::vector<T*> Entity::GetChildrenImpl() {
	if constexpr (B) return GetChildrenImplTrue<T>();
	else return GetChildrenImplFalse<T>();
}

template <bool B, typename T, typename... Args>
constexpr T* Entity::AddChildImpl(Args... args) {
	if constexpr (B) return AddChildImplTrue<T, Args...>(args...);
	else return AddChildImplFalse<T>();
}

template <bool B, typename T, typename... Args>
constexpr T* Entity::InsertChildImpl(uint32_t index, Args... args) {
	if constexpr (B) return InsertChildImplTrue<T, Args...>(args...);
	else return InsertChildImplFalse<T>();
}

template <bool B, typename T>
constexpr T* Entity::GetComponentImpl() {
	if constexpr (B) return GetComponentImplTrue<T>();
	else return GetComponentImplFalse<T>();
}

template <bool B, typename T>
constexpr std::vector<T*> Entity::GetComponentsImpl() {
	if constexpr (B) return GetComponentsImplTrue<T>();
	else return GetComponentsImplFalse<T>();
}

template <bool B, typename T, typename... Args>
constexpr T* Entity::AddComponentImpl(Args... args) {
	if constexpr (B) return AddComponentImplTrue<T, Args...>(args...);
	else return AddComponentImplFalse<T>();
}