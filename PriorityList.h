#pragma once
#include "Header.h"
#include "LinkedList.h"

namespace list {
	enum class PRIORITY { HIGHEST, LOWEST };

	template <typename T, typename U = int>
	class PriorityList : private LinkedList<T> {
	public:
		
		PriorityList(PRIORITY state) : LinkedList<T>(), state(state) {};
		~PriorityList();
		void queue_list(T item, U priority_value);
		T get(U& store_priority);
		void deque_list();
		size_t size();
		bool is_empty();
	private:
		PRIORITY state;
		std::pair<T, U> item_priority;
		std::pair<T, U>* pair_array;
		void priority_low();
		void priority_high();
	};

	template<typename T, typename U>
	PriorityList<T, U>::~PriorityList()
	{
	}

	template<typename T, typename U>
	inline void PriorityList<T, U>::queue_list(T item, U priority_value)
	{
		item_priority = std::make_pair(item, priority_value);
		LinkedList<T>::add(item_priority);
	}

	template<typename T, typename U>
	inline T PriorityList<T, U>::get(U& store_priority)
	{
		if (state == PRIORITY::LOWEST)
			priority_low();


		else
			priority_high();

		const std::pair<T, size_t> p = LinkedList<T>::get_pair(size() - 1);
		store_priority = p.second;
		return p.first;
	}

	template<typename T, typename U>
	inline void PriorityList<T, U>::deque_list() {
		if (state == PRIORITY::LOWEST) {
			priority_low();
			LinkedList<T>::remove(size() - 1);
		}

		else {
			priority_high();
			LinkedList<T>::remove(size() - 1);
		}

	}

	template<typename T, typename U>
	inline size_t PriorityList<T, U>::size()
	{
		return LinkedList<T>::size();
	}

	template<typename T, typename U>
	inline bool PriorityList<T, U>::is_empty()
	{
		return LinkedList<T>::is_empty();
	}

	//helper functions
	template<typename T, typename U>
	void PriorityList<T, U>::priority_high() {
		if (size() < 2)
			return;
		size_t len = size();
		pair_array = new std::pair<T, U>[len];
		for (size_t i = 0; i < len; i++)
			pair_array[i] = LinkedList<T>::get_pair(i);
		LinkedList<T>::clear();

		for (int i = len - 1; i >= 0; i--) {
			for (int j = 1; j <= i; j++) {
				if (pair_array[j - 1].second > pair_array[j].second)
					std::swap(pair_array[j - 1], pair_array[j]);
			}
		}

		for (size_t j = 0; j < len; j++)
			LinkedList<T>::add(pair_array[j]);

		delete[] pair_array;
	}

	template<typename T, typename U>
	void PriorityList<T, U>::priority_low() {
		if (size() < 2)
			return;
		size_t len = size();
		pair_array = new std::pair<T, U>[len];
		for (size_t i = 0; i < len; i++)
			pair_array[i] = LinkedList<T>::get_pair(i);
		LinkedList<T>::clear();

		for (int i = len - 1; i >= 0; i--) {
			for (int j = 1; j <= i; j++) {
				if (pair_array[j - 1].second < pair_array[j].second)
					std::swap(pair_array[j - 1], pair_array[j]);
			}
		}

		for (size_t j = 0; j < len; j++)
			LinkedList<T>::add(pair_array[j]);

		delete[] pair_array;
	}

}; //end namespace list
