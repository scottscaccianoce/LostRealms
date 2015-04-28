#include "WinDir.h"

void GetFilesInDir(std::vector<std::string> &output) {
	std::string line;
	std::ifstream myfile ("obj_files.txt");
	if (myfile.is_open())
	{
		while ( myfile.good() )
		{
			getline (myfile,line);
			output.push_back(line);
		}
	}
	myfile.close();

}
