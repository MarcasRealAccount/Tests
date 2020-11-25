#include "Scene.h"
#include "Component.h"

#include <iostream>
#include <chrono>

int main() {
	Scene scene;

	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000; i++) {
		scene.AddChild<Entity>();
	}
	auto end = std::chrono::high_resolution_clock::now();

	std::cout << ((end - start).count() / 1e9f) << std::endl;

	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 1000; i++) {
		std::vector<Entity*> entities = scene.GetChildren<Entity>();
	}
	end = std::chrono::high_resolution_clock::now();

	std::cout << ((end - start).count() / 1e9f) << std::endl;
}