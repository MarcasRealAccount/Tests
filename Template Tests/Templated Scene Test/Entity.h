#pragma once

#include <vector>
#include <any>
#include <type_traits>

class Component;

class Entity {
public:
	template <typename T> inline constexpr T* GetChild(uint32_t index);
	template <typename T> inline constexpr std::vector<T*> GetChildren();
	template <typename T, typename... Args> inline constexpr T* AddChild(Args... args);
	template <typename T, typename... Args> inline constexpr T* InsertChild(uint32_t index, Args... args);
	
	template <typename T> inline constexpr T* GetComponent();
	template <typename T> inline constexpr std::vector<T*> GetComponents();
	template <typename T, typename... Args> inline constexpr T* AddComponent(Args... args);

private:
	template <typename T> T* GetChildImplFalse();
	template <typename T> T* GetChildImplTrue(uint32_t index);

	template <typename T> std::vector<T*> GetChildrenImplFalse();
	template <typename T> std::vector<T*> GetChildrenImplTrue();

	template <typename T> T* AddChildImplFalse();
	template <typename T, typename... Args> T* AddChildImplTrue(Args... args);

	template <typename T> T* InsertChildImplFalse();
	template <typename T, typename... Args> T* InsertChildImplTrue(uint32_t index, Args... args);

	template <typename T> T* GetComponentImplFalse();
	template <typename T> T* GetComponentImplTrue();

	template <typename T> std::vector<T*> GetComponentsImplFalse();
	template <typename T> std::vector<T*> GetComponentsImplTrue();

	template <typename T> T* AddComponentImplFalse();
	template <typename T, typename... Args> T* AddComponentImplTrue(Args... args);

	template <bool B, typename T> inline constexpr T* GetChildImpl(uint32_t index);
	template <bool B, typename T> inline constexpr std::vector<T*> GetChildrenImpl();
	template <bool B, typename T, typename... Args> inline constexpr T* AddChildImpl(Args... args);
	template <bool B, typename T, typename... Args> inline constexpr T* InsertChildImpl(uint32_t index, Args... args);

	template <bool B, typename T> inline constexpr T* GetComponentImpl();
	template <bool B, typename T> inline constexpr std::vector<T*> GetComponentsImpl();
	template <bool B, typename T, typename... Args> inline constexpr T* AddComponentImpl(Args... args);

private:
	std::vector<std::any> children;

	std::vector<std::any> components;
};

#include "EntityImpl.h"