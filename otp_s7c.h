/**
 * @file otp_s7c.h
 * Purpose: Header file of otp_s7c.cpp
 *
 * @author Samega 7Cattac
 *
 * @version 1.2
 *
 * 7Guard Copyright (C) 2018 Samega 7Cattac // see more: LICENSE
 */
#ifdef _WIN32
//#define _CRT_SECURE_NO_WARNINGS // Enable the use of "fopen()" instead of "fopen_s()" (Only for Windows)
#elif __linux__
#include <cstring> // memset() (Only for linux)
#endif
#include <immintrin.h> // _rdrand16_step()
#include <iomanip> // std::setprecision()
#include <iostream>
#include <thread> // std::thread
#include <chrono>

using namespace std::this_thread;
using namespace std::chrono;
using std::cout;
using std::endl;
using std::fixed;
using std::setprecision;
using std::string;
using std::thread;

class otp_s7c
{
public:
	/**
		Function with the ops to crypt a file.

		@param[in] Filename Path to the file to crypt.
		@param[in] output Path to the folder to output (if == "" then use the Filename path).
		@param[in] buf_size Size of the buffers.
		@return 0 if success or negative if fail.
	*/
	int crypt(string Filename, string output, size_t buf_size, unsigned int threads);

	/**
		Function with the ops to decrypt a file.

		@param[in] Filename Path to the crypted file.
		@param[in] key Path to the key file.
		@param[in] output Path to the folder to output (if == "" then use the Filename path).
		@param[in] buf_size Size of the buffers.
		@return 0 if success or negative if fail.
	*/
	int decrypt(string Filename, string key, string output, size_t buf_size, int threads);

private:

	struct block
	{
		unsigned char * c = nullptr;
		unsigned int size = 0;
		block * next = nullptr;
	};

	static void calc(block * rc, block * cw, block * ck, unsigned int * rc_n, unsigned int * cw_n, unsigned int * ck_n, bool * read_done, bool * calc_done);

	static void write(block * cw, unsigned int * cw_n, bool * calc_done, FILE * d, size_t * writed);

	/**
		Function to allocate memory to the buffers.

		@param[in, out] Pointer to "buf" buffer.
		@param[in, out] Pointer to "num" buffer.
		@param[in] size Size of the buffer.
		@param[in] max If true then is to allocate with max space possible.
		@return Size of the buffer if success or 0 if fail.
	*/
	int alloc(unsigned char ** buf, unsigned char ** num, size_t size, bool max);

	/**
		Displays the percentage of the processed file.

		@param[in] Pointer to size of the file.
		@param[in] Pointer to number of bytes that already have been read.
		@param[in] decrypt If true then use word "decrypt" instead of "crypt".
	*/
	static void feedback(size_t * size, size_t * readed, bool decrypt);

	/**
		Function to get the filename from a path.

		@param[in] path Path of the file.
		@return string with the filename.
	*/
	string GetFileName(string path);
};
