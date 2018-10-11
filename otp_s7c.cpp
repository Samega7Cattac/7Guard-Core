/**
 * @file otp_s7c.cpp
 * Purpose: File with the functions for crypt and decrypt, also some aux functions
 *
 * @author Samega 7Cattac
 *
 * @version 1.2
 *
 * 7Guard Copyright (C) 2018 Samega 7Cattac // see more: LICENSE
 */
#include "otp_s7c.h"

int otp_s7c::alloc(unsigned char ** buf, unsigned char ** num, size_t size, bool max)
{
	if (size < 1) return 0;
	if (max)
	{
		bool done = false;
		int i = 0;
		int last_try = 0;
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

void otp_s7c::feedback(size_t * size, size_t * readed, bool decrypt)
{
	float p = 0;
	while (*readed < *size || p != 100)
	{
		sleep_for(seconds(1));
		p = *readed * (float)100.00 / *size;
		cout << "\r" << "[INFO] Start" << (decrypt ? " de" : " ") << "crypting... (" << fixed << setprecision(2) << p << "%)";
	}
	return;
}

string otp_s7c::GetFileName(string path)
{
	string filename = "";
	for (unsigned int i = 0; i < path.length(); ++i)
	{
		if (path[i] == '\\') filename = "";
		else filename += path[i];
	}
	return filename;
}

int otp_s7c::crypt(string Filename, string output, size_t buf_size, unsigned int threads)
{
	int error = 0;
	FILE * file = fopen(Filename.c_str(), "rb");
	if (file)
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
		FILE * key_file = fopen(Filename_key.c_str(), "wb");
		if (key_file)
		{
			string Filename_crypt = "\0";
			if (output != "")
			{
				Filename_crypt = output;
				Filename_crypt.append(otp_s7c::GetFileName(Filename) + ".7cy");
			}
			else Filename_crypt = Filename + string(".7cy");
			FILE * crypt_file = fopen(Filename_crypt.c_str(), "wb");
			if (crypt_file)
			{
				unsigned short val = 0;
				if (_rdrand16_step(&val))
				{
					fseek(file, 0L, SEEK_END);
					size_t size = ftell(file);
					rewind(file);
					if (threads)
					{
						cout << "[WARNING] This is a experimental option (may cause loss of data)!" << endl;
						block * rc = (block *)malloc(sizeof(block));
						block * cw = (block *)malloc(sizeof(block));
						block * ck = (block *)malloc(sizeof(block));
						unsigned int rc_n = 0;
						unsigned int cw_n = 0;
						unsigned int ck_n = 0;
						bool read_done = false;
						bool calc_done = false;
						size_t writed = 0;
						size_t writedk = 0;
						thread calc_t(otp_s7c::calc, rc, cw, ck, &rc_n, &cw_n, &ck_n, &read_done, &calc_done);
						thread write_t(otp_s7c::write, cw, &cw_n, &calc_done, crypt_file, &writed);
						thread writeK_t(otp_s7c::write, ck, &ck_n, &calc_done, key_file, &writedk);
						char * line = (char *)malloc(sizeof(char) * buf_size);
						thread feedback_t(feedback, &size, &writed, false);
						while (!feof(file))
						{
							if (rc_n < threads)
							{
								rc->size = fread(line, sizeof(char), buf_size, file);
								rc->c = (unsigned char *)malloc(sizeof(char) * rc->size);
								memcpy_s(rc->c, rc->size, line, rc->size);
								rc->next = (block *)malloc(sizeof(block));
								rc = rc->next;
								++rc_n;
							}
						}
						read_done = true;
						calc_t.join();
						free(rc);
						write_t.join();
						writeK_t.join();
						feedback_t.join();
						cout << endl << "[INFO] Successfuly Crypted!" << endl;
					}
					else
					{
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
							size_t readed = 0;
							thread feedback_t(feedback, &size, &readed, false);
							while (!feof(file))
							{
								size_t read = fread(buf, sizeof(char), buf_size, file);
								for (unsigned long long i = 0; i < read; ++i)
								{
									_rdrand16_step(&val);
									char rnd = val % 255 + 1;
									buf[i] += rnd;
									num[i] = rnd;
								}
								fwrite(num, sizeof(char), read, crypt_file);
								fwrite(buf, sizeof(char), read, key_file);
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
							cout << "[ERROR] Cannot allocate buffers" << endl;
							error = -5;
						}
					}
				}
				else
				{
					cout << "[ERROR] Error generating hardware random value" << endl;
					error = -4;
				}
				fclose(key_file);
			}
			else
			{
				perror("[ERROR] Error opening the output");
				error = -3;
			}
			fclose(crypt_file);
		}
		else
		{
			perror("[ERROR] Error opening the key file");
			error = -2;
		}
		fclose(file);
	}
	else
	{
		perror("[ERROR] Error opening the file to crypt");
		error = -1;
	}
	return error;
}

int otp_s7c::decrypt(string crypt, string key, string output, size_t buf_size, int threads)
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
				size_t size = ftell(filename_key);
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
						size_t readed = 0;
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
							for (size_t i = 0; i < read_c; i++) buf[i] -= num[i];
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

void otp_s7c::calc(block * rc, block * cw, block * ck, unsigned int * rc_n, unsigned int * cw_n, unsigned int * ck_n, bool * read_done, bool * calc_done)
{
	while (!*read_done)
	{
		while (*rc_n > 0)
		{
			cw->c = (unsigned char *)malloc(sizeof(char) * rc->size);
			ck->c = (unsigned char *)malloc(sizeof(char) * rc->size);
			for (unsigned int i = 0; i < rc->size; ++i)
			{
				unsigned short rnd = 0;
				_rdrand16_step(&rnd);
				cw->c[i] = rc->c[i] + (char)rnd;
				ck->c[i] = (char)rnd;
			}
			cw->size = rc->size;
			ck->size = rc->size;
			block * rc_tmp = rc;
			rc = rc->next;
			free(rc_tmp->c);
			free(rc_tmp);
			cw->next = (block *)malloc(sizeof(block));
			cw = cw->next;
			ck->next = (block *)malloc(sizeof(block));
			ck = ck->next;
			--*rc_n;
			++*cw_n;
			++*ck_n;
		}
	}
	*calc_done = true;
	return;
}

void otp_s7c::write(block * cw, unsigned int * cw_n, bool * calc_done, FILE * d, size_t * writed)
{
	block * cw_w = cw;
	while (!*calc_done)
	{
		while (*cw_n > 0)
		{
			fwrite(cw_w->c, sizeof(char), cw_w->size, d);
			*writed += cw_w->size;
			block * cw_tmp = cw_w;
			cw_w = cw_w->next;
			free(cw_tmp->c);
			free(cw_tmp);
			--*cw_n;
		}
	}
	return;
}
