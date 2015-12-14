#include <string>
#include <list>
#include "symbtable.h"

using namespace std;

Symbol::Symbol()
	: nam(), typ(TY_BAD)
{}

Symbol::Symbol(const string &name, type_t type, int address, type_t returnval, int args, type_t atype)
	: nam(name), typ(type), addr(address), rtype(returnval), argnum(args), argtype(atype)
{}

const string &Symbol::name() const
{
	return nam;
}

type_t &Symbol::type()
{
	return typ;
}

type_t Symbol::type() const
{
	return typ;
}

int &Symbol::address()
{
	return addr;
}

int Symbol::address() const
{
	return addr;
}

int &Symbol::argn()
{
	return argnum;
}

int Symbol::argn() const
{
	return argnum;
}

type_t &Symbol::return_type()
{
	return rtype;
}

type_t Symbol::return_type() const
{
	return rtype;
}

type_t &Symbol::arg_type()
{
        return argtype;
}

type_t Symbol::arg_type() const
{
        return argtype;
}

bool Symbol::operator<(const Symbol &s) const
{
	return nam < s.nam;
}

bool Symbol::operator==(const Symbol &s) const
{
	return nam == s.nam;
}

bool Symbol::operator!=(const Symbol &s) const
{
	return nam != s.nam;
}

SymbolTable::SymbolTable()
	: hashes(1) // one (empty) hash table
{}

SymbolTable::SymbolTable(const SymbolTable &st)
	: hashes(st.hashes)
{}

SymbolTable &SymbolTable::operator=(const SymbolTable &st)
{
	if(this != &st)
		hashes = st.hashes;

	return *this;
}

Symbol *SymbolTable::operator[](const string &s) 
{
	for(symtabint::iterator i=hashes.begin(); i != hashes.end(); ++i) {
		symtabsingle::iterator pos = i->find(s);
		if (pos != i->end())
			return &(pos->second);
	}
	
	return 0;
}

const Symbol *SymbolTable::operator[](const string &s) const
{
	for(symtabint::const_iterator i=hashes.begin(); i != hashes.end(); ++i) {
		symtabsingle::const_iterator pos = i->find(s);
		if (pos != i->end())
			return &(pos->second);
	}
	
	return 0;
}

bool SymbolTable::insert(Symbol s) 
{
	hashes.front().insert(make_pair(s.name(), s));
	return true;
}

void SymbolTable::enter() 
{
	hashes.push_front(symtabsingle());
}

void SymbolTable::leave() 
{
	// We should not pop off the first element
	if(hashes.size() > 1)
		hashes.pop_front();
}

bool SymbolTable::exists(const string &str) const 
{
	for(symtabint::const_iterator i=hashes.begin(); i!=hashes.end(); ++i)
		if(i->count(str))
			return true;

	return false;
}

bool is_data_type(type_t ty)
{
	return ty == TY_INT || ty == TY_DOUBLE;
}

int SymbolTable::numvars() const
{
	return hashes.front().size();
}

int SymbolTable::levelof(const string &str) const 
{
	symtabint::const_iterator i;
	int j = 0;
	for(i=hashes.begin(); i != hashes.end(); ++i, ++j)
		if(i->count(str))
			return j;

	return -1;
}

const char* type_name(type_t ty)
{
	switch (ty) {
	case TY_BAD: return "ERROR";
	case TY_INT: return "int";
	case TY_DOUBLE: return "double";
	case TY_FUNC: return "function";
	default: return "ERROR?";
	}
}

void SymbolTable::dump(std::ostream &out) const
{
	int j = 0;
	symtabint::const_reverse_iterator i;
	for(i=hashes.rbegin(); i != hashes.rend(); ++i, ++j)
	{
		out << "level " << j << ": [ ";
		symtabsingle::const_iterator j;
		for (j=i->begin(); j != i->end(); ++j)
		{
			out << j->second.name() << ":"
			    << type_name(j->second.type()) << " ";
		}
		out << "]" << endl;
	}
}
