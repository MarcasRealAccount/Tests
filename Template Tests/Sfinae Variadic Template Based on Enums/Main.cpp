class ClassOne {
public:
	template <typename... Args>
	void func(Args... args);
};

template <typename Arg0>
void ClassOne::func<Arg0>(Arg0 arg0) {

}

template <typename Arg0, typename Arg1>
void ClassOne::func<Arg0, Arg1>(Arg0 arg0, Arg1 arg1) {

}

int main() {
	ClassOne one;
	one.func(1);
	one.func(1, 2);
}