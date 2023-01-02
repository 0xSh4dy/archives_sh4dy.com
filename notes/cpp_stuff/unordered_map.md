### std::unordered_map

`std::unordered_map` is an unordered associative container that contains key-value pairs with unique keys. The elements are not sorted in any particular order, but organized into buckets. Which bucket an element is placed into depends entirely on the hash of its key. Keys with the same hash code are placed in the same bucket. `std::unordered_map::bucket() ` is a builtin function that returns the bucket number where the element is stored.

```cpp
template<typename _Key,
	   typename _Tp,
	   typename _Hash = hash<_Key>,
	   typename _Pred = std::equal_to<_Key>,
	   typename _Alloc = std::allocator<std::pair<const _Key, _Tp> >,
	   typename _Tr = __umap_traits<__cache_default<_Key, _Hash>::value>>

    using __umap_hashtable = _Hashtable<_Key, std::pair<const _Key, _Tp>,
                                        _Alloc, __detail::_Select1st,
				        _Pred, _Hash,
				        __detail::_Mod_range_hashing,
				        __detail::_Default_ranged_hash,
				        __detail::_Prime_rehash_policy, _Tr>;

				        ...
 typedef __umap_hashtable<_Key, _Tp, _Hash, _Pred, _Alloc>  _Hashtable;
 ....



template<typename _Key, typename _Tp,
	   typename _Hash = hash<_Key>,
	   typename _Pred = equal_to<_Key>,
	   typename _Alloc = allocator<std::pair<const _Key, _Tp>>>

    class unordered_map
    {
      typedef __umap_hashtable<_Key, _Tp, _Hash, _Pred, _Alloc>  _Hashtable;
      _Hashtable _M_h;

      ....
    }
size_type bucket(const key_type& __key) const
      { return _M_h.bucket(__key); }
```

```cpp
     size_type
      bucket(const key_type& __k) const
      { return _M_bucket_index(this->_M_hash_code(__k)); }


            size_type
      _M_bucket_index(const __node_value_type& __n) const noexcept
      { return __hash_code_base::_M_bucket_index(__n, _M_bucket_count); }

      size_type
      _M_bucket_index(__hash_code __c) const
      { return __hash_code_base::_M_bucket_index(__c, _M_bucket_count); }
```

```cpp
https://gcc.gnu.org/onlinedocs/gcc-4.8.2/libstdc++/api/a01282_source.html
       _M_hash_code(const _Key& __k) const
        { return _M_h1()(__k); }

        std::size_t
        _M_bucket_index(const _Key&, __hash_code __c, std::size_t __n) const
        { return _M_h2()(__c, __n); }
```

```cpp
      std::size_t
      _M_bucket_index(__hash_code __c, std::size_t __bkt_count) const
      { return _RangeHash{}(__c, __bkt_count); }

      std::size_t
      _M_bucket_index(const _Hash_node_value<_Value, false>& __n,
		      std::size_t __bkt_count) const
	noexcept( noexcept(declval<const _Hash&>()(declval<const _Key&>()))
		  && noexcept(declval<const _RangeHash&>()((__hash_code)0,
							   (std::size_t)0)) )
      {
	return _RangeHash{}(_M_hash_code(_ExtractKey{}(__n._M_v())),
			    __bkt_count);
      }

      std::size_t
      _M_bucket_index(const _Hash_node_value<_Value, true>& __n,
		      std::size_t __bkt_count) const
	noexcept( noexcept(declval<const _RangeHash&>()((__hash_code)0,
							(std::size_t)0)) )
      { return _RangeHash{}(__n._M_hash_code, __bkt_count); }

```

```cpp
https://gcc.gnu.org/onlinedocs/gcc-4.7.0/libstdc++/api/a01199_source.html
00678       typedef _Hashtable_ebo_helper<1, _H1> _EboH1;
00679       typedef _Hashtable_ebo_helper<2, _H2> _EboH2;

const _H1&
00735       _M_h1() const { return _EboH1::_S_cget(*this); }
00736       _H1&
00737       _M_h1() { return _EboH1::_S_get(*this); }
00738       const _H2&
00739       _M_h2() const { return _EboH2::_S_cget(*this); }
00740       _H2&
00741       _M_h2() { return _EboH2::_S_get(*this); }

  984       _S_cget(const _Hashtable_ebo_helper& __eboh)
  985       { return static_cast<const _Tp&>(__eboh); }
  986
  987       static _Tp&
  988       _S_get(_Hashtable_ebo_helper& __eboh)
  989       { return static_cast<_Tp&>(__eboh); }
```

According to GCC docs,

```
_RangeHash	The range-hashing function (in the terminology of Tavori and Dreizin). A binary function object whose argument types and result type are all size_t. Given arguments r and N, the return value is in the range [0, N).
```

A complex procedure is used to calculate the bucket index.

To be continued...