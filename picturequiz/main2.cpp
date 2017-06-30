#include "picturequiz2.h"
#include <iostream>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_JPEG_Image.H>
#include <fstream>
#include <sstream>
#include <string>
#include <FL/fl_ask.H>
#include <string>
#include <random>
#include <memory>
#include "deck.h"
#include <iostream>
#include <locale>
#include <codecvt>
#include "bad_image.xpm"
#include <FL/Fl_Pixmap.H>

std::shared_ptr<Fl_JPEG_Image> g_image;
Deck::type deck;
Card::type word;
void show_image();
inline bool exists_test0 (const std::string& name)
{
#if WIN32
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring wstr = converter.from_bytes(name);

	std::ifstream f(wstr);
    return f.good();
#else
	std::ifstream f(name);
    return f.good();
#endif
}

std::string c;

void next_word()
{
	std::cout << "remaining: " << deck->get_remaining() << std::endl;
	if (deck->get_remaining()) {
		word = deck->pull();
		c = word->word();
		std::cout << "Word: " << c.c_str() << std::endl;
		image_box->label(c.c_str());
		for (int i = 0; i <= strlen(c.c_str()); i++) {
			std::cout << i << ": " << (int)c.c_str()[i] << " " << c.c_str()[i] << std::endl;
		}
		std::cout << "imagebox_label: " << image_box->label() << std::endl;
		//image_box->label("Bla");
		cards_in_session->value(deck->get_remaining());
		wizard->value(image_box);
		show_image();
	} else {
		wizard->value(session_over);
	}

}
static Fl_Pixmap  G_bad_image(bad_image_xpm);
void show_image()
{
	std::ostringstream str;
	const char* prefix = "../picturequiz/data/images/";
	const char* postfix = ".jpg";

	std::random_device rd;
	std::uniform_int_distribution<int> dist(0,8);
	int random_number = dist(rd);
	str << prefix << word->word() << "-" << random_number << postfix;
/*
	std::cout << "Filename: " << str.str() << std::endl;
	
	std::cout << "Exists: " << exists_test0(str.str()) << std::endl;
*/
	image_box->image(G_bad_image);
	if (exists_test0(str.str())) {
		std::cout << "Loading: " << str.str() << std::endl;
		auto jpg = std::shared_ptr<Fl_JPEG_Image>(new Fl_JPEG_Image(str.str().c_str()));      // load jpeg image into ram
		switch ( jpg->fail() ) {
    		case Fl_Image::ERR_NO_IMAGE:
    		case Fl_Image::ERR_FILE_ACCESS:
                //fl_alert("/tmp/foo.jpg: %s");//, strerror(errno));    // shows actual os error to user
        		fl_alert("/tmp/foo.jpg: ");
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
}

void go()
{
	auto guess = answer_input->value();

	if (wizard->value() == good_box) {
		next_word();
	}
	if (wizard->value() == bad_box) {
		next_word();
	}
	
	if (wizard->value() != image_box) {
		//wizard->value(image_box);
		return;
	}

	if (word->word() == guess) {
		answer_input->value("");
		wizard->value(good_box);
		deck->review(word, 1);
	} else {
		wizard->value(bad_box);
		answer_box->label(c.c_str());
		deck->review(word, 0);
	}

}
void add_card()
{
	auto card = fl_input("Add Card");
	deck->add_card(card);

}
void start_session()
{
	deck->start_session();
	session_nr->value(deck->session_nr());
	next_word();
}

int main(int argc, const char *argv[])
{
	deck = Deck::create();

	{
		std::ifstream is("deck.json", std::ios::binary);
		cereal::JSONInputArchive archive(is);
		archive(deck);
	}

	auto window = make_window();
	wizard->value(session_over);
	image_box->label("[label]");
	window->show();
	auto result = Fl::run();
	std::cout << "Autosave" << std::endl;

  	std::ofstream os("deck.json", std::ios::binary);
      //cereal::BinaryOutputArchive archive( os );
  	cereal::JSONOutputArchive archive( os );

  	archive(deck);


	return result;
}
