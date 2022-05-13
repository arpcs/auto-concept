
// Setup
/*template<typename T>
T f(T t);
void useIt() {
	f<int>(5);
}*/
#define CONCAT_INDIRECT(x, y) x ## y
#define CONCAT(x, y) CONCAT_INDIRECT(x, y)
#define UNIQUE_NS CONCAT(AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE , __LINE__ )

// Test
#include <limits>
#include <type_traits>
namespace UNIQUE_NS {
	template<class T1, class T2, class TWrong> 
	bool isfinite(T1 x, T2 y, TWrong z)
	{
		constexpr auto maxVal = std::numeric_limits<T1>::max();
		constexpr auto minVal = std::numeric_limits<T2>::min();
		return x <= maxVal && x >= -maxVal;
	}
	void test() { isfinite(5, 6, 7); }
}
// Expected
#include <limits>
#include <type_traits>
namespace UNIQUE_NS {
	template<class T1, class T2, class TWrong>
	requires std::is_arithmetic_v<T1>
	bool isfinite(T1 x, T2 y, TWrong z)
	{
		constexpr auto maxVal = std::numeric_limits<T1>::max();
		constexpr auto minVal = std::numeric_limits<T2>::min();
		return x <= maxVal && x >= -maxVal;
	}
	void test() { isfinite(5, 6, 7); }
}