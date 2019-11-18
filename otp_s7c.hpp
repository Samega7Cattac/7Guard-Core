/**
 * @file otp_s7c.hpp
 * Purpose: Header file of otp_s7c.cpp
 *
 * @author Samega 7Cattac
 *
 * @version v2.0
 *
 * 7Guard Copyright (C) 2019 Samega 7Cattac // see more: LICENSE
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef OTP_S7C_H
#define OTP_S7C_H

#if defined(_M_AMD64) && defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS // Thanks Microsoft
#elif __linux__
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
        If want to use threads method
    */
	bool use_threads = false;
    /**
        Size of the buffers per block (Default: 99999)
    */
	size_t buffer_size = 99999;
    /**
        Number max of blocks (Default: 10240)
    */
	unsigned int queue_size = 10240;
    /**
        Refresh time of the percentage in seconds (Default: 1)
    */
	unsigned int percentage_interval = 1;

	/**
		Function with the ops to crypt a file.

		@param[in] Filename Path to the file to crypt.
		@param[in] output Path to the folder to output (if == "" then use the Filename path).
		@return 0 if success or negative if fail.
	*/
	int crypt(std::string Filename, std::string output);

	/**
		Function with the ops to decrypt a file.

		@param[in] Filename Path to the crypted file.
		@param[in] key Path to the key file.
		@param[in] output Path to the folder to output (if == "" then use the Filename path).
		@return 0 if success or negative if fail.
	*/
	int decrypt(std::string Filename, std::string key, std::string output);

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
	static void feedback(long int * size, size_t * readed, bool decrypt, unsigned int time);

	/**
		Function to get the filename from a path.

		@param[in] path Path of the file.
		@return string with the filename.
	*/
	std::string GetFileName(std::string path);
};
#endif
