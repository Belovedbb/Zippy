#pragma once
#include "PriorityList.h"

using std::map;
using list::PriorityList;

namespace algo {

	struct huffman_code {
		unsigned char code[256];
		int32_t length;
	};

	class huffman_algo {
	public:
		explicit huffman_algo(const char* content);
		map<uint32_t, huffman_code> hash();
		huffman_algo(const huffman_algo&) = delete;
	private:
		char freq[256];
		struct huffman_node {
			unsigned char symbol;
			huffman_node* left, * right;
		}; 
		PriorityList<huffman_node> tree{ list::PRIORITY::LOWEST };
		void construct_tree(char*);
		huffman_node* base_root;
		void symbol_table(huffman_node*, short, char*);
		map<uint32_t, huffman_code> hashmap;
		huffman_node& operator=(huffman_node&) {};
	};
};//end namespace algo