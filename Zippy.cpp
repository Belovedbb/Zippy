#include "Zippy.h"


namespace zippy {

	//*********************************COMPRESSOR***********************************************************************//
	compressor::compressor(const char* filename) : file_name_size(strlen(filename)){

		this->filename = new char[file_name_size];
		
		this->content = new char[file_size];

		strcpy_s(this->filename,file_name_size, filename);
		const ifstream reader = ifstream(filename, std::ios_base::in);
		std::stringstream buffer;
		buffer << reader.rdbuf();
		strcpy_s(content, file_size, buffer.str().c_str());
	}

	compressor::~compressor() {
		delete[] content;
		delete[] filename;
	}

	size_t compressor::compressed_size() const {
		return 0;
	}

	void compressor::compress() {
		compress(encrypt_code::NONE);
	}

	void compressor::utils() {
		zippy::sink sink1;
		zippy::sink::file_type f_type = zippy::sink::NORMAL;
		sink1.create_sink(f_type);

		algo::huffman_algo compressor(content);
		string str(content);
		zippy::buffer buf(str);

		zippy::transform trans(compressor.hash(), filename, file_name_size);
		trans.write_to_sink(sink1);
	}

	void compressor::compress(encrypt_code code) {
		switch (code) {
		case encrypt_code::NONE:
			utils();
			break;
		case encrypt_code::AES:
			break;
		case encrypt_code::BASE64:
			break;
		case encrypt_code::DES:
			break;
		case encrypt_code::XOR:
			break;
		default:
			break;
		};

	}

	
	//**************************end compressor************************************************************************//
	//**************************DECOMPRESSOR**************************************************************************//
	void decompressor::load_content() {
		ifstream input_stream;
		input_stream.open(file_name, std::ios_base::in | std::ios_base::binary);
		if (!input_stream.is_open()) {
			std::cerr << "panic: error in opening compressed file";
			exit(-1);
		}
		string str;
		input_stream.read((char*)& str, file_content_len);
		memcpy(file_content, str.c_str(), sizeof(str.c_str()));
	}
	int validate_compressor() {
		return -1;
	}

	decompressor::decompressor(const char* compressed_file_name) {
		file_name_len = strlen(compressed_file_name);
		file_content_len = fs::file_size(compressed_file_name);
		memcpy(file_name, compressed_file_name, sizeof(compressed_file_name));

	}

	size_t decompressor::decompressed_size() const {
		return 0;
	}

	void decompressor::decompress() {
		load_content();
		//validate_compressor();
	}

	void decompressor::decompress(encrypt_code code) {

	}

	decompressor::~decompressor() {

	}
	//*********************************end decompressor**************************************************************//
};//end namespace zippy