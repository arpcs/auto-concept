#include <vector>
#include <array>
#include <concepts>
#include <ranges>
#include <deque>

using namespace std;
using namespace std::ranges;

template<typename T>
void foo(T x)
{
	if (x[2]) return;
}
template<typename T1, typename T2>
void bar(T1 x, T2 y)
{
	y++;
	if (x[y]) return;
}

void test() {
	foo(vector<int>{1, 2, 3});
	bar(vector<int>{1, 2, 3}, 2);
}
