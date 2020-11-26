#include "SharedPointer.h"

struct Vector3 {
public:
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

public:
	float x, y, z;
};

int main() {
	{
		SharedPointer<Vector3, 1> intPtr{ 1.0f, 2.0f, 3.0f };
		{
			SharedPointer<Vector3, 1> otherIntPtr = intPtr;
		}
	}
}