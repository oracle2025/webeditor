#include "webeditor.h"
#include "markdown.h"
#include "html.h"
#include "buffer.h"
#include "FTPClient.h"
#include "FTPFileStatus.h"
#include "FTPDataTypes.h"
//#include <string>
#include <iostream>
#include <FL/fl_ask.H>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"
//#include <FL/gl.h>

std::string markdown_to_html(const std::string text);

class Logger : public nsFTP::CFTPClient::CNotification
{
	public:
		void OnInternalError(const tstring& strErrorMsg, const tstring& strFileName, DWORD dwLineNr)
		{
			std::cout << "Error: (" << strFileName << ":" << dwLineNr << ")" << strErrorMsg << std::endl;
		}
};

void list()
{
	std::cout << "List" << std::endl;
	const char* server;
	const char* user;
	const char* password;

	server = server_input->value();
	user = user_input->value();
	password = password_input->value();


	Logger logger;
	nsFTP::CFTPClient ftpClient;
	ftpClient.AttachObserver(&logger);
	nsFTP::CLogonInfo logonInfo(server, 21, user, password);

	// connect to server

	if (!ftpClient.Login(logonInfo)) {
		std::cout << "ERROR Login" << std::endl;
	} else {
		std::cout << "LOGIN Success" << std::endl;
	}


	// get directory listing

	nsFTP::TStringVector list;
	if(!ftpClient.NameList("/", list, true)) {
		std::cout << "Error List" << std::endl;
	} else {
		std::cout << "List success" << std::endl;
	}

	// iterate listing
	
	file_browser->clear();

	for( nsFTP::TStringVector::iterator it=list.begin(); 
            it!=list.end(); ++it ) {
    	std::cout << (*it).substr(1) << std::endl;
    	file_browser->add((*it).substr(1).c_str());
    }

	// do file operations

	/*ftpClient.DownloadFile("/pub/test.txt", "c:\\temp\\test.txt");

	ftpClient.UploadFile("c:\\temp\\test.txt", "/upload/test.txt");

	ftpClient.Rename("/upload/test.txt", "/upload/NewName.txt");

	ftpClient.Delete("/upload/NewName.txt");*/

	// disconnect

	ftpClient.Logout(); 
}

void upload_str_to_file(const std::string &content, const std::string &filename)
{
	const char* server;
	const char* user;
	const char* password;

	server = server_input->value();
	user = user_input->value();
	password = password_input->value();


	Logger logger;
	nsFTP::CFTPClient ftpClient;
	ftpClient.AttachObserver(&logger);
	nsFTP::CLogonInfo logonInfo(server, 21, user, password);

	// connect to server

	if (!ftpClient.Login(logonInfo)) {
		std::cout << "ERROR Login" << std::endl;
	} else {
		std::cout << "LOGIN Success" << std::endl;
	}

	//auto temp_file = tmpfile();
	
   /*int posix_handle = ::_fileno(::fopen("test.txt", "r"));

    ifstream ifs(::_fdopen(posix_handle, "r")); // 1

    string line;
    getline(ifs, line);
    ifs.close();*/
	
	
	std::ofstream f("uploadfile.txt");

	f << content;
	f.close();



/*	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
    ofstream f;
    int fd = mkstemp(tmpname);
    f.attach(fd);

FILE * tmpfile ( void );
*/
	if (!ftpClient.UploadFile("uploadfile.txt", filename, false, nsFTP::CRepresentation(nsFTP::CType::Image()), true)) {
		std::cout << "ERROR Upload" << std::endl;
	} else {
		std::cout << "SUCCESS Upload" << std::endl;
	}
	unlink("uploadfile.txt");
	ftpClient.Logout(); 

}

void upload()
{
	std::string filename = filename_output->value();
	if (filename.empty()) {
		return;
	}
	std::cout << "Upload" << std::endl;

	upload_str_to_file(text_editor->buffer()->text(), filename);
	{
    	auto const pos=filename.find_last_of('.');
    	const auto extension=filename.substr(pos+1);
    	if (extension == "md") {
    		auto markdown = markdown_to_html(text_editor->buffer()->text());
			auto filename_html = filename.substr(0, pos) + ".html";
			upload_str_to_file(markdown, filename_html);
    	}
    }

	return;
	const char* server;
	const char* user;
	const char* password;

	server = server_input->value();
	user = user_input->value();
	password = password_input->value();


	Logger logger;
	nsFTP::CFTPClient ftpClient;
	ftpClient.AttachObserver(&logger);
	nsFTP::CLogonInfo logonInfo(server, 21, user, password);

	// connect to server

	if (!ftpClient.Login(logonInfo)) {
		std::cout << "ERROR Login" << std::endl;
	} else {
		std::cout << "LOGIN Success" << std::endl;
	}

	//auto temp_file = tmpfile();
	
   /*int posix_handle = ::_fileno(::fopen("test.txt", "r"));

    ifstream ifs(::_fdopen(posix_handle, "r")); // 1

    string line;
    getline(ifs, line);
    ifs.close();*/
	
	
	std::ofstream f("uploadfile.txt");

	f << text_editor->buffer()->text();
	f.close();



/*	char *tmpname = strdup("/tmp/tmpfileXXXXXX");
    ofstream f;
    int fd = mkstemp(tmpname);
    f.attach(fd);

FILE * tmpfile ( void );
*/
	if (!ftpClient.UploadFile("uploadfile.txt", filename, false, nsFTP::CRepresentation(nsFTP::CType::Image()), true)) {
		std::cout << "ERROR Upload" << std::endl;
	} else {
		std::cout << "SUCCESS Upload" << std::endl;
	}
	unlink("uploadfile.txt");

	//Convert Markdown to HTML and upload
	//const std::string path="/root/config";
    auto const pos=filename.find_last_of('.');
    const auto extension=filename.substr(pos+1);
    std::cout << extension << '\n';
    if (extension == "md") {
    	auto markdown = markdown_to_html(text_editor->buffer()->text());
		std::ofstream f("uploadfile2.txt");

		f << markdown;
		f.close();

		auto filename_html = filename.substr(0, pos) + ".html";
		std::cout << filename_html << std::endl;
		if (!ftpClient.UploadFile("uploadfile2.txt", filename_html, false, nsFTP::CRepresentation(nsFTP::CType::Image()), true)) {
			std::cout << "ERROR Upload" << std::endl;
		} else {
			std::cout << "SUCCESS Upload" << std::endl;
		}
		unlink("uploadfile2.txt");
    }


	// get directory listing

	nsFTP::TStringVector list;
	if(!ftpClient.NameList("/", list, true)) {
		std::cout << "Error List" << std::endl;
	} else {
		std::cout << "List success" << std::endl;
	}

	// iterate listing
	
	file_browser->clear();

	for( nsFTP::TStringVector::iterator it=list.begin(); 
            it!=list.end(); ++it ) {
    	std::cout << (*it) << std::endl;
    	file_browser->add((*it).c_str());
    }

	// do file operations

	/*ftpClient.DownloadFile("/pub/test.txt", "c:\\temp\\test.txt");

	ftpClient.UploadFile("c:\\temp\\test.txt", "/upload/test.txt");

	ftpClient.Rename("/upload/test.txt", "/upload/NewName.txt");

	ftpClient.Delete("/upload/NewName.txt");*/

	// disconnect

	ftpClient.Logout(); 
}

void create()
{
	const char *name = fl_input("Enter Filename", "");
	text_editor->buffer()->text("");
	filename_output->value(name);

}
void delete_()
{
}
std::string
getFileContent(const std::string& path)
{
  std::ifstream file(path);
  std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  return content;
}
void edit()
{
	if (!file_browser->value()) {
		return;
	}
	const char *filename = file_browser->text(file_browser->value());
	std::cout << "Edit" << std::endl;
	const char* server;
	const char* user;
	const char* password;

	server = server_input->value();
	user = user_input->value();
	password = password_input->value();


	Logger logger;
	nsFTP::CFTPClient ftpClient;
	ftpClient.AttachObserver(&logger);
	nsFTP::CLogonInfo logonInfo(server, 21, user, password);

	// connect to server

	if (!ftpClient.Login(logonInfo)) {
		std::cout << "ERROR Login" << std::endl;
	} else {
		std::cout << "LOGIN Success" << std::endl;
	}
	
	/*std::ofstream f("uploadfile.txt");

	f << text_editor->buffer()->text();
	f.close();*/



	std::cout << "Download: " << filename << std::endl;
	unlink("uploadfile.txt");
	if(!ftpClient.DownloadFile(filename, "uploadfile.txt", nsFTP::CRepresentation(nsFTP::CType::Image()), true)) {
		std::cout << "ERROR Edit" << std::endl;
	} else {
		std::cout << "SUCCESS Edit" << std::endl;
		filename_output->value(filename);
	}
	text_editor->buffer()->text(getFileContent("uploadfile.txt").c_str());
	/*if (!ftpClient.UploadFile("uploadfile.txt", filename, false, nsFTP::CRepresentation(nsFTP::CType::Image()), true)) {
		std::cout << "ERROR Upload" << std::endl;
	} else {
		std::cout << "SUCCESS Upload" << std::endl;
	}*/

	// get directory listing

	/*nsFTP::TStringVector list;
	if(!ftpClient.NameList("/", list, true)) {
		std::cout << "Error List" << std::endl;
	} else {
		std::cout << "List success" << std::endl;
	}*/

	// iterate listing
	
	/*file_browser->clear();

	for( nsFTP::TStringVector::iterator it=list.begin(); 
            it!=list.end(); ++it ) {
    	std::cout << (*it) << std::endl;
    	file_browser->add((*it).c_str());
    }*/

	// do file operations

	/*ftpClient.DownloadFile("/pub/test.txt", "c:\\temp\\test.txt");

	ftpClient.UploadFile("c:\\temp\\test.txt", "/upload/test.txt");

	ftpClient.Rename("/upload/test.txt", "/upload/NewName.txt");

	ftpClient.Delete("/upload/NewName.txt");*/

	// disconnect

	ftpClient.Logout(); 
}

#define OUTPUT_UNIT 64

std::string markdown_to_html(const std::string text)
{
	struct buf ib = { (uint8_t *)text.c_str(), strlen(text.c_str()), 0, 0};

	struct sd_callbacks callbacks;
	struct html_renderopt options;
	struct sd_markdown *markdown;

	struct buf *ob = bufnew(OUTPUT_UNIT);

	sdhtml_renderer(&callbacks, &options, 0);
	markdown = sd_markdown_new(0, 16, &callbacks, &options);

	sd_markdown_render(ob, ib.data, ib.size, markdown);
	sd_markdown_free(markdown);

	//std::string output(ob->data, ob->size):

	//html_view->value(text);
	ob->data[ob->size] = '\0';
	std::string result = (char*)ob->data;
	bufrelease(ob);

	return std::string("<!doctype html><html lang=\"de\"><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n") + result + "</html>";
}

void preview()
{
	const char* text = text_editor->buffer()->text();
	auto html = markdown_to_html(text);
	html_view->value(html.c_str());
	source_view->buffer()->text(html.c_str());
}

using json = nlohmann::json;

int main(int argc, const char *argv[])
{
	auto window = make_window();

	std::ifstream i("login.json");
	if (i.is_open()) {
		json j;
		i >> j;
		server_input->value(j["server"].get<std::string>().c_str());
		user_input->value(j["user"].get<std::string>().c_str());
		password_input->value(j["password"].get<std::string>().c_str());
	}
	window->show();
    return Fl::run();
}
