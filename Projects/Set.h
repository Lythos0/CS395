#ifndef SET_H
#define SET_H

#include <iostream>
#include <string>
#include "Object.h"
#include <stdexcept>
#include <fstream>
#include <cctype>

namespace tcp
{
    template <class T>
    class Set : public Object
    {
        private:
        struct Node {
            T data;
            Node* next;
            Node(T d, Node* n = nullptr) : data(d), next(n) {}
        };
        Node* head;
        size_t sz;

        public:
        Set() : head(nullptr), sz(0) {}
        Set(const Set<T>& obj) : head(nullptr), sz(0) {*this = obj;}

        Set<T>& operator=(const Set<T>& rhs)
        {
            if(this != &rhs) {
                // Clear current list
                Node* current = head;
                while(current) {
                    Node* temp = current;
                    current = current->next;
                    delete temp;
                }
                head = nullptr;
                sz = 0;
                // Copy rhs list
                Node* rhsCurrent = rhs.head;
                Node** currentPtr = &head;
                while(rhsCurrent) {
                    *currentPtr = new Node(rhsCurrent->data);
                    currentPtr = &((*currentPtr)->next);
                    rhsCurrent = rhsCurrent->next;
                }
                sz = rhs.sz;
            }
            return *this;
        }

        virtual ~Set() {
            Node* current = head;
            while(current) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }

        size_t size() const {return sz;}

        bool contains(const T& obj) const
        {
            Node* current = head;
            while(current) {
                if(current->data == obj) return true;
                current = current->next;
            }
            return false;
        }

        void insert(const T& obj)
        {
            if(!contains(obj)) {
                head = new Node(obj, head);
                sz++;
            }
        }

        const T& operator[](size_t i) const
        {
            if(i >= sz) throw std::out_of_range("out of bound");
            Node* current = head;
            for(size_t j = 0; j < i; j++) {
                current = current->next;
            }
            return current->data;
        }

        std::string toString() const override
        {
            std::stringstream out;
            out << "{";
            Node* current = head;
            while(current) {
                out << current->data;
                if(current->next) out << ",";
                current = current->next;
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

        const std::string& first() const {return ends[0];}

        const char& second() const {return mid;};

        const std::string& third() const {return ends[1];}

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

    bool Valid(char ch);
    bool FileReader(std::string& filename, Set<std::string>& Q, Set<char>& S, Set<Triple>& d, std::string& q0, Set<std::string>& F);
} 

#endif