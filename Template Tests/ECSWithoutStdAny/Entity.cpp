#include "Entity.h"
#include "Component.h"

Entity::~Entity() {
	{
		auto itr = this->children.begin();
		while (itr != this->children.end()) {
			Entity* entity = itr->Cast<Entity>();
			if (entity) delete entity;
			itr++;
		}
	}
	{
		auto itr = this->components.begin();
		while (itr != this->components.end()) {
			Component* component = itr->Cast<Component>();
			if (component) delete component;
			itr++;
		}
	}
}