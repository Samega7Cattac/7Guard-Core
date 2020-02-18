/**
 * @file otp_s7c.cpp
 * Purpose: File with the functions for crypt and decrypt, also some aux functions
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
#include "otp_s7c.hpp"

void otp_s7c::feedback(unsigned long long int * size, unsigned long long int * readed, bool decrypt, unsigned int time)
{
	
	float p = 0;
	while (*readed < *size || p < 100)
	{
		p = *readed * (float)100.00 / *size;
		std::cout << "\r" << "[INFO] Start" << (decrypt ? " de" : " ") << "crypting... (" << std::fixed << std::setprecision(2) << p << "%)";
		fflush(stdout);
		if (time != 0 && p < 100) std::this_thread::sleep_for(std::chrono::seconds(time));
	}
	std::cout << std::endl;
	return;
}

std::string otp_s7c::GetFileName(std::string path)
{
	std::string filename = "";
	for (unsigned int i = 0; i < path.length(); ++i)
	{
		if (path[i] == '\\') filename = "";
		else filename += path[i];
	}
	return filename;
}

int otp_s7c::crypt(std::string Filename, std::string output)
{
	int error = 0;
    umask(0000);
    int file = open(Filename.c_str(), O_RDONLY);
	if (file > 0)
	{
		std::string Filename_key = "\0";
		if (output != "")
		{
			if (output.find_last_of("\"") == output.length() - 1) output = output.substr(0, output.length() - 1);
			if (output.find_last_of("\\") != std::string::npos) output.append("\\");
			Filename_key = output;
			Filename_key.append(GetFileName(Filename) + ".7ky");
		}
		else Filename_key = Filename + std::string(".7ky");
		int key_file = open(Filename_key.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0770);
		if (key_file > 0)
		{
			std::string Filename_crypt = "\0";
			if (output != "")
			{
				Filename_crypt = output;
				Filename_crypt.append(GetFileName(Filename) + ".7cy");
			}
			else Filename_crypt = Filename + std::string(".7cy");
			int crypt_file = open(Filename_crypt.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0770);
			if (crypt_file > 0)
			{
				unsigned char val[8];
				if (_rdrand64_step((long long unsigned int *)val))
				{
                    unsigned long long int file_size = lseek64(file, 0, SEEK_END);
                    ftruncate64(key_file, file_size);
                    ftruncate64(crypt_file, file_size);
                    unsigned char * file_mmap = static_cast<unsigned char *>(mmap64(nullptr, file_size, PROT_READ, MAP_SHARED/*MAP_PRIVATE*/, file, 0));
                    unsigned char * key_mmap = static_cast<unsigned char *>(mmap64(nullptr, file_size, PROT_WRITE | PROT_READ, MAP_SHARED/*MAP_PRIVATE*/, key_file, 0));
                    unsigned char * crypt_mmap = static_cast<unsigned char *>(mmap64(nullptr, file_size, PROT_WRITE | PROT_READ, MAP_SHARED/*MAP_PRIVATE*/, crypt_file, 0));
                    unsigned long long int readed = 0;
                    std::thread feedback_t(feedback, &file_size, &readed, false, percentage_interval);
                    short p = 0;
                    for (unsigned long long int i = 0; i < file_size; i += p, readed += p)
                    {
                        _rdrand64_step((long long unsigned int *)val);
                        for (p = 0; p < 8 && i + p < file_size; ++p)
                        {
                            crypt_mmap[i + p] = file_mmap[i + p] + val[p];
                            key_mmap[i + p] = val[p];
                        }
                    }
                    memset(&val, 0, sizeof(val));
                    feedback_t.join();
                    msync(crypt_mmap, file_size, MS_SYNC);
                    msync(key_mmap, file_size, MS_SYNC);
                    munmap(file_mmap, file_size);
                    munmap(key_mmap, file_size);
                    munmap(crypt_mmap, file_size);
				}
				else
				{
					std::cerr << "[ERROR] Error generating hardware random value" << std::endl;
					error = -4;
				}
				close(key_file);
			}
			else
			{
				perror("[ERROR] Error opening the output");
				error = -3;
			}
			close(crypt_file);
		}
		else
		{
			perror("[ERROR] Error opening the key file");
			error = -2;
		}
		close(file);
	}
	else
	{
		perror("[ERROR] Error opening the file to crypt");
		error = -1;
	}
	return error;
}

int otp_s7c::decrypt(std::string Filename, std::string key, std::string output)
{
	int error = 0;
	int filename_crypt = open(Filename.c_str(), O_RDONLY);
	if (filename_crypt)
	{
		int filename_key = open(key.c_str(), O_RDONLY);
		if (filename_key)
		{
			if (output != "")
			{
				if (output.find_last_of("\"") == (output.length() - 1)) output = output.substr(0, output.length() - 1);
				if (output.find_last_of("\\") != std::string::npos) output.append("\\");
				output.append(GetFileName(Filename));
			}
			else output = Filename;
			output.erase(Filename.find_last_of(".7cy") - 3, 5);
			int filename = open(output.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0770);
			if (filename)
			{
                unsigned long long int key_size = lseek64(filename_key, 0, SEEK_END);
                unsigned long long int crypt_size = lseek64(filename_crypt, 0, SEEK_END);
                unsigned long long int file_size = lseek64(filename, 0, SEEK_END);
				if (key_size == crypt_size)
				{
                    ftruncate64(filename, key_size);
                    unsigned char * file_mmap = static_cast<unsigned char *>(mmap64(nullptr, file_size, PROT_READ, MAP_SHARED/*MAP_PRIVATE*/, filename, 0));
                    unsigned char * key_mmap = static_cast<unsigned char *>(mmap64(nullptr, file_size, PROT_WRITE | PROT_READ, MAP_SHARED/*MAP_PRIVATE*/, filename_key, 0));
                    unsigned char * crypt_mmap = static_cast<unsigned char *>(mmap64(nullptr, file_size, PROT_WRITE | PROT_READ, MAP_SHARED/*MAP_PRIVATE*/, filename_crypt, 0));
                    unsigned long long int readed = 0;
                    std::thread feedback_t(feedback, &file_size, &readed, true, percentage_interval);
                    for (unsigned long long int i = 0; i < file_size; ++i, ++readed) file_mmap[i] = crypt_mmap[i] - key_mmap[i];
                    msync(crypt_mmap, file_size, MS_SYNC);
                    msync(key_mmap, file_size, MS_SYNC);
                    munmap(file_mmap, file_size);
                    munmap(key_mmap, file_size);
                    munmap(crypt_mmap, file_size);
                    feedback_t.join();
                    std::cout << "[INFO] Successfuly decrypted!" << std::endl;
				}
				else
				{
					std::cerr << "[ERROR] Crypt-Key pair don't match!" << std::endl;
					error = -6;
				}
				close(filename);
			}
			else
			{
				perror("[ERROR] Error opening the output file");
				error = -3;
			}
			close(filename_key);
		}
		else
		{
			perror("[ERROR] Error opening the key file");
			error = -2;
		}
		close(filename_crypt);
	}
	else
	{
		perror("[ERROR] Error opening the crypt file");
		error = -1;
	}
	return error;
}
