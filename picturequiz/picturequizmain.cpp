#include "picturequiz.h"
#include <iostream>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include "../sqlite3.h"
#include <fstream>
#include <sstream>
#include <string>
#include <FL/fl_ask.H>

std::string word;
auto dbfile = "../picturequiz/data/picturequiz.sqlite3";

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
      	printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
      	word = argv[i];
    }
    printf("\n");
    return 0;
}

void next_word()
{
   	sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open(dbfile, &db);
    if( rc ){
      	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      	sqlite3_close(db);
      	return;
    }
	auto sql = "CREATE TABLE IF NOT EXISTS words (id INTEGER PRIMARY KEY, word VARCHAR);";
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		  fprintf(stderr, "SQL error: %s\n", zErrMsg);
		  sqlite3_free(zErrMsg);
	}
	auto sql_select = std::string("SELECT word FROM words ORDER BY RANDOM() LIMIT 1;");
	rc = sqlite3_exec(db, sql_select.c_str(), callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	question_box->label(word.c_str());

	/*
	auto wordlist = "../picturequiz/data/4000-most-common-english-words-csv.csv";
	std::ifstream infile(wordlist);
	std::string line;
	auto sql_begin = std::string("BEGIN TRANSACTION;");
	rc = sqlite3_exec(db, sql_begin.c_str(), callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	while (std::getline(infile, line))
	{
		auto sql = std::string("INSERT INTO words (word) Values ('") + line + "');";
		std::cout << sql << std::endl;
		rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
		if( rc!=SQLITE_OK ){
		  	fprintf(stderr, "SQL error: %s\n", zErrMsg);
		  	sqlite3_free(zErrMsg);
		}
	}
	auto sql_commit = std::string("COMMIT;");
	rc = sqlite3_exec(db, sql_commit.c_str(), callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}*/


    sqlite3_close(db);
}
void go()
{
	std::cout << "GO!" << std::endl;
	auto guess = answer_input->value();
	if (word == guess) {
		std::cout << "Good" << std::endl;
		answer_input->value("");
		next_word();
	} else {
		std::cout << "Bad" << std::endl;
	}
}

int main(int argc, const char *argv[])
{
	auto window = make_window();

	auto filename = "../picturequiz/data/frog-2240764_640.jpg";

	Fl_JPEG_Image jpg(filename);      // load jpeg image into ram
	image_box->image(jpg);


    next_word();

	
	window->show();
	return Fl::run();
}
