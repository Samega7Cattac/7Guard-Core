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
#include <sys/mman.h>
#include <sys/stat.h>
//#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif
#include <immintrin.h> // _rdrand16_step()
#include <iomanip> // std::setprecision()
#include <iostream>
#include <thread>
#include <chrono>
#include <semaphore.h>

class otp_s7c
{
public:
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

	/**
		Displays the percentage of the processed file.

		@param[in] Pointer to size of the file.
		@param[in] Pointer to number of bytes that already have been read.
		@param[in] decrypt If true then use word "decrypt" instead of "crypt".
		@param[in] time Time (in seconds) to refresh the percentage.
	*/
	static void feedback(unsigned long long int * size, unsigned long long int * readed, bool decrypt, unsigned int time);
    
	/**
		Function to get the filename from a path.

		@param[in] path Path of the file.
		@return string with the filename.
	*/
	std::string GetFileName(std::string path);
};
#endif
