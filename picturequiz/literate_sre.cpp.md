# Spaced Repetition

So wie ich das verstehe gibt es 12 Decks, die jeweils Karten enthalten.

Für die Serialisierung verwende ich json und da die json.hpp library

```C++

#include <algorithm>
#include <stdexcept>
#include "json.hpp"
using json = nlohmann::json;

```


# Karte

Eine Karte enthält ein Wort, und in diesem Fall wird das Wort mit einem
Bild verglichen. Es gibt noch eine zweite Art von Karte, die 4 Zahlen
enthält

Was sind die Tests

```C++

struct Card {
	std::string word;
};

struct Stack
{
	int size() const;
	std::vector<Card> cards;
	std::vector<int> number_card;
};

int Stack::size() const
{
	return cards.size();
}

struct Deck
{
	std::string serialize();
	void add_card(const std::string &word);
	void start_session();
	Card pull();
	int get_remaining();
	void review(Card &c, int result);
	Stack& get_stack(int number);

	Stack m_current_stack;
	std::vector<Stack> m_working_stacks;
	Stack m_retired_stack;
	Card m_card;

	std::vector<Card> m_session_cards;

	int m_current_session = 9;
};
std::string Deck::serialize()
{
	json j;
	j["current_session"] = m_current_session;
	json current_stack;
	json retired_stack;
	json working_stacks;
	for (auto c: m_current_stack.cards) {
		current_stack.push_back(c.word);
	}
	j["current_stack"] = current_stack;
	for (auto c: m_retired_stack.cards) {
		retired_stack.push_back(c.word);
	}
	j["retired_stack"] = retired_stack;
	for (int i = 0; i < 10; i++) {
		json st;
		for (auto j : m_working_stacks[i].cards) {
			st.push_back(j.word);
		}
		working_stacks[i] = st;
	}
	j["working_stacks"] = working_stacks;
	return j.dump();
}

const int GOOD = 1;
const int BAD = 0;

Deck init_deck() {
	Deck result;
	result.m_working_stacks.resize(10);
	result.m_working_stacks[0].number_card = {2, 5, 9};
	result.m_working_stacks[1].number_card = {3, 6, 0};
	result.m_working_stacks[2].number_card = {4, 7, 1};
	result.m_working_stacks[3].number_card = {5, 8, 2};
	result.m_working_stacks[4].number_card = {6, 9, 3};
	result.m_working_stacks[5].number_card = {7, 0, 4};
	result.m_working_stacks[6].number_card = {8, 1, 5};
	result.m_working_stacks[7].number_card = {9, 2, 6};
	result.m_working_stacks[8].number_card = {0, 3, 7};
	result.m_working_stacks[9].number_card = {1, 4, 8};
	
	return result;
	}
	
Deck unserialize_deck(const std::string &stored)
{
	Deck result;
	json j = json::parse(stored);
	result.m_current_session = j["current_session"].get<int>();
	result.m_working_stacks.resize(10);

	for (int i = 0; i < 10; i++) {
		if (!j["working_stacks"][i].is_null()) {
			for (auto k: j["working_stacks"][i]) {
				 result.m_working_stacks[i].cards.push_back({k.get<std::string>()});
			}
		}
	}

	if (!j["current_stack"].is_null()) {
		for (auto k: j["current_stack"]) {
			 result.m_current_stack.cards.push_back({k.get<std::string>()});
		}
	}
	if (!j["retired_stack"].is_null()) {
		for (auto k: j["retired_stack"]) {
			 result.m_retired_stack.cards.push_back({k.get<std::string>()});
		}
	}

	

	
	return result;
}

```

```C++
void test1() {

	auto deck = init_deck();

	deck.add_card("cinco");

	assert(deck.get_remaining() == 0);

	deck.start_session(); // 0
	
	assert(deck.get_remaining() == 1);

	auto card = deck.pull();

	assert(deck.get_remaining() == 0);

	deck.review(card, GOOD);

	assert(deck.get_stack(0).size() == 1);

	deck.start_session(); // 1
	assert(deck.get_remaining() == 0);
	deck.start_session(); // 2

	assert(deck.get_remaining() == 1);

	card = deck.pull();
	deck.review(card, BAD);
	deck.start_session();

	assert(deck.get_remaining() == 1);
	card = deck.pull();
	deck.review(card, GOOD);

	std::string stored = deck.serialize();
	std::cout << stored << std::endl;

	Deck deck2 = unserialize_deck(stored);
	std::string stored2 = deck2.serialize();
	std::cout << stored2 << std::endl;

	assert(stored2 == stored);
	
}
void test2() {
	
}
```

```C++


class WordCard : public Card
{
	std::string word;
};

class NumberCard : public Card
{
	int numbers[4];
};


```

# Stapel

Die Erste Karte in einem Stapel ist die Number Card


# Session

Wie läuft eine Session ab? Alle Sessions sind durchnummeriert von 0 bis 9, und wenn man bei 9 angelangt ist, dann fängt man wieder bei 0 an.

```C++

json j = R"({
"FlashCards": {
	"deck_current": {}
}
})"_json;


```

FlashCards {
	deck_current {
		"cinco",
		"pé",
		"baixo",
		"mulher",
		"vestir",
		"orelha",
		"anel",
		"apertado",
		"saia"
	},
	deck_0 {
		number_card{0, 2, 5, 9}
	},
    deck_1 {
		number_card{1, 3, 6, 0}
	},
    deck_2 {
		number_card{2, 4, 7, 1}
	},
    deck_3 {
		number_card{3, 5, 8, 2}
	},
    deck_4 {
		number_card{4, 6, 9, 3}
	},
    deck_5 {
		number_card{5, 7, 0, 4}
	},
    deck_6 {
		number_card{6, 8, 1, 5}
	},
    deck_7 {
		number_card{7, 9, 2, 6}
	},
    deck_8 {
		number_card{8, 0, 3, 7}
	},
    deck_9 {
		number_card{9, 1, 4, 8}
	},
	deck_retired {}
};

In diesem Fall sind alle Karten in der aktuellen Session 

# Run the Program

```C++
void Deck::add_card(const std::string &word)
{
	m_current_stack.cards.push_back(Card({word}));
}
void Deck::start_session()
{
	m_current_session++;
	m_current_session %= 10;
	for (auto i: m_current_stack.cards) {
		m_session_cards.push_back(i);
	}
	for (auto &s: m_working_stacks) {
		if (std::find(s.number_card.begin(), s.number_card.end(), m_current_session) != s.number_card.end()) {
			for (auto i : s.cards) {
				m_session_cards.push_back(i);
			}
			s.cards.resize(0);
		}
	}
	m_current_stack.cards.resize(0);
}
Card Deck::pull()
{
	if (m_session_cards.size() == 0) {
		throw std::runtime_error("session_cards empty");
	}
	auto result = m_session_cards.back();
	m_session_cards.pop_back();
	return result;
}
int Deck::get_remaining()
{
	return m_session_cards.size();
}
void Deck::review(Card &c, int result)
{
	//session
	if (result) {
		m_working_stacks[m_current_session].cards.push_back(c);		
	} else {
		m_current_stack.cards.push_back(c);
	}
}
Stack& Deck::get_stack(int number)
{
	return m_working_stacks[number];
}

#include <iostream>

int main () {
	std::cout << "Test Hello" << std::endl;
	test1();
}
```

