#include "gogoote/model/Assertion.h"
#include <assert.h>
#include <map>
#include <regex>
#include <sstream>


namespace gogoote {
namespace model {


// TODO(KNR): prevent CPPUNIT_FAIL_HAHA from matching, consider to generate regex from
// assertion_lookup_table keys
const std::regex cppunit_assertion_pattern{"^(CPPUNIT_(ASSERT|FAIL)\\w*)\\s*[(](.*)[)]$"};
const std::regex message_pattern{"_MESSAGE$"};
const char* ws = " \t\n\r\f\v";

const std::map<std::string, std::string> assertion_lookup_table{
  {"CPPUNIT_ASSERT", "ASSERT_TRUE"},
  {"CPPUNIT_ASSERT_MESSAGE", "ASSERT_TRUE"},
  {"CPPUNIT_ASSERT_DOUBLES_EQUAL", "ASSERT_NEAR"},
  {"CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE", "ASSERT_NEAR"},
  {"CPPUNIT_ASSERT_EQUAL", "ASSERT_EQ"},
  {"CPPUNIT_ASSERT_EQUAL_MESSAGE", "ASSERT_EQ"},
  {"CPPUNIT_FAIL", "FAIL"}
};


// modified version of https://stackoverflow.com/a/25385766/1776679, which
// takes a const reference and returns by copy
inline std::string ltrim(const std::string& str, const char* t = ws) {
  auto s{str};
  s.erase(0, s.find_first_not_of(t));
  return s;
}


std::pair<std::string, std::string> tail(const std::string& parameters) {
  const auto position{parameters.rfind(',')};
  assert(position != std::string::npos);
  // if (position == std::string::npos) {
  //   return std::make_pair("", parameters);
  // }
  return std::make_pair(parameters.substr(0, position), ltrim(parameters.substr(position + 1)));
}


Assertion::Assertion(const std::string& assertion) {
  std::smatch match;
  const bool ok{std::regex_search(assertion, match, cppunit_assertion_pattern)};
  assert(ok && match.size() == 4);
  const std::string cppunit_assertion = match[1];
  const auto lookup = assertion_lookup_table.find(cppunit_assertion);
  assert(lookup != assertion_lookup_table.end());
  assertion_ = lookup->second;
  parameters_ = match[3];
  if (std::regex_search(cppunit_assertion, message_pattern)) {
    const auto list_and_tail = tail(parameters_);
    parameters_ = list_and_tail.first;
    message_ = list_and_tail.second;
  } else if (cppunit_assertion == "CPPUNIT_FAIL") {
    message_ = parameters_;
    parameters_ = "";
  }
}


std::ostream& operator<<(std::ostream& os, const Assertion& value) {
  os << value.assertion_ << "(" << value.parameters_ << ")";
  if (value.message_.size() > 0) {
    os << " << " << value.message_;
  }
  os << ";";
  return os;
}

std::string to_string(const Assertion &value) {
  std::ostringstream ss;
  ss << value;
  return ss.str();
}

}  // namespace model
}  // namespace gogoote
