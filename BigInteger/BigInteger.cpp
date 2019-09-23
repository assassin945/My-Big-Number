#ifndef BIGINTEGER_
#define BIGINTEGER_

#include "BigInteger.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#define MAX 10000   //the largest length of the integer
using namespace std;
//empty constructor initializes zero
BigInteger::BigInteger(){
    number = "0";
    sign = false;
}
//string constructor
BigInteger::BigInteger(string value){
    if( isdigit(value[0]) ) // if not signed
	{
		setNumber(value);
		sign = false; // +ve
	}
	else
	{
		setNumber( value.substr(1) );
		sign = (value[0] == '-');
	}
}
//string constructor
BigInteger::BigInteger(string value, bool sin){
    setNumber(value);
    setSign(sin);
}
//integer constructor
BigInteger::BigInteger(int n){
    stringstream ss;
	string s;
	ss << n;
	ss >> s;

	if( isdigit(s[0]) ) // if not signed
	{
		setNumber( s );
		setSign( false ); // +ve
	}
	else
	{
		setNumber( s.substr(1) );
		setSign( s[0] == '-' );
	}
}

void BigInteger::setNumber(string value){
    number = value;
}

const string& BigInteger::getNumber(){
    return number;
}

void BigInteger::setSign(bool sin){
    sign = sin;
}

const bool& BigInteger::getSign(){
    return sign;
}

BigInteger BigInteger::absolute(){
    return BigInteger(getNumber());
}

void BigInteger::operator = (BigInteger b) {
	setNumber(b.getNumber());
	setSign(b.getSign());
}

bool BigInteger::operator == (BigInteger b) {
	return equals((*this), b);
}

bool BigInteger::operator != (BigInteger b) {
	return !equals((*this), b);
}

bool BigInteger::operator > (BigInteger b) {
	return greater((*this), b);
}

bool BigInteger::operator<(BigInteger b) {
	return less((*this), b);
}

bool BigInteger::operator>=(BigInteger b) {
	return equals((*this), b) || greater((*this), b);
}

bool BigInteger::operator<=(BigInteger b) {
	return equals((*this), b) || less((*this), b);
}

BigInteger& BigInteger::operator ++() // prefix
{
	(*this) = (*this) + 1;
	return (*this);
}

BigInteger BigInteger::operator ++(int) // postfix
{
	BigInteger before = (*this);

	(*this) = (*this) + 1;

	return before;
}

BigInteger& BigInteger::operator --() // prefix
{
	(*this) = (*this) - 1;
	return (*this);

}

BigInteger BigInteger::operator + (BigInteger b)
{
	BigInteger addition;
	if (getSign() == b.getSign()) // both +ve or -ve
	{
		addition.setNumber(add(getNumber(), b.getNumber()));
		addition.setSign(getSign());
	}
	else // sign different
	{
		if (absolute() > b.absolute())
		{
			addition.setNumber(subtract(getNumber(), b.getNumber()));
			addition.setSign(getSign());
		}
		else
		{
			addition.setNumber(subtract(b.getNumber(), getNumber()));
			addition.setSign(b.getSign());
		}
	}
	if (addition.getNumber() == "0") // avoid (-0) problem
		addition.setSign(false);

	return addition;
}

BigInteger BigInteger::operator - (BigInteger b)
{
	b.setSign(!b.getSign()); // x - y = x + (-y)
	return (*this) + b;
}

BigInteger BigInteger::operator * (BigInteger b)
{
	BigInteger mul;

	mul.setNumber(multiply(getNumber(), b.getNumber()));
	mul.setSign(getSign() != b.getSign());

	if (mul.getNumber() == "0") // avoid (-0) problem
		mul.setSign(false);

	return mul;
}

BigInteger BigInteger::operator / (BigInteger b)
{
	long long den = toInt(b.getNumber());
	BigInteger div;

	div.setNumber(divide(getNumber(), den).first);
	div.setSign(getSign() != b.getSign());

	if (div.getNumber() == "0") // avoid (-0) problem
		div.setSign(false);

	return div;
}

BigInteger BigInteger::operator % (BigInteger b)
{
	long long den = toInt(b.getNumber());

	BigInteger rem;
	long long rem_int = divide(number, den).second;
	rem.setNumber(toString(rem_int));
	rem.setSign(getSign() != b.getSign());

	if (rem.getNumber() == "0") // avoid (-0) problem
		rem.setSign(false);

	return rem;
}

BigInteger& BigInteger::operator += (BigInteger b)
{
	(*this) = (*this) + b;
	return (*this);
}

BigInteger& BigInteger::operator -= (BigInteger b)
{
	(*this) = (*this) - b;
	return (*this);
}

BigInteger& BigInteger::operator *= (BigInteger b)
{
	(*this) = (*this) * b;
	return (*this);
}

BigInteger& BigInteger::operator /= (BigInteger b)
{
	(*this) = (*this) / b;
	return (*this);
}

BigInteger& BigInteger::operator %= (BigInteger b)
{
	(*this) = (*this) % b;
	return (*this);
}

BigInteger& BigInteger::operator [] (int n)
{
	return *(this + (n * sizeof(BigInteger)));
}

BigInteger BigInteger::operator -() // unary minus sign
{
	return (*this) * -1;
}

BigInteger::operator string() // for conversion from BigInteger to string
{
	string signedString = (getSign()) ? "-" : ""; // if +ve, don't print + sign
	signedString += number;
	return signedString;
}

bool BigInteger:: equals(BigInteger n1, BigInteger n2) {
	return n1.getNumber() == n2.getNumber() && n1.getSign() == n2.getSign();
}

bool BigInteger::less(BigInteger n1, BigInteger n2) {
	bool sign1 = n1.getSign();
	bool sign2 = n2.getSign();

	if (!sign1 && sign2)
	{
		return false;
	}
	else if (sign1 && !sign2)
	{
		return true;
	}
	else if (!sign1)
	{
		if (n1.getNumber().length() < n2.getNumber().length())
		{
			return true;
		}
		if (n1.getNumber().length() > n2.getNumber().length())
		{
			return false;
		}
		return n1.getNumber() < n2.getNumber();
	}
	else
	{
		if (n1.getNumber().length() > n2.getNumber().length())
		{
			return true;
		}
		if (n1.getNumber().length() < n2.getNumber().length())
		{
			return false;
		}
		return n1.getNumber().compare(n2.getNumber()) > 0;
	}
}

bool BigInteger::greater(BigInteger n1, BigInteger n2) {
	return !equals(n1, n2) && !less(n1, n2);
}

string BigInteger::add(string n1, string n2) {
	string add = (n1.length() > n2.length()) ? n1 : n2;
	char ans = '0';
	int differenceInLength = abs((int)(n1.size() - n2.size()));

	if (n1.size() > n2.size())
	{
		n2.insert(0, differenceInLength, '0');
	}
	else
	{
		n1.insert(0, differenceInLength, '0');
	}
	for (int i = n1.size() - 1; i >= 0; --i) {
		add[i] = ((ans - '0') + (n1[i] - '0') + (n2[i] - '0')) + '0';
		if (i != 0)
		{
			if (add[i] > '9')
			{
				add[i] -= 10;
				ans = '1';
			}
			else
			{
				ans = '0';
			}
		}
	}
	if (add[0] > '9')
	{
		add[0] -= 10;
		add.insert(0, 1, '1');
	}
	return add;
}

string BigInteger::subtract(string n1, string n2) {
	string sub = (n1.length() > n2.length()) ? n1 : n2;
	int differenceInLength = abs((int)(n1.size() - n2.size()));
	if (n1.size() > n2.size())
		n2.insert(0, differenceInLength, '0');

	else
		n1.insert(0, differenceInLength, '0');

	for (int i = n1.length() - 1; i >= 0; --i)
	{
		if (n1[i] < n2[i])
		{
			n1[i] += 10;
			n1[i - 1]--;
		}
		sub[i] = ((n1[i] - '0') - (n2[i] - '0')) + '0';
	}

	while (sub[0] == '0' && sub.length() != 1) // erase leading zeros
		sub.erase(0, 1);

	return sub;
}

string BigInteger::multiply(string n1, string n2)
{
	if (n1.length() > n2.length())
		n1.swap(n2);

	string res = "0";
	for (int i = n1.length() - 1; i >= 0; --i)
	{
		string temp = n2;
		int currentDigit = n1[i] - '0';
		int carry = 0;

		for (int j = temp.length() - 1; j >= 0; --j)
		{
			temp[j] = ((temp[j] - '0') * currentDigit) + carry;

			if (temp[j] > 9)
			{
				carry = (temp[j] / 10);
				temp[j] -= (carry * 10);
			}
			else
				carry = 0;

			temp[j] += '0'; // back to string mood
		}

		if (carry > 0)
			temp.insert(0, 1, (carry + '0'));

		temp.append((n1.length() - i - 1), '0'); // as like mult by 10, 100, 1000, 10000 and so on

		res = add(res, temp); // O(n)
	}

	while (res[0] == '0' && res.length() != 1) // erase leading zeros
		res.erase(0, 1);

	return res;
}

pair<string, long long> BigInteger::divide(string n, long long den)
{
	long long rem = 0;
	string result; result.resize(MAX);

	for (int indx = 0, len = n.length(); indx < len; ++indx)
	{
		rem = (rem * 10) + (n[indx] - '0');
		result[indx] = rem / den + '0';
		rem %= den;
	}
	result.resize(n.length());

	while (result[0] == '0' && result.length() != 1)
		result.erase(0, 1);

	if (result.length() == 0)
		result = "0";

	return make_pair(result, rem);
}

string BigInteger::toString(long long n)
{
	stringstream ss;
	string temp;

	ss << n;
	ss >> temp;

	return temp;
}

long long BigInteger::toInt(string s)
{
	long long sum = 0;

	for (int i = 0; i < s.length(); i++)
		sum = (sum * 10) + (s[i] - '0');

	return sum;
}

#endif