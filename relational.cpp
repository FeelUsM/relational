#include <tuple>
#include <iostream>
#include <type_traits>
using namespace std;

template<template<size_t...> class T, size_t N, size_t ...Ns > struct 
Ns_impl{
	typedef typename Ns_impl<T,N-1,Ns... , sizeof...(Ns)>::type type;
};
template<template<size_t...> class T, size_t ...Ns > struct 
Ns_impl<T,0,Ns...>{
	typedef typename T<Ns...>::type type;
};
template<template<size_t...> class T, size_t N > struct 
Ns{
	typedef typename Ns_impl<T,N>::type type;
};

template<template<typename X, size_t...> class T, typename X, size_t N, size_t ...Ns > struct 
Ns_type_impl{
	typedef typename Ns_type_impl<T,X,N-1,Ns... , sizeof...(Ns)>::type type;
};
template<template<typename X, size_t...> class T, typename X, size_t ...Ns > struct 
Ns_type_impl<T,X,0,Ns...>{
	typedef typename T<X,Ns...>::type type;
};
template<template<typename X, size_t...> class T, typename X, size_t N > struct 
Ns_type{
	typedef typename Ns_type_impl<T,X,N>::type type;
};

template<
	template<typename X, typename Y, size_t...> class T, typename X, typename Y, 
	size_t N, size_t ...Ns > struct 
Ns_type_type_impl{
	typedef typename Ns_type_type_impl<T,X,Y,N-1,Ns... , sizeof...(Ns)>::type type;
};
template<
	template<typename X, typename Y, size_t...> class T, typename X, typename Y, 
	size_t ...Ns > struct 
Ns_type_type_impl<T,X,Y,0,Ns...>{
	typedef typename T<X,Y,Ns...>::type type;
};
template<
	template<typename X, typename Y, size_t...> class T, typename X, typename Y, 
	size_t N > struct 
Ns_type_type{
	typedef typename Ns_type_type_impl<T,X,Y,N>::type type;
};

template<
	template<template<typename...> class X, typename Y, size_t...> class T, 
	template<typename...> class X, typename Y, size_t N, size_t ...Ns > struct 
Ns_templ_type_impl{
	typedef typename Ns_templ_type_impl<T,X,Y,N-1,Ns... , sizeof...(Ns)>::type type;
};
template<
	template<template<typename...> class X, typename Y, size_t...> class T, 
	template<typename...> class X, typename Y, size_t ...Ns> struct 
Ns_templ_type_impl<T,X,Y,0,Ns...>{
	typedef typename T<X,Y,Ns...>::type type;
};
template<
	template<template<typename...> class X, typename Y, size_t...> class T, 
	template<typename...> class X, typename Y, size_t N > struct 
Ns_templ_type{
	typedef typename Ns_templ_type_impl<T,X,Y,N>::type type;
};

template<typename tuple_t, typename T, size_t ...Ns> struct
tuple_push_back_impl{
	typedef tuple<typename tuple_element<Ns,tuple_t>::type... , T> type;
};
template<typename tuple_t, typename T> struct
tuple_push_back{
	typedef typename Ns_type_type<tuple_push_back_impl,tuple_t,T,tuple_size<tuple_t>::value>::type type;
};

//======================================================
template<typename Data, typename ...cont>
using Row = pair<Data, tuple<typename cont::links...>>;

template<
	class row_t, 
	class tuple_t, 
	size_t N, 
	size_t sizeof_remain_cont,
	typename first_cont,
	typename ...cont
>
struct __tuple_cont{
	typedef typename __tuple_cont<
		row_t, 
		typename tuple_push_back<tuple_t,typename first_cont::template cont<N,row_t>>::type,
		N+1,
		sizeof...(cont),
		cont...
	>::type type;
};
template<
	class row_t, 
	class tuple_t, 
	size_t N,
	typename first_cont
>
struct __tuple_cont<row_t,tuple_t,N,1,first_cont>{
	typedef typename tuple_push_back<tuple_t,typename first_cont::template cont<N,row_t>>::type
		type_tuple;
};

template<typename Allocator, typename Data, typename ...cont>
class relational 
: public __tuple_cont<
	Row<Data,cont...>, 
	tuple<>,
	0,
	sizeof...(cont),
	cont...
>::type_tuple{
public:
	void test(){
		get<0>(*this).test();
	}
};
//======================================================

template <class getter_key/*, class key_comp*/ >
struct raw_map{
	struct links {
		void * left, * right;
		bool is_red;
	};
	template<size_t index, class Row>
	class cont{
		typedef typename Row::first_type value_type;
		static_assert(offsetof(Row,first)==0,"offsetof(Row,first)==0");
		int some_data[13];
	public:
		void test(){
			Row x;
			cout <<getter_key::get(x.first) <<endl;
		}
	};
};


#define DEFINE_GETTER(name)\
struct getter_##name{\
	template<typename T> static constexpr \
	typename add_lvalue_reference<typename add_const<decltype(T::name)>::type>::type \
	get(const T & x)\
	{ return x.name; }\
	template<typename T> static constexpr \
	typename add_lvalue_reference<decltype(T::name)>::type \
	get(T & x)\
	{ return x.name; }\
	template<typename T> static constexpr \
	typename add_rvalue_reference<decltype(T::name)>::type \
	get(T && x)\
	{ return forward<typename add_rvalue_reference<decltype(stru::a)>::type>(x.a); }\
};//
template<size_t __i>
struct tuple_getter{
  /// Return a reference to the ith element of a tuple.
  template<typename... _Elements> constexpr static
  tuple_element<__i, tuple<_Elements...>>&
    get(tuple<_Elements...>& __t) noexcept
    { return std::get<__i>(__t); }

  /// Return a const reference to the ith element of a const tuple.
  template<typename... _Elements> constexpr static
  const tuple_element<__i, tuple<_Elements...>>&
    get(const tuple<_Elements...>& __t) noexcept
    { return std::get<__i>(__t); }

  /// Return an rvalue reference to the ith element of a tuple rvalue.
  template<typename... _Elements> constexpr static
  tuple_element<__i, tuple<_Elements...>>&&
    get(tuple<_Elements...>&& __t) noexcept
    {
      typedef tuple_element<__i, tuple<_Elements...>> __element_type;
      return std::forward<__element_type&&>(std::get<__i>(__t));
    }

};
struct stru{
	int a, b;
	double c;
};

DEFINE_GETTER(a)
DEFINE_GET(b)
DEFINE_GET(c)

//class get_a{};

int main(){
	//relational<tuple<int,int,double>,raw_map<get<0>>> x;
	relational<std::allocator<int/*does not matter*/>,stru,raw_map<getter_a>> x;
	x.test();
	
	typedef tuple<int,double,char*> tuple1;
	typedef typename tuple_push_back<tuple1,int>::type tuple2;
	cout << tuple_size<tuple<>>::value << " " << tuple_size<tuple2>::value << " " << endl;
}