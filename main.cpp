/**
 * @file main.cpp
 * Purpose: File with the main functions
 *
 * @author Samega 7Cattac
 *
 * @version v2.0
 *
 * 7Guard Copyright (C) 2018 Samega 7Cattac
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
#include <charconv>

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
	if (argc > 1)
	{
		unsigned int crypt = 0;
		unsigned int decrypt = 0;
		unsigned int key = 0;
		unsigned int o = 0;
		std::from_chars_result t;
		bool h = false;
		otp_s7c s7c;
		for (int i = 1; i < argc; ++i)
		{
			if ((std::string)argv[i] == "-crypt")
			{
				if (crypt)
				{
					std::cerr << "[ERROR] Multiple \"-crypt\" args!" << std::endl;
					return -9;
				}
				else
				{
					if (argc > i + 1) crypt = ++i;
					else
					{
						std::cerr << "[ERROR] Missing \"-crypt\" value!" << std::endl;
						return -9;
					}
				}
			}
			else if ((std::string)argv[i] == "-decrypt")
			{
				if (decrypt)
				{
					std::cerr << "[ERROR] Multiple \"-decrypt\" args!" << std::endl;
					return -9;
				}
				else
				{
					if (argc > i + 1) decrypt = ++i;
					else
					{
						std::cerr << "[ERROR] Missing \"-decrypt\" value!" << std::endl;
						return -9;
					}
				}
			}
			else if ((std::string)argv[i] == "-key")
			{
				if (key)
				{
					std::cerr << "[ERROR] Multiple \"-key\" args!" << std::endl;
					return -9;
				}
				else
				{
					if (argc > i + 1) key = ++i;
					else
					{
						std::cerr << "[ERROR] Missing \"-key\" value!" << std::endl;
						return -9;
					}
				}
			}
			else if ((std::string)argv[i] == "-o")
			{
				if (o)
				{
					std::cerr << "[ERROR] Multiple \"-o\" args!" << std::endl;
					return -9;
				}
				else
				{
					if (argc > i + 1) o = ++i;
					else
					{
						std::cerr << "[ERROR] Missing \"-o\" value!" << std::endl;
						return -9;
					}
				}
			}
			else if ((std::string)argv[i] == "-i")
			{
				if (!(argc < i + 1))
				{
					t = std::from_chars(argv[++i], argv[i] + strlen(argv[i]), s7c.percentage_interval, 10);
					if(t.ec == std::errc::invalid_argument || t.ec == std::errc::result_out_of_range)
					{
						std::cerr << "[ERROR] Invalid \"-i\" value!" << std::endl;
						return -9;
					}
				}
				else
				{
					std::cerr << "[ERROR] Missing \"-i\" value!" << std::endl;
					return -9;
				}
			}
			else if ((std::string)argv[i] == "-h") h = true;
			else
			{
				std::cerr << "[ERROR] Invalid option \"" << argv[i] << "\"!" << std::endl;
				std::cout << "use option \"-h\" for help" << std::endl;
				return -9;
			}
		}
		if (((crypt && decrypt) || (crypt || decrypt)) && h)
		{
			std::cerr << "[ERROR] Select only 1 mode (crypt | decrypt | help)!" << std::endl;
			return -9;
		}
		else if (h) help(argv[0]);
		else if (crypt)
		{
			if (key) std::cerr << "[WARNING] \"-key\" option will be ignored" << std::endl;
			std::cout << "[INFO] Initializing..." << std::endl;
			return s7c.crypt(argv[crypt], (o ? argv[o] : ""));
		}
		else if (decrypt)
		{
			if (!key)
			{
				std::cerr << "[ERROR] Missing \"-key\" option!" << std::endl;
				return -9;
			}
			std::cout << "[INFO] Initializing..." << std::endl;
			return s7c.decrypt(argv[decrypt], argv[key], (o ? argv[o] : ""));
		}
	}
	else cred();
	return 0;
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
	std::cout << "\n\t" << "7Guard Copyright (C) 2019 Samega 7Cattac" << std::endl;
    std::cout << "This program comes with ABSOLUTELY NO WARRANTY" << std::endl;
    std::cout << "This is free software, and you are welcome to redistribute it" << std::endl;
    std::cout << "under certain conditions" << std::endl;
}
