#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>

namespace tc
{
	class Object 
	{
		private:
		virtual std::string toString() const = 0;

		public:
		virtual ~Object() {}

		friend std::ostream& operator<<(std::ostream& out,const Object& obj)
		{
			out << obj.toString();
			return out;
		}
	};

	template <class T>
	class Set : public Object 
	{
		private:
		std::vector<T> data;

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

		public:
		Set() {}
		
		Set(const Set<T>& obj) {*this=obj;}
		
		~Set() {}

		Set<T>& operator=(const Set<T>& rhs) 
		{
			if(this != &rhs) {data = rhs.data;}
			return *this;
		}

		//retrieves the size of the set
		size_t size() const {return data.size();}

		//inserts an object into the set 
		void insert(const T& obj) 
		{
			size_t i = 0, n = data.size();

			while(i < n && data[i] != obj) {i += 1;}
		        if(i == n) {data.push_back(obj);}	
		}

		//inserts an object at the beginning of the set
		void first(const T& obj)
		{
			size_t i = 0, n = data.size();

			while(i < n && data[i] != obj) {i += 1;}
			if(i == n) {data.insert(data.begin(),obj);}
		}

		//removes an object from the set
		void remove(const T& obj)
		{
			size_t i = 0, n = data.size();

			while(i < n && data[i] != obj) {i += 1;}
			if(i != n) {data.erase(data.begin()+i);}
		}

		//accesses an element of the set by its index in the set
		const T& operator[](size_t idx) const 
		{
			size_t n = data.size();
			if(idx < n) {return data[idx];}
			throw std::out_of_range("out of bound");
		}
	};

	class Rule : public Object
	{
		private:
		char key;
		std::string value;

		std::string toString() const override
		{
			std::stringstream out;
			
			out << key << "->" << value;
			return out.str();
		}

		bool valid(char i) const {return isalnum(i) || i == '.';}

		public:
		Rule() : key('A'), value(".") {}

		Rule(char key) : Rule(key,".") {}
		
		Rule(char key,std::string value) : Rule()
		{
			dom(key);
			sub(value);
		}

		Rule(const Rule& obj) {*this = obj;}

		Rule& operator=(const Rule& rhs) 
		{
			if(this != &rhs)
			{
				key = rhs.key;
				value = rhs.value;
			}
			return *this;
		}

		virtual ~Rule() {}

		char dom() const {return key;}

		void dom(char obj)
		{
			if(isupper(obj)) {key = obj;}
		}

		std::string sub() const {return value;}

		void sub(std::string obj)
		{
			if(obj.size() == 1 && valid(obj[0]))
			{
				value = obj;
			}
			else 
			{
				std::string str;

				for(char i : obj)
				{
					if(isalnum(i)) {str += i;}
				        else if(i != '.') {return;}
				}
				value = str;	
			}
		}

		bool self() const 
		{
			return value.size() == 1 && value[0] == key;
		}

		friend bool operator==(const Rule& lhs,const Rule& rhs)
		{
			return lhs.key == rhs.key && lhs.value == rhs.value;
		}

		friend bool operator!=(const Rule& lhs,const Rule& rhs)
		{
			return !(lhs == rhs);
		}	
	};
}

#endif
