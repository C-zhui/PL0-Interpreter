#pragma once
#include "enums.h"
#include <string>
#include <set>

namespace PL0{
	using namespace std;
	class Token {
	public:
		Symbol type = Symbol::null;
		string value;
		Token() {}
		Token(string _value, Symbol _type) :type(_type), value(_value) {}
		bool operator < (const Token& o)const { return this->value < o.value; }
	};

	class __Global {
	private:
		set<Token> _keywords;
	public:
		const set<Token>& keywords() { return _keywords; }
		__Global() {
			_keywords.insert(Token("begin", Symbol::beginsym));
			_keywords.insert(Token("end", Symbol::endsym));
			_keywords.insert(Token("if", Symbol::ifsym));
			_keywords.insert(Token("then", Symbol::thensym));
			_keywords.insert(Token("while", Symbol::whilesym));
			_keywords.insert(Token("write", Symbol::writesym));
			_keywords.insert(Token("read", Symbol::readsym));
			_keywords.insert(Token("do", Symbol::dosym));
			_keywords.insert(Token("call", Symbol::callsym));
			_keywords.insert(Token("const", Symbol::constsym));
			_keywords.insert(Token("var", Symbol::varsym));
			_keywords.insert(Token("procedure", Symbol::procsym));
		}
	};

	class inputCharStream {

		istream &in;
		bool got = false;
		char buf;
		char get() {
			if (got)
				return got = false, buf;
			else
				return got = true, buf = in.get();
		}

		void unget() { got = true; }

	public:
		inputCharStream(istream& InBuf) :in(InBuf) {}

		char peek() {
			char ret = get();
			unget();
			return ret;
		}

		inputCharStream& move() { return got = false, *this; }
	};

	class TokenStream {
		__Global global;
		inputCharStream  cstream;
		bool got = false;
		Token buf;

		Token get() {
			if (got)
				return got = false, buf;
			else {
				char c = cstream.peek();
				while (c == ' ' || c == '\n' || c == '\t')  //È¥³ý¿Õ°×
					c = cstream.move().peek();

				string s = "";
				if (isalpha(c)) {
					do {
						s += c;
						c = cstream.move().peek();
					} while (isalnum(c));
					const set<Token>& k = global.keywords();
					const auto i = k.find(Token(s, Symbol::null));
					if (i != k.end())
						buf = Token(i->value, i->type);
					else buf = Token(s, Symbol::identifier);
				}
				else if (isdigit(c)) {
					do {
						s += c;
						c = cstream.move().peek();
					} while (isdigit(c));
					buf = Token(s, Symbol::number);
				}
				else {
					cstream.move();
					switch (c)
					{
					case '+':
						buf = Token("", Symbol::plus);
						break;
					case '-':
						buf = Token("", Symbol::minus);
						break;
					case '*':
						buf = Token("", Symbol::times);
						break;
					case '/':
						buf = Token("", Symbol::slash);
						break;
					case '=':
						buf = Token("", Symbol::eql);
						break;
					case '#':
						buf = Token("", Symbol::neq);
						break;
					case '<':
						buf = cstream.peek() == '=' ? (cstream.move(), Token("", Symbol::leq)) : Token("", Symbol::lss);
						break;
					case '>':
						buf = cstream.peek() == '=' ? (cstream.move(), Token("", Symbol::geq)) : Token("", Symbol::gtr);
						break;
					case '(':
						buf = Token("", Symbol::lparen);
						break;
					case ')':
						buf = Token("", Symbol::rparen);
						break;
					case ',':
						buf = Token("", Symbol::comma);
						break;
					case ';':
						buf = Token("", Symbol::semicolon);
						break;
					case '.':
						buf = Token("", Symbol::period);
						break;
					case ':':
						if (cstream.peek() == '=') {
							buf = (cstream.move(), Token("", Symbol::becomes));
							break;
						}
					default:
						throw string("err lexical");
					}
				} 
				return buf;
			}
		}

		void unget() { got = true; }

	public:
		TokenStream(istream& inBuf) :cstream(inBuf) {}

		Token peek() {
			Token ret = get();
			unget();
			return ret;
		}

		TokenStream& move() {
			return got = false, *this;
		}
	};
}