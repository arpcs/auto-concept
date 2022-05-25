// [Comment]	This is a dynamically read test file
// [Comment]	It's possible to use it from the unit test framework
// [Comment]	As well as from the AutoConcept console app directly
// [Setup]
#define CONCAT_INDIRECT(x, y) x ## y
#define CONCAT(x, y) CONCAT_INDIRECT(x, y)
#define UNIQUE_NS CONCAT(AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE , __LINE__ )
#include <type_traits>
#include <concepts>
#include <math.h>
#include <ranges>
#include <vector>
using namespace std;
using namespace ranges;

// [Test]
// [Comment] This should be an arithmetic type, but chrono duration wont allow it, this will be out test base
// [Arg] -test-concept=is_trivial_v
// [Arg] -test-concept=is_arithmetic_v
namespace UNIQUE_NS {
	template<class T1>
	void foo(T1 x)
	{
		pow(x, 2);
		x *= 2.0;
	}
}
// [Expected]
// [Comment] So the more general conept trivial should appear here
namespace UNIQUE_NS {
	template<class T1>
	requires is_trivial_v<T1>
	void foo(T1 x)
	{
		pow(x, 2);
		x *= 2.0;
	}
}
// [Expected]
// [Comment] Now ignoring choro duration, it should recognize this as arithmetic
// [Arg] -ignore-type=class std::chrono::duration<int>
namespace UNIQUE_NS {
	template<class T1>
	requires is_arithmetic_v<T1>
	void foo(T1 x)
	{
		pow(x, 2);
		x *= 2.0;
	}
}
// [Expected]
// [Comment] Or allowing one spec to be prevented should also do the trick
// [Arg] -max-prevent=1
namespace UNIQUE_NS {
	template<class T1>
	requires is_arithmetic_v<T1>
	void foo(T1 x)
	{
		pow(x, 2);
		x *= 2.0;
	}
}
// [Expected]
// [Comment] Or specify that we dont want too general concepts
// [Arg] -min-prevent=25
namespace UNIQUE_NS {
	template<class T1>
	void foo(T1 x)
	{
		pow(x, 2);
		x *= 2.0;
	}
}
// [Test]
// [Comment] This should be a random_access_range type, try to test it
// [Arg] -test-concept=random_access_range
namespace UNIQUE_NS {
	template<class T1>
	void foo(T1 x)
	{
		x[2];
		x < x;
	}
}
// [Expected]
// [Comment] Initializer list is allowed here, so random_access_range should be wrong here
namespace UNIQUE_NS {
	template<class T1>
	void foo(T1 x)
	{
		x[2];
		x < x;
	}
}
// [Expected]
// [Comment] But if we ignore 1 incorrect type it should apply
// [Arg] -max-allow=1
namespace UNIQUE_NS {
	template<class T1>
	requires random_access_range<T1>
	void foo(T1 x)
	{
		x[2];
		x < x;
	}
}
// [Test]
// [Comment] Testing probe skipping, we only get information from user defined specializations
namespace UNIQUE_NS {
	template<class T1>
	void foo(T1 x)
	{
		if (x[2]) return;
	}
	void test() {
		foo(vector<int>{1, 2, 3});
		foo(vector<float>{1, 2, 3});
	}
}
// [Expected]
// [Comment] Here we have initialization list, so random_access_range or contiguous_range shouldn't apply
namespace UNIQUE_NS {
	template<class T1>
	void foo(T1 x)
	{
		if (x[2]) return;
	}
	void test() {
		foo(vector<int>{1, 2, 3});
		foo(vector<float>{1, 2, 3});
	}
}
// [Expected]
// [Comment] But now it should, because we skipped probing the function
// [Comment] The types also fit the more specific contiguous_range, so that should be our type
// [Arg] -skip-probing
namespace UNIQUE_NS {
	template<class T1>
	requires contiguous_range<T1>
	void foo(T1 x)
	{
		if (x[2]) return;
	}
	void test() {
		foo(vector<int>{1, 2, 3});
		foo(vector<float>{1, 2, 3});
	}
}