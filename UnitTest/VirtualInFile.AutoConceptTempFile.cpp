#include<string>
#include<array>
#include<vector>
#include<deque>
#include<set>
#include<list>
#include<forward_list>
#include<map>
#include<unordered_set>
#include<unordered_map>
#line 1
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
	bool foo(T1 x, T2 y)
	{
		if (x[5]) return false;
		return true;
	}
namespace AutoConceptProbing0{
	void tester(){
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<void,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<bool,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<char,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<char*,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<short,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<int,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<int*,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<long long,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<long long*,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<unsigned char,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<unsigned char*,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<unsigned short,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<unsigned int,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<unsigned int*,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<unsigned long long,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<unsigned long long*,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::string,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::array<int,8>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::vector<int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::deque<int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::list<int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::forward_list<int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::set<int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::multiset<int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::map<int,int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::multimap<int,int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::unordered_set<int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::unordered_multiset<int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::unordered_map<int,int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::unordered_multimap<int,int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::array<bool,8>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::vector<bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::deque<bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::list<bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::forward_list<bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::set<bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::multiset<bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::map<bool,bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::multimap<bool,bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::unordered_set<bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::unordered_multiset<bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::unordered_map<bool,bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<std::unordered_multimap<bool,bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,void>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,bool>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,char>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,char*>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,short>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,int*>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,long long>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,long long*>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,unsigned char>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,unsigned char*>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,unsigned short>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,unsigned int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,unsigned int*>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,unsigned long long>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,unsigned long long*>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::string>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::array<int,8>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::vector<int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::deque<int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::list<int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::forward_list<int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::set<int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::multiset<int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::map<int,int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::multimap<int,int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::unordered_set<int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::unordered_multiset<int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::unordered_map<int,int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::unordered_multimap<int,int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::array<bool,8>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::vector<bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::deque<bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::list<bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::forward_list<bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::set<bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::multiset<bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::map<bool,bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::multimap<bool,bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::unordered_set<bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::unordered_multiset<bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::unordered_map<bool,bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE13::foo<class std::basic_string<char>,std::unordered_multimap<bool,bool>>({},{});
		}
	}
#line 19


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
	bool foo2(T1 x, T2 y)
	{
		if (x[5]) return false;
		return true;
	}
namespace AutoConceptProbing1{
	void tester(){
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<void,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<bool,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<char,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<char*,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<short,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<int,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<int*,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<long long,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<long long*,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<unsigned char,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<unsigned char*,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<unsigned short,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<unsigned int,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<unsigned int*,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<unsigned long long,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<unsigned long long*,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::string,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::array<int,8>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::vector<int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::deque<int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::list<int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::forward_list<int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::set<int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::multiset<int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::map<int,int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::multimap<int,int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::unordered_set<int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::unordered_multiset<int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::unordered_map<int,int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::unordered_multimap<int,int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::array<bool,8>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::vector<bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::deque<bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::list<bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::forward_list<bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::set<bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::multiset<bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::map<bool,bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::multimap<bool,bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::unordered_set<bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::unordered_multiset<bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::unordered_map<bool,bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<std::unordered_multimap<bool,bool>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,void>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,bool>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,char>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,char*>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,short>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,int*>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,long long>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,long long*>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,unsigned char>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,unsigned char*>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,unsigned short>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,unsigned int>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,unsigned int*>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,unsigned long long>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,unsigned long long*>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::string>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::array<int,8>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::vector<int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::deque<int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::list<int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::forward_list<int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::set<int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::multiset<int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::map<int,int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::multimap<int,int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::unordered_set<int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::unordered_multiset<int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::unordered_map<int,int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::unordered_multimap<int,int>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::array<bool,8>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::vector<bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::deque<bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::list<bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::forward_list<bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::set<bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::multiset<bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::map<bool,bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::multimap<bool,bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::unordered_set<bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::unordered_multiset<bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::unordered_map<bool,bool>>({},{});
		AUTO_CONCEPT_TEST_UNIQUE_NAMESPACE30::foo2<class std::vector<int>,std::unordered_multimap<bool,bool>>({},{});
		}
	}
#line 36

	void test() {
		//foo(std::vector<int>{}, int{});
		foo2<std::vector<int>, int>({}, {});
		foo2<std::string, int>({}, {});
		//foo(1, 1);
		//foo(1, std::vector<int>{1});
	}
}
