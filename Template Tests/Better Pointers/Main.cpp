#include <stdint.h>

#include <map>

struct SharedPointers {
public:
	static bool PointerExists(void* pointer) {
		if (!pointer) return false;
		auto itr = SharedPointers::pointers.find(pointer);
		return itr != SharedPointers::pointers.end();
	}

	static void CreateCountPointer(void* pointer) {
		if (!pointer) return;
		auto itr = SharedPointers::pointers.find(pointer);
		if (itr != SharedPointers::pointers.end()) return;
		auto res = SharedPointers::pointers.insert({ pointer, 0 });
	}

	static void IncCount(void* pointer) {
		if (!pointer) return;
		auto itr = SharedPointers::pointers.find(pointer);
		if (itr != SharedPointers::pointers.end()) itr->second++;
	}

	static bool DecCount(void* pointer) {
		if (!pointer) return false;
		auto itr = SharedPointers::pointers.find(pointer);
		if (itr != SharedPointers::pointers.end()) {
			itr->second--;
			bool ret = itr->second == 0;
			if (ret) SharedPointers::pointers.erase(itr);
			return ret;
		}
		return false;
	}

private:
	static std::map<void*, uint64_t> pointers;
};

std::map<void*, uint64_t> SharedPointers::pointers;

template <typename T, uint64_t Count>
struct SharedPointer {
public:
	SharedPointer() {
		this->ptr = new T[Count];
		SharedPointers::CreateCountPointer(this->ptr);
		SharedPointers::IncCount(this->ptr);
	}

	SharedPointer(T ptr[Count]) {
		this->ptr = ptr;
		SharedPointers::CreateCountPointer(this->ptr);
		SharedPointers::IncCount(this->ptr);
	}

	SharedPointer(const SharedPointer<T, Count>& copy) {
		this->ptr = copy.ptr;
		SharedPointers::IncCount(this->ptr);
	}

	~SharedPointer() {
		if (SharedPointers::DecCount(this->ptr)) delete[] this->ptr;
	}

	T* operator[](uint64_t index) {
		if (!SharedPointers::PointerExists(this->ptr)) {
			this->ptr = nullptr;
			return nullptr;
		}
		return this->ptr[index];
	}

	const T* operator[](uint64_t index) const {
		if (!SharedPointers::PointerExists(this->ptr)) {
			this->ptr = nullptr;
			return nullptr;
		}
		return this->ptr[index];
	}

private:
	T* ptr;
};

template <typename T>
struct SharedPointer<T, 1> {
public:
	template <typename... Args>
	SharedPointer(Args... args) {
		this->ptr = new T(args...);
		SharedPointers::CreateCountPointer(this->ptr);
		SharedPointers::IncCount(this->ptr);
	}

	SharedPointer(T* ptr) {
		this->ptr = ptr;
		SharedPointers::CreateCountPointer(this->ptr);
		SharedPointers::IncCount(this->ptr);
	}

	SharedPointer(const SharedPointer<T, 1>& copy) {
		this->ptr = copy.ptr;
		SharedPointers::IncCount(this->ptr);
	}

	~SharedPointer() {
		if (SharedPointers::DecCount(this->ptr)) delete this->ptr;
	}

	T* operator->() const {
		if (!SharedPointers::PointerExists(this->ptr)) this->ptr = nullptr;
		return this->ptr;
	}

	T* operator*() const {
		if (!SharedPointers::PointerExists(this->ptr)) this->ptr = nullptr;
		return this->ptr;
	}

private:
	T* ptr;
};

template <typename T>
struct SharedPointer<T, 0> {};

struct Vector3 {
public:
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

public:
	float x, y, z;
};

int main() {
	{
		SharedPointer<Vector3, 0> intPtr;
		{
			SharedPointer<Vector3, 0> otherIntPtr = intPtr;
		}
	}
}