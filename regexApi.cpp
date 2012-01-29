#include "regexApi.h"

regexApiMatch::regexApiMatch(){
    this->matches.clear();
}

regexApiMatch::regexApiMatch(vector<string> matches){
    this->matches = matches;
}

int regexApiMatch::size(){
    return this->matches.size();
}

string regexApiMatch::itemAt(int position){
    if(position > this->matches.size())
        return "";
    else
        return this->matches.at(position);
}

string regexApiMatch::operator[](int position){
    return this->itemAt(position);
}

void regexApiMatch::clear(){
    this->matches.clear();
}

void regexApiMatch::itemAdd(string match){
    this->matches.push_back(match);
}

bool regexApi::preg_match(string pattern, string subject){
    boost::xpressive::sregex bpattern = boost::xpressive::sregex::compile(pattern);
    boost::xpressive::smatch match;

    return boost::xpressive::regex_match(subject, match, bpattern);
}

bool regexApi::preg_match(string pattern, string subject, regexApiMatch& matches){
    matches.clear();

    boost::xpressive::sregex bpattern = boost::xpressive::sregex::compile(pattern);
    boost::xpressive::smatch match;

    if(boost::xpressive::regex_match(subject, match, bpattern)){
        for(boost::xpressive::smatch::iterator p = match.begin(); p != match.end(); p++)
            matches.itemAdd((*p));

        return true;
    }
    else
        return false;
}
