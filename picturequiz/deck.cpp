#include "deck.h"

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
int Deck::get_remaining()
{
	return m_session_cards.size();
}
void Deck::review(Card::type card, int result)
{
	if (result) {
		m_working_stacks[m_current_session]->cards().push_back(card);
	} else {
		m_current_stack->cards().push_back(card);
	}
}
Stack::type Deck::get_stack(int number)
{
	return m_working_stacks[number];
}
Deck::Deck()
{
	//m_working_stacks.push_back();
}

void Stack::append_card(Card::type card)
{
	m_cards.push_back(card);
}
void Stack::clear()
{
	m_cards.clear();
}

