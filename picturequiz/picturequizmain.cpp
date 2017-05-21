#include "picturequiz.h"
#include <iostream>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include "../sqlite3.h"
#include <fstream>
#include <sstream>
#include <string>
#include <FL/fl_ask.H>
#include <string>
#include <random>
#include <memory>
#include <locale>
#include <codecvt>

inline bool exists_test0 (const std::string& name)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wstr = converter.from_bytes(name);

	std::ifstream f(wstr);
    return f.good();
}
std::string word;
auto dbfile = "../picturequiz/data/picturequiz.sqlite3";

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
          //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
      	word = argv[i];
    }
    //printf("\n");
    return 0;
}
std::shared_ptr<Fl_JPEG_Image> g_image;

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

	std::ostringstream str;
	const char* prefix = "../picturequiz/data/images/";
	const char* postfix = ".jpg";

	std::random_device rd;
	std::uniform_int_distribution<int> dist(0,8);
	int random_number = dist(rd);
	str << prefix << word << "-" << random_number << postfix;
/*
	std::cout << "Filename: " << str.str() << std::endl;
	
	std::cout << "Exists: " << exists_test0(str.str()) << std::endl;
*/
	if (exists_test0(str.str())) {
		std::cout << "Loading: " << str.str() << std::endl;
		auto jpg = std::shared_ptr<Fl_JPEG_Image>(new Fl_JPEG_Image(str.str().c_str()));      // load jpeg image into ram
		switch ( jpg->fail() ) {
    		case Fl_Image::ERR_NO_IMAGE:
    		case Fl_Image::ERR_FILE_ACCESS:
        		fl_alert("/tmp/foo.jpg: %s");//, strerror(errno));    // shows actual os error to user
    		case Fl_Image::ERR_FORMAT:
        		fl_alert("/tmp/foo.jpg: couldn't decode image");
        	default:
        		std::cout << "Success!" << std::endl;
				image_box->image(*jpg.get());
				g_image = jpg;
		}
	} else {
		std::cout << "Not Available: " << str.str() << std::endl;
	}

	/*
	//auto wordlist = "../picturequiz/data/4000-most-common-english-words-csv.csv";
	auto wordlist = "../picturequiz/data/list_of_words.txt";
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
	//std::cout << "GO!" << std::endl;
	auto guess = answer_input->value();
	
	if (wizard->value() == good_box) {
		next_word();
	}

	if (wizard->value() != image_box) {
		wizard->value(image_box);
		return;
	}

	if (word == guess) {
		//std::cout << "Good" << std::endl;
		answer_input->value("");
		wizard->value(good_box);
	} else {
		wizard->value(bad_group);
		solution_box->label(word.c_str());
		//std::cout << "Bad" << std::endl;
	}
}


/*
 * Resoirvar: Alle Karten die es gibt. In der Reihenfolge in der Sie gelernt werden sollen.
 *
 * Bucket 1: Limit: 5 Karten : Testen: Täglich
 * Bucket 2: Testen: Wöchentlich
 * Bucket 3: Testen : 2 Wöchentlich
 * Bucket 4: Limit: keins: Testen: Monatlich
 *
 *
 *They might choose to study the Box 1 cards once a day, Box 2 every 3 days, and the Box 3 cards every 5 days

 


Proficiency levels

This example uses 5 proficiency levels and 12 decks of flash cards. Cards at Proficiency Level 1 are reviewed at every learning session; those at Level 5 are retired and no longer in use. Those at Levels 2, 3, and 4 are reviewed every 2nd, 3rd, and 4th session, respectively.

Learning sessions are numbered from 0 to 9, then the numbering starts over again (that is, 0, 1, 2, ... 8, 9, 0, 1, 2 ...). Cards at Level 1 are in Deck Current; those at Level 5 are in Deck Retired; all other cards are in 1 of these 10 "progress" decks, each of which begins with a title card sporting 4 digits:

    0-2-5-9
    1-3-6-0
    2-4-7-1
    3-5-8-2
    4-6-9-3
    5-7-0-4
    6-8-1-5
    7-9-2-6
    8-0-3-7
    9-1-4-8

If a learner is successful at a card from Deck Current, it gets transferred into the progress deck that begins with that session's number. (For example, success at a card during Session 6 transfers it from Deck Current to Deck 6-8-1-5.) Cards from that deck are reviewed whenever a number from the deck title matches the session number. (For example, cards from Deck 6-8-1-5 will be reviewed again at Sessions 8, 1, and 5.) If a learner has difficulty with a card during a subsequent review, the card is returned to Deck Current; otherwise it stays in its progress deck. When a learner is successful at a card during a session that matches the last number on the deck (for example, Session 5 for Deck 6-8-1-5), that card goes into Deck Retired, and the title card for that progress deck is freed up for use at the following session.
 *
 *
 */

int main(int argc, const char *argv[])
{
	fl_register_images();
	auto window = make_window();

	auto filename = "../picturequiz/data/frog-2240764_640.jpg";

	Fl_JPEG_Image jpg(filename);      // load jpeg image into ram
	image_box->image(jpg);
	wizard->value(image_box);

    next_word();

	
	window->show();
	return Fl::run();
}
