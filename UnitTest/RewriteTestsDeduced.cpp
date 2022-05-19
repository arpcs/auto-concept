/*
//Test
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
//Expected
namespace UNIQUE_NS {
	template<class T1, class T2, class TWrong>
	requires std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2>
	bool isfinite(T1 x, T2 y, TWrong z)
	{
		constexpr auto maxVal = std::numeric_limits<T1>::max();
		constexpr auto minVal = std::numeric_limits<T2>::min();
		return x <= maxVal && x >= -maxVal;
	}
	void test() { isfinite(5, 6, 7); }
}
*/
// Setup
#define CONCAT_INDIRECT(x, y) x ## y
#define CONCAT(x, y) CONCAT_INDIRECT(x, y)
#define UNIQUE_NS CONCAT(AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE , __LINE__ )
#include <limits>
#include <type_traits>
#include <vector>
#include <ranges>
using namespace std;
using namespace ranges;

// Test
namespace UNIQUE_NS {
	template<class T1, class T2>
	bool foo(T1 x, T2 y)
	{
		if (x[5]) return false;
		return true;
	}
	void test() {
		foo(std::vector<int>{1}, 1);
		foo(1, 1);
	}
}
// Expected
namespace UNIQUE_NS {
	template<class T1, class T2>
	requires contiguous_range<T1> && signed_integral<T2> 
	bool foo(T1 x, T2 y)
	{
		if (x[5]) return false;
		return true;
	}
	void test() {
		foo(std::vector<int>{1}, 1);
		foo(1, 1);
	}
}