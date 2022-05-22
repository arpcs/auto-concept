#define CONCAT_INDIRECT(x, y) x ## y
#define CONCAT(x, y) CONCAT_INDIRECT(x, y)
#define UNIQUE_NS CONCAT(AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE , __LINE__ )
#include <limits>
#include <type_traits>
#include <vector>
#include <ranges>
#include <string>
#include <array>
using namespace std;
using namespace ranges;

namespace UNIQUE_NS {
	template<class T1, class T2>
	requires signed_integral<T2> 
	bool foo(T1 x, T2 y)
	{
		if (x[5]) return false;
		return true;
	}

	void test() {
		//foo(std::vector<int>{}, int{});
		foo<std::string, int>({}, {});
		foo<std::vector<int>, int>({}, {});
		foo<int,int>({}, {});
		//foo(1, 1);
		//foo(1, std::vector<int>{1});
	}
}
namespace UNIQUE_NS {
	template<class T1, class T2>
	requires signed_integral<T2> 
	bool foo2(T1 x, T2 y)
	{
		if (x[5]) return false;
		return true;
	}
	void test() {
		//foo(std::vector<int>{}, int{});
		foo2<std::vector<int>, int>({}, {});
		foo2<std::string, int>({}, {});
		//foo(1, 1);
		//foo(1, std::vector<int>{1});
	}
}
