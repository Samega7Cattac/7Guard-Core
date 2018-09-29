/**
 * @file Source.cpp
 * Purpose: File with the functions for help, credits and to handle the inputs
 *
 * @author Samega 7Cattac
 *
 * @version 1.2
 *
 * 7Guard Copyright (C) 2018 Samega 7Cattac // see more: LICENSE
 */
#define BUFFER_SIZE 99999 // Default size of the buffers
#include "otp_s7c.h"

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
	cout << endl;
	if (argc > 1)
	{
		int crypt = 0;
		int decrypt = 0;
		int key = 0;
		int o = 0;
		int buf = 0;
		bool h = false;
		for (int i = 1; i < argc; i++)
		{
			if ((string)argv[i] == "-crypt")
				if (!crypt)
					if(argc > i + 1) crypt = ++i;
					else
					{
						cout << "[ERROR] Missing \"-crypt\" value!" << endl;
						return -9;
					}
				else
				{
					cout << "[ERROR] Multiple \"-crypt\" args!" << endl;
					return -9;
				}
			else if ((string)argv[i] == "-decrypt")
				if (!decrypt)
					if (argc > i + 1) decrypt = ++i;
					else
					{
						cout << "[ERROR] Missing \"-decrypt\" value!" << endl;
						return -9;
					}
				else
				{
					cout << "[ERROR] Multiple \"-decrypt\" args!" << endl;
					return -9;
				}
			else if ((string)argv[i] == "-key")
				if (!key)
					if (argc > i + 1) key = ++i;
					else
					{
						cout << "[ERROR] Missing \"-key\" value!" << endl;
						return -9;
					}
				else
				{
					cout << "[ERROR] Multiple \"-key\" args!" << endl;
					return -9;
				}
			else if ((string)argv[i] == "-o")
				if (!o)
					if (argc > i + 1) o = ++i;
					else
					{
						cout << "[ERROR] Missing \"-o\" value!" << endl;
						return -9;
					}
				else
				{
					cout << "[ERROR] Multiple \"-o\" args!" << endl;
					return -9;
				}
			else if ((string)argv[i] == "--buf")
				if (!buf)
					if (!(argc < i + 1)) buf = ++i;
					else
					{
						cout << "[ERROR] Missing \"-buf\" value!" << endl;
						return -9;
					}
				else
				{
					cout << "[ERROR] Multiple \"-buf\" args!" << endl;
					return -9;
				}
			else if ((string)argv[i] == "-h") h = true;
			else
			{
				cout << "[ERROR] Invalid option \"" << argv[i] << "\"!" << endl;
				cout << "use option \"-h\" for help" << endl;
				return -9;
			}
		}
		otp_s7c s7c;
		if ((crypt && decrypt) || (crypt || decrypt) && h)
		{
			cout << "[ERROR] Select only 1 mode (crypt | decrypt | help)!" << endl;
			return -1;
		}
		else if (h) help(argv[0]);
		else if (crypt)
		{
			if (key) cout << "[WARNING] \"-key\" option will be ignored" << endl;
			cout << "[INFO] Initializing..." << endl;
			return s7c.crypt(argv[crypt], (o ? argv[o] : ""), (!buf ? BUFFER_SIZE : strtoull(argv[buf], NULL, 10)));
		}
		else if (decrypt)
		{
			if (!key)
			{
				cout << "[ERROR] Missing \"-key\" option!" << endl;
				return -2;
			}
			cout << "[INFO] Initializing..." << endl;
			return s7c.decrypt(argv[decrypt], argv[key], (o ? argv[o] : ""), (!buf ? BUFFER_SIZE : strtoull(argv[buf], NULL, 10)));
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
	cout << "Options:" << endl;
	cout << '\t' << "-crypt - path to the file to crypt" << endl;
	cout << '\t' << "-decrypt - path to the crypted file" << endl;
	cout << '\t' << "-key - path to the key file (only with \"-decypt\")" << endl;
	cout << '\t' << "-h - display this help message" << endl;
	cout << "Usage:" << endl;
	cout << '\t' << call << " [ -crypt <file_to_crypt.*> | -decrypt <file_to_decrypt.7cy> -key <file_with_key.7ky]" << endl;
	cout << endl << "Optional:" << endl;
	cout << '\t' << "-o - Path to output folder" << endl;
	cout << '\t' << "--buf - specify the size used each of the 2 buffers (O = max)" << endl;
}

/**
	Display the credits message.
*/
void cred()
{
	cout << "_________  ________                       .___" << endl;
	cout << "\\______  \\/  _____/ __ _______ _______  __| _/" << endl;
	cout << "    /    /   \\  ___|  |  \\__  \\\\_  __ \\/ __ |" << endl;
	cout << "   /    /\\    \\_\\  \\  |  // __ \\|  | \\/ /_/ |" << endl;
	cout << "  /____/  \\______  /____/(____  /__|  \\____ |" << endl;
	cout << "                 \\/           \\/           \\/" << endl;
	cout << "7Guard - OTP encryption software" << endl;
	cout << "By: Samega 7Cattac" << endl;
	cout << endl << "GitHub: https://github.com/Samega7Cattac/7Guard-Core/" << endl;
	cout << endl << "use option \"-h\" for help" << endl;
	cout << endl << "7Guard Copyright (C) 2018 Samega 7Cattac" << endl;
}