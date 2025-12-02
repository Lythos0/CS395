#ifndef SET_H
#define SET_H

#include <iostream>
#include <string>
#include "Object.h"
#include <stdexcept>
#include <fstream>
#include <cctype>
#include <vector>

namespace tcp
{
    template <class T>
    class Set : public Object 
    {
        private:
        std::vector<T> data;

        public:
        Set() {}
        Set(const Set<T>& obj) {*this = obj;}
        
        Set<T>& operator=(const Set<T>& rhs)
        {
            if(this != &rhs) {data = rhs.data;}
            return *this;
        }

        virtual ~Set() {}

        size_t size() const {return data.size();}

        bool contains(const T& obj) 
        {
            size_t i = 0, n = data.size();

            while(i < n && data[i] != obj) {i += 1;}
            return i != n;
        }

        void insert(const T& obj)
        {
            size_t i = 0, n = data.size();

            while(i < n && data[i] != obj) {i += 1;}
            if(i == n) {data.push_back(obj);}
        }

        const T& operator[](size_t i) const  
        {
            if(i < data.size()) {return data[i];}
            throw std::out_of_range("out of bound");
        }

        std::string toString() const override 
        {
            std::stringstream out;

            out << "{";

            for(size_t i = 0,n = data.size();i < n;i += 1)
            {
                out << data[i];

                if(i + 1 < n) {out << ",";}
            }
            out << "}";
            return out.str();
        }
    };

    class Triple : public Object
    {
        private:
        std::string ends[2];
        char mid;

        public:
        Triple() : ends{"",""}, mid('\0') {}
        Triple(std::string a,char b,std::string c) : ends{a,c}, mid(b) {}
        Triple(const Triple& obj) {*this = obj;}

        Triple& operator=(const Triple& rhs)
        {
            if(this != &rhs)
            {
                ends[0] = rhs.ends[0];
                ends[1] = rhs.ends[1];
                mid = rhs.mid;
            }
            return *this;
        }

        const string& first() const {return ends[0];}

        const char& second() const {return mid;};

        const string& third() const {return ends[1];}

        virtual ~Triple() {}

        std::string toString() const override
        {
            std::stringstream out;
            out << "(" << ends[0] << "," << mid << "," << ends[1] << ")";
            return out.str(); 
        }

        friend bool operator==(const Triple& lhs,const Triple& rhs)
        {
            return lhs.ends[0] == rhs.ends[0] && lhs.mid == rhs.mid && lhs.ends[1] == rhs.ends[1]; 
        }

        friend bool operator!=(const Triple& lhs,const Triple& rhs)
        {
            return !(lhs == rhs);
        }
    };

    bool Valid(char ch)
    {
        return isalpha(ch) || isdigit(ch) || ch == '_';
    }

    bool FileReader(std::string& filename,Set<std::string>& Q,Set<char>& S,Set<Triple>& d,std::string& q0,Set<std::string>& F)
    {
        std::fstream fread(filename.data(),std::fstream::in);

        if(fread.is_open())
        {
            std::string line;
            std::getline(fread,line);
            std::string str;

            for(char i : line)
            {
                if(Valid(i) || i == ' ')
                {
                    if(Valid(i)) {str += i;}
                    else if(i == ' ' && !str.empty()) 
                    {
                        Q.insert(str);
                        str = "";
                    }
                }
                else {return false;}
            }
            if(!str.empty()) {Q.insert(str);}

            std::getline(fread,line);

            for(char i : line) 
            {
                if(islower(i) || isdigit(i)) {S.insert(i);}
                else {return false;}
            }
            std::getline(fread,line);

            if(Q.contains(line)) {q0 = line;}
            else {return false;}
            std::getline(fread,line);
            str = "";

            for(char i : line)
            {
                if(Valid(i) || i == ' ')
                {
                    if(Valid(i)) {str += i;}
                    else if(i == ' ' && !str.empty()) 
                    {
                        if(Q.contains(str))
                        {
                            F.insert(str);
                            str = "";
                        }
                        else {return false;}
                    }
                }
                else {return false;}
            }
            if(!str.empty() && Q.contains(str)) {F.insert(str);}
            else {return false;}

            size_t f, l; 
            std::string x, y, z;

            while(std::getline(fread,line))
            {
                std::cout << line << "\n";
                f = line.find_first_of(" ");
                l = line.find_last_of(" ");
                
                if(f == std::string::npos || l == std::string::npos)
                {
                    return false;
                }
                else 
                {
                    x = line.substr(0,f);
                    y = line.substr(f+1,l-f-1);
                    z = line.substr(l+1);

                    if(!Q.contains(x) || !Q.contains(z)) {return false;}
                    
                    for(char i : y)
                    {
                        if(S.contains(i)) {d.insert(Triple(x,i,z));}
                        else {return false;}
                    }
                }
            }
        }
        fread.close();
        return Q.size() * S.size() == d.size();
    }
} 

#endif