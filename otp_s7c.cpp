/**
 * @file otp_s7c.cpp
 * Purpose: File with the functions for crypt and decrypt, also some aux functions
 *
 * @author Samega 7Cattac
 *
 * @version 1.1b
 *
 * 7Guard Copyright (C) 2018 Samega 7Cattac // see more: LICENSE
 */
#include "otp_s7c.h"

int otp_s7c::alloc(unsigned char ** buf, unsigned char ** num, unsigned long long size, bool max)
{
	if (size < 1) return 0;
	if (max)
	{
		bool done = false;
		int i = 0;
		unsigned long long last_try = 0;
		for (i = 1; !done && (size / i) > 0; ++i)
		{
			try
			{
				last_try = (size / i);
				*buf = (unsigned char *)malloc(sizeof(char) * last_try);
				*num = (unsigned char *)malloc(sizeof(char) * last_try);
			}
			catch (...)
			{
				done = false;
			}
			if (*buf != NULL && *num != NULL) done = true;
			else
			{
				if (*buf != NULL) free(*buf);
				if (*num != NULL) free(*num);
			}
		}
		if (!done) return 0;
		cout << "[INFO] Buffers size = " << last_try << endl;
		return last_try;
	}
	else
	{
		try
		{
			*num = (unsigned char *)malloc(sizeof(char) * (size));
			*buf = (unsigned char *)malloc(sizeof(char) * (size));
		}
		catch (...)
		{
			return 0;
		}
		cout << "[INFO] Buffers size = " << size << endl;
		return size;
	}
}

void otp_s7c::feedback(unsigned long long * size, unsigned long long * readed, bool decrypt)
{
	float p = 0;
	while (*readed < *size || p != 100)
	{
		p = *readed * 100.00 / *size;
		cout << "\r" << "[INFO] Start" << (decrypt ? " de" : " ") << "crypting... (" << fixed << setprecision(2) << p << "%)";
	}
	return;
}

string otp_s7c::GetFileName(string path)
{
	string filename = "";
	for (int i = 0; i < path.length(); ++i)
	{
		if (path[i] == '\\') filename = "";
		else filename += path[i];
	}
	return filename;
}

int otp_s7c::crypt(string Filename, string output, unsigned long long buf_size)
{
	int error = 0;
	FILE * filename = fopen(Filename.c_str(), "rb");
	if (filename)
	{
		string Filename_key = "\0";
		if (output != "")
		{
			if (output.find_last_of("\"") == output.length() - 1) output = output.substr(0, output.length() - 1);
			if (output.find_last_of("\\") != string::npos) output.append("\\");
			Filename_key = output;
			Filename_key.append(otp_s7c::GetFileName(Filename) + ".7ky");
		}
		else Filename_key = Filename + string(".7ky");
		FILE * filename_key = fopen(Filename_key.c_str(), "wb");
		if (filename_key)
		{
			string Filename_crypt = "\0";
			if (output != "")
			{
				Filename_crypt = output;
				Filename_crypt.append(otp_s7c::GetFileName(Filename) + ".7cy");
			}
			else Filename_crypt = Filename + string(".7cy");
			FILE * filename_crypt = fopen(Filename_crypt.c_str(), "wb");
			if (filename_crypt)
			{
				fseek(filename, 0L, SEEK_END);
				unsigned long long size = ftell(filename);
				rewind(filename);
				unsigned char * buf = nullptr;
				unsigned char * num = nullptr;
				if (!buf_size)
				{
					cout << "[INFO] Buffers size set to \"auto\"\n";
					buf_size = otp_s7c::alloc(&buf, &num, size, true);
				}
				else buf_size = otp_s7c::alloc(&buf, &num, (buf_size > size ? size : buf_size), false);
				if (buf_size)
				{
					unsigned short val = 0;
					if (_rdrand16_step(&val))
					{
						unsigned long long readed = 0;
						thread feedback_t(feedback, &size, &readed, false);
						while (!feof(filename))
						{
							size_t read = fread(buf, sizeof(char), buf_size, filename);
							for (unsigned long long i = 0; i < read; ++i)
							{
								_rdrand16_step(&val);
								char rnd = val % 255 + 1;
								buf[i] += rnd;
								num[i] = rnd;
							}
							fwrite(num, sizeof(char), read, filename_crypt);
							fwrite(buf, sizeof(char), read, filename_key);
							readed += read;
						}
						memset(num, NULL, buf_size);
						memset(buf, NULL, buf_size);
						free(buf);
						free(num);
						feedback_t.join();
						cout << endl << "[INFO] Successfuly Crypted!" << endl;
					}
					else
					{
						cout << "[ERROR] Error generating hardware random value" << endl;
						error = -4;
					}
				}
				else
				{
					cout << "[ERROR] Cannot allocate buffers" << endl;
					error = -5;
				}
				fclose(filename_key);
			}
			else
			{
				perror("[ERROR] Error opening the output");
				error = -3;
			}
			fclose(filename_crypt);
		}
		else
		{
			perror("[ERROR] Error opening the key file");
			error = -2;
		}
		fclose(filename);
	}
	else
	{
		perror("[ERROR] Error opening the file to crypt");
		error = -1;
	}
	return error;
}

int otp_s7c::decrypt(string crypt, string key, string output, unsigned long long buf_size)
{
	cout << "[INFO] Initializing..." << endl;
	int error = 0;
	FILE * filename_crypt = fopen(crypt.c_str(), "rb");
	if (filename_crypt)
	{
		FILE * filename_key = fopen(key.c_str(), "rb");
		if (filename_key)
		{
			if (output != "")
			{
				if (output.find_last_of("\"") == (output.length() - 1)) output = output.substr(0, output.length() - 1);
				if (output.find_last_of("\\") != string::npos) output.append("\\");
				output.append(otp_s7c::GetFileName(crypt));
			}
			else output = crypt;
			output.erase(crypt.find_last_of(".7cy") - 3, 5);
			FILE * filename = fopen(output.c_str(), "wb");
			if (filename)
			{
				fseek(filename_key, 0L, SEEK_END);
				unsigned long long size = ftell(filename_key);
				fseek(filename_crypt, 0L, SEEK_END);
				if (size == ftell(filename_crypt))
				{
					rewind(filename_key);
					rewind(filename_crypt);
					unsigned char * buf = nullptr;
					unsigned char * num = nullptr;
					if (!buf_size)
					{
						cout << "[INFO] Buffers size set to \"max\"" << endl;
						buf_size = alloc(&buf, &num, size, true);
					}
					else buf_size = alloc(&buf, &num, buf_size, false);
					if (buf_size)
					{
						unsigned long long readed = 0;
						thread feedback_t(feedback, &size, &readed, true);
						while (!feof(filename_crypt) || !feof(filename_key))
						{
							size_t read_k = fread(num, sizeof(char), buf_size, filename_crypt);
							size_t read_c = fread(buf, sizeof(char), buf_size, filename_key);
							readed += read_c;
							if (read_c != read_k)
							{
								cout << "[WARNING] Crypt file or key file seems corrupt!" << endl;
								continue;
							}
							for (unsigned long long i = 0; i < read_c; i++) buf[i] -= num[i];
							fwrite(buf, sizeof(char), read_c, filename);
						}
						memset(num, NULL, buf_size);
						memset(buf, NULL, buf_size);
						free(buf);
						free(num);
						feedback_t.join();
						cout << endl << "[INFO] Successfuly decrypted!" << endl;
					}
					else
					{
						cout << "[ERROR] Cannot allocate buffers" << endl;
						error = -5;
					}
				}
				else
				{
					cout << "[ERROR] Crypt-Key pair don't match!" << endl;
					error = -6;
				}
				fclose(filename);
			}
			else
			{
				perror("[ERROR] Error opening the output file");
				error = -3;
			}
			fclose(filename_key);
		}
		else
		{
			perror("[ERROR] Error opening the key file");
			error = -2;
		}
		fclose(filename_crypt);
	}
	else
	{
		perror("[ERROR] Error opening the crypt file");
		error = -1;
	}
	return error;
}
