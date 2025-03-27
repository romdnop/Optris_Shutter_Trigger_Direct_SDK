#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "direct_binding.h"
#include "snapshot_export.h"
#include "windows.h"

bool _run = true;

int _w;
int _h;

std::string destinationFolder;

const int MAX_SNAPHOT_DELAY = 5000;

std::string timestampCmdReceived;
std::string timestampFileSaved;

bool add_filename_postfix = false;


int saveTemperatureCSV(const unsigned short* thermal, unsigned int w, unsigned int h, std::string filename)
{
	std::ofstream file(filename);

	if (!file.is_open())
	{
		std::cout << "Failed to save CSV file!" << std::endl;
		return -1;
	}

	for (unsigned int y = 0; y < h; y++)
	{
		for (unsigned int x = 0; x < w; x++)
		{
			float t = (float)thermal[y * w + x] / 10.0f - 100.0f;
			file << t;
			if (x < w - 1) file << ",";  // Add comma between columns
		}
		file << "\n";  // New row
	}

	file.close();
	std::cout << "Saved thermal data to " << filename << std::endl;
	return 0;
}


std::string AddPostfix(std::string &fileName, std::string &ts_cmd_received, std::string &ts_frame_received)
{
	size_t dotPos = fileName.find_last_of('.'); // Find the last dot (for file extension)

	if (dotPos == std::string::npos) {
		// No extension found, just append timestamps
		return fileName + "_" + ts_cmd_received + "_" + ts_frame_received;
	}

	// Construct new filename: filename_<ts1>_<ts2>.csv
	return fileName.substr(0, dotPos) + "_" + ts_cmd_received + "_" + ts_frame_received + fileName.substr(dotPos);
}


// Function to display help message
void DisplayHelp() {
	std::cout << "Usage: OptrisShutterTrigger.exe [options]\n\n"
		<< "Options:\n"
		<< "  -ip <TCP daemon IP> Address of the TCP daemon. Default is: \"localhost\" or \"127.0.0.1\"\n"
		<< "  -e <path>      Specify the destination folder for the file copy.\n"
		<< "  -t <milliseconds> Add a delay (in milliseconds) before copying the file.\n"
		<< "  -n <file name> Set output file name\n"
		<< "  -p add postfix with <Timestamp CMD RECEIVED> and <Timestamp FRAME RECEIVD> to the filename\n"
		<< "  --help         Display this help message.\n\n"
		<< "Example:\n"
		<< "  OptrisShutterTrigger.exe -e \"C:\\Path With Spaces\\DestinationFolder\" -t 5000\n"
		<< "    Copies the latest modified file to the specified destination with a 5-second delay.\n";
}


DWORD WINAPI camWorker(LPVOID lpParam)
{
	std::vector<unsigned short> thermal(_w * _h);
	while (_run)
	{
		if (::evo_irimager_get_thermal_image(&_w, &_h, &thermal[0]) == 0)
		{	
			timestampFileSaved = GetUnixTimestamp();

			if (add_filename_postfix)
			{
				outputFileName = AddPostfix(outputFileName, timestampCmdReceived, timestampFileSaved);
			}

			std::string outputFilePath = destinationFolder + "\\" + outputFileName;

			if (saveTemperatureCSV(&thermal[0], _w, _h, outputFilePath) == 0)
			{
				std::cout << "Written file: " << outputFilePath << std::endl;
			}
			else
			{
				std::cout << "Failed to write file: " << outputFilePath << std::endl;
			}
			
			_run = false;
		}
	}

	::evo_irimager_terminate();
	return 0;
}

int main(int argc, char *argv[])
{
	char default_ip[] = "localhost";
	char* ip_appr = default_ip;
	DWORD snapshotDelay = 0;



	for (int i = 1; i < argc; ++i) {
		if (std::string(argv[i]) == "-ip" && i + 1 < argc)
		{
			ip_appr = argv[i + 1];
			++i;
		}
		else if (std::string(argv[i]) == "-e" && i + 1 < argc) {
			destinationFolder = argv[i + 1];
			++i; // Skip the next argument as it's already processed
		}
		else if (std::string(argv[i]) == "-t" && i + 1 < argc) {
			snapshotDelay = std::strtoul(argv[i + 1], nullptr, 10); // Convert argument to DWORD
			++i; // Skip the next argument as it's already processed
		}
		else if (std::string(argv[i]) == "-n" && i + 1 < argc) {
			outputFileName = argv[i + 1];
			++i; // Skip the next argument as it's already processed
		}
		else if (std::string(argv[i]) == "-p" && i < argc) {
			add_filename_postfix = true;
			//++i; // Skip the next argument as it's already processed
		}
		else if (std::string(argv[i]) == "--help") {
			DisplayHelp();
			return 0; // Exit after displaying help
		}
		else {
			std::cerr << "Unknown argument: " << argv[i] << "\nUse --help to display usage information." << std::endl;
			return 1;
		}
	}


	std::cout << "try to connect to " << ip_appr << ":1337" << std::endl;
	int ret = ::evo_irimager_tcp_init(ip_appr, 1337);
	if (ret != 0)
	{
		std::cout << "error at init" << std::endl;
		std::cin.ignore();
		return -1;
	}

	std::cout << "client connected" << std::endl;


	timestampCmdReceived = GetUnixTimestamp();


	if (snapshotDelay > 0)
	{
		Sleep(snapshotDelay);
	}

	if (evo_irimager_get_palette_image_size(&_w, &_h) == 0)
	{
		HANDLE th = CreateThread(NULL, 0, camWorker, NULL, 0, NULL);
		int w = _w;
		int h = _h;
		if (w < 120) w *= 2;
		if (h < 120) h *= 2;

		WaitForSingleObject(th, INFINITE);
		CloseHandle(th);
	}

  evo_irimager_terminate();

  return 0;
}
