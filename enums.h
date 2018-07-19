#pragma once

namespace PL0 {
	enum class Symbol { // 词素常量
		null, identifier, number,
		plus, minus, times,slash,
		oddsym, eql, neq,
		lss, leq, gtr, geq, lparen, rparen,
		comma, semicolon, period, becomes,

		beginsym, endsym, ifsym, thensym, whilesym,
		writesym, readsym, dosym, callsym, constsym,
		varsym, procsym
	};

	enum class Operator { // 指令常量
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
begin
a:=1;
b:=4;
call sum;
write a
end
.
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

/* // 打印 100 内的质数
const max = 100;
var arg, ret,ccc;
procedure isprime;
var i;
begin
	ret := 1;
	i := 2;
	ccc := arg/2+1;
	while i < ccc do
	begin
		if arg / i * i = arg then
		begin
			ret := 0;
			i := ccc
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

/* // 斐波那契数列

var a,b,c,d;
const n = 25;
a:=0;
b:=1;
d:=1;
while d<=n do
begin 
	write a;
	c:=a+b;
	a:=b;
	b:=c;
	d:=d+1
end
*/