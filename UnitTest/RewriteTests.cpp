
// Setup
/*template<typename T>
T f(T t);
void useIt() {
	f<int>(5);
}*/
#define CONCAT_INDIRECT(x, y) x ## y
#define CONCAT(x, y) CONCAT_INDIRECT(x, y)
#define UNIQUE_NS CONCAT(AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE , __LINE__ )

// Test [Source: Boost/core/include/boost/core/cmath.hpp]
#include <limits>
namespace UNIQUE_NS {
	template<class T> bool isfinite(T x)
	{
		return x <= (std::numeric_limits<T>::max)() && x >= -(std::numeric_limits<T>::max)();
	}
	void test() {isfinite(5);}
}
// Expected
#include <limits>
namespace UNIQUE_NS {
	template<class T> bool isfinite(T x)
	{
		if constexpr (true) return false; else
		return x <= (std::numeric_limits<T>::max)() && x >= -(std::numeric_limits<T>::max)();
	}
	void test() { isfinite(5); }
}