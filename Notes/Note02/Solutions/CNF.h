#ifndef CNF_H
#define CNF_H

#include "Utility.h"

namespace tc
{
    bool NewStart(Set<Rule>& R,std::string& available)
    {
        if(available.empty()) {return false;}
        else if(R.size() == 0) {return true;}
        char a = available[0];
        std::string b = std::string(1,R[0].dom());
        available = available.substr(1); 
        Rule c(a,b);
        R.first(c);
        return true;
    }

    Set<Rule> transitions(const Rule& a,char A)
    {
        std::string b = a.sub();
        Set<std::string> c;
        c.insert(b);
        size_t d, i, n;
        std::string e, f;
        Set<Rule> g;
        Rule h = a;

        while(true)
        {
            for(i = 0,n = c.size();i < n;i += 1)
            {   
                e = c[i];
                d = e.find(A);

                while(d != std::string::npos)
                {
                    f = e.substr(0,d) + e.substr(d+1);
                    
                    if(f.empty()) {c.insert(".");}
                    else {c.insert(f);}
                    d = e.find_first_of(A,d+1);
                }
            }
            if(n == c.size()) {break;}
        }
        
        for(i = 1,n = c.size();i < n;i += 1)
        {
            h.sub(c[i]);

            if(!h.self()) {g.insert(h);}
        }
        return g;
    }

    //union of sets
    template <typename T>
    void Union(Set<T>& lhs,Set<T>& rhs)
    {
        for(size_t i = 0,n = rhs.size();i < n;i += 1)
        {
            lhs.insert(rhs[i]);
        }
    }

    //difference of sets
    template <typename T>
    void Difference(Set<T>& lhs,Set<T>& rhs)
    {
        for(size_t i = 0,n = rhs.size();i < n;i += 1)
        {
            lhs.remove(rhs[i]);
        }
    }

    bool RemoveEpsilons(Set<Rule>& R,std::string& available)
    {
        if(R.size() == 0) {return true;}
        Set<char> Q;
        Set<Rule> P, O, N;
        char S = R[0].dom();
        size_t i, n, j, m;

        while(true)
        {
            //empty helper sets
            Q = Set<char>();
            P = Set<Rule>();
            N = Set<Rule>();

            //find and collect variables with epsilon-rules and the rules
            for(i = 0,n = R.size();i < n;i += 1)
            {
                if(R[i].dom() != S && R[i].sub() == ".")
                {
                    Q.insert(R[i].dom());
                    N.insert(R[i]);
                }
            }
            //terminate loop if no epsilon-rules found 
            if(Q.size() == 0) {break;}
            //remove epsilon-rules from R 
            Difference(R,N);

            //create new rules formed from epsilon-rule derivations
            for(j = 0,m = Q.size();j < m;j += 1)
            {
                for(i = 0,n = R.size();i < n;i += 1)
                {
                    if(R[i].sub().find(Q[j]) != std::string::npos)
                    {
                        O = transitions(R[i],Q[j]);
                        Union(P,O);
                    }
                }
            }
            //add new rules to R
            Union(R,P);
        }
        return true;
    }

    bool RemoveUnits(Set<Rule>& R,std::string& available)
    {
        if(R.size() == 0) {return true;}
        Set<char> Q;
        Set<std::string> P;
        Set<Rule> O, N;
        size_t i, n, j, m, k, o;

        //remove start variable unit rule and add its new rules
        Rule S = R[0];
        char V = R[0].sub()[0];
        R.remove(R[0]);

        for(i = 0,n = R.size();i < n;i += 1)
        {
            if(R[i].dom() == V) 
            {
                S.sub(R[i].sub());
                O.insert(S);
            }
        }

        for(i = 0, n = O.size();i < n;i += 1)
        {
            R.first(O[i]);
        }

        //remove all unit rules
        while(true)
        {
            //empty helper sets
            Q = Set<char>();
            N = Set<Rule>();
            O = Set<Rule>();

            //find and collect unit rules
            for(i = 0,n = R.size();i < n;i += 1)
            {
                if(R[i].sub().size() == 1 && isupper(R[i].sub()[0]))
                {
                    Q.insert(R[i].sub()[0]);
                    N.insert(R[i]);
                }
            }
            //terminate loop if no unit rules found 
            if(Q.size() == 0) {break;}

            //create new rules  
            for(j = 0,m = Q.size();j < m;j += 1)
            {
                P = Set<std::string>();
                //collect substitutions for unit rule substitution
                for(i = 0,n = R.size();i < n;i += 1)
                {
                    if(R[i].dom() == Q[j]) {P.insert(R[i].sub());}
                }
                //add new rules to R
                for(k = 0, o = N.size();k < o;k += 1)
                {
                    if(N[k].sub()[0] == Q[j])
                    {
                        S.dom(N[k].dom());
                        for(i = 0, n = P.size();i < n;i += 1)
                        {
                            S.sub(P[i]);
                            if(!S.self()) {O.insert(S);}
                        }
                    }
                }
            }
            //remove unit rules from R 
            Difference(R,N);
            //add new rules to R
            Union(R,O);
        }
        return true;
    }

    std::string replace(std::string str,char src,char des)
    {
        std::string cp = str;
        for(char& i : cp) {if(i == src) {i = des;}}
        return cp;
    }

    Set<char> terminals(std::string str)
    {
        Set<char> a;
        for(char i : str) {if(!isupper(i)) {a.insert(i);}}
        return a;
    }

    bool contains(std::string str,char ch)
    {
        for(char i : str) {if(i == ch) {return true;}}
        return false;
    }

    bool ConstructLexicals(Set<Rule>& R,std::string& available)
    {
        if(R.size() == 0) {return true;}
        Set<char> Q, P;
        Set<Rule> O, N;
        size_t i, n, j, m;
        char A;
        Rule B;

        //find and collect all terminals that are in substitutions with lengths 
        //greater than 1
        for(i = 0,n = R.size();i < n;i += 1)
        {
            if(R[i].sub().size() > 1) 
            {
                P = terminals(R[i].sub());
                Union(Q,P);
            }
        }
        //create new rule for each terminal found in previous loop
        for(i = 0,n = Q.size();i < n;i += 1)
        {
            if(available.empty()) {return false;}
            A = available[0];
            available = available.substr(1);
            B.dom(A);
            B.sub(std::string(1,Q[i]));
            O.insert(B);
        }
        //modify the substitutions for each rule in R that applies
        for(i = 0,n = R.size();i < n;i += 1)
        {
            if(R[i].sub().size() <= 1) {N.insert(R[i]);}
            else  
            { 
                B = R[i];
                for(j = 0,m = O.size();j < m;j += 1)
                {
                    if(contains(B.sub(),O[j].sub()[0])) 
                    {
                        B.sub(replace(B.sub(),O[j].sub()[0],O[j].dom()));
                    }
                }
                N.insert(B);
            }
        }
        //add new rules to R
        R = N;
        Union(R,O);
        return true;
    }
}

#endif