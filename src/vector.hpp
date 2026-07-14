#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <cstring>
#include <new>

namespace sjtu
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the
	// iterator points to.
	// STL algorithms and containers may use these type_traits (e.g. the following
	// typedef) to work properly. In particular, without the following code,
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://blog.csdn.net/u014299153/article/details/72419713
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		pointer ptr;
	public:
		iterator() : ptr(nullptr) {}
		iterator(pointer p) : ptr(p) {}

		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const
		{
			//TODO
			return iterator(ptr + n);
		}
		iterator operator-(const int &n) const
		{
			//TODO
			return iterator(ptr - n);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			//TODO
			return static_cast<int>(ptr - rhs.ptr);
		}
		iterator& operator+=(const int &n)
		{
			//TODO
			ptr += n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			//TODO
			ptr -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator tmp = *this;
			++ptr;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			++ptr;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator tmp = *this;
			--ptr;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			--ptr;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return *ptr;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const {
			return ptr == rhs.get_ptr();
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const {
			return ptr != rhs.get_ptr();
		}

		// Helper to get the pointer for const_iterator comparison
		pointer get_ptr() const { return ptr; }
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/*TODO*/
		pointer ptr;
	public:
		const_iterator() : ptr(nullptr) {}
		const_iterator(pointer p) : ptr(p) {}
		const_iterator(const iterator& it) : ptr(it.get_ptr()) {}

		pointer get_ptr() const { return ptr; }

		const_iterator operator+(const int &n) const {
			return const_iterator(ptr + n);
		}
		const_iterator operator-(const int &n) const {
			return const_iterator(ptr - n);
		}
		int operator-(const const_iterator &rhs) const {
			return static_cast<int>(ptr - rhs.ptr);
		}
		const_iterator& operator+=(const int &n) {
			ptr += n;
			return *this;
		}
		const_iterator& operator-=(const int &n) {
			ptr -= n;
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator tmp = *this;
			++ptr;
			return tmp;
		}
		const_iterator& operator++() {
			++ptr;
			return *this;
		}
		const_iterator operator--(int) {
			const_iterator tmp = *this;
			--ptr;
			return tmp;
		}
		const_iterator& operator--() {
			--ptr;
			return *this;
		}
		const T& operator*() const {
			return *ptr;
		}
		bool operator==(const iterator &rhs) const {
			return ptr == rhs.get_ptr();
		}
		bool operator==(const const_iterator &rhs) const {
			return ptr == rhs.ptr;
		}
		bool operator!=(const iterator &rhs) const {
			return ptr != rhs.get_ptr();
		}
		bool operator!=(const const_iterator &rhs) const {
			return ptr != rhs.ptr;
		}

		// Allow iterator to access get_ptr
		friend class iterator;
	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
private:
	T* data_;
	size_t size_;
	size_t capacity_;

public:
	vector() : data_(nullptr), size_(0), capacity_(0) {}
	vector(const vector &other) : data_(nullptr), size_(0), capacity_(0) {
		if (other.size_ > 0) {
			data_ = static_cast<T*>(::operator new(other.capacity_ * sizeof(T)));
			capacity_ = other.capacity_;
			size_ = other.size_;
			for (size_t i = 0; i < size_; ++i) {
				new (data_ + i) T(other.data_[i]);
			}
		}
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		if (data_) {
			for (size_t i = 0; i < size_; ++i) {
				data_[i].~T();
			}
			::operator delete(data_);
		}
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if (this != &other) {
			// Destroy existing elements
			for (size_t i = 0; i < size_; ++i) {
				data_[i].~T();
			}

			// If capacity is not enough, reallocate
			if (capacity_ < other.capacity_) {
				::operator delete(data_);
				data_ = static_cast<T*>(::operator new(other.capacity_ * sizeof(T)));
				capacity_ = other.capacity_;
			}

			size_ = other.size_;
			for (size_t i = 0; i < size_; ++i) {
				new (data_ + i) T(other.data_[i]);
			}
		}
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) {
		if (pos >= size_) {
			throw index_out_of_bound();
		}
		return data_[pos];
	}
	const T & at(const size_t &pos) const {
		if (pos >= size_) {
			throw index_out_of_bound();
		}
		return data_[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		if (pos >= size_) {
			throw index_out_of_bound();
		}
		return data_[pos];
	}
	const T & operator[](const size_t &pos) const {
		if (pos >= size_) {
			throw index_out_of_bound();
		}
		return data_[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const {
		if (size_ == 0) {
			throw container_is_empty();
		}
		return data_[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const {
		if (size_ == 0) {
			throw container_is_empty();
		}
		return data_[size_ - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		return iterator(data_);
	}
	const_iterator begin() const {
		return const_iterator(data_);
	}
	const_iterator cbegin() const {
		return const_iterator(data_);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(data_ + size_);
	}
	const_iterator end() const {
		return const_iterator(data_ + size_);
	}
	const_iterator cend() const {
		return const_iterator(data_ + size_);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return size_ == 0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return size_;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for (size_t i = 0; i < size_; ++i) {
			data_[i].~T();
		}
		size_ = 0;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		size_t index = pos - begin();
		if (index > size_) {
			throw index_out_of_bound();
		}
		return insert(index, value);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		if (ind > size_) {
			throw index_out_of_bound();
		}
		// Check if we need to reallocate
		if (size_ + 1 > capacity_) {
			size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
			T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));

			// Copy/move elements before ind
			for (size_t i = 0; i < ind; ++i) {
				new (new_data + i) T(data_[i]);
			}

			// Construct new element
			new (new_data + ind) T(value);

			// Copy/move elements after ind
			for (size_t i = ind; i < size_; ++i) {
				new (new_data + i + 1) T(data_[i]);
				data_[i].~T();
			}

			// Destroy old data
			if (data_) {
				::operator delete(data_);
			}

			data_ = new_data;
			capacity_ = new_capacity;
			size_ += 1;
		} else {
			// No reallocation needed
			// Use a temporary copy of value to avoid issues if value is in the vector
			T tmp_value = value;
			
			// Shift elements to the right using placement new and explicit destroy
			if (size_ > 0) {
				// Construct copy of last element at position size_
				new (data_ + size_) T(data_[size_ - 1]);
				
				// Shift elements right one by one
				for (size_t i = size_; i > ind; --i) {
					// Destroy element at i first
					data_[i].~T();
					// Then placement new to copy from i-1
					new (data_ + i) T(data_[i - 1]);
				}
				// Destroy element at ind to make room
				data_[ind].~T();
			}
			
			// Place the new element
			new (data_ + ind) T(tmp_value);
			size_ += 1;
		}

		return iterator(data_ + ind);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		size_t index = pos - begin();
		if (index >= size_) {
			throw index_out_of_bound();
		}
		return erase(index);
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) {
		if (ind >= size_) {
			throw index_out_of_bound();
		}
		// Destroy element at ind
		data_[ind].~T();

		// Shift elements to the left
		for (size_t i = ind; i + 1 < size_; ++i) {
			new (data_ + i) T(data_[i + 1]);
			data_[i + 1].~T();
		}

		size_ -= 1;

		return iterator(data_ + ind);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		if (size_ + 1 > capacity_) {
			size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
			T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));

			// Copy existing elements
			for (size_t i = 0; i < size_; ++i) {
				new (new_data + i) T(data_[i]);
				data_[i].~T();
			}

			// Add new element
			new (new_data + size_) T(value);

			// Delete old data
			if (data_) {
				::operator delete(data_);
			}

			data_ = new_data;
			capacity_ = new_capacity;
			size_ += 1;
		} else {
			new (data_ + size_) T(value);
			size_ += 1;
		}
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() {
		if (size_ == 0) {
			throw container_is_empty();
		}
		data_[size_ - 1].~T();
		size_ -= 1;
	}
};


}

#endif