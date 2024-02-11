#include "HashSuperClass.h"
HashSuperClass::HashSuperClass(){}
HashSuperClass::HashSuperClass(string s):text(s){}

void HashSuperClass::update(string s) {
	text += s;
}