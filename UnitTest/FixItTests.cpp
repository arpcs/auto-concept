// [Comment]	This is a dynamically read test file
// [Comment]	It's possible to use it from the unit test framework
// [Comment]	As well as from the AutoConcept console app directly
// [Setup]
#define CONCAT_INDIRECT(x, y) x ## y
#define CONCAT(x, y) CONCAT_INDIRECT(x, y)
#define UNIQUE_NS CONCAT(AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE , __LINE__ )
#include <type_traits>
#include <vector>
#include <ranges>
using namespace std;
using namespace ranges;

// [Test]
// [Comment] Testing random acces range
// [Arg] -ignore-type=class std::initializer_list<int>
// [Arg] -test-concept=random_access_range
// [Arg] -test-concept=contiguous_range
namespace UNIQUE_NS {
	template<class T1>
	void foo(T1 x)
	{
		x[2];
	}
}
// [Expected]
// [Comment] This can still be a bitset so it shouldn't add concept
namespace UNIQUE_NS {
	template<class T1>
	void foo(T1 x)
	{
		x[2];
	}
}
// [Expected]
// [Comment] Now it should
// [Arg] -ignore-type=class std::bitset<4>
namespace UNIQUE_NS {
	template<class T1>
	requires random_access_range<T1>
	void foo(T1 x)
	{
		x[2];
	}
}
// [Test]
// [Comment] Testing random acces range with comparison
// [Arg] -ignore-type=class std::initializer_list<int>
namespace UNIQUE_NS {
	template<class T1>
	void foo(T1 x)
	{
		x < x;
		x[2];
	}
}
// [Expected]
// [Comment] Now this can't be a bitset
namespace UNIQUE_NS {
	template<class T1>
	requires random_access_range<T1>
	void foo(T1 x)
	{
		x < x;
		x[2];
	}
}
// [Test]
// [Comment] Testing integral point
namespace UNIQUE_NS {
	template<class T1>
	void foo(T1 x)
	{
		x <<= 2;
	}
}
// [Expected]
// [Comment] Now this again can be a bitset so we should get nothing
namespace UNIQUE_NS {
	template<class T1>
	void foo(T1 x)
	{
		x <<= 2;
	}
}
// [Expected]
// [Arg] -ignore-type=class std::bitset<4>
// [Comment] Now we should
namespace UNIQUE_NS {
	template<class T1>
	requires integral<T1>
	void foo(T1 x)
	{
		x <<= 2;
	}
}