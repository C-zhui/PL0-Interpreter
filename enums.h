#pragma once

namespace PL0 {
	enum class Symbol {
		null, identifier, number,
		plus, minus, times,slash,
		oddsym, eql, neq,
		lss, leq, gtr, geq, lparen, rparen,
		comma, semicolon, period, becomes,

		beginsym, endsym, ifsym, thensym, whilesym,
		writesym, readsym, dosym, callsym, constsym,
		varsym, procsym
	};

	enum class Operator {
		add, minus, times, div,odd,
		neg, push, jmp, jz, load, store,call,
		eql, neq, lss, gtr, leq, geq,
		read,write
	};
}

/*
var a,b;
begin
a:=1;
b:=2;
a:=a+b;
write a
end
.
*/

/* // 简单求和
var a,b; 
procedure sum;
begin
a:=a+b
end;
 
a:=1;
b:=4;
call sum;
write a
*/

/* 求1加到10
var a,b,sum;
begin
	a:=1;
	b:=10;
	sum:=0;
	while a<=b
	do 
	begin 
		sum:=sum+a;
		a:=a+1
	end;
	write sum
end
*/

/* // 打印 100 内的质数
const max = 100;
var arg, ret;

procedure isprime;
var i;
begin
	ret := 1;
	i := 2;
	while i < arg do
		begin
		if arg / i * i = arg then
		begin
			ret := 0;
			i := arg
		end;
		i := i + 1
	end
end;

procedure primes;
begin
arg := 2;
while arg < max do
begin
call isprime;
if ret = 1 then write arg;
arg := arg + 1
end
end;

call primes
*/