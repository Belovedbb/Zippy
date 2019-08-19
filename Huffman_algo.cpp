#include "Huffman_algo.h"

namespace algo {

	huffman_algo::huffman_algo(const char* content) {
		memset(freq, 0, sizeof freq);
		int i = 0;
		while (content[i] != '\0') {
			freq[(unsigned char)content[i]]++;
			i++;
		}
		this->construct_tree(freq);

	}

	void huffman_algo::construct_tree(char* freq) {
		for (int i = 0; i < 256; i++)
			if (freq[i] != 0) {
				auto* node = new huffman_node();
				node->left = nullptr;
				node->right = nullptr;
				node->symbol = (unsigned char)i;
				this->tree.queue_list(*node, static_cast<uint32_t>(freq[i]));
			}
		while (tree.size() > 1) {
			int total_freq = 0, freq_left = 0, freq_right = 0;
			huffman_node* left = nullptr, * right = nullptr, * new_node = nullptr;
			left = new huffman_node();
			right = new huffman_node();
			*left = this->tree.get(freq_left);
			this->tree.deque_list();
			*right = this->tree.get(freq_right);
			this->tree.deque_list();
			total_freq = freq_left + freq_right;
			new_node = new huffman_node();
			new_node->left = left;
			new_node->right = right;
			this->tree.queue_list(*new_node, static_cast<uint32_t>(total_freq));
		}

		int total_freq_;
		base_root = new huffman_node();
		*base_root = tree.get(total_freq_);
		tree.deque_list();
		char code[256];
		symbol_table(base_root, 0, code);
	}

	void huffman_algo::symbol_table(huffman_algo::huffman_node* top_base, short index, char* code_storage) {

		if (top_base->left == nullptr && top_base->right == nullptr) {
			code_storage[index] = '\0';
			huffman_code main_code{};
			memcpy(main_code.code, code_storage, static_cast<size_t>(index));
			main_code.length = index;
			hashmap[(uint32_t)top_base->symbol] = main_code;
			std::cout << std::endl;
			printf("k %d, code :%s symbol %d-> %c\n", index, main_code.code, top_base->symbol, top_base->symbol);
		}

		if (top_base->left != nullptr) {
			code_storage[index] = '0';
			index++;
			symbol_table(top_base->left, index, code_storage);
		}

		if (top_base->right != nullptr) {
			code_storage[index] = '1';
			index++;
			symbol_table(top_base->right, index, code_storage);
		}
	}


	map<uint32_t, huffman_code> huffman_algo::hash() {
		return hashmap;
	};

};//end namespace algo