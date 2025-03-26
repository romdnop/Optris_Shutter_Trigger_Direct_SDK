#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "direct_binding.h"
#include "windows.h"

bool _run = true;

int _w;
int _h;


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


DWORD WINAPI camWorker(LPVOID lpParam)
{
	std::vector<unsigned short> thermal(_w * _h);
	while (_run)
	{
		if (::evo_irimager_get_thermal_image(&_w, &_h, &thermal[0]) == 0)
		{	
			std::string fileName = "thermal.csv";
			if (saveTemperatureCSV(&thermal[0], _w, _h, fileName) == 0)
			{
				std::cout << "Written tiff file: " << fileName << std::endl;
			}
			else
			{
				std::cout << "Failed to write tiff file: " << fileName << std::endl;
			}
			
			_run = false;
		}
	}

	::evo_irimager_terminate();
	return 0;
}

int main(int argc, char *argv[])
{
	char default_arg[] = "localhost";
	char* arg = default_arg;

	if(argc<2)
	{
		std::cout << "usage: " << argv[0] << " <IP>" << std::endl;
		return -1;
	}

	std::cout << "try to connect to " << argv[1] << ":1337" << std::endl;
	int ret = ::evo_irimager_tcp_init(argv[1], 1337);
	if (ret != 0)
	{
		std::cout << "error at init" << std::endl;
		std::cin.ignore();
		return -1;
	}

	std::cout << "client connected" << std::endl;

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
