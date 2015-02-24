/*
 * NativeValue.cpp
 *
 *  Created on: Feb 24, 2015
 *      Author: pepo
 */

#include "NativeValue.h"

NativeValue::NativeValue() {
	m_type = NativeValueNull;
	m_d = 0;
	m_b = false;
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

void NativeValue::pushArrayItem(NativeValuePtr value)
{
	if (m_type == NativeValueArray) {
		m_v.push_back(value);
	}
}
