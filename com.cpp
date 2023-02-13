#include <iostream>
#include <fstream>
#include <zlib.h>

const int CHUNK_SIZE = 1024;

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " [input file] [output file]" << std::endl;
    return 1;
  }

  // Open input and output files
  std::ifstream input_file(argv[1], std::ios::binary);
  std::ofstream output_file(argv[2], std::ios::binary);

  // Initialize zlib structures
  z_stream strm;
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  deflateInit(&strm, Z_DEFAULT_COMPRESSION);

  // Buffer for the input and compressed data
  char input_data[CHUNK_SIZE];
  char compressed_data[CHUNK_SIZE];

  // Compress the input data chunk by chunk
  do {
    input_file.read(input_data, CHUNK_SIZE);
    int bytes_read = input_file.gcount();
    strm.avail_in = bytes_read;
    strm.next_in = (Bytef *)input_data;
    do {
      strm.avail_out = CHUNK_SIZE;
      strm.next_out = (Bytef *)compressed_data;
      deflate(&strm, Z_NO_FLUSH);
      output_file.write(compressed_data, CHUNK_SIZE - strm.avail_out);
    } while (strm.avail_out == 0);
  } while (input_file);

  // Finish compression
  strm.avail_in = 0;
  do {
    strm.avail_out = CHUNK_SIZE;
    strm.next_out = (Bytef *)compressed_data;
    deflate(&strm, Z_FINISH);
    output_file.write(compressed_data, CHUNK_SIZE - strm.avail_out);
  } while (strm.avail_out == 0);

  // Clean up and return
  deflateEnd(&strm);
  return 0;
}
