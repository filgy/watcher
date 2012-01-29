#include "regexApi.h"

/*  - - - - - - - - - - - - - - - - - - - - - -
        Class: regexApiMatch
    ------------------------------------------- */
regexApiMatch::regexApiMatch(){
    this->matches.clear();
}

regexApiMatch::regexApiMatch(vector<string> matches){
    this->matches = matches;
}

int regexApiMatch::size(){
    return this->matches.size();
}

void regexApiMatch::clear(){
    this->matches.clear();
}

string regexApiMatch::operator[](unsigned int position){
    if(position >= this->matches.size())
        return "";
    else
        return this->matches.at(position);
}

void regexApiMatch::insert(string match){
    this->matches.push_back(match);
}


/*  - - - - - - - - - - - - - - - - - - - - - -
        Class: regexApi
    ------------------------------------------- */
bool regexApi::preg_match(string pattern, string subject){
    boost::xpressive::sregex bpattern = boost::xpressive::sregex::compile(pattern);
    boost::xpressive::smatch match;

    return boost::xpressive::regex_match(subject, match, bpattern);
}

bool regexApi::preg_match(string pattern, string subject, regexApiMatch& matches){
    boost::xpressive::sregex bpattern = boost::xpressive::sregex::compile(pattern);
    boost::xpressive::smatch match;

    matches.clear();

    if(boost::xpressive::regex_match(subject, match, bpattern)){
        for(boost::xpressive::smatch::iterator p = match.begin(); p != match.end(); p++)
            matches.insert((*p));

        return true;
    }

    return false;
}
