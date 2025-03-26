#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

#include "direct_binding.h"

#include "windows.h"
/**
* Visualization
*/
//#include "VideoDisplay.h"

//VideoDisplay* _display = NULL;

// Optris tiff image file writer
#include "extras/IRTiffWriter.h"


bool _run = true;

int _w;
int _h;


void saveTemperatureCSV(const unsigned short* thermal, unsigned int w, unsigned int h, int frameID)
{
	std::string filename = "thermal.csv";
	std::ofstream file(filename);

	if (!file.is_open())
	{
		std::cout << "Failed to save CSV file!" << std::endl;
		return;
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
}

evo::IRImager* _imager = nullptr; // Global pointer to IRImager

void onThermalFrame(unsigned short* thermal, unsigned int w, unsigned int h, void* arg)
{
	static evo::IRTiffWriter tiffWriter;  // Create a static instance of the TIFF writer


	// Create a unique filename
	std::string fileName = "test.tiff";

	// Write the TIFF file without an IRImager instance
	if (tiffWriter.writeTiff(fileName.c_str(), nullptr, thermal, w, h) == 0)
	{
		std::cout << "Written TIFF file: " << fileName << std::endl;
	}
	else
	{
		std::cout << "Failed to write TIFF file: " << fileName << std::endl;
	}
}

//static evo::IRTiffWriter _irTiffWriter;

DWORD WINAPI camWorker(LPVOID lpParam)
{
	//std::vector<unsigned char> img(_w * _h * 3);
	//static evo::IRTiffWriter tiffWriter;  // Create a static instance of the TIFF writer
	std::vector<unsigned short> thermal(_w * _h);
	while (_run)
	{
		if (::evo_irimager_get_thermal_image(&_w, &_h, &thermal[0]) == 0)
		{
			std::string fileName = "test.tiff";

			std::cout << "Saving file " << fileName << "..." << std::endl;
			
			//if(tiffWriter.writeTiff(fileName.c_str(), _imager,&thermal[0], _w, _h) == 0)
			if (evo_irimager_to_palette_save_png(&thermal[0], _w, _h, fileName.c_str(), 1, 1) == 0)
			{
				saveTemperatureCSV(&thermal[0], _w, _h, 0);
				std::cout << "Written tiff file: " << fileName << std::endl;
			}
			else
			{
				std::cout << "Failed to write tiff file: " << fileName << std::endl;
			}
			
			_run = false;
			//if (_display) _display->drawCapture(0, 0, _w, _h, 24, &img[0]);
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

	/*
	if (evo_irimager_get_palette_image(&_w, &_h, &img[0]) == 0)
	{
		std::string fileName = "test.png";
		//if(_display) _display->drawCapture(0, 0, _w, _h, 24, &img[0]);
		//_irTiffWriter.
		std::vector<unsigned short> thermal(_w * _h);
		evo_irimager_get_thermal_image(&_w, &_h, &thermal[0]);
		
		if (evo_irimager_to_palette_save_png(&thermal[0], _w, _h, fileName.c_str(), 1, 1) == 0)
		{
			std::cout << "Written tiff file: " << fileName << std::endl;
		}
		else
		{
			std::cout << "Failed to write tiff file: " << fileName << std::endl;
		}
	}
	*/
	  //WaitForSingleObject(th, INFINITE);
	  //CloseHandle(th);
  

  evo_irimager_terminate();

  return 0;
}
