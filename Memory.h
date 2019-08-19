#pragma once
#include "huffman_algo.h"
#include <string_view>

namespace zippy {

	typedef short small;
	using std::map;
	using std::string;
	using std::list;
	class transform;

	class sink {
	public:
		enum status_flag { CORRUPTED = -1, GOOD, BAD, INCOMPLETE, COMPLETE };
		enum file_type { NORMAL, TEMP };
		status_flag write(transform&, char*);
		sink() = default;
		void create_sink(file_type&);
		status_flag get_status();
	private:
		struct compressed {
			const char* compressed_content;
			size_t len = 0;
		};
		status_flag stat = status_flag(0);
	};

	class transform {
	public:
		transform(map<uint32_t, algo::huffman_code> header, const char* file_name, size_t file_name_size) :
			header(std::move(header)) {
			this->file_name_size = file_name_size;
			strcpy(this->file_name, file_name);
		}
		const char* get_header();
		const char* get_content();
		small write_to_sink(sink&);
	private:
		map<uint32_t, algo::huffman_code> header;
		string content;
		friend bool validate_length(const transform&, int&);
		friend size_t header_len_(zippy::transform a);
		size_t file_name_size;
		char* file_name = new char[file_name_size];
	};

	class buffer {
	private:
		enum { BUFFER_SIZE = 256 };
		enum class BUFFER_STATE { START, TRANSIT, END };
		friend size_t compute_rem_size(const string& content);
		friend size_t incrementer(size_t rem_size);
	public:
		buffer(const std::string&);
		void flush_input();
		inline static bool is_empty();
		static string& send_buf();
		static inline bool end_of_content();
		struct internals {
			char* internal_buffer = new char[BUFFER_SIZE];
			size_t* b_location;
		};
	private:
		BUFFER_STATE buf_state;
		internals* load_internals();
		void setup(const string&, internals&);
		static size_t content_index;
		size_t index_temp = 0;
		void update();

	};

};//end namespace zippy