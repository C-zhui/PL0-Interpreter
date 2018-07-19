#pragma once
#include "component.h"
#include <iostream>

namespace PL0 {
	using namespace std;

	class InstructionRunner {
	private:
		vector<int> cmptStack; // 计算栈
		vector<Variable> &varSpace;
		vector<Procedure> &procSpace;
		int run(const Instruction& i);
	public:
		InstructionRunner(vector<Variable> &varspace, vector<Procedure> &procspace) :
			varSpace(varspace), procSpace(procspace) {}

		void runInstructions(vector<Instruction>& instructions, bool procInstruction);
	};

	void InstructionRunner::runInstructions(vector<Instruction>& instructions, bool procInstruction = false) {
		for (int i = 0, size = instructions.size(); i < size; i++) {
			int to = run(instructions[i]);
			i = to >= 0 ? to - 1 : i;
		}
		if (!procInstruction)
			instructions.resize(0);//除非是过程调用，执行完就清除指令
	}

	int InstructionRunner::run(const Instruction& i) {
		int a, b;
		switch (i.opr)
		{
		case Operator::add:
			a = cmptStack.back();
			cmptStack.pop_back();
			b = cmptStack.back();
			cmptStack.pop_back();
			cmptStack.push_back(a + b); break;
		case Operator::minus:
			a = cmptStack.back();
			cmptStack.pop_back();
			b = cmptStack.back();
			cmptStack.pop_back();
			cmptStack.push_back(b - a); break;
		case Operator::times:
			a = cmptStack.back();
			cmptStack.pop_back();
			b = cmptStack.back();
			cmptStack.pop_back();
			cmptStack.push_back(b * a); break;
		case Operator::div:
			a = cmptStack.back();
			cmptStack.pop_back();
			b = cmptStack.back();
			cmptStack.pop_back();
			cmptStack.push_back(b / a); break;
		case Operator::odd:
			a = cmptStack.back();
			cmptStack.pop_back();
			cmptStack.push_back(a & 1); break;
		case Operator::neg:
			a = cmptStack.back();
			cmptStack.pop_back();
			cmptStack.push_back(-a); break;
		case Operator::push:
			cmptStack.push_back(i.operand); break;
		case Operator::jmp:
			return i.operand;
		case Operator::jz:
			a = cmptStack.back();
			cmptStack.pop_back();
			if (!a) return i.operand;
			break;
		case Operator::load:
			cmptStack.push_back(varSpace[i.operand].value); break;
		case Operator::store:
			a = cmptStack.back();
			cmptStack.pop_back();
			varSpace[i.operand].value = a; break;
		case Operator::call:
			runInstructions(procSpace[i.operand].instructions, true); break;
		case Operator::eql:
			a = cmptStack.back();
			cmptStack.pop_back();
			b = cmptStack.back();
			cmptStack.pop_back();
			cmptStack.push_back(a == b ? 1 : 0); break;
		case Operator::neq:
			a = cmptStack.back();
			cmptStack.pop_back();
			b = cmptStack.back();
			cmptStack.pop_back();
			cmptStack.push_back(a != b ? 1 : 0); break;
		case Operator::lss:
			a = cmptStack.back();
			cmptStack.pop_back();
			b = cmptStack.back();
			cmptStack.pop_back();
			cmptStack.push_back(b < a ? 1 : 0); break;
		case Operator::leq:
			a = cmptStack.back();
			cmptStack.pop_back();
			b = cmptStack.back();
			cmptStack.pop_back();
			cmptStack.push_back(b <= a ? 1 : 0); break;
		case Operator::gtr:
			a = cmptStack.back();
			cmptStack.pop_back();
			b = cmptStack.back();
			cmptStack.pop_back();
			cmptStack.push_back(b > a ? 1 : 0); break;
		case Operator::geq:
			a = cmptStack.back();
			cmptStack.pop_back();
			b = cmptStack.back();
			cmptStack.pop_back();
			cmptStack.push_back(b >= a ? 1 : 0); break;
		case Operator::read:
			cout << ">> ";
			cin >> varSpace[i.operand].value;
			break;
		case Operator::write:
			//cout << "<< " << varSpace[i.operand].value << endl;
			a = cmptStack.back();
			cmptStack.pop_back();
			cout << "<< " << a << endl;
			break;
		}
		return -1;
	}
}