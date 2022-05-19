#define CONCAT_INDIRECT(x, y) x ## y
#define CONCAT(x, y) CONCAT_INDIRECT(x, y)
#define UNIQUE_NS CONCAT(AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE , __LINE__ )
#include <limits>
#include <type_traits>
#include <vector>
#include <ranges>
using namespace std;
using namespace ranges;

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
