#pragma once
#include "Header.h"

namespace list {

	template <typename T, template<typename U, typename V > typename S = std::pair>
	class LinkedList {
	public:
		LinkedList();
		~LinkedList();
		//element as single item
		void add(T item);
		//element as pair of items
		void add(S<T, size_t> item);
		T pop();
		T get(const size_t index);
		S<T, size_t> pop_pair();
		S<T, size_t> get_pair(const size_t index);
		void remove(const size_t index);
		void clear();
		inline size_t size() const;
		bool is_empty();
	private:
		struct Mode {
			bool mode_none;
			bool mode_single;
			bool mode_pair;
		};
		struct Node {
			Node* prev;
			Node* next;
			T item;
			S<T, size_t> item_pair;
		};
		size_t length;
		Node* node;
		Node* temp;
		Node* current;
		Mode mode;
		void deallocation();
		friend ostream& operator<<(ostream&, LinkedList<T, S>&);
	};

	template<typename T, template<typename U, typename V> typename S>
	LinkedList<T, S>::LinkedList() {
		//node = new Node();
		node = nullptr;
		current = node;
		temp = nullptr;
		//node->prev = nullptr;
		//node->next = nullptr;
		//node->item = NULL;
		//node->item_pair = std::make_pair(NULL, NULL);
		mode = { 1,0,0 };
		length = 0;
	}


	template<typename T, template<typename U, typename V> typename S>
	LinkedList<T, S>::~LinkedList()
	{
		deallocation();
	}

	template<typename T, template<typename U, typename V> typename S>
	void LinkedList<T, S>::add(T item)
	{
		mode.mode_single = true;
		if (mode.mode_pair == true)
			exit(-1);

		temp = &(*node);
		if (temp == nullptr) {
			node = new Node();
			temp = node;
			temp->item = item;
			current = node;
			++length;
			return;
		}


		while ((temp->next) != nullptr) {
			temp = temp->next;
			current = temp;
		}

		temp = new Node();
		temp->item = item;
		temp->next = nullptr;
		temp->prev = current;
		current->next = temp;
		++length;
	}

	template <typename T, template <typename U, typename V> typename S>
	void LinkedList<T, S>::add(S<T, size_t> item)
	{
		mode.mode_pair = true;
		if (mode.mode_single == true)
			exit(-1);

		temp = &(*node);
		//&& temp->item_pair.first == NULL 
		if (temp == nullptr) {
			node = new Node();
			temp = node;
			temp->item_pair = item;
			current = node;
			++length;
			return;
		}

		while ((temp->next) != nullptr) {
			temp = temp->next;
			current = temp;
		}


		temp = new Node();
		temp->item_pair = item;
		temp->next = nullptr;
		temp->prev = current;
		current->next = temp;
		++length;
	}

	template<typename T, template<typename U, typename V> typename S>
	T LinkedList<T, S>::pop() {
		if (mode.mode_pair == true)
			exit(-1);
		temp = &(*node);

		while ((temp = temp->next) != nullptr)
			current = temp;

		T ele = current->item;
		if(current->prev != nullptr){
			current = current->prev;
			delete current->next;
			current->next = nullptr;
		}
		else if(size() == 1){
			delete current;
		}
		--length;
		return ele;
	}

	template<typename T, template<typename U, typename V> typename S>
	T LinkedList<T, S>::get(const size_t index)
	{
		if (mode.mode_pair == true)
			exit(-1);

		Node * temp;
		int i = 0;
		for (temp = node; temp != nullptr; temp = temp->next)
			if (i++ == index)
				return temp->item;
		return NULL;
	}

	template<typename T, template<typename U, typename V> typename S>
	S<T, size_t> LinkedList<T, S>::pop_pair()
	{
		if (mode.mode_single == true)
			exit(-1);

		temp = &(*node);

		while ((temp = temp->next) != nullptr)
			current = temp;

		S<T, size_t> ele = current->item_pair;
		current = current->prev;
		delete current->next;
		current->next = nullptr;
		--length;
		return ele;
	}

	template<typename T, template<typename U, typename V> typename S>
	S<T, size_t> LinkedList<T, S>::get_pair(const size_t index)
	{
		if (mode.mode_single == true)
			exit(-1);
		if (index >= size())
			exit(-1);
		Node * temp;
		int i = 0;
		for (temp = node; temp != nullptr; temp = temp->next)
			if (i++ == index)
				return temp->item_pair;

		//T tt = NULL;
		return std::make_pair(temp->item_pair.first, NULL);
	}

	//to be constructed
	template<typename T, template<typename U, typename V> typename S>
	void LinkedList<T, S>::remove(const size_t index)
	{
		Node* temp;
		Node* prev;
		Node* next;
		int i = 0;
		for (temp = node; temp != nullptr; temp = temp->next)
			if (i++ == index) {
				prev = temp->prev;
				next = temp->next;

				//if not head
				if (i - 1 != 0) {
					prev->next = next;
					//handle last index
					if (next != nullptr)
						next->prev = prev;
					//handle current
					if (i == size() - 1)
						current = next;
				}
				else
					node = nullptr;


				delete temp;
				--length;

				return;
			}
	}
	//to be constructed
	template<typename T, template<typename U, typename V> typename S>
	inline void LinkedList<T, S>::clear()
	{
		deallocation();
		node = nullptr;
		current = node;
		temp = nullptr;
		mode = { 1,0,0 };
		length = 0;
	}

	template<typename T, template<typename U, typename V> typename S>
	size_t LinkedList<T, S>::size() const
	{
		return length;
	}

	template<typename T, template<typename U, typename V> typename S>
	bool LinkedList<T, S>::is_empty()
	{
		return length == 0;
	}

	template<typename T, template<typename U, typename V> typename S>
	inline void LinkedList<T, S>::deallocation()
	{
		if (node == nullptr)
			;
		else if ((node->prev == nullptr && node->next == nullptr) || is_empty()) {
			delete node;
			node = nullptr;
		}

		else {
			Node* temp;
			Node* del_next;
			for (temp = current->next == nullptr ? current : current->next; true; ) {
				if (temp->next == nullptr)
					break;
				del_next = temp->next;
				delete temp;
				temp = del_next;
			}

			node = nullptr;
		}
	}

	template<typename T, template<typename U, typename V> typename S>
	ostream& operator<<(ostream & out, LinkedList<T, S> & list) {
		out << "list: [";
		for (LinkedList::Node* temp = list.node; temp != nullptr; temp = temp->next)
			out << "item " << list.node->item << std::endl;
		out << "]";
		return out;
	}

}; //end namespace list 