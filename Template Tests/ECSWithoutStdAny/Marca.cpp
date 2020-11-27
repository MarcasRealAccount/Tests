#include "Scene.h"
#include "Component.h"

#include <iostream>
#include <chrono>

class SomeComponent : public Component {
public:
	virtual void SomeFunc() override {
		std::cout << "This is a component" << std::endl;
	}
};

int main() {
	Scene scene;

	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100000; i++) {
		Entity* entity = scene.AddChild<Entity>();
		entity->AddComponent<SomeComponent>();
	}
	auto end = std::chrono::high_resolution_clock::now();

	std::cout << ((end - start).count() / 1e9f) << std::endl;

	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 10; i++) {
		std::vector<Entity*> entities = scene.GetChildren<Entity>();
	}
	end = std::chrono::high_resolution_clock::now();

	std::cout << ((end - start).count() / 1e9f) << std::endl;
}