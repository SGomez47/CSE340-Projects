#ifndef __PROJECT2__H__
#define __PROJECT2__H__

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "lexer.h"

using namespace std;

struct rule
{
	string LHS;
	vector<string> RHS;
};

class Project2
{
	private:
		
	public:
		LexicalAnalyzer lexer;
		vector<rule> ruleSet;
		struct rule Rule;

		vector<string> terminals;
		vector<string> nonTerminals;

		int  get_index(vector<string>* universe, string str);
		bool str_is_in_set(vector<string>* srcSet, string str);
		bool check_disjoint(vector<string>* set1, vector<string>* set2);
		bool combine_sets(vector<string>* dstSet, vector<string>* srcSet);
		bool is_epsilon_in(vector<string>* set);
		bool add_epsilon(vector<string>* set);

		void print_set(vector<string>* set);
		void print_set_in_order(vector<string>* set, vector<string>* order);

		void syntax_error();
		void expect(TokenType token);

		void parse_grammar();
		void parse_rule_list();
		void parse_id_list();
		void parse_rule();
		void parse_rhs();

		vector<string> get_nonterminals(vector<rule> rules);
		vector<string> get_terminals(vector<rule> rules, vector<string> nonterminals);
		vector<string> get_universe(vector<string> nonterminals, vector<string> terminals);
		vector<string> get_set_without_epsilon(vector<string> set);
		vector<string> get_nonterminal_order(vector<rule> rules, vector<string> nonterminals);
		vector<string> get_RHS_first_set(vector<rule> firstSet, rule Rule, vector<string> nonterminals);
		
		vector<rule> get_first_set(vector<rule> rules, vector<string> nonterminals, vector<string> terminals);
		vector<rule> get_follow_set(vector<rule> rules, vector<rule> first_set, vector<string> nonterminals);

		bool* check_if_generate(vector<rule> rules);
		bool* check_if_reachable(vector<rule> rulesGen);

		void ReadGrammar();
		void printTerminalsAndNoneTerminals();
		void RemoveUselessSymbols();
		void CalculateFirstSets();
		void CalculateFollowSets();
		void CheckIfGrammarHasPredictiveParser();
};

#endif  //__PROJECT2__H__