#pragma once
#include "Header.h"
#include "Memory.h"
#include "Huffman_algo.h"

extern size_t file_size;
namespace fs = std::experimental::filesystem;
namespace zippy {

	using std::ifstream;
	using std::string;


	enum class encrypt_code {
		XOR,
		DES,
		AES,
		BASE64,
		NONE
	};

	class compressor {
	public:
		explicit compressor(const char* filename);
		inline size_t compressed_size() const;
		void compress();
		void compress(encrypt_code code);
		void utils();
		~compressor();
	private:
		const int file_name_size;
		char* filename = nullptr; 
		char* content = nullptr; 
	};

	class decompressor {
	public:
		explicit decompressor(const char*);
		inline size_t decompressed_size() const;
		void decompress();
		void decompress(encrypt_code code);
		~decompressor();
	private:
		char* file_name = new char[file_name_len];
		void load_content();
		char* file_content = new char[file_content_len];
		size_t file_name_len;
		size_t file_content_len;
	};


};//end namespace zippy
