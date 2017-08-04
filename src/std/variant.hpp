// -*- C++ -*-
#ifndef _RUOKA_VARIANT
#define _RUOKA_VARIANT

/*

// Header <variant> synopsis

namespace std {
// 20.7.2, variant of value types
template <class... Types>
class variant;
// 20.7.3, variant helper classes
template <class T>
struct variant_size; // undefined
template <class T>
struct variant_size<const T>;
template <class T>
struct variant_size<volatile T>;
template <class T>
struct variant_size<const volatile T>;
template <class T>
constexpr size_t variant_size_v = variant_size<T>::value;
template <class... Types>
struct variant_size<variant<Types...>>;
template <size_t I, class T>
struct variant_alternative; // undefined
template <size_t I, class T>
struct variant_alternative<I, const T>;
template <size_t I, class T>
struct variant_alternative<I, volatile T>;
template <size_t I, class T>
struct variant_alternative<I, const volatile T>;
template <size_t I, class T>
using variant_alternative_t = typename variant_alternative<I, T>::type;
template <size_t I, class... Types>
struct variant_alternative<I, variant<Types...>>;
constexpr size_t variant_npos = -1;
// 20.7.4, value access
template <class T, class... Types>
constexpr bool holds_alternative(const variant<Types...>&) noexcept;
template <size_t I, class... Types>
constexpr variant_alternative_t<I, variant<Types...>>&
get(variant<Types...>&);
template <size_t I, class... Types>
constexpr variant_alternative_t<I, variant<Types...>>&&
get(variant<Types...>&&);
template <size_t I, class... Types>
constexpr variant_alternative_t<I, variant<Types...>> const&
get(const variant<Types...>&);
template <size_t I, class... Types>
constexpr variant_alternative_t<I, variant<Types...>>
const&& get(const variant<Types...>&&);
template <class T, class... Types>
constexpr T& get(variant<Types...>&);
template <class T, class... Types>
constexpr T&& get(variant<Types...>&&);
template <class T, class... Types>
constexpr const T& get(const variant<Types...>&);
template <class T, class... Types>
constexpr const T&& get(const variant<Types...>&&);
template <size_t I, class... Types>
constexpr add_pointer_t<variant_alternative_t<I, variant<Types...>>>
get_if(variant<Types...>*) noexcept;
template <size_t I, class... Types>
constexpr add_pointer_t<const variant_alternative_t<I, variant<Types...>>>
get_if(const variant<Types...>*) noexcept;
template <class T, class... Types>
constexpr add_pointer_t<T>
get_if(variant<Types...>*) noexcept;
template <class T, class... Types>
constexpr add_pointer_t<const T>
get_if(const variant<Types...>*) noexcept;
// 20.7.5, relational operators
template <class... Types>
constexpr bool operator==(const variant<Types...>&,
                          const variant<Types...>&);
template <class... Types>
constexpr bool operator!=(const variant<Types...>&,
                          const variant<Types...>&);
template <class... Types>
constexpr bool operator<(const variant<Types...>&,
                         const variant<Types...>&);
template <class... Types>
constexpr bool operator>(const variant<Types...>&,
                         const variant<Types...>&);
template <class... Types>
constexpr bool operator<=(const variant<Types...>&,
                          const variant<Types...>&);
template <class... Types>
constexpr bool operator>=(const variant<Types...>&,
                          const variant<Types...>&);
// 20.7.6, visitation
// template <class Visitor, class... Variants>
// constexpr see below visit(_Visitor&&, Variants&&...);
// 20.7.7, class monostate
struct monostate;
// 20.7.8, monostate relational operators
constexpr bool operator<(monostate, monostate) noexcept;
constexpr bool operator>(monostate, monostate) noexcept;
constexpr bool operator<=(monostate, monostate) noexcept;
constexpr bool operator>=(monostate, monostate) noexcept;
constexpr bool operator==(monostate, monostate) noexcept;
constexpr bool operator!=(monostate, monostate) noexcept;
// 20.7.9, specialized algorithms
template <class... Types>
void swap(variant<Types...>&, variant<Types...>&) noexcept; //(see below)
// 20.7.10, class bad_variant_access
class bad_variant_access;
// 20.7.11, hash support
// template <class T> struct hash;
template <class... Types> struct hash<variant<Types...>>;
template <> struct hash<monostate>;
// 20.7.12, allocator-related traits
// template <class T, class Alloc> struct uses_allocator;
template <class... Types, class Alloc>
struct uses_allocator<variant<Types...>, Alloc>;
} // namespace std

namespace std {
template <class... Types>
class variant {
public:
  // 20.7.2.1, constructors
  constexpr variant() noexcept; //(see below)
  variant(const variant&);
  variant(variant&&) noexcept; //(see below)
  template <class T>
  constexpr variant(_T&&) noexcept; //(see below)
  template <class T, class... Args>
  constexpr explicit variant(in_place_type_t<T>, Args&&...);
  template <class T, class U, class... Args>
  constexpr explicit variant(in_place_type_t<T>, initializer_list<U>, Args&&...);
  template <size_t I, class... Args>
  constexpr explicit variant(in_place_index_t<I>, Args&&...);
  template <size_t I, class U, class... Args>
  constexpr explicit variant(in_place_index_t<I>, initializer_list<U>, Args&&...);
  // allocator-extended constructors
  template <class Alloc>
  variant(allocator_arg_t, const Alloc&);
  template <class Alloc>
  variant(allocator_arg_t, const Alloc&, const variant&);
  template <class Alloc>
  variant(allocator_arg_t, const Alloc&, variant&&);
  template <class Alloc, class T>
  variant(allocator_arg_t, const Alloc&, T&&);
  template <class Alloc, class T, class... Args>
  variant(allocator_arg_t, const Alloc&, in_place_type_t<T>, Args&&...);
  template <class Alloc, class T, class U, class... Args>
  variant(allocator_arg_t, const Alloc&, in_place_type_t<T>, initializer_list<U>, Args&&...);
  template <class Alloc, size_t I, class... Args>
  variant(allocator_arg_t, const Alloc&, in_place_index_t<I>, Args&&...);
  template <class Alloc, size_t I, class U, class... Args>
  variant(allocator_arg_t, const Alloc&, in_place_index_t<I>, initializer_list<U>, Args&&...);
  // 20.7.2.2, destructor
  ~variant();
  // 20.7.2.3, assignment
  variant& operator=(const variant&);
  variant& operator=(variant&&) noexcept; //(see below)
  template <class T> variant& operator=(_T&&) noexcept; //(see below)
  // 20.7.2.4, modifiers
  template <class T, class... Args>
  void emplace(_Args&&...);
  template <class T, class U, class... Args>
  void emplace(initializer_list<U>, Args&&...);
  template <size_t I, class... Args>
  void emplace(_Args&&...);
  template <size_t I, class U, class... Args>
  void emplace(initializer_list<U>, Args&&...);
  // 20.7.2.5, value status
  constexpr bool valueless_by_exception() const noexcept;
  constexpr size_t index() const noexcept;
  // 20.7.2.6, swap
  void swap(variant&) noexcept; //(see below)
  };
} // namespace std

*/

#include <tuple>
#include <array>
#include <utility>
#include <functional>
#include <cassert>

// UNSUPPORTED: c++98, c++03, c++11, c++14

namespace std {

// 20.7.10, class bad_variant_access
class bad_variant_access : logic_error {
public:
  bad_variant_access() : logic_error{"Bad variant Access"} {};
  virtual ~bad_variant_access() noexcept {};
};

constexpr size_t variant_npos = -1;

template <class _Type, class... _Types>
union __variant_storage; // undefined

struct __trivial{};

struct __non_trivial{};

template <class _Type>
union __variant_storage<_Type,__trivial>
{
  _Type __m_type;

  constexpr __variant_storage() : __m_type{}
  {}

  template<class... _Args>
  constexpr __variant_storage(in_place_type_t<_Type>, _Args&&... __args) :
    __m_type{forward<_Args>(__args)...}
  {}
};

template <class _Type>
union __variant_storage<_Type,__non_trivial>
{
  aligned_storage_t<sizeof(_Type), alignof(_Type)> __m_storage;

  constexpr __variant_storage() : __m_storage{}
  {}

  template<class... _Args>
  __variant_storage(in_place_type_t<_Type>, _Args&&... __args) :
    __m_storage{}
  {
    new(&__m_storage)_Type(forward<_Args>(__args)...);
  }
};

template <>
union __variant_storage<void,__trivial>
{
  constexpr __variant_storage() = default;
};

template <class _Type>
union __variant_storage<_Type&,__trivial>
{
  reference_wrapper<remove_reference_t<_Type>> __m_type;

  constexpr __variant_storage() : __m_type{}
  {}

  template<class... _Args>
  constexpr __variant_storage(in_place_type_t<_Type&>, _Args&&... __args) :
    __m_type{forward<_Args>(__args)...}
  {}
};

template <class _Type>
union __variant_storage<_Type>
{
  static constexpr bool trivial = is_trivially_destructible_v<_Type> ||
                                  is_same_v<_Type,void>;

  __variant_storage<_Type, conditional_t<trivial, __trivial, __non_trivial>>
  __m_head;

  constexpr __variant_storage() : __m_head{} {};

  template <class _T,
            class... _Args,
            enable_if_t<is_same_v<_T,_Type>, int> = 0
            >
  constexpr __variant_storage(in_place_type_t<_T>, _Args&&... __args) :
    __m_head{in_place_type<_T>, forward<_Args>(__args)...}
  {}

  template <class _T, enable_if_t<is_same_v<_T,_Type> && trivial, int> = 0>
  constexpr _T& get()
  {
    return __m_head.__m_type;
  }

  template <class _T, enable_if_t<is_same_v<_T,_Type> && trivial, int> = 0>
  constexpr const _T& get() const
  {
    return __m_head.__m_type;
  }

  template <class _T, enable_if_t<is_same_v<_T,_Type> && !trivial, int> = 0>
  constexpr _T& get()
  {
    return *reinterpret_cast<_T*>(&__m_head.__m_storage);
  }

  template <class _T, enable_if_t<is_same_v<_T,_Type> && !trivial, int> = 0>
  constexpr const _T& get() const
  {
    return *reinterpret_cast<const _T*>(&__m_head.__m_storage);
  }
};

template <class _Type, class... _Types>
union __variant_storage
{
  static constexpr bool trivial = is_trivially_destructible_v<_Type> ||
                                  is_same_v<_Type,void>;

  __variant_storage<_Type, conditional_t<trivial, __trivial, __non_trivial>>
  __m_head;

  __variant_storage<_Types...>
  __m_tail;

  __variant_storage()
  {}

  template <class _T,
            class... _Args,
            enable_if_t<is_same_v<_T,_Type>, int> = 0
            >
  constexpr __variant_storage(in_place_type_t<_T>, _Args&&... __args) :
    __m_head{in_place_type<_T>, forward<_Args>(__args)...}
  {}

  template <class _T,
            class... _Args,
            enable_if_t<!is_same_v<_T,_Type>, int> = 0
            >
  constexpr __variant_storage(in_place_type_t<_T>, _Args&&... __args) :
    __m_tail{in_place_type<_T>, forward<_Args>(__args)...}
  {}

  template <class _T, enable_if_t<is_same_v<_T,_Type> && trivial, int> = 0>
  constexpr _T& get()
  {
    return __m_head.__m_type;
  }

  template <class _T, enable_if_t<is_same_v<_T,_Type> && trivial, int> = 0>
  constexpr const _T& get() const
  {
    return __m_head.__m_type;
  }

  template <class _T, enable_if_t<is_same_v<_T,_Type> && !trivial, int> = 0>
  constexpr _T& get()
  {
    return *reinterpret_cast<_T*>(&__m_head.__m_storage);
  }

  template <class _T, enable_if_t<is_same_v<_T,_Type> && !trivial, int> = 0>
  constexpr const _T& get() const
  {
    return *reinterpret_cast<const _T*>(&__m_head.__m_storage);
  }

  template <class _T, enable_if_t<!is_same_v<_T,_Type>, int> = 0>
  constexpr _T& get()
  {
    return __m_tail.template get<_T>();
  }

  template <class _T, enable_if_t<!is_same_v<_T,_Type>, int> = 0>
  constexpr const _T& get() const
  {
    return __m_tail.template get<_T>();
  }
};

template <class _T, class... _Types>
struct __variant_index; // undefined

template <class _T, class... _Types>
constexpr size_t __variant_index_v = __variant_index<_T, _Types...>::value;

template <class _T>
struct __variant_index<_T> :
  integral_constant<size_t, 1> {};

template <class _T, class... _Types>
struct __variant_index<_T, _T, _Types...> :
  integral_constant<size_t, 0> {};

template <class _T, class _U, class... _Types>
struct __variant_index<_T, _U, _Types...> :
  integral_constant<size_t, 1 + __variant_index_v<_T, _Types...>> {};

// Variant Base Class

template <bool _TiviallyCopyConstructible,
          bool _CopyConstructible,
          bool _TiviallyMoveConstructible,
          bool _MoveConstructible,
          bool _CopyAssignable,
          bool _MoveAssignable,
          bool _TiviallyDestructible,
          class... _Types>
class __variant_base; // undefined

// Non_TiviallyDestructible Specialisation

template <class... _Types>
class __variant_base<false,false,false,false,false,false,false, _Types...>
{
public:

  __variant_storage<_Types...> __m_storage;

protected:

  size_t __m_index = variant_npos;

  __variant_base(const __variant_base&) = delete;
  __variant_base(__variant_base&&) = delete;
  __variant_base& operator=(const __variant_base&) = delete;
  __variant_base& operator=(__variant_base&&) = delete;

  __variant_base() :
    __m_storage{},
    __m_index{variant_npos}
  {}

  template <class _T, class... _Args>
  __variant_base(in_place_type_t<_T>, _Args&&... __args) :
    __m_storage{},
    __m_index{variant_npos}
  {
    __construct(in_place_type<_T>, forward<_Args>(__args)...);
  }

  template <class _Alloc, class _T, class... _Args>
  __variant_base(allocator_arg_t, const _Alloc& __a, in_place_type_t<_T>, _Args&&... __args) :
    __m_storage{},
    __m_index{variant_npos}
  {
    __construct(allocator_arg, __a, in_place_type<_T>, forward<_Args>(__args)...);
  }

  ~__variant_base()
  {
    if(__m_index != variant_npos)
      __destroy();
  }

  template <class _T,
            class... _Args,
            enable_if_t<is_constructible_v<_T,_Args...>,int> = 0
            >
  void __construct(in_place_type_t<_T>, _Args&&... __args)
  {
    assert(__m_index == variant_npos);
    new(&__m_storage) decay_t<_T>{forward<_Args>(__args)...};
    __m_index = __variant_index_v<_T, _Types...>;
  };

  template <class _Alloc,
            class _T,
            class... _Args,
            enable_if_t<is_constructible_v<_T,allocator_arg_t,const _Alloc&,_Args&&...>,int> = 0
            >
  void __construct(allocator_arg_t, const _Alloc& __a, in_place_type_t<_T>, _Args&&... __args)
  {
    assert(__m_index == variant_npos);
    new(&__m_storage) decay_t<_T>{allocator_arg, __a, forward<_Args>(__args)...};
    __m_index = __variant_index_v<_T, _Types...>;
  };

  template <class _Alloc,
            class _T,
            class... _Args,
            enable_if_t<is_constructible_v<_T,_Args&&...,const _Alloc&>,int> = 0
            >
  void __construct(allocator_arg_t, const _Alloc& __a, in_place_type_t<_T>, _Args&&... __args)
  {
    assert(__m_index == variant_npos);
    new(&__m_storage) decay_t<_T>{forward<_Args>(__args)...,  __a};
    __m_index = __variant_index_v<_T, _Types...>;
  };

  template <class _Alloc,
            class _T,
            class... _Args,
            enable_if_t<!is_constructible_v<_T,allocator_arg_t,const _Alloc&,_Args&&...> &&
                        !is_constructible_v<_T,_Args&&...,const _Alloc&>,int> = 0
            >
  void __construct(allocator_arg_t, const _Alloc& __a, in_place_type_t<_T>, _Args&&... __args)
  {
    assert(__m_index == variant_npos);
    new(&__m_storage) decay_t<_T>{forward<_Args>(__args)...};
    __m_index = __variant_index_v<_T, _Types...>;
  };

private:

  template<class _T>
  void __private_copy(const __variant_base& __v)
  {
    __construct(in_place_type<_T>, __v.__m_storage.template get<_T>());
  }

  template <class _Alloc,
            class _T
            >
  void __private_copy(allocator_arg_t, const _Alloc& __a, const __variant_base& __v)
  {
    __construct(allocator_arg, __a, in_place_type<_T>, __v.__m_storage.template get<_T>());
  }

protected:

  void __copy(const __variant_base& __v)
  {
    assert(__v.__m_index != variant_npos);
    assert(__v.__m_index < sizeof...(_Types));
    assert(__m_index == variant_npos);
    using function = void(__variant_base::*)(const __variant_base&);
    constexpr function __array[sizeof...(_Types)] = {&__variant_base::__private_copy<_Types>...};
    (this->*__array[__v.__m_index])(__v);
  }

  template <class _Alloc>
  void __copy(allocator_arg_t, const _Alloc& __a, const __variant_base& __v)
  {
    assert(__v.__m_index != variant_npos);
    assert(__v.__m_index < sizeof...(_Types));
    assert(__m_index == variant_npos);
    using function = void(__variant_base::*)(allocator_arg_t, const _Alloc&, const __variant_base&);
    constexpr function __array[sizeof...(_Types)] = {&__variant_base::__private_copy<_Alloc, _Types>...};
    (this->*__array[__v.__m_index])(allocator_arg, __a, __v);
  }

private:

  template<class _T>
  void __private_move(__variant_base&& __v)
  {
    __construct(in_place_type<_T>, move(__v.__m_storage.template get<_T>()));
  }

  template <class _Alloc,
            class _T
            >
  void __private_move(allocator_arg_t, const _Alloc& __a, __variant_base&& __v)
  {
    __construct(allocator_arg, __a, in_place_type<_T>, move(__v.__m_storage.template get<_T>()));
  }

protected:

  void __move(__variant_base&& __v)
  {
    assert(__v.__m_index != variant_npos);
    assert(__v.__m_index < sizeof...(_Types));
    assert(__m_index == variant_npos);
    using function = void(__variant_base::*)(__variant_base&&);
    constexpr function __array[sizeof...(_Types)] = {&__variant_base::__private_move<_Types>...};
    (this->*__array[__v.__m_index])(forward<__variant_base>(__v));
    __v.__m_index = variant_npos;
  }

  template <class _Alloc>
  void __move(allocator_arg_t, const _Alloc& __a, __variant_base&& __v)
  {
    assert(__v.__m_index != variant_npos);
    assert(__v.__m_index < sizeof...(_Types));
    assert(__m_index == variant_npos);
    using function = void(__variant_base::*)(allocator_arg_t, const _Alloc&, __variant_base&&);
    constexpr function __array[sizeof...(_Types)] = {&__variant_base::__private_move<_Alloc, _Types>...};
    (this->*__array[__v.__m_index])(allocator_arg, __a, forward<__variant_base>(__v));
    __v.__m_index = variant_npos;
  }

private:

  template<class _T, enable_if_t<is_class_v<decay_t<_T>>,int> = 0>
  void __private_destroy()
  {
    assert(__m_index != variant_npos);
    reinterpret_cast<add_pointer_t<_T>>(&__m_storage)->~_T();
    __m_index = variant_npos;
  };

  template<class _T, enable_if_t<!is_class_v<decay_t<_T>>,int> = 0>
  void __private_destroy()
  {
    assert(__m_index != variant_npos);
    __m_index = variant_npos;
  };

protected:

  void __destroy()
  {
    assert(__m_index != variant_npos);
    assert(__m_index < sizeof...(_Types));
    using function = void(__variant_base::*)();
    constexpr function __array[sizeof...(_Types)] = {&__variant_base::__private_destroy<_Types>...};
    (this->*__array[__m_index])();
  };
};

// _TiviallyDestructible Specialisation

template <class... _Types>
class __variant_base<false,false,false,false,false,false,true, _Types...>
{
public:

  __variant_storage<_Types...> __m_storage;

protected:

  size_t __m_index = variant_npos;

//  __variant_base(const __variant_base&) = delete;
  __variant_base(const __variant_base&) = default;
  __variant_base(__variant_base&&) = default;
  __variant_base& operator=(const __variant_base&) = delete;
  __variant_base& operator=(__variant_base&&) = delete;
  ~__variant_base() = default;

  __variant_base() :
    __m_storage{},
    __m_index{variant_npos}
  {}

  template <class _T, class... _Args>
  constexpr __variant_base(in_place_type_t<_T>, _Args&&... __args) :
    __m_storage{in_place_type<_T>, forward<_Args>(__args)...},
    __m_index{__variant_index_v<_T, _Types...>}
  {}

  template <class _T, class... _Args>
  void __construct(in_place_type_t<_T>, _Args&&... __args)
  {
    assert(__m_index == variant_npos);
    new(&__m_storage) remove_reference_t<_T>{forward<_Args>(__args)...};
    __m_index = __variant_index_v<_T, _Types...>;
  };

private:

  template<class _T>
  void __private_copy(const __variant_base& __v)
  {
    __construct(in_place_type<_T>, __v.__m_storage.template get<_T>());
  }

protected:

  void __copy(const __variant_base& __v)
  {
    assert(__v.__m_index != variant_npos);
    assert(__v.__m_index < sizeof...(_Types));
    assert(__m_index == variant_npos);
    using __function = void(__variant_base::*)(const __variant_base&);
    constexpr __function __array[sizeof...(_Types)] = {&__variant_base::__private_copy<_Types>...};
    (this->*__array[__v.__m_index])(__v);
  }

  void __move(__variant_base&& __v)
  {
    __copy(__v);
    __v.__m_index = variant_npos;
  }

  void __destroy()
  {
    assert(__m_index != variant_npos);
    __m_index = variant_npos;
  };
};

// _MoveAssignable Specialisation

template <bool _TiviallyDestructible,
          class... _Types>
class __variant_base<false,false,false,false,false,true,_TiviallyDestructible,_Types...> :
public __variant_base<false,false,false,false,false,false,_TiviallyDestructible,_Types...>
{
  using __base =
  __variant_base<false,false,false,false,false,false,_TiviallyDestructible,_Types...>;

protected:

  using __base::__base;
  using __base::__move;
  using __base::__destroy;
  using __base::__m_index;

  __variant_base&
  operator=(__variant_base&& __rhs)
    noexcept(conjunction_v<is_nothrow_move_constructible<_Types>...,
                           is_nothrow_move_assignable<_Types>...   >)
  {
    static_assert(conjunction_v<is_move_constructible<_Types>...,
                                is_move_assignable   <_Types>...> ,
      R"(This function shall not participate in overload resolution
      unless is_move_constructible_v<_Ti> && s_move_assignable_v<_Ti> is true for all i.)");
    if(__m_index != variant_npos)
      __destroy();
    if(__rhs.__m_index != variant_npos)
      __move(move(__rhs));
    return *this;
  }

  __variant_base(const __variant_base&) = default;
  __variant_base(__variant_base&&) = default;
};

// _CopyAssignable Specialisation

template <bool _MoveAssignable,
          bool _TiviallyDestructible,
          class... _Types>
class __variant_base<false,false,false,false,true,_MoveAssignable,_TiviallyDestructible,_Types...> :
public __variant_base<false,false,false,false,false,_MoveAssignable,_TiviallyDestructible,_Types...>
{
  using __base =
  __variant_base<false,false,false,false,false,_MoveAssignable,_TiviallyDestructible,_Types...>;

protected:

  using __base::__base;
  using __base::__copy;
  using __base::__destroy;
  using __base::__m_index;

  __variant_base& operator=(const __variant_base& __rhs)
    noexcept(conjunction_v<is_nothrow_copy_constructible<_Types>...,
                           is_nothrow_copy_assignable<_Types>...   >)
  {
    static_assert(conjunction_v<is_copy_constructible<_Types>...,
                                is_move_constructible<_Types>...,
                                is_copy_assignable   <_Types>...>,
      R"(This function shall not participate in overload resolution
      unless is_copy_constructible_v<_Ti> && is_move_constructible_v<_Ti> &&
      is_copy_assignable_v<_Ti> is true for all i.)");
    if(__m_index != variant_npos)
      __destroy();
    if(__rhs.__m_index != variant_npos)
      __copy(__rhs);
    return *this;
  }

  __variant_base(const __variant_base&) = default;
  __variant_base(__variant_base&&) = default;
  __variant_base& operator=(__variant_base&&) = default;
};

// _MoveConstructible Specialisation

template <bool _CopyAssignable,
          bool _MoveAssignable,
          bool _TiviallyDestructible,
          class... _Types>
class __variant_base<false,false,false,true,_CopyAssignable,_MoveAssignable,_TiviallyDestructible,_Types...> :
public __variant_base<false,false,false,false,_CopyAssignable,_MoveAssignable,_TiviallyDestructible,_Types...>
{
  using __base =
  __variant_base<false,false,false,false,_CopyAssignable,_MoveAssignable,_TiviallyDestructible,_Types...>;

protected:

  using __base::__base;
  using __base::__move;
  using __base::__m_index;

  __variant_base(__variant_base&& __v)
    noexcept(conjunction_v<is_nothrow_move_constructible<_Types>...>) :
      __base{}
  {
    static_assert(conjunction_v<is_move_constructible<_Types>...>,
      R"(This function shall not participate in overload resolution
      unless is_move_constructible_v<_Ti> is true for all i.)");
    if(__v.__m_index != variant_npos)
    {
      __move(forward<__variant_base>(__v));
      __v.__m_index = variant_npos;
    }
  }

  __variant_base(const __variant_base&) = default;
  __variant_base& operator=(const __variant_base&) = default;
  __variant_base& operator=(__variant_base&&) = default;
};

// _TriviallyMoveConstructible Specialisation

template <bool _CopyAssignable,
          bool _MoveAssignable,
          bool _TiviallyDestructible,
          class... _Types>
class __variant_base<false,false,true,false,_CopyAssignable,_MoveAssignable,_TiviallyDestructible,_Types...> :
public __variant_base<false,false,false,false,_CopyAssignable,_MoveAssignable,_TiviallyDestructible,_Types...>
{
  using __base =
  __variant_base<false,false,false,false,_CopyAssignable,_MoveAssignable,_TiviallyDestructible,_Types...>;

protected:

  using __base::__base;
  using __base::__move;
  using __base::__m_index;

  constexpr __variant_base(__variant_base&& __v)
    noexcept(conjunction_v<is_nothrow_move_constructible<_Types>...>) :
      __base{forward<__variant_base>(__v)}
  {
    static_assert(conjunction_v<is_trivially_move_constructible<_Types>...>,
      R"(This function shall not participate in overload resolution
      unless is_copy_constructible_v<_Ti> is true for all i.)");
  }

  __variant_base(const __variant_base&) = default;
  __variant_base& operator=(const __variant_base&) = default;
  __variant_base& operator=(__variant_base&&) = default;
};

// _CopyConstructible Specialisation

template <bool _TriviallyMoveConstructible,
          bool _MoveConstructible,
          bool _CopyAssignable,
          bool _MoveAssignable,
          bool _TiviallyDestructible,
          class... _Types>
class __variant_base<false,true,_TriviallyMoveConstructible,_MoveConstructible,_CopyAssignable,_MoveAssignable,_TiviallyDestructible,_Types...> :
public __variant_base<false,false,_TriviallyMoveConstructible,_MoveConstructible,_CopyAssignable,_MoveAssignable,_TiviallyDestructible,_Types...>
{
  using __base =
  __variant_base<false,false,_TriviallyMoveConstructible,_MoveConstructible,_CopyAssignable,_MoveAssignable,_TiviallyDestructible,_Types...>;

protected:

  using __base::__base;
  using __base::__copy;

  __variant_base(const __variant_base& __v)
      noexcept(conjunction_v<is_nothrow_copy_constructible<_Types>...>) :
      __base{}
  {
    static_assert(conjunction_v<is_move_constructible<_Types>...>,
      R"(This function shall not participate in overload resolution
      unless is_move_constructible_v<_Ti> is true for all i.)");
    if(__v.__m_index != variant_npos)
      __copy(__v);
  }

  __variant_base(__variant_base&&) = default;
  __variant_base& operator=(const __variant_base&) = default;
  __variant_base& operator=(__variant_base&&) = default;
};

// _TriviallyCopyConstructible Specialisation

template <bool _TriviallyMoveConstructible,
          bool _MoveConstructible,
          bool _CopyAssignable,
          bool _MoveAssignable,
          bool _TiviallyDestructible,
          class... _Types>
class __variant_base<true,false,_TriviallyMoveConstructible,_MoveConstructible,_CopyAssignable,_MoveAssignable,_TiviallyDestructible,_Types...> :
public __variant_base<false,false,_TriviallyMoveConstructible,_MoveConstructible,_CopyAssignable,_MoveAssignable,_TiviallyDestructible,_Types...>
{
  using __base =
  __variant_base<false,false,_TriviallyMoveConstructible,_MoveConstructible,_CopyAssignable,_MoveAssignable,_TiviallyDestructible,_Types...>;

protected:

  using __base::__base;

  constexpr __variant_base(const __variant_base& __v) noexcept :
      __base{__v}
  {
    static_assert(conjunction_v<is_trivially_copy_constructible<_Types>...>,
      R"(This function shall not participate in overload resolution
      unless is_move_constructible_v<_Ti> is true for all i.)");
  }

  __variant_base(__variant_base&&) = default;
  __variant_base& operator=(const __variant_base&) = default;
  __variant_base& operator=(__variant_base&&) = default;
};

// Some helper templates

template < template <class> class _Test, class... _Types>
struct __first_or_last; //undefined

template < template <class> class _Test, class... _Types>
using __first_or_last_t = typename __first_or_last<_Test, _Types...>::type;

template < template <class> class _Test, class _T>
struct __first_or_last<_Test, _T> {using type = _T;};

template < template <class> class _Test, class _T, class... _Types>
struct __first_or_last<_Test, _T, _Types...>
{
  static constexpr _Test<_T> test;
  using type = conditional_t<!test, _T, __first_or_last_t<_Test, _Types...>>;
};

template <typename _T>
struct __argument {using type = _T;};

template <typename... _Ts>
struct __imaginary_function; // undefined

template <>
struct __imaginary_function<> {constexpr void operator()() const noexcept;};

template <typename... _Ts>
struct __imaginary_function<void, _Ts...> : __imaginary_function<_Ts...>
{
  using __imaginary_function<_Ts...>::operator();
  constexpr __argument<void> operator()() const noexcept;
};

template <typename _T, typename... _Ts>
struct __imaginary_function<_T, _Ts...> : __imaginary_function<_Ts...>
{
  using __imaginary_function<_Ts...>::operator();
  constexpr __argument<_T> operator()(_T) const noexcept;
};

template <class _T, class... _Types>
using __imaginary_function_argument_t =
  typename result_of_t<__imaginary_function<_Types...>(_T)>::type;

template <class _T, class... _Types>
struct __count; // undefined

template <class _T, class... _Types>
constexpr size_t __count_v = __count<_T, _Types...>::value;

template <class _T,class _U>
struct __count<_T,_U> :
  integral_constant<size_t, is_same_v<_T,_U >> {};

template <class _T, class _U, class... _Types>
struct __count<_T, _U, _Types...> :
  integral_constant<size_t, is_same_v<_T,_U > + __count_v<_T, _Types...>> {};

template <class _T, class... _Types>
struct __is_imaginary_function_well_formed :
  bool_constant<(__count_v<_T, _Types...> == 1)> {};

template <class _T, class... _Types>
constexpr bool __is_imaginary_function_well_formed_v =
  __is_imaginary_function_well_formed<_T, _Types...>::value;

// 20.7.2, variant of value types
template <class... _Types>
class variant;

// 20.7.3, variant helper classes
template <class _T> struct variant_size; // undefined
template <class _T> struct variant_size<const _T>;
template <class _T> struct variant_size<volatile _T>;
template <class _T> struct variant_size<const volatile _T>;

template <class _T> constexpr size_t variant_size_v = variant_size<_T>::value;

template <class... _Types>
struct variant_size<variant<_Types...>> :
  integral_constant<size_t, sizeof...(_Types)> {};

template <size_t _I, class _T> struct variant_alternative; // undefined
template <size_t _I, class _T> struct variant_alternative<_I, const _T>;
template <size_t _I, class _T> struct variant_alternative<_I, volatile _T>;
template <size_t _I, class _T> struct variant_alternative<_I, const volatile _T>;

template <size_t _I, class _T>
using variant_alternative_t = typename variant_alternative<_I, _T>::type;

template <size_t _I, class... _Types>
struct variant_alternative<_I, variant<_Types...>>
{
  static_assert(_I < sizeof...(_Types), "variant_alternative index out of range");
  using type = tuple_element_t<_I, tuple<_Types...>>;
};

// 20.7.4, value access
template <class _T, class... _Types>
inline
constexpr bool holds_alternative(const variant<_Types...>& __v) noexcept
{
  return __variant_index_v<_T, _Types...> == __v.index();
};

template <size_t _I, class... _Types>
inline
constexpr variant_alternative_t<_I, variant<_Types...>>&
get(variant<_Types...>& __v)
{
  using _T = variant_alternative_t<_I, variant<_Types...>>;
  return get<_T>(__v);
};

template <size_t _I, class... _Types>
inline
constexpr variant_alternative_t<_I, variant<_Types...>>&&
get(variant<_Types...>&& __v)
{
  using _T = variant_alternative_t<_I, variant<_Types...>>;
  return move(get<_T>(__v));
};

template <size_t _I, class... _Types>
inline
constexpr variant_alternative_t<_I, variant<_Types...>> const&
get(const variant<_Types...>& __v)
{
  using _T = variant_alternative_t<_I, variant<_Types...>>;
  return get<_T>(__v);
};

template <size_t _I, class... _Types>
inline
constexpr variant_alternative_t<_I, variant<_Types...>> const&&
get(const variant<_Types...>&& __v)
{
  using _T = variant_alternative_t<_I, variant<_Types...>>;
  return move(get<_T>(__v));
};

template <class _T, class... _Types>
inline
constexpr _T&
get(variant<_Types...>& __v)
{
  static_assert(__count_v<_T, _Types...> > 0, "_Type is not an alternative");
  if(holds_alternative<_T>(__v))
  return __v.__m_storage.template get<_T>();
  throw bad_variant_access{};
};

template <class _T, class... _Types>
inline
constexpr _T&&
get(variant<_Types...>&& __v)
{
  static_assert(__count_v<_T, _Types...> > 0, "_Type is not an alternative");
  if(holds_alternative<_T>(__v))
  return __v.__m_storage.template get<_T>();
  throw bad_variant_access{};
};

template <class _T, class... _Types>
inline
constexpr const _T&
get(const variant<_Types...>& __v)
{
  static_assert(__count_v<_T, _Types...> > 0, "_Type is not an alternative");
  if(holds_alternative<_T>(__v))
  return __v.__m_storage.template get<_T>();
  throw bad_variant_access{};
};

template <class _T, class... _Types>
inline
constexpr const _T&&
get(const variant<_Types...>&& __v)
{
  static_assert(__count_v<_T, _Types...> > 0, "_Type is not an alternative");
  if(holds_alternative<_T>(__v))
  return __v.__m_storage.template get<_T>();
  throw bad_variant_access{};
};

template <size_t _I, class... _Types>
inline
constexpr add_pointer_t<variant_alternative_t<_I, variant<_Types...>>>
get_if(variant<_Types...>* __v) noexcept
{
  using _T = variant_alternative_t<_I, variant<_Types...>>;
  return get_if<_T>(__v);
};

template <size_t _I, class... _Types>
inline
constexpr add_pointer_t<const variant_alternative_t<_I, variant<_Types...>>>
get_if(const variant<_Types...>* __v) noexcept
{
  using _T = variant_alternative_t<_I, variant<_Types...>>;
  return get_if<_T>(__v);
};

template <class _T, class... _Types>
inline
constexpr add_pointer_t<_T>
get_if(variant<_Types...>* __v) noexcept
{
  static_assert(__count_v<_T, _Types...> > 0, "_Type is not an alternative");
  if(holds_alternative<_T>(*__v))
  return &(__v->__m_storage.template get<_T>());
  return nullptr;
};

template <class _T, class... _Types>
inline
constexpr add_pointer_t<const _T>
get_if(const variant<_Types...>* __v) noexcept
{
  static_assert(__count_v<_T, _Types...> > 0, "_Type is not an alternative");
  if(holds_alternative<_T>(*__v))
  return &(__v->__m_storage.template get<_T>());
  return nullptr;
};

template <class... _Types>
using __comparison_function_type =
  bool(*)(const variant<_Types...>&, const variant<_Types...>&);

template <class _T, class... _Types>
constexpr bool
__variant_equal_to(const variant<_Types...>& __v, const variant<_Types...>& __w)
{
  return get<_T>(__v) == get<_T>(__w);
}

template <class... _Types>
constexpr array<__comparison_function_type<_Types...>, sizeof...(_Types)>
__make_equal_to_array()
{
  return { __variant_equal_to<_Types, _Types...>... };
}

template <class _T, class... _Types>
constexpr
std::enable_if_t<!std::is_null_pointer_v<_T>,bool>
__variant_less(const variant<_Types...>& __v, const variant<_Types...>& __w)
{
  return get<_T>(__v) < get<_T>(__w);
}

template <class _T, class... _Types>
constexpr
std::enable_if_t<std::is_null_pointer_v<_T>,bool>
__variant_less(const variant<_Types...>& __v, const variant<_Types...>& __w)
{
  return false;
}

template <class... _Types>
constexpr array<__comparison_function_type<_Types...>, sizeof...(_Types)>
__make_less_array()
{
  return { __variant_less<_Types, _Types...>... };
}

template <class _Visitor, class... _Variants>
using __invoker_function_type = auto(*)(_Visitor&&, _Variants&&...)
-> result_of_t<_Visitor&&(variant_alternative_t<0, decay_t<_Variants>>&&...)>;

template <size_t _I, class _Visitor, class... _Variants>
auto
__alternative_invoker(_Visitor&& __vis, _Variants&&... __vars)
{
    return invoke(forward<_Visitor>(__vis), get<_I>(forward<_Variants>(__vars))...);
}

template <class _Visitor, class... _Variants, size_t... _I>
constexpr array<__invoker_function_type<_Visitor, _Variants...>, sizeof...(_I)>
__make_invoker_array(index_sequence<_I...>)
{
    return { __alternative_invoker<_I, _Visitor, _Variants...>... };
}

template <class _Visitor, class... _Types, class... _Variants>
auto
__variant_invoke(_Visitor&& __vis, variant<_Types...>& __var, _Variants&&... __vars)
{
  constexpr auto __array =
    __make_invoker_array<_Visitor, variant<_Types...>, _Variants...>
    (index_sequence_for<_Types...>{});

  return __array[__var.index()](forward<_Visitor>(__vis),
                              forward<variant<_Types...>>(__var),
                              forward<_Variants>(__vars)...);
}

template <class _Visitor, class... _Types, class... _Variants>
auto
__variant_invoke(_Visitor&& __vis, const variant<_Types...>& __var, _Variants&&... __vars)
{
  constexpr auto __array =
    __make_invoker_array<_Visitor, const variant<_Types...>&, _Variants...>
    (index_sequence_for<_Types...>{});

  return __array[__var.index()](forward<_Visitor>(__vis), __var, __vars...);
}

template <class... _Types>
using __swap_function_type = void(*)(variant<_Types...>&, variant<_Types...>&);

template <class _T, class... _Types>
constexpr void
__swap(variant<_Types...>& __v, variant<_Types...>& __w) noexcept
{
  swap(get<_T>(__v), get<_T>(__w));
}

template <class... _Types>
constexpr array<__swap_function_type<_Types...>, sizeof...(_Types)>
__make_swap_array()
{
  return { __swap<_Types, _Types...>... };
}

template <class... _Types>
using __hash_function_type = size_t(*)(const variant<_Types...>&);

template <class _T, class... _Types>
constexpr size_t __hash(const variant<_Types...>& __v)
{
  return hash<_T>{}(get<_T>(__v));
}

template <class... _Types>
constexpr array<__hash_function_type<_Types...>, sizeof...(_Types)>
__make_hash_array()
{
  return { __hash<_Types, _Types...>... };
}

// 20.7.5, relational operators
template <class... _Types>
inline
constexpr bool operator==(const variant<_Types...>& __v, const variant<_Types...>& __w)
{
  constexpr auto __equals_to = __make_equal_to_array<_Types...>();
  return (__v.valueless_by_exception() && __w.valueless_by_exception()) ||
         (__v.index() == __w.index() && __equals_to[__v.index()](__v,__w));
};

template <class... _Types>
inline
constexpr bool operator!=(const variant<_Types...>& __v, const variant<_Types...>& __w)
{
  return !(__v == __w);
};

template <class... _Types>
inline
constexpr bool operator<(const variant<_Types...>& __v, const variant<_Types...>& __w)
{
  constexpr auto __less = __make_less_array<_Types...>();
  return (__v.index() < __w.index()) || (__v.index() == __w.index() &&
          !__v.valueless_by_exception() && __less[__v.index()](__v,__w));
};

template <class... _Types>
inline
constexpr bool operator>(const variant<_Types...>& __v, const variant<_Types...>& __w)
{
  return __w < __v;
}

template <class... _Types>
constexpr bool operator<=(const variant<_Types...>& __v, const variant<_Types...>& __w)
{
  return !(__v > __w);
}

template <class... _Types>
inline
constexpr bool operator>=(const variant<_Types...>& __v, const variant<_Types...>& __w)
{
  return !(__v < __w);
}

// 20.7.6, visitation
template <class _Visitor, class... _Variants>
inline
constexpr auto visit(_Visitor&& __vis, _Variants&&... __vars)
{
  return __variant_invoke(forward<_Visitor>(__vis), forward<_Variants>(__vars)...);
};

// 20.7.7, class monostate
struct monostate {};

// 20.7.8, monostate relational operators
inline
constexpr bool operator<(monostate, monostate) noexcept {return false;};
inline
constexpr bool operator>(monostate, monostate) noexcept {return false;};
inline
constexpr bool operator<=(monostate, monostate) noexcept {return true;};
inline
constexpr bool operator>=(monostate, monostate) noexcept {return true;};
inline
constexpr bool operator==(monostate, monostate) noexcept {return true;};
inline
constexpr bool operator!=(monostate, monostate) noexcept {return false;};

// 20.7.9, specialized algorithms
template <class... _Types>
inline
void swap(variant<_Types...>& __lhs, variant<_Types...>& __rhs)
  noexcept(conjunction_v<is_nothrow_move_constructible<_Types>...,
                         is_nothrow_swappable<_Types>...>)
{
  __lhs.swap(__rhs);
};

template <class... _Types>
class variant : public __variant_base<conjunction_v<is_trivially_copy_constructible<_Types>...>,
                                      conjunction_v<is_copy_constructible<_Types>...> && !conjunction_v<is_trivially_copy_constructible<_Types>...>,
                                      conjunction_v<is_trivially_move_constructible<_Types>...>,
                                      conjunction_v<is_move_constructible<_Types>...> && !conjunction_v<is_trivially_move_constructible<_Types>...>,
                                      conjunction_v<is_copy_assignable<_Types>..., is_copy_constructible<_Types>...>,
                                      conjunction_v<is_move_assignable<_Types>..., is_move_constructible<_Types>...>,
                                      conjunction_v<is_trivially_destructible<_Types>...>,
                                      _Types...>
{
  using __base = __variant_base<conjunction_v<is_trivially_copy_constructible<_Types>...>,
                                conjunction_v<is_copy_constructible<_Types>...> && !conjunction_v<is_trivially_copy_constructible<_Types>...>,
                                conjunction_v<is_trivially_move_constructible<_Types>...>,
                                conjunction_v<is_move_constructible<_Types>...> && !conjunction_v<is_trivially_move_constructible<_Types>...>,
                                conjunction_v<is_copy_assignable<_Types>..., is_copy_constructible<_Types>...>,
                                conjunction_v<is_move_assignable<_Types>..., is_move_constructible<_Types>...>,
                                conjunction_v<is_trivially_destructible<_Types>...>,
                                _Types...>;

  using __base::__copy;
  using __base::__move;
  using __base::__construct;
  using __base::__destroy;
  using __base::__m_index;

public:

  // 20.7.2.1, constructors

  template <class _T0 = variant_alternative_t<0,variant>,
            enable_if_t<is_default_constructible_v<_T0>, int> = 0>
  inline
  constexpr
  variant() noexcept(is_nothrow_default_constructible_v<_T0>) :
    __base{in_place_type<_T0>}
  {
    static_assert(is_default_constructible_v<_T0>,
      R"(This function shall not participate in overload resolution
      unless is_default_constructible_v<_T0> is true.)");
  };

  template <class _T0 = variant_alternative_t<0,variant>,
            enable_if_t<!is_default_constructible_v<_T0>, int> = 0
            >
  inline
  constexpr
  variant() = delete;

  // We'll inherit the copy and move constructors from the __base

  inline
  constexpr variant(const variant&) = default;

  inline
  variant(variant&&) = default;

  template <class _T,
            class _Tj = __imaginary_function_argument_t<_T, _Types...>,
            enable_if_t<conjunction_v<negation<is_same<decay_t<_T>,variant>>,
                                      is_constructible<_Tj, _T>,
                                      __is_imaginary_function_well_formed<_Tj, _Types...>>,int> = 0
            >
  inline
  constexpr
  variant(_T&&  __t) noexcept(is_nothrow_constructible_v<_Tj, _T>) :
      __base{in_place_type<_Tj>, forward<_T>(__t)}
  {
    static_assert(conjunction_v<negation<is_same<decay_t<_T>,variant>>,
                                is_constructible<_Tj, _T>,
                                __is_imaginary_function_well_formed<_Tj, _Types...>>,
      R"(This function shall not participate in overload resolution
      unless is_same_v<decay_t<_T>, variant> is false,
      unless is_constructible_v<_Tj, _T> is true, and
      unless the expression FUN(std::forward<_T>(__t))
      (with FUN being the above-mentioned set of imaginary functions)
      is well formed.)");
  };

  template <class _T,
            class... _Args,
            enable_if_t<is_constructible_v<_T, _Args...>, int> = 0
            >
  inline
  constexpr explicit
  variant(in_place_type_t<_T>, _Args&&... __args) :
    __base{in_place_type<_T>, forward<_Args>(__args)...}
  {};

  template <class _T,
            class _U,
            class... _Args,
            enable_if_t<is_constructible_v<_T, initializer_list<_U>&, _Args...>, int> = 0
            >
  inline
  constexpr explicit
  variant(in_place_type_t<_T>, initializer_list<_U> __il, _Args&&... __args) :
    __base{in_place_type<_T>, forward<initializer_list<_U>>(__il), forward<_Args>(__args)...}
  {};

  template <size_t _I,
            class... _Args,
            class _Ti = variant_alternative_t<_I, variant>,
            enable_if_t<is_constructible_v<_Ti, _Args...>, int> = 0
            >
  inline
  constexpr explicit
  variant(in_place_index_t<_I>, _Args&&... __args) :
    __base{in_place_type<_Ti>, forward<_Args>(__args)...}
  {};

  template <size_t _I,
            class _U,
            class... _Args,
            class _Ti = variant_alternative_t<_I, variant>,
            enable_if_t<is_constructible_v<_Ti, initializer_list<_U>&, _Args...>, int> = 0
            >
  inline
  constexpr explicit
  variant(in_place_index_t<_I>, initializer_list<_U> __il, _Args&&... __args) :
    __base{in_place_type<_Ti>, forward<initializer_list<_U>>(__il), forward<_Args>(__args)...}
  {};

  // allocator-extended constructors
  template <class _Alloc,
            class _T0 = variant_alternative_t<0,variant>,
            enable_if_t<is_constructible_v<_T0>, int> = 0>
  inline
  variant(allocator_arg_t, const _Alloc& __a) :
      __base{allocator_arg, __a, in_place_type<_T0>}
  {
    static_assert(is_default_constructible_v<_T0>,
      R"(This function shall not participate in overload resolution
      unless is_default_constructible_v<_T0> is true.)");
  };

  template <class _Alloc,
            class _TN = __first_or_last_t<is_copy_constructible,_Types...>,
            enable_if_t<is_copy_constructible_v<_TN>, int> = 0
            >
  inline
  variant(allocator_arg_t, const _Alloc& __a, const variant& __v) :
      __base{}
  {
    static_assert(conjunction_v<is_copy_constructible<_Types>...>,
      R"(This function shall not participate in overload resolution
      unless is_copy_constructible_v<_Ti> is true for all i.)");
    if(!__v.valueless_by_exception())
      __copy(allocator_arg, __a, __v);
  }

  template <class _Alloc,
            class _TN = __first_or_last_t<is_move_constructible,_Types...>,
            enable_if_t<is_move_constructible_v<_TN>, int> = 0
            >
  inline
  variant(allocator_arg_t, const _Alloc& __a, variant&& __v) :
      __base{}
  {
    static_assert(conjunction_v<is_move_constructible<_Types>...>,
      R"(This function shall not participate in overload resolution
        unless is_move_constructible_v<_Ti> is true for all i.)");
    if(!__v.valueless_by_exception())
      __move(allocator_arg, __a, forward<variant>(__v));
  }

  template <class _Alloc,
            class _T,
            class _Tj = __imaginary_function_argument_t<_T, _Types...>,
            enable_if_t<conjunction_v<negation<is_same<decay_t<_T>,variant>>,
                                      is_constructible<_Tj, _T>,
                                      __is_imaginary_function_well_formed<_Tj, _Types...>>,int> = 0
            >
  inline
  variant(allocator_arg_t, const _Alloc& __a, _T&&  __t) :
      __base{allocator_arg, __a, in_place_type<_Tj>, forward<_T>(__t)}
  {
    static_assert(!is_same_v<decay_t<_T>,variant> &&
                  is_constructible_v<_Tj, _T> &&
                  __is_imaginary_function_well_formed_v<_Tj, _Types...>,
      R"(This function shall not participate in overload resolution
      unless is_same_v<decay_t<_T>, variant> is false,
      unless is_constructible_v<_Tj, _T> is true, and
      unless the expression FUN( std::forward<_T>(__t))
      (with FUN being the above-mentioned set of imaginary functions)
      is well formed.)");
  };

  template <class _Alloc,
            class _T,
            class... _Args,
            enable_if_t<is_constructible_v<_T, _Args...>, int> = 0
            >
  inline
  variant(allocator_arg_t, const _Alloc& __a, in_place_type_t<_T>, _Args&&... __args) :
      __base{allocator_arg, __a, in_place_type<_T>, forward<_Args>(__args)...}
  {};

  template <class _Alloc,
            class _T,
            class _U,
            class... _Args,
            enable_if_t<is_constructible_v<_T, initializer_list<_U>&, _Args...>, int> = 0
            >
  inline
  variant(allocator_arg_t, const _Alloc& __a,
          in_place_type_t<_T>, initializer_list<_U> __il, _Args&&... __args) :
    __base{allocator_arg, __a, in_place_type<_T>, forward<initializer_list<_U>>(__il), forward<_Args>(__args)...}
  {};

  template <class _Alloc,
            size_t _I,
            class... _Args,
            class _Ti = variant_alternative_t<_I, variant>,
            enable_if_t<is_constructible_v<_Ti, _Args...>, int> = 0
            >
  inline
  variant(allocator_arg_t, const _Alloc& __a, in_place_index_t<_I>, _Args&&... __args) :
    __base{allocator_arg, __a, in_place_type<_Ti>, forward<_Args>(__args)...}
  {};

  template <class _Alloc,
            size_t _I,
            class _U,
            class... _Args,
            class _Ti = variant_alternative_t<_I, variant>,
            enable_if_t<is_constructible_v<_Ti, initializer_list<_U>&, _Args...>, int> = 0
            >
  inline
  variant(allocator_arg_t, const _Alloc& __a, in_place_index_t<_I>, initializer_list<_U> __il, _Args&&... __args) :
    __base{allocator_arg, __a, in_place_type<_Ti>, forward<initializer_list<_U>>(__il), forward<_Args>(__args)...}
  {};

  // 20.7.2.2, destructor
  inline
  ~variant() = default;

  // 20.7.2.3, assignment

  // We'll inherit the copy and move assignment operators from the __base

  inline
  variant& operator=(const variant&) = default;

  inline
  variant& operator=(variant&&) = default;

  template <class _T,
            class _Tj = __imaginary_function_argument_t<_T, _Types...>,
            enable_if_t<conjunction_v<negation<is_same<decay_t<_T>,variant>>,
                                      is_assignable<_Tj&, _T>,
                                      is_constructible<_Tj, _T>,
                                      __is_imaginary_function_well_formed<_Tj, _Types...>>,int> = 0
            >
  inline
  variant& operator=(_T&& __rhs)
    noexcept(conjunction_v<is_nothrow_assignable<_Types&, _Types>...,
                           is_nothrow_constructible<_Types&, _Types>...>)
  {
    static_assert(conjunction_v<negation<is_same<decay_t<_T>, variant>>,
                                is_assignable<_Tj&, _T>,
                                is_constructible<_Tj, _T>,
                                __is_imaginary_function_well_formed<_Tj, _Types...>>,
      R"(This function shall not participate in overload resolution
         unless is_same_v<decay_t<_T>, variant> is false,
         unless is_assignable_v<_Tj&, _T> && is_constructible_v<_Tj, _T> is true, and
         unless the expression FUN (std::forward<_T>(__t))
         (with FUN being the above-mentioned set of imaginary functions) is well formed.)");
    if(!valueless_by_exception())
      __destroy();
    __construct(in_place_type<_Tj>, forward<_T>(__rhs));
    return *this;
  };

  // 20.7.2.4, modifiers

  template <class _T, class... _Args>
  inline
  void emplace(_Args&&... __args)
  {
    static_assert(__count_v<_T, _Types...> > 0, "_Type is not an alternative");
    static_assert(!is_lvalue_reference_v<_T>, "references cannot be reassigned");
    if(!valueless_by_exception())
      __destroy();
    __construct(in_place_type<_T>, forward<_Args>(__args)...);
  };

  template <class _T, class _U, class... _Args>
  inline
  void emplace(initializer_list<_U> __il, _Args&&... __args)
  {
    static_assert(__count_v<_T, _Types...> > 0, "_Type is not an alternative");
    static_assert(!is_lvalue_reference_v<_T>, "references cannot be reassigned");
    if(!valueless_by_exception())
      __destroy();
    __construct(in_place_type<_T>, __il, forward<_Args>(__args)...);
  };

  template <size_t _I, class... _Args>
  inline
  void emplace(_Args&&... __args)
  {
    using _T = variant_alternative_t<_I, variant>;
    emplace<_T>(forward<_Args>(__args)...);
  };

  template <size_t _I, class _U, class... _Args>
  inline
  void emplace(initializer_list<_U> __il, _Args&&... __args)
  {
    using _T = variant_alternative_t<_I, variant>;
    emplace<_T>(__il, forward<_Args>(__args)...);
  };

  // 20.7.2.5, value status
  inline
  constexpr bool valueless_by_exception() const noexcept
  {
    return __m_index == variant_npos;
  }

  inline
  constexpr size_t index() const noexcept
  {
    return __m_index;
  };

  // 20.7.2.6, swap
  inline
  void swap(variant& __rhs)
    noexcept(conjunction_v<is_nothrow_move_constructible<_Types>...,
                           is_nothrow_swappable<_Types>...>)
  {
    constexpr auto __swap = __make_swap_array<_Types...>();
    variant& __lhs = *this;
    if(__lhs.valueless_by_exception() && __rhs.valueless_by_exception())
      return;
    if(__lhs.__m_index == __rhs.__m_index)
      __swap[__lhs.__m_index](__lhs,__rhs);
    else
    {
      variant __tmp{__rhs};
      __rhs.__m_index = __lhs.__m_index;
      __swap[__lhs.__m_index](__rhs,__lhs);
      __lhs.__m_index = __tmp.__m_index;
      __swap[__lhs.__m_index](__lhs,__tmp);
    }
  }
};

template<>
class variant<>
{
  variant() = delete;
  variant(const variant&) = delete;
  variant(variant&&) = delete;
};

// 20.7.11, hash support
// template <class _T> struct hash;
template <class... _Types>
struct hash<variant<_Types...>>
{
  inline
  size_t operator()(const variant<_Types...>& __v) noexcept
  {
    constexpr auto __hash = __make_hash_array<_Types...>();
    return __hash[__v.index()](__v);
  }
};

template <>
struct hash<monostate>
{
  inline
  size_t operator()(monostate) noexcept
  {
    return 42;
  }
};

// 20.7.12, allocator-related traits
// template <class _T, class _Alloc> struct uses_allocator;

template <class... _Types, class _Alloc>
struct uses_allocator<variant<_Types...>, _Alloc> : true_type {};

} // namespace std

#endif // _RUOKA_VARIANT
