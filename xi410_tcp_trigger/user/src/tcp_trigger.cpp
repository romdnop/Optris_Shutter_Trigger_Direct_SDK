#include <iostream>
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

evo::IRTiffWriter _irTiffWriter;

DWORD WINAPI camWorker(LPVOID lpParam)
{
	std::vector<unsigned char> img(_w * _h * 3);
	while (_run)
	{
		if (::evo_irimager_get_palette_image(&_w, &_h, &img[0]) == 0)
		{
			std::string fileName = "test.png";


			std::cout << "Written tiff file: " << fileName << std::endl;
			
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
