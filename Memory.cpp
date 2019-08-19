#include "Memory.h"

namespace zippy {
	static buffer::internals* loads;
	const string* p_content = nullptr;
	//**************************************TRANSFORM******************************************************************//
	bool validate_length(const transform&, int&);

	const char* transform::get_header() {
		const short ZIPPY_LEN = 5;
		char* zip_header = new char[header_len_(*this) + 1];
		zip_header[0] = 'z';
		zip_header[1] = 'i';
		zip_header[2] = 'p';
		zip_header[3] = 'p';
		zip_header[4] = 'y';
		int k = ZIPPY_LEN;
		for (auto i = header.begin(); i != header.end(); i++, k++) {
			zip_header[k] = static_cast<char>(i->first);
			int j = 0;
			while (i->second.code[j] != '\0' && j != i->second.length) {
				zip_header[++k] = i->second.code[j];
				j++;
			}
		}
		//if(validate_length(*this, k))//
		return zip_header;
		//return nullptr;
	}

	const char* transform::get_content() {
		char* zip_content = new char[p_content->size()];
		while ((!buffer::end_of_content() || !buffer::is_empty())) {
			content.append(buffer::send_buf());
			loads->b_location = reinterpret_cast<size_t*>(&loads->internal_buffer[0]);
		}
		strcpy(zip_content, content.c_str());
		return zip_content;
	}

	small transform::write_to_sink(class sink& sink1) {
		sink::file_type f_type = sink::file_type::NORMAL;
		sink1.create_sink(f_type);
		sink::status_flag  flag = sink1.write(*this, file_name);
		return flag;
	}

	bool validate_length(const transform& trans, int& written_len) {
		size_t count = 0;
		count = header_len_(trans);
		return count == written_len;
	}
	size_t header_len_(zippy::transform a) {
		size_t count = 0;
		for (auto i = a.header.begin(); i != a.header.end(); i++)
			count += i->second.length;
		return count;
	}
	//**************************************END TRANSFORM************************************************************//

//*****************************************BUFFER******************************************************************//
	//initialize statics
	size_t buffer::content_index = -1;
	//note size might be 0
	int rem_pass_id = 0;
	size_t len = 0;
	buffer* a;

	size_t compute_rem_size(const string & content) {
		if (rem_pass_id++ == 0)
			len = content.size();
		if (len < buffer::BUFFER_SIZE)
			return len;

		else {
			len -= buffer::BUFFER_SIZE;
			return len;
		}
	}

	size_t incrementer(const size_t rem_size) {
		if (rem_size < buffer::BUFFER_SIZE) {
			buffer::content_index = 0;
			return (buffer::content_index += rem_size);
		}
		else {
			buffer::content_index += buffer::BUFFER_SIZE;
			return buffer::content_index;
		}
	}

	void grow_buffer(buffer::internals * stub) {
		size_t new_len = compute_rem_size(*p_content) + 1;
		char* new_buf = new char[new_len];
		stub->internal_buffer = new_buf;
	}

	void send_to_buf() {
		memcpy(loads->internal_buffer, p_content->substr(static_cast<unsigned long>(compute_rem_size(*p_content)),
			p_content->size()).c_str(),sizeof(loads->internal_buffer));
	}

	bool buffer::is_empty() {
		return loads->b_location == reinterpret_cast<size_t*>(&loads->internal_buffer[0]);
	}


	//force content from source into buffer
	void buffer::flush_input() {
		if (content_index != 0) {
			grow_buffer(loads);
			send_to_buf();
			content_index = 0;
		}
	}


	buffer::buffer(const string & content) {
		p_content = &content;
		a = this;
		loads = load_internals();
		buf_state = BUFFER_STATE::START;
	}



	void buffer::setup(const string & content, internals & skeleton) {
		buf_state = BUFFER_STATE::TRANSIT;
		//discard elements
		//tackle is_full()
		if (index_temp < BUFFER_SIZE - 1 && rem_pass_id != 0) {
			memset(skeleton.internal_buffer, 0, sizeof(skeleton.internal_buffer));
			strcpy(skeleton.internal_buffer, content.substr(index_temp, index_temp + buffer::BUFFER_SIZE).c_str());
			skeleton.b_location = reinterpret_cast<size_t*>(&skeleton.internal_buffer[0]);
			content_index = 0;
			len = 0;
			buf_state = BUFFER_STATE::END;
		}
		else if (is_empty()) {
			memset(skeleton.internal_buffer, 0, sizeof(skeleton.internal_buffer));
			strcpy(skeleton.internal_buffer, content.substr(index_temp, index_temp + buffer::BUFFER_SIZE).c_str());
			skeleton.b_location = reinterpret_cast<size_t*>(&skeleton.internal_buffer[256]);
			size_t len = compute_rem_size(content);
			index_temp = incrementer(len);
		}
		else {
			std::cerr << "unable to write to buffer";
			buf_state = BUFFER_STATE::END;
			exit(EXIT_FAILURE);
		}
	}

	buffer::internals* buffer::load_internals() {
		internals* intern = new internals();
		memset(intern->internal_buffer, 0, sizeof(intern->internal_buffer));
		intern->b_location = reinterpret_cast<size_t*>(&intern->internal_buffer[0]);
		return intern;
	}

	string& buffer::send_buf() {
		a->update();
		string* str = new (std::nothrow) string(loads->internal_buffer);
		if (str == nullptr)
			exit(EXIT_FAILURE);
		return *str;
	}

	void buffer::update() {
		// content_index+=BUFFER_SIZE;
		if (content_index == 0 && (buf_state == BUFFER_STATE::END)) {
			loads->b_location = reinterpret_cast<size_t*>(&loads->internal_buffer[0]);
			return;
		}
		else if (buf_state == BUFFER_STATE::START || buf_state == BUFFER_STATE::TRANSIT)
			setup(*p_content, *loads);
	}

	bool buffer::end_of_content() {
		return content_index == 0;
	}

	//**************************************END BUFFER******************************************************************//

	//*****************************************SINK******************************************************************//
		//create duplicate file for sink operation

	void sink::create_sink(sink::file_type&) {

	}

	sink::status_flag sink::get_status() {
		return GOOD;
	}
	static size_t ref_count = 0;
	string hdr;
	void mapping(string & sink, const char* header, const char* content, size_t pos) {
		if (ref_count == 0) {
			hdr.assign(header);
			++ref_count;
		}
		size_t content_len = strlen(content);
		if (pos == content_len) {
			return;
		}

		else {
			char current_char = content[pos];
			size_t char_pos = hdr.find(current_char);
			++char_pos;
			while (header[char_pos] == '0' || header[char_pos] == '1') {
				sink.push_back(header[char_pos]);
				++char_pos;
			}
			mapping(sink, header, content, ++pos);
		}
	}
	const char* get_compressed_filename(const char* original, sink::file_type ft = sink::file_type::NORMAL) {
		string str(original);
		string str_new;
		if (ft == sink::file_type::NORMAL) {
			size_t trunk_len = str.rfind('.');
			if (trunk_len > str.size())
				return nullptr;
			return str.substr(0, trunk_len).append(".zippy").c_str();
		}
		else if (ft == sink::file_type::TEMP) {

		}
		return nullptr;
	}

	size_t rem_size = 0;
	size_t tracker = 0;
	std::string_view rem_str;
	void internal_bin(const std::string_view & str, std::vector<std::bitset<8>> & appender, const int factor) {

		if (rem_size == 0)
			return;
		else if (rem_size <= factor) {
			std::string_view strr = (rem_str.substr(0, rem_size));
			std::bitset<8> bit(strr.data());
			appender.push_back(bit);
			rem_str = "";
			rem_size = rem_str.size();
			internal_bin(rem_str, appender, factor);
		}
		else if (rem_size > factor && rem_size <= factor * 2) {
			std::string_view strr = rem_str.substr(0, factor);
			std::bitset<8> bit(strr.data());
			appender.push_back(bit);
			rem_str = rem_str.substr(0, rem_size - factor);
			rem_size = rem_str.size();
			internal_bin(rem_str, appender, factor);
		}
		else {
			std::string_view strr = rem_str.substr(0, factor);
			std::bitset<8> bit(strr.data());
			appender.push_back(bit);
			rem_str = rem_str.substr(factor, rem_size - factor);
			rem_size = rem_str.size();
			internal_bin(rem_str, appender, factor);
		}

	}

	std::vector<std::bitset<8>> bin_dec(const string & binary_str) {
		std::vector<std::bitset<8>> lt;
		lt.reserve(binary_str.size() * 4);
		rem_str = binary_str;
		rem_size = rem_str.size();
		internal_bin(binary_str, lt, 8);
		return lt;
	}

	sink::status_flag sink::write(transform & encoders, char* filename) {
		std::string_view header(encoders.get_header());
		std::string_view content(encoders.get_content());
		
		//char* zip_header = new char[strlen(encoders.get_header())];
		//char* zip_content = new char[p_content->size()];

		//strcpy(zip_header, encoders.get_header());
		//strcpy(zip_content, encoders.get_content());

		string compressed_char;
		compressed_char.append(header);
		//std::bitset<32> header_bits(zip_header);
		string mapped_string;
		mapped_string.reserve(content.size() * 4);
		size_t position = 0;
		mapping(mapped_string, header.data(), content.data(), position);
		//compressed_char.append(mapped_string);
		std::vector<std::bitset<8>> compressed_bits = bin_dec(mapped_string);
		std::ofstream  out(get_compressed_filename(filename), std::ios_base::out | std::ios_base::binary);
		if (!out.is_open()) {
			std::cerr << "panic: failed to open";
			return BAD;
		}
		out.write((char*)& compressed_char, sizeof(compressed_char));
		out.write((char*)& compressed_bits, compressed_bits.size());
		out.close();
		return GOOD;
	}


}
//*************************************END SINK******************************************************************//