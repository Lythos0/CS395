#include "Set.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace tcp
{
    bool Valid(char ch)
    {
        return isalpha(ch) || isdigit(ch) || ch == '_';
    }

    bool FileReader(std::string& filename, Set<std::string>& Q, Set<char>& S, Set<Triple>& d, std::string& q0, Set<std::string>& F)
    {
        std::fstream fread(filename.data(), std::fstream::in);

        if (fread.is_open())
        {
            std::string line;
            std::getline(fread, line);
            std::string str;

            for (char i : line)
            {
                if (Valid(i) || i == ' ')
                {
                    if (Valid(i)) { str += i; }
                    else if (i == ' ' && !str.empty())
                    {
                        Q.insert(str);
                        str = "";
                    }
                }
                else { return false; }
            }
            if (!str.empty()) { Q.insert(str); }

            std::getline(fread, line);

            for (char i : line)
            {
                if (islower(i) || isdigit(i)) { S.insert(i); }
                else { return false; }
            }
            std::getline(fread, line);

            if (Q.contains(line)) { q0 = line; }
            else { return false; }
            std::getline(fread, line);
            str = "";

            for (char i : line)
            {
                if (Valid(i) || i == ' ')
                {
                    if (Valid(i)) { str += i; }
                    else if (i == ' ' && !str.empty())
                    {
                        if (Q.contains(str))
                        {
                            F.insert(str);
                            str = "";
                        }
                        else { return false; }
                    }
                }
                else { return false; }
            }
            if (!str.empty() && Q.contains(str)) { F.insert(str); }
            else { return false; }

            size_t f, l;
            std::string x, y, z;

            while (std::getline(fread, line))
            {
                std::cout << line << "\n";
                f = line.find_first_of(" ");
                l = line.find_last_of(" ");

                if (f == std::string::npos || l == std::string::npos)
                {
                    return false;
                }
                else
                {
                    x = line.substr(0, f);
                    y = line.substr(f + 1, l - f - 1);
                    z = line.substr(l + 1);

                    if (!Q.contains(x) || !Q.contains(z)) { return false; }

                    for (char i : y)
                    {
                        if (S.contains(i)) { d.insert(Triple(x, i, z)); }
                        else { return false; }
                    }
                }
            }
        }
        fread.close();
        return Q.size() * S.size() == d.size();
    }
}
