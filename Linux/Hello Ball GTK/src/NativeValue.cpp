/*
 * NativeValue.cpp
 *
 *  Created on: Feb 24, 2015
 *      Author: pepo
 */

#include <iostream>
#include <sstream>

#include "NativeValue.h"


NativeValue::NativeValue() {
	m_type = NativeValueUndefined;
	m_d = 0;
	m_b = false;
}

void NativeValue::setUndefinedValue()
{
	m_type = NativeValueUndefined;
}

void NativeValue::setNullValue()
{
	m_type = NativeValueNull;
}

void NativeValue::setBoolValue(bool value)
{
	m_type = NativeValueBoolean;
	m_b = value;
}

void NativeValue::setDoubleValue(double value)
{
	m_type = NativeValueNumber;
	m_d = value;
}

void NativeValue::setStringValue(const std::string &value)
{
	m_type = NativeValueString;
	m_s = value;
}

void NativeValue::setObjectValue()
{
	m_type = NativeValueObject;
	m_o.clear();
}

void NativeValue::setArrayValue()
{
	m_type = NativeValueArray;
	m_v.clear();
}

void NativeValue::addObjectEntry(const std::string &key, NativeValuePtr value)
{
	if (m_type == NativeValueObject) {
		m_o[key] = value;
	}
}

std::vector<std::string> NativeValue::objectKeys()
{
	std::vector<std::string> k;
	std::map<std::string, NativeValuePtr>::iterator i;
	for (i = m_o.begin(); i != m_o.end(); i++) {
		k.push_back(i->first);
	}
	return k;
}

NativeValuePtr NativeValue::objectEntry(const std::string &key)
{
	return m_o[key];
}

void NativeValue::pushArrayItem(NativeValuePtr value)
{
	if (m_type == NativeValueArray) {
		m_v.push_back(value);
	}
}

std::string NativeValue::toString()
{
	std::ostringstream s;
	dump(s, 0);
	return s.str();
}

void NativeValue::dump(std::ostringstream &output, size_t indentLevel)
{
	size_t i;
	std::string indentString;
	for (i = 0; i < indentLevel; i++) {
		indentString += "\t";
	}

	std::string typeString, valueString;
	if (m_type == NativeValueUndefined) {
		typeString = "undefined";
	} else if (m_type == NativeValueNull) {
		typeString = "NULL";
	} else if (m_type == NativeValueBoolean) {
		typeString = "Boolean";
		valueString = m_b ? "true" : "false";
	} else if (m_type == NativeValueNumber) {
		typeString = "Number";
		std::ostringstream s;
		s << m_d;
		valueString = s.str();
	} else if (m_type == NativeValueString) {
		typeString = "String";
		valueString = m_s;
	} else if (m_type == NativeValueArray) {
		typeString = "Array";
		std::ostringstream s;
		s << "[" << std::endl;
		size_t j, l = arrayLength();
		for (j = 0; j < l; j++) {
			arrayItem(j)->dump(s, indentLevel + 1);
		}
		s <<  indentString << "]";
		valueString = s.str();
	} else if (m_type == NativeValueObject) {
		typeString = "Object";
		std::ostringstream s;
		s << "{" << std::endl;
		std::vector<std::string> keys = objectKeys();
		size_t j;
		for (j = 0; j < keys.size(); j++) {
			objectEntry(keys[j])->dump(s, indentLevel + 1);
		}
		s << indentString << "}";
		valueString = s.str();
	}

	output << indentString << typeString << ": " << valueString << std::endl;
}
