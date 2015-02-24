/*
 * NativeValue.h
 *
 *  Created on: Feb 24, 2015
 *      Author: pepo
 */

#ifndef NATIVEVALUE_H_
#define NATIVEVALUE_H_

#include <memory>
#include <map>
#include <vector>


typedef enum {
	NativeValueNull = 0,
	NativeValueBoolean = 1,
	NativeValueNumber = 2,
	NativeValueString = 3,
	NativeValueArray = 4,
	NativeValueObject = 5
} NativeValueType;


class NativeValue;
typedef std::shared_ptr<NativeValue> NativeValuePtr;

class NativeValue {
public:

	NativeValue();

	NativeValueType type() { return m_type; }

	void setNullValue();

	void setBoolValue(bool value);
	bool boolValue() { return m_b; }

	void setDoubleValue(double value);
	double doubleValue() { return  m_d; }

	void setStringValue(const std::string &value);
	const std::string & stringValue() { return m_s; }

	void setObjectValue();
	void addObjectEntry(const std::string &key, NativeValuePtr value);
	const std::map<std::string, NativeValuePtr> & objectValue() { return m_o; }
	NativeValuePtr objectEntry(const std::string &key);

	void setArrayValue();
	void pushArrayItem(NativeValuePtr value);
	const std::vector<NativeValuePtr> & arrayValue() { return m_v; }
	size_t arrayLength() { return m_v.size(); }
	NativeValuePtr arrayItem(size_t index) { return m_v[index]; }

	void dump();

protected:
	NativeValueType m_type;

	bool m_b;
	double m_d;
	std::string m_s;
	std::map<std::string, NativeValuePtr> m_o;
	std::vector<NativeValuePtr> m_v;

	void dump(std::ostringstream &output, size_t indentLevel);
};

#endif /* NATIVEVALUE_H_ */
