#include <iostream>
#include "FTPClient.h"
#include "FTPFileStatus.h"
#include "FTPDataTypes.h"

int main(int argc, const char *argv[])
{
	nsFTP::CFTPClient ftpClient;
	nsFTP::CLogonInfo logonInfo("localhost", 21, "anonymous", 
            "anonymous@user.com");

	// connect to server

	ftpClient.Login(logonInfo);

	// get directory listing

	nsFTP::TStringVector list;
	ftpClient.List("/", list);

	// iterate listing

	for( nsFTP::TStringVector::iterator it=list.begin(); 
            it!=list.end(); ++it ) {
    	std::cout << (*it) << std::endl;
    }

	// do file operations

	ftpClient.DownloadFile("/pub/test.txt", "c:\\temp\\test.txt");

	ftpClient.UploadFile("c:\\temp\\test.txt", "/upload/test.txt");

	ftpClient.Rename("/upload/test.txt", "/upload/NewName.txt");

	ftpClient.Delete("/upload/NewName.txt");

	// disconnect

	ftpClient.Logout(); 
	return 0;
}
