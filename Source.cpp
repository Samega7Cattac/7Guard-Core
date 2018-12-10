/**
 * @file Source.cpp
 * Purpose: File with the main functions
 *
 * @author Samega 7Cattac
 *
 * @version 1.2
 *
 * 7Guard Copyright (C) 2018 Samega 7Cattac // see more: LICENSE
 */
#define BUFFER_SIZE 99999 // Default size of the buffers
#define N_BUFFER 10240
#define BUF_MAX 4294967295
#define TIME_MAX 59
#include "otp_s7c.h"

bool checks(long buf, long threads, long time);
void help(char * call);
void cred();

/**
	Handle the inputs and pass that to the correct function.

	@param[in] argc Counts the number of inputs.
	@param[in] argv Inputs from the user.
	@return 0 if success or negative if fail.
*/
int main(int argc, char * argv[])
{
	std::cout << std::endl;
	if (argc > 1)
	{
		unsigned int crypt = 0;
		unsigned int decrypt = 0;
		unsigned int key = 0;
		unsigned int o = 0;
		unsigned int t = 0;
		unsigned int buf = 0;
		unsigned int threads = 0;
		bool h = false;
		for (int i = 1; i < argc; i++)
		{
			if ((std::string)argv[i] == "-crypt")
			{
				if (crypt)
				{
					std::cout << "[ERROR] Multiple \"-crypt\" args!" << std::endl;
					return -9;
				}
				else
				{
					if (argc > i + 1) crypt = ++i;
					else
					{
						std::cout << "[ERROR] Missing \"-crypt\" value!" << std::endl;
						return -9;
					}
				}
			}
			else if ((std::string)argv[i] == "-decrypt")
			{
				if (decrypt)
				{
					std::cout << "[ERROR] Multiple \"-decrypt\" args!" << std::endl;
					return -9;
				}
				else
				{
					if (argc > i + 1) decrypt = ++i;
					else
					{
						std::cout << "[ERROR] Missing \"-decrypt\" value!" << std::endl;
						return -9;
					}
				}
			}
			else if ((std::string)argv[i] == "-key")
			{
				if (key)
				{
					std::cout << "[ERROR] Multiple \"-key\" args!" << std::endl;
					return -9;
				}
				else
				{
					if (argc > i + 1) key = ++i;
					else
					{
						std::cout << "[ERROR] Missing \"-key\" value!" << std::endl;
						return -9;
					}
				}
			}
			else if ((std::string)argv[i] == "-o")
			{
				if (o)
				{
					std::cout << "[ERROR] Multiple \"-o\" args!" << std::endl;
					return -9;
				}
				else
				{
					if (argc > i + 1) o = ++i;
					else
					{
						std::cout << "[ERROR] Missing \"-o\" value!" << std::endl;
						return -9;
					}
				}
			}
			else if ((std::string)argv[i] == "-i")
			{
				if (t)
				{
					std::cout << "[ERROR] Multiple \"-i\" args!" << std::endl;
					return -9;
				}
				else
				{
					if (!(argc < i + 1)) t = ++i;
					else
					{
						std::cout << "[ERROR] Missing \"-i\" value!" << std::endl;
						return -9;
					}
				}
			}
			else if ((std::string)argv[i] == "--buf")
			{
				if (buf)
				{
					std::cout << "[ERROR] Multiple \"--buf\" args!" << std::endl;
					return -9;
				}
				else
				{
					if (!(argc < i + 1)) buf = ++i;
					else
					{
						std::cout << "[ERROR] Missing \"--buf\" value!" << std::endl;
						return -9;
					}
				}
			}
			else if ((std::string)argv[i] == "--threads")
			{
				if (threads)
				{
					std::cout << "[ERROR] Multiple \"--threads\" args!" << std::endl;
					return -9;
				}
				else
				{
					if (!(argc < i + 1)) threads = ++i;
					else
					{
						std::cout << "[ERROR] Missing \"--threads\" value!" << std::endl;
						return -9;
					}
				}
			}
			else if ((std::string)argv[i] == "-h") h = true;
			else
			{
				std::cout << "[ERROR] Invalid option \"" << argv[i] << "\"!" << std::endl;
				std::cout << "use option \"-h\" for help" << std::endl;
				return -9;
			}
		}
		otp_s7c s7c;
		if ((crypt && decrypt) || (crypt || decrypt) && h)
		{
			std::cout << "[ERROR] Select only 1 mode (crypt | decrypt | help)!" << std::endl;
			return -9;
		}
		else if (h) help(argv[0]);
		else if (crypt)
		{
			if (key) std::cout << "[WARNING] \"-key\" option will be ignored" << std::endl;
			std::cout << "[INFO] Initializing..." << std::endl;
			if (checks(atoi(argv[buf]), (!threads ? 0 : atoi(argv[threads])), atoi(argv[t]))) return -9;
			return s7c.crypt(argv[crypt], (o ? argv[o] : ""), (!buf ? BUFFER_SIZE : strtoull(argv[buf], NULL, 10)), (!threads ? 0 : ((std::string)argv[threads] == "0" ? N_BUFFER : strtoul(argv[threads], NULL, 10))), (!t ? 1 : atoi(argv[t])));
		}
		else if (decrypt)
		{
			if (!key)
			{
				std::cout << "[ERROR] Missing \"-key\" option!" << std::endl;
				return -9;
			}
			else if (threads) std::cout << "[WARNING] \"--threads\" option will be ignored" << std::endl;
			std::cout << "[INFO] Initializing..." << std::endl;
			if (checks(atoi(argv[buf]), atoi(argv[threads]), atoi(argv[t]))) return -9;
			return s7c.decrypt(argv[decrypt], argv[key], (o ? argv[o] : ""), (!buf ? BUFFER_SIZE : strtoull(argv[buf], NULL, 10)), (!t ? 1 : atoi(argv[t])));
		}
	}
	else cred();
	return 0;
}

/**
	Check the range of the numeric inputs.

	@param[in] buf Counts the number of inputs.
	@param[in] threads Inputs from the user.
	@param[in] time Inputs from the user.
	@return 0 if success or negative if fail.
*/
bool checks(long buf, long threads, long time)
{
	if (buf < 0 || buf > BUF_MAX)
	{
		std::cout << "[ERROR] Invalid buf size! [0 - " << BUF_MAX << "]" << std::endl;
		return true;
	}
	else if (threads < 0 || threads > BUF_MAX)
	{
		std::cout << "[ERROR] Invalid threads number! [0 - " << BUF_MAX << "]" << std::endl;
		return true;
	}
	else if (time < 0 || time > TIME_MAX)
	{
		std::cout << "[ERROR] Invalid refresh time! [0 - " << TIME_MAX << "]" << std::endl;
		return true;
	}
	return false;
}

/**
	Display the help message.

	@param[in] call Name witch this software was called.
*/
void help(char * call)
{
	std::cout << "Options:" << '\n';
	std::cout << '\t' << "-crypt - path to the file to crypt" << '\n';
	std::cout << '\t' << "-decrypt - path to the crypted file" << '\n';
	std::cout << '\t' << "-key - path to the key file (only with \"-decypt\")" << '\n';
	std::cout << '\t' << "-h - display this help message" << '\n';
	std::cout << "Usage:" << '\n';
	std::cout << '\t' << call << " [ -crypt <file_to_crypt.*> | -decrypt <file_to_decrypt.7cy> -key <file_with_key.7ky>]" << '\n';
	std::cout << '\n' << "Optional:" << '\n';
	std::cout << '\t' << "-o - Path to output folder" << '\n';
	std::cout << '\t' << "-t - refresh time in seconds" << '\n';
	std::cout << '\t' << "-i - percentage refresh time in sec" << '\n';
	std::cout << '\t' << "--buf - specify the size used for each buffer (O = max)" << '\n';
	std::cout << '\t' << "--threads - EXPERIMENTAL, uses threads with a limited number of blocks in queue (O = default)" << '\n';
	std::cout << '\t' << '\t' << "(Only high-end CPUs recomendaded)" << std::endl;
}

/**
	Display the credits message.
*/
void cred()
{
	std::cout << "_________  ________                       .___" << '\n';
	std::cout << "\\______  \\/  _____/ __ _______ _______  __| _/" << '\n';
	std::cout << "    /    /   \\  ___|  |  \\__  \\\\_  __ \\/ __ |" << '\n';
	std::cout << "   /    /\\    \\_\\  \\  |  // __ \\|  | \\/ /_/ |" << '\n';
	std::cout << "  /____/  \\______  /____/(____  /__|  \\____ |" << '\n';
	std::cout << "                 \\/           \\/           \\/" << '\n';
	std::cout << "7Guard - OTP encryption software" << '\n';
	std::cout << "By: Samega 7Cattac" << '\n';
	std::cout << '\n' << "GitHub: https://github.com/Samega7Cattac/7Guard-Core/" << '\n';
	std::cout << "Discord: Samega 7Cattac#5966" << '\n';
	std::cout << '\n' << "use option \"-h\" for help" << '\n';
	std::cout << '\n' << "7Guard Copyright (C) 2018 Samega 7Cattac" << std::endl;
}