#define CONCAT_INDIRECT(x, y) x ## y
#define CONCAT(x, y) CONCAT_INDIRECT(x, y)
#define UNIQUE_NS CONCAT(AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE , __LINE__ )
#include <limits>
#include <type_traits>
#include <vector>
#include <ranges>
#include <string>
#include <array>
#include <bitset>
using namespace std;
using namespace ranges;

namespace UNIQUE_NS {
	template<class T1>
	void foo(T1 x)
	{
		x[2];
	}
}
#include<string_view>
#include<string>
#include<variant>
#include<array>
#include<vector>
#include<deque>
#include<set>
#include<list>
#include<forward_list>
#include<map>
#include<unordered_set>
#include<unordered_map>
#include<memory>
#include<any>
#include<bitset>
#include<chrono>
#include<functional>
#include<initializer_list>
#include<optional>
#include<span>

namespace AutoConceptProbingID_3529517{
	void tester(){
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<void>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<bool>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<char>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<char*>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<short>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<int>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<int*>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<long long>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<long long*>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<unsigned char>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<unsigned char*>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<unsigned short>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<unsigned int>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<unsigned int*>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<unsigned long long>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<unsigned long long*>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::basic_string<char>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::array<int,8>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::vector<int>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::deque<int>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::list<int>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::forward_list<int>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::set<int>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::multiset<int>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::map<int,int>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::multimap<int,int>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::unordered_set<int>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::unordered_multiset<int>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::unordered_map<int,int>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::unordered_multimap<int,int>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::unique_ptr<int>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::shared_ptr<int>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::any>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::bitset<4>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::chrono::duration<int>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::function<void(int)>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::initializer_list<int>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::optional<int>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::variant<int, float>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::basic_string_view<char>>({});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE14::foo<class std::span<int>>({});
	}
}