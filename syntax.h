#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include "enums.h"
#include "inputstream.h"
#include "component.h"
#include "instructionRunner.h"

/*  下面的代码都是根据这套 PL/0 语法进行编写
	program = block "." .

	block = [ "const" ident "=" number {"," ident "=" number} ";"]
			[ "var" ident {"," ident} ";"]
			{ "procedure" ident ";" block ";" } statement .

	statement = [ ident ":=" expression 
				| "call" ident
				| "read" ident 
				| "write" expression
				| "begin" statement {";" statement } "end"
				| "if" condition "then" statement
				| "while" condition "do" statement ].

	condition = "odd" expression 
				|expression ("="|"#"|"<"|"<="|">"|">=") expression .

	expression = [ "+"|"-"] term { ("+"|"-") term}.

	term = factor {("*"|"/") factor}.

	factor = ident | number | "(" expression ")".

*/

namespace PL0 {
	using namespace std;

	class Syntax {
		TokenStream &ts; //词素流
		IdentifierTable& identifierTable; //标识符表
		ProcedureTable& procedureTable; //过程表
		InstructionRunner & runner; // 执行器
	public:

		Syntax(TokenStream &_ts, IdentifierTable&_identifierTable,
			ProcedureTable& _procedureTable, InstructionRunner&_runner) :
			ts(_ts), identifierTable(_identifierTable),
			procedureTable(_procedureTable), runner(_runner) {}

		void program(vector<Instruction>& instructions);
		void block(vector<Instruction>& instructions, bool inProc = false);
		void declareProcedure();
		void  declareVar();
		void declareConst();
		void condition(vector<Instruction>& instructions);
		void statement(vector<Instruction>& instructions);
		void expression(vector<Instruction>& instructions);
		void term(vector<Instruction>& instructions);
		void factor(vector<Instruction>& instructions);
	};

	void Syntax::program(vector<Instruction>& instructions) {
		block(instructions);
		if (ts.peek().type == Symbol::period) {
			return;
		}
		else throw string("espect a period");
	}

	void Syntax::block(vector<Instruction>& instructions, bool inProc) {
		Token t;
		while (true) {
			try {
				t = ts.peek();
				switch (t.type)
				{
				case  Symbol::constsym:
					declareConst();
					break;
				case  Symbol::varsym:
					declareVar();
					break;
				case  Symbol::procsym:
					if (!inProc)
						declareProcedure();
					else throw string("can't define procedure in procedure");
					break;
				case Symbol::period: //程序结束标志
					return;
				case Symbol::semicolon:
					if (inProc)
						return;
					ts.move();
					break;
				default:
					statement(instructions);
					if (!inProc)runner.runInstructions(instructions);
					break;
				}
			}
			catch (const string &s) {
				instructions.resize(0);
				cout << s << endl;
				cout << "ignore until next semicolon...";
				while (ts.peek().type != Symbol::semicolon)
					ts.move();
			}
		}
	}

	void  Syntax::declareProcedure() {
		Token name = ts.move().peek();
		if (procedureTable.checkExists(name.value))
			throw string("redefine procedure!");
		if (ts.move().peek().type != Symbol::semicolon)
			throw string("espect a semicolon!");
		ts.move();
		Procedure p;
		block(p.instructions, true);
		Token t = ts.peek();
		if (t.type == Symbol::semicolon) {
			procedureTable.add(name.value, p);
			ts.move();
			return;
		}
	}
	void Syntax::declareVar() {
		Token token;
		do {
			token = ts.move().peek();
			if (token.type == Symbol::identifier) {
				if (!identifierTable.add(token.value))
					throw string("repeatedly declare var : ") + token.value;
			}
			else throw string("espect an identifier!");
			token = ts.move().peek();
		} while (token.type == Symbol::comma);
		if (token.type != Symbol::semicolon)
			throw string("espect an semicolon here!");
		ts.move();
	}

	void Syntax::declareConst() {
		Token t;
		do {
			t = ts.move().peek();
			if (t.type == Symbol::identifier) {
				Token eql = ts.move().peek();
				if (eql.type == Symbol::eql) {
					Token num = ts.move().peek();
					if (num.type == Symbol::number) {
						istringstream iss(num.value.data());
						int constV;
						iss >> constV;
						if (!identifierTable.add(t.value, true, constV))
							throw string("repeatedly declare const : ") + t.value;
					}
					else throw string("espect a const number");
				}
				else throw string("espect an assignment");
			}
			else throw string("espect an identifier!");
			t = ts.move().peek();
		} while (t.type == Symbol::comma);
		if (t.type != Symbol::semicolon)
			throw string("espect an semicolon!");
		ts.move();
	}
	void Syntax::statement(vector<Instruction>& instructions) {
		Token t = ts.peek(), t2, t3;
		int before_while, ind_jz, ind_jmp_to;

		switch (t.type)
		{
		case Symbol::identifier:
			t2 = ts.move().peek();
			if (t2.type != Symbol::becomes)throw string("espect an assignment");
			ts.move();
			expression(instructions);
			if (identifierTable.checkExists(t.value)) {
				if (identifierTable.varSpace[identifierTable.getVarSpaceIndexOf(t.value)].isConst == false)
					instructions.push_back(Instruction(Operator::store, identifierTable.getVarSpaceIndexOf(t.value)));
				else throw string("can't assign a value to a const variable");
			}
			else throw string("Undeclare variable : ") + t.value;
			break;
		case Symbol::callsym:
			t2 = ts.move().peek();
			if (t2.type == Symbol::identifier) {
				if (procedureTable.checkExists(t2.value))
					instructions.push_back(Instruction(Operator::call, procedureTable.getProcIndexOf(t2.value)));
				else throw string("this procedure doesn't exist !");
				ts.move();
			}
			else throw string("espect an identifier!");
			break;
		case Symbol::readsym:
			t2 = ts.move().peek();
			if (t2.type != Symbol::identifier)
				throw string("espect an identifier!");
			if (identifierTable.checkExists(t2.value))
				instructions.push_back(Instruction(Operator::read, identifierTable.getVarSpaceIndexOf(t2.value)));
			else throw string("Undeclare variable : ") + t2.value;
			ts.move();
			break;
		case Symbol::writesym:
			ts.move();
			expression(instructions);
			instructions.push_back(Instruction(Operator::write));
			/*t2 = ts.move().peek();
			if (t2.type != Symbol::identifier)
			throw string("espect a identifier");
			if (identifierTable.checkExists(t2.value))
			instructions.push_back(Instruction(Operator::write, identifierTable.getVarSpaceIndexOf(t2.value)));
			else throw string("Undeclare variable : ") + t.value;
			ts.move();*/
			break;
		case Symbol::beginsym:
			ts.move();
			statement(instructions);
			t = ts.peek();
			while (t.type == Symbol::semicolon) {
				ts.move();
				statement(instructions);
				t = ts.peek();
			}
			if (t.type != Symbol::endsym)
				throw string("espect a end keyword!");
			ts.move();
			break;
		case Symbol::ifsym:
			ts.move();
			condition(instructions);
			instructions.push_back(Instruction(Operator::jz));
			ind_jz = instructions.size() - 1;
			t = ts.peek();
			if (t.type == Symbol::thensym) {
				ts.move();
				statement(instructions);
				int ind_jmp_to = instructions.size(); //if 块的下一条指令
				instructions[ind_jz].operand = ind_jmp_to;
			}
			else throw string("espect then keyword here");
			break;
		case Symbol::whilesym:
			ts.move();
			before_while = instructions.size(); //condition的第一条指令
			condition(instructions);
			instructions.push_back(Instruction(Operator::jz));
			ind_jz = instructions.size() - 1;
			t = ts.peek();
			if (t.type == Symbol::dosym) {
				ts.move();
				statement(instructions);
				instructions.push_back(Instruction(Operator::jmp, before_while));
				ind_jmp_to = instructions.size(); //while 块的下一条指令
				instructions[ind_jz].operand = ind_jmp_to;
			}
			else throw string("espect do keyword here");
			break;
		case Symbol::period:
			break;
		case Symbol::semicolon:
			ts.move();
			break;
		default:
			throw string("Statement Err");
		}
	}

	void Syntax::condition(vector<Instruction>& instructions) {
		Token t = ts.peek();
		if (t.type == Symbol::oddsym) {
			expression(instructions);
			instructions.push_back(Instruction(Operator::odd));
			return;
		}
		expression(instructions);
		t = ts.peek();
		switch (t.type)
		{
		case Symbol::eql:
			ts.move();
			expression(instructions);
			instructions.push_back(Instruction(Operator::eql));
			break;
		case Symbol::neq:
			ts.move();
			expression(instructions);
			instructions.push_back(Instruction(Operator::neq));
			break;
		case Symbol::lss:
			ts.move();
			expression(instructions);
			instructions.push_back(Instruction(Operator::lss));
			break;
		case Symbol::gtr:
			ts.move();
			expression(instructions);
			instructions.push_back(Instruction(Operator::gtr));
			break;
		case Symbol::leq:
			ts.move();
			expression(instructions);
			instructions.push_back(Instruction(Operator::leq));
			break;
		case Symbol::geq:
			ts.move();
			expression(instructions);
			instructions.push_back(Instruction(Operator::geq));
			break;
		default:
			break;
		}
	}
	void Syntax::expression(vector<Instruction>& instructions) {
		Token t = ts.peek();
		switch (t.type)
		{
		case Symbol::plus:
			ts.move();
			term(instructions);
			break;
		case Symbol::minus:
			ts.move();
			term(instructions);
			instructions.push_back(Instruction(Operator::neg));
			break;
		default:
			term(instructions);
		}
		do {
			t = ts.peek();
			if (t.type == Symbol::plus) {
				ts.move();
				term(instructions);
				instructions.push_back(Instruction(Operator::add));
			}
			else if (t.type == Symbol::minus) {
				ts.move();
				term(instructions);
				instructions.push_back(Instruction(Operator::minus));
			}
			else break;
		} while (true);
	}
	void Syntax::term(vector<Instruction>& instructions) {
		factor(instructions);
		do {
			Token t = ts.peek();

			if (t.type == Symbol::times) {
				ts.move();
				factor(instructions);
				instructions.push_back(Instruction(Operator::times));
			}
			else if (t.type == Symbol::slash) {
				ts.move();
				factor(instructions);
				instructions.push_back(Instruction(Operator::div));
			}
			else break;
		} while (true);
	}

	void Syntax::factor(vector<Instruction>& instructions) {
		Token t = ts.peek();
		istringstream is(t.value.data());
		switch (t.type)
		{
		case Symbol::identifier:
			if (identifierTable.checkExists(t.value))
				instructions.push_back(Instruction(Operator::load, identifierTable.getVarSpaceIndexOf(t.value)));
			else throw string("Undeclare variable : ") + t.value;
			break;
		case Symbol::number:
			int value;
			is >> value;
			instructions.push_back(Instruction(Operator::push, value));
			break;
		case Symbol::lparen:
			ts.move();
			expression(instructions);
			if (ts.peek().type != Symbol::rparen)
				throw string("espect a right parenthesis!");
			break;
		default:
			throw string("espect a factor!");
		}
		ts.move();
	}



	class PL_0 {
		InstructionRunner runner;
		vector<Instruction> instructions;
		TokenStream ts;
		Syntax syntax;
		ProcedureTable procedureTable;
		IdentifierTable identifierTable;
	public:
		PL_0(istream &Inbuf) :ts(Inbuf),
			runner(identifierTable.varSpace, procedureTable.procSpace),
			syntax(ts, identifierTable, procedureTable, runner)
		{}

		void run() {
			syntax.program(instructions);
		}
	};
}