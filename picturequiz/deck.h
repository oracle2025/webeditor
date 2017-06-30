#ifndef DECK_H
#define DECK_H

#include <memory>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>

class Card
{
	public:
		typedef std::shared_ptr<Card> type;
		static type create(const std::string &word);

		std::string word() const;

		template <class Archive>
		void serialize( Archive & ar )
		{
			ar(m_word);
		}
	private:
		Card(const std::string &word);

		std::string m_word;
};

class Stack
{
	public:
		typedef std::shared_ptr<Stack> type;
		static type create(std::vector<int> number_card);

		void append_card(Card::type card);

		bool is_on_number_card(int number);

		size_t size() const;
		std::vector<Card::type> cards();

		void clear();

		template <class Archive>
		void serialize( Archive & ar )
		{
			ar(m_cards, m_number_cards);
		}
	private:
		Stack(std::vector<int> number_card);
		std::vector<Card::type> m_cards;
		std::vector<int> m_number_card;
};

class Deck
{
	public:
		typedef std::shared_ptr<Deck> type;
		static type create();
		~Deck();

		void add_card(const std::string &word);
		void start_session();
		Card::type pull();
		int get_remaining();
		void review(Card::type card, int result); //use enum class
		Stack::type get_stack(int number);

		template <class Archive>
		void serialize( Archive & ar )
		{
			ar( 
				m_current_stack,
				m_working_stacks,
				m_retired_stack,
				m_current_session );
		}
	private:
		Deck();
		//Disable Copy Constructor Assignment, etc.
		Stack::type m_current_stack;
		std::vector<Stack::type> m_working_stacks;
		Stack::type m_retired_stack;
		std::vector<Card::type> m_session_cards;
		int m_current_session = 9;
};

#endif /* DECK_H */
