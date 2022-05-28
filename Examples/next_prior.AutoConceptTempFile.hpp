//  Boost next_prior.hpp header file  ---------------------------------------//

//  (C) Copyright Dave Abrahams and Daniel Walker 1999-2003. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/utility for documentation.

//  Revision History
//  13 Dec 2003  Added next(x, n) and prior(x, n) (Daniel Walker)

#ifndef BOOST_NEXT_PRIOR_HPP_INCLUDED
#define BOOST_NEXT_PRIOR_HPP_INCLUDED

#include <iterator>

namespace boost {

//  Helper functions for classes like bidirectional iterators not supporting
//  operator+ and operator-
//
//  Usage:
//    const std::list<T>::iterator p = get_some_iterator();
//    const std::list<T>::iterator prev = boost::prior(p);
//    const std::list<T>::iterator next = boost::next(prev, 2);

//  Contributed by Dave Abrahams

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

} // namespace boost

#endif  // BOOST_NEXT_PRIOR_HPP_INCLUDED#include<string_view>
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

namespace AutoConceptProbingID_944319{
	void tester(){
		boost::next<void>({});
		boost::next<bool>({});
		boost::next<char>({});
		boost::next<char*>({});
		boost::next<short>({});
		boost::next<int>({});
		boost::next<int*>({});
		boost::next<float>({});
		boost::next<float*>({});
		boost::next<double>({});
		boost::next<double*>({});
		boost::next<long long>({});
		boost::next<long long*>({});
		boost::next<unsigned char>({});
		boost::next<unsigned char*>({});
		boost::next<unsigned short>({});
		boost::next<unsigned int>({});
		boost::next<unsigned int*>({});
		boost::next<unsigned long long>({});
		boost::next<unsigned long long*>({});
		boost::next<class std::basic_string<char>>({});
		boost::next<class std::array<int,8>>({});
		boost::next<class std::vector<int>>({});
		boost::next<class std::deque<int>>({});
		boost::next<class std::list<int>>({});
		boost::next<class std::forward_list<int>>({});
		boost::next<class std::set<int>>({});
		boost::next<class std::multiset<int>>({});
		boost::next<class std::map<int,int>>({});
		boost::next<class std::multimap<int,int>>({});
		boost::next<class std::unordered_set<int>>({});
		boost::next<class std::unordered_multiset<int>>({});
		boost::next<class std::unordered_map<int,int>>({});
		boost::next<class std::unordered_multimap<int,int>>({});
		boost::next<class std::unique_ptr<int>>({});
		boost::next<class std::shared_ptr<int>>({});
		boost::next<class std::any>({});
		boost::next<class std::bitset<4>>({});
		boost::next<class std::chrono::duration<int>>({});
		boost::next<class std::function<void(int)>>({});
		boost::next<class std::initializer_list<int>>({});
		boost::next<class std::optional<int>>({});
		boost::next<class std::variant<int, float>>({});
		boost::next<class std::basic_string_view<char>>({});
		boost::next<class std::span<int>>({});
	}
}
namespace AutoConceptProbingID_944573{
	void tester(){
		boost::prior<void>({});
		boost::prior<bool>({});
		boost::prior<char>({});
		boost::prior<char*>({});
		boost::prior<short>({});
		boost::prior<int>({});
		boost::prior<int*>({});
		boost::prior<float>({});
		boost::prior<float*>({});
		boost::prior<double>({});
		boost::prior<double*>({});
		boost::prior<long long>({});
		boost::prior<long long*>({});
		boost::prior<unsigned char>({});
		boost::prior<unsigned char*>({});
		boost::prior<unsigned short>({});
		boost::prior<unsigned int>({});
		boost::prior<unsigned int*>({});
		boost::prior<unsigned long long>({});
		boost::prior<unsigned long long*>({});
		boost::prior<class std::basic_string<char>>({});
		boost::prior<class std::array<int,8>>({});
		boost::prior<class std::vector<int>>({});
		boost::prior<class std::deque<int>>({});
		boost::prior<class std::list<int>>({});
		boost::prior<class std::forward_list<int>>({});
		boost::prior<class std::set<int>>({});
		boost::prior<class std::multiset<int>>({});
		boost::prior<class std::map<int,int>>({});
		boost::prior<class std::multimap<int,int>>({});
		boost::prior<class std::unordered_set<int>>({});
		boost::prior<class std::unordered_multiset<int>>({});
		boost::prior<class std::unordered_map<int,int>>({});
		boost::prior<class std::unordered_multimap<int,int>>({});
		boost::prior<class std::unique_ptr<int>>({});
		boost::prior<class std::shared_ptr<int>>({});
		boost::prior<class std::any>({});
		boost::prior<class std::bitset<4>>({});
		boost::prior<class std::chrono::duration<int>>({});
		boost::prior<class std::function<void(int)>>({});
		boost::prior<class std::initializer_list<int>>({});
		boost::prior<class std::optional<int>>({});
		boost::prior<class std::variant<int, float>>({});
		boost::prior<class std::basic_string_view<char>>({});
		boost::prior<class std::span<int>>({});
	}
}