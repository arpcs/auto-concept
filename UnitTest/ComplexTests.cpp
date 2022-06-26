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
#include <bitset>
#include <algorithm>
using namespace std;
using namespace ranges;
// [Test]
// [Comment] Testing multiple template parameters
// [Arg] -max-allow=2
// [Arg] -max-prevent=1
namespace UNIQUE_NS {
	template<class T1, class T2>
	void foo(T1 x, T2 y)
	{
		if (x.size() == 0) return;
		if (*(y.begin()) > 0) return;
		y[2] = y[2];
		*y.begin() = y[2];
	}
	void test() {
		foo(vector<long long>{1, 2, 3}, vector<long long>{1, 2, 3});
	}
}
// [Expected]
// [Comment] Both concepts should appear
namespace UNIQUE_NS {
	template<class T1, class T2>
	requires sized_range<T1> && contiguous_range<T2>
	void foo(T1 x, T2 y)
	{
		if (x.size() == 0) return;
		if (*(y.begin()) > 0) return;
		y[2] = y[2];
		*y.begin() = y[2];
	}
	void test() {
		foo(vector<long long>{1, 2, 3}, vector<long long>{1, 2, 3});
	}
}
// [Test]
// [Comment] Testing more functions at once
// [Arg] -ignore-type=bool
// [Arg] -test-concept=is_trivial_v
// [Arg] -test-concept=is_arithmetic_v
namespace UNIQUE_NS {
	template <class T>
	inline T foo(T x) {
		x *= 2;
		return ++x;
	}

	template <class T, class Distance>
	inline T foo(T x, Distance n)
	{
		std::advance(x, n);
		return x;
	}

	template <class T>
	inline T bar(T x) {
		x *= 2;
		return --x;
	}

	template <class T, class Distance>
	inline T bar(T x, Distance n)
	{
		std::advance(x, -n);
		return x;
	}
}
// [Expected]
// [Comment] Nothing should appear because of chrono
namespace UNIQUE_NS {
	template <class T>
	inline T foo(T x) {
		x *= 2;
		return ++x;
	}

	template <class T, class Distance>
	inline T foo(T x, Distance n)
	{
		std::advance(x, n);
		return x;
	}

	template <class T>
	inline T bar(T x) {
		x *= 2;
		return --x;
	}

	template <class T, class Distance>
	inline T bar(T x, Distance n)
	{
		std::advance(x, -n);
		return x;
	}
}
// [Expected]
// [Comment] The correct types should appear at the correct position, even with command line options
// [Arg] -ignore-type=class std::chrono::duration<int>
namespace UNIQUE_NS {
	template <class T>
	requires is_arithmetic_v<T>
	inline T foo(T x) {
		x *= 2;
		return ++x;
	}

	template <class T, class Distance>
	inline T foo(T x, Distance n)
	{
		std::advance(x, n);
		return x;
	}

	template <class T>
	requires is_arithmetic_v<T>
	inline T bar(T x) {
		x *= 2;
		return --x;
	}

	template <class T, class Distance>
	inline T bar(T x, Distance n)
	{
		std::advance(x, -n);
		return x;
	}
}
// [Test]
// [Comment] Testing multiple arguments
namespace UNIQUE_NS {
	template <class T>
	inline T foo(T x) {
		return foo(x, 1);
	}

	template <class T, class T2>
	inline T foo(T x, T2 n)
	{
		*x; ++x;
		std::find(x, x, n);
		return x;
	}

	template <class T, class T2>
	inline T bar(T x, T2 n)
	{
		*x; ++x;
		std::sort(x, x);
		return x;
	}

	template <class T>
	inline T bar(T x) { return bar(x, 1); }
}
// [Expected]
// [Comment] The correct types should appear at the correct positions for each parameter. ( Chaining is not yet implemented )
// [Arg] -test-concept=input_or_output_iterator
// [Arg] -test-concept=is_pointer_v
// [Arg] -keep-temp-files
namespace UNIQUE_NS {
	template <class T>
	requires semiregular<T>
	inline T foo(T x) {
		return foo(x, 1);
	}

	template <class T, class T2>
	requires input_iterator<T>
	inline T foo(T x, T2 n)
	{
		*x; ++x;
		std::find(x, x, n);
		return x;
	}

	template <class T, class T2>
	requires input_iterator<T>
	inline T bar(T x, T2 n)
	{
		*x; ++x;
		std::sort(x, x);
		return x;
	}

	template <class T>
	requires semiregular<T>
	inline T bar(T x) { return bar(x, 1); }
}