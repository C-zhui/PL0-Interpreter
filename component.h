#pragma once
#include <map>
#include <vector>
#include "enums.h"

namespace PL0 {
	using namespace std;

	class Variable {
	public:
		Variable(bool _isConst, int _value = 0) :
			isConst(_isConst), value(_value) {}
		int value;
		bool isConst;
	};

	class IdentifierTable {
	public:
		vector<Variable> varSpace;
		map<string, int> varTable;

		unsigned int getVarSpaceIndexOf(string name) {
			return varTable[name];
		}

		bool checkExists(string name) {
			return varTable.count(name) != 0;
		}

		bool add(string name, bool isConst = false, int constValue = 0) {
			if (varTable.count(name) != 0)
				return false;
			else {
				varTable.insert_or_assign(name, varSpace.size());
				varSpace.push_back(Variable(isConst, constValue));
				return true;
			}
		}
	};

	class Instruction {

	public:
		Operator opr = Operator::add;
		int operand;
		Instruction() {}
		Instruction(Operator o, int _operand = 0) :opr(o), operand(_operand) {}
	};

	class Procedure {
	public:
		vector<Instruction> instructions;
	};

	class ProcedureTable {
	public:
		vector<Procedure> procSpace;
		map<string, int> procTable;

		int getProcIndexOf(const string& name) {
			if (checkExists(name))
				return procTable[name];
			return -1;
		}

		bool add(const string &name, const Procedure& p) {
			if (procTable.count(name) != 0)
				return false;
			procTable.insert_or_assign(name, procSpace.size());
			procSpace.push_back(p);
			return true;
		}

		bool checkExists(const string &name) {
			return procTable.count(name) != 0 ? true : false;
		}
	};
}