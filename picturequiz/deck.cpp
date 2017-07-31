#include "deck.h"
#include <algorithm>

Card::type Card::create(const std::string &word)
{
	return Card::type(new Card(word));
}
std::string Card::word() const
{
	return m_word;
}
Card::Card(const std::string &word) :
	m_word(word)
{
}
Card::Card()
{
}


Stack::type Stack::create(std::vector<int> number_card)
{
	return Stack::type(new Stack(number_card));
}
size_t Stack::size() const
{
	return m_cards.size();
}
std::vector<Card::type> Stack::cards()
{
	return m_cards;
}
Stack::Stack()
{
}
Stack::Stack(std::vector<int> number_card) :
	m_number_card(number_card)
{
}
bool Stack::is_on_number_card(int number)
{
		return std::find(
				m_number_card.begin(), 
				m_number_card.end(), 
				number) != m_number_card.end();
}



Deck::type Deck::create()
{
	return Deck::type(new Deck());
}
Deck::~Deck()
{
}
void Deck::add_card(const std::string &word)
{
	m_current_stack->append_card(Card::create(word));
}
void Deck::start_session()
{
	m_current_session++;
	m_current_session %= 10;
	for (auto card: m_current_stack->cards()) {
		m_session_cards.push_back(card);
	}
	for (auto s: m_working_stacks) {
		if ( s->is_on_number_card(m_current_session) ) {
			for (auto i: s->cards()) {
				m_session_cards.push_back(i);
			}
		}
	}
	m_current_stack->clear();
}
Card::type Deck::pull()
{
	if (m_session_cards.size() == 0) {
		throw std::runtime_error("session_cards emtpy");
	}
	auto result = m_session_cards.back();
	m_session_cards.pop_back();
	return result;
}
size_t Deck::get_remaining()
{
	return m_session_cards.size();
}
void Deck::review(Card::type card, int result)
{
	if (result) {
		m_working_stacks[m_current_session]->append_card(card);
	} else {
		m_current_stack->append_card(card);
	}
}
Stack::type Deck::get_stack(int number)
{
	return m_working_stacks[number];
}
Deck::Deck()
{
	//m_working_stacks.push_back();
		m_current_stack = Stack::create({});
		m_retired_stack = Stack::create({});
		m_working_stacks.push_back(Stack::create({2, 5, 9}));
		m_working_stacks.push_back(Stack::create({3, 6, 0}));
		m_working_stacks.push_back(Stack::create({4, 7, 1}));
		m_working_stacks.push_back(Stack::create({5, 8, 2}));
		m_working_stacks.push_back(Stack::create({6, 9, 3}));
		m_working_stacks.push_back(Stack::create({7, 0, 4}));
		m_working_stacks.push_back(Stack::create({8, 1, 5}));
		m_working_stacks.push_back(Stack::create({9, 2, 6}));
		m_working_stacks.push_back(Stack::create({0, 3, 7}));
		m_working_stacks.push_back(Stack::create({1, 4, 8}));
}
int Deck::session_nr()
{
	return m_current_session;
}

void Stack::append_card(Card::type card)
{
	m_cards.push_back(card);
}
void Stack::clear()
{
	m_cards.clear();
}

