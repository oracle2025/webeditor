#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "gtdwindow.h"

#ifdef WIN32
	#define PATH_SEP "\\"
	#include "dirent.h"
	#include <windows.h>
	#include <FL/x.H>
#else
 	 #include <sys/types.h>
     #include <sys/dir.h>
	#define PATH_SEP "/"
#endif

/*
DIR *dir;
struct dirent *ent;
if ((dir = opendir ("c:\\src\\")) != NULL) {
  // print all the files and directories within directory
  while ((ent = readdir (dir)) != NULL) {
    printf ("%s\n", ent->d_name);
  }
  closedir (dir);
} else {
  // could not open directory
  perror ("");
  return EXIT_FAILURE;
}

https://stackoverflow.com/questions/9542611/how-to-get-the-current-users-home-directory-in-windows#9543049
#include <Shlobj.h>  // need to include definitions of constants

// .....

WCHAR path[MAX_PATH];
if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, path))) {
  ...
}

SHGetFolderPath meanwhile is deprecated, and the successor SHGetKnownFolderPath is not available on Windows XP.

Just use the environment variables, in this particular case you want %HOMEPATH% and combine that with %HOMEDRIVE%

oder: %USERPROFILE%


*/

std::string user_home_dir()
{
	/*std::cout << "HOMEPATH: " << std::getenv("HOMEPATH") << std::endl;
	std::cout << "HOMEDRIVE: " << std::getenv("HOMEDRIVE") << std::endl;
	std::cout << "USERPROFILE: " << std::getenv("USERPROFILE") << std::endl;*/
	return std::getenv("USERPROFILE");
}

std::vector<std::string> get_folder_by_type(const std::string& path, int type)
{
	std::vector<std::string> result;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (path.c_str())) != NULL) {
  		// print all the files and directories within directory
  		while ((ent = readdir (dir)) != NULL) {
  			if (ent->d_type == type) {
    			result.push_back(ent->d_name);
    		}
  		}
  		closedir (dir);
	} else {
  		// could not open directory
  		perror ("");
	}
	return result;
}
std::vector<std::string> get_files(const std::string& path)
{
	return get_folder_by_type(path, DT_REG);
}
std::vector<std::string> get_folders(const std::string& path)
{
	return get_folder_by_type(path, DT_DIR);
}

void refresh_path_list()
{
	const char* path_gtd = "\\Dropbox\\Apps\\Plain.txt";
	auto result = get_folders(user_home_dir()+path_gtd);
	sections->clear();
	for (auto i: result) {
		sections->add(i.c_str());
	}
}
void refresh_file_list()
{
	const char* path_gtd = "\\Dropbox\\Apps\\Plain.txt\\GTD_01_INBOX";
	auto result = get_files(user_home_dir()+path_gtd);
	items->clear();
	for (auto i: result) {
		items->add(i.c_str());
	}
}

int main(int argc, const char *argv[])
{
	const char* path = "c:\\";
	const char* path_gtd = "\\Dropbox\\Apps\\Plain.txt\\GTD_01_INBOX";
	auto result = get_files(user_home_dir()+path_gtd);
	for (auto i: result) {
		std::cout << i << std::endl;
	}
	
	auto window = make_window();
#ifdef WIN32
	window->icon((char*)LoadIcon(fl_display, MAKEINTRESOURCE(101)));
#endif
	refresh_path_list();
	refresh_file_list();
	window->show();
	return Fl::run();
}
