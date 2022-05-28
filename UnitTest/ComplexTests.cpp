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
// [Comment] Testing more functions at once
// [Arg] -test-concept=is_trivial_v
namespace UNIQUE_NS {
	template <class T>
	inline T next(T x) { return ++x; }
	template <class T, class Distance>
	inline T next(T x, Distance n)
	{
		std::advance(x, n);
		return x;
	}
	template <class T>
	inline T prior(T x) { return --x; }
	template <class T, class Distance>
	inline T prior(T x, Distance n)
	{
		std::advance(x, -n);
		return x;
	}
}
// [Expected]
// [Comment] The correct types should appear at the correct position
namespace UNIQUE_NS {
	template <class T>
	requires is_trivial_v<T>
	inline T next(T x) { return ++x; }
	template <class T, class Distance>
	inline T next(T x, Distance n)
	{
		std::advance(x, n);
		return x;
	}
	template <class T>
	requires is_trivial_v<T>
	inline T prior(T x) { return --x; }
	template <class T, class Distance>
	inline T prior(T x, Distance n)
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
	inline T next(T x) { return ++x; }
	template <class T, class Distance>
	inline T next(T x, Distance n)
	{
		std::advance(x, n);
		return x;
	}
	template <class T>
	requires is_arithmetic_v<T>
	inline T prior(T x) { return --x; }
	template <class T, class Distance>
	inline T prior(T x, Distance n)
	{
		std::advance(x, -n);
		return x;
	}
}
// [Test]
// [Comment] Testing multiple arguments
namespace UNIQUE_NS {
	template <class T>
	inline T next(T x) { return next(x, 1); }
	template <class T, class Distance>
	inline T next(T x, Distance n)
	{
		*x; ++x;
		std::advance(x, n);
		return x;
	}
	template <class T, class Distance>
	inline T prior(T x, Distance n)
	{
		*x; ++x;
		std::advance(x, -n);
		return x;
	}
	template <class T>
	inline T prior(T x) { return prior(x, 1); }
}
// [Expected]
// [Comment] The correct types should appear at the correct positions for each parameter. ( Chaining is not yet implemented )
// [Arg] -test-concept=input_or_output_iterator
namespace UNIQUE_NS {
	template <class T>
	requires copy_constructible<T>
	inline T next(T x) { return next(x, 1); }
	template <class T, class Distance>
	requires input_or_output_iterator<T> && signed_integral<Distance>
	inline T next(T x, Distance n)
	{
		*x; ++x;
		std::advance(x, n);
		return x;
	}
	template <class T, class Distance>
	requires input_or_output_iterator<T> && signed_integral<Distance>
	inline T prior(T x, Distance n)
	{
		*x; ++x;
		std::advance(x, -n);
		return x;
	}
	template <class T>
	requires copy_constructible<T>
	inline T prior(T x) { return prior(x, 1); }
}