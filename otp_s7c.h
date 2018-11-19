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
#if __linux__
#include <cstring> // memset() (Only for linux)
#endif
#include <immintrin.h> // _rdrand16_step()
#include <iomanip> // std::setprecision()
#include <iostream>
#include <thread>
#include <chrono>

class otp_s7c
{
public:
	/**
		Function with the ops to crypt a file.

		@param[in] Filename Path to the file to crypt.
		@param[in] output Path to the folder to output (if == "" then use the Filename path).
		@param[in] buf_size Size of the buffers.
		@param[in] threads Number max of block in queue.
		@param[in] time Time (in seconds) to refresh the percentage.
		@return 0 if success or negative if fail.
	*/
	int crypt(std::string Filename, std::string output, size_t buf_size, unsigned int threads, unsigned int time);

	/**
		Function with the ops to decrypt a file.

		@param[in] Filename Path to the crypted file.
		@param[in] key Path to the key file.
		@param[in] output Path to the folder to output (if == "" then use the Filename path).
		@param[in] buf_size Size of the buffers.
		@param[in] time Time (in seconds) to refresh the percentage.
		@return 0 if success or negative if fail.
	*/
	int decrypt(std::string Filename, std::string key, std::string output, size_t buf_size, unsigned int time);

private:

	// Block used to create a queue for threads processing
	struct block
	{
		unsigned char * c = nullptr; // Pointer to the char array
		unsigned int size = 0; // Size of the array
		block * next = nullptr; // Pointer to the next block
	};

	/**
		Thread that will handle the calculations.

		@param[in, out] rc Pointer to the block list Read-Calc.
		@param[in, out] cw Pointer to the block list Calc-Write.
		@param[in, out] ck Pointer to the block list Calc-WriteK.
		@param[in, out] rc_n Pointer to the number of blocks in queue Read-Calc.
		@param[in, out] cw_n Pointer to the number of blocks in queue Calc-Write.
		@param[in, out] ck_n Pointer to the number of blocks in queue Calc-WriteK.
		@param[in] read_done If the Read process is completed.
		@param[in, out] calc_done If the Calc process is completed.
	*/
	static void calc(block * rc, block * cw, block * ck, unsigned int * rc_n, unsigned int * cw_n, unsigned int * ck_n, bool * read_done, bool * calc_done);

	/**
		Thread that will handle the write process.

		@param[in, out] cw Pointer to the block list Calc-Write.
		@param[in, out] cw_n Pointer to the number of blocks in queue Calc-Write.
		@param[in] calc_done If the Calc process is completed.
		@param[in, out] d Pointer to the output file.
		@param[in, out] writed Pointer to number of bytes already wrote.
	*/
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
		@param[in] time Time (in seconds) to refresh the percentage.
	*/
	static void feedback(size_t * size, size_t * readed, bool decrypt, unsigned int time);

	/**
		Function to get the filename from a path.

		@param[in] path Path of the file.
		@return string with the filename.
	*/
	std::string GetFileName(std::string path);
};
