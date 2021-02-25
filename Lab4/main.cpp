#include <iostream>
#include <vector>

#include "RNAlgorithm.hpp"
#include "RNPredicate.hpp"
#include "CPoint.hpp"

int main() {
    std::vector<int> a = {3, 3, 3, 1};
    std::vector<int> b = {3, 3, 1, 3};
    std::vector<int> c = {3, 1, 3, 3};
    std::vector<int> d = {3, 3, 4, 3, 3};
    std::vector<int> e = {3, 3, 4, 2, 3};
    std::vector<int> f = {3, 2, 4, 3, 3};
    std::vector<int> g = {3, 3, 3, 3, 3};
    std::vector<int> h = {3, 3, 4, 5, 5};

    std::vector<int> inc = {1, 2, 3, 4};
    std::vector<int> dec = {4, 3, 2, 1};

    std::string s = "aaa";
    std::string s1 = "aab";
    std::string s2 = "abc";

    std::vector<CPoint> p{{1, 0},
                          {0, 1},
                          {1, 1}};
    std::vector<CPoint> p1{{1, 1},
                           {2, 1},
                           {3, 1}};

    /** all_of **/
    {
        std::cout << "all_of\n";
        std::cout << "\tint\n\t\t";
        std::cout << RNAlgorithm::all_of<int>(a.begin(), a.end(), RNPredicate::MoreThan(0)) << " – 1\n\t\t";
        std::cout << RNAlgorithm::all_of<int>(a.begin(), a.end(), RNPredicate::MoreThan(1)) << " – 0\n\t\t";
        std::cout << RNAlgorithm::all_of<int>(a.begin(), a.end(), RNPredicate::EqualTo(3)) << " – 0\n\t\t";
        std::cout << RNAlgorithm::all_of<int>(g.begin(), g.end(), RNPredicate::EqualTo(3)) << " – 1\n\n";

        std::cout << "\tstd::string\n\t\t";
        std::cout << RNAlgorithm::all_of<std::string>(s.begin(), s.end(), RNPredicate::EqualTo('a')) << " – 1\n\t\t";
        std::cout << RNAlgorithm::all_of<std::string>(s1.begin(), s1.end(), RNPredicate::EqualTo('a')) << " – 0\n\n";

        std::cout << "\tCPoint\n\t\t";
        std::cout << RNAlgorithm::all_of<CPoint>(p.begin(), p.end(), RNPredicate::MoreThan(CPoint(1, 0)))
                  << " – 0\n\t\t";
        std::cout << RNAlgorithm::all_of<CPoint>(p1.begin(), p1.end(), RNPredicate::MoreThan(CPoint(1, 0)))
                  << " – 1\n\n\n";
    }

    /** any_of **/
    {
        std::cout << "any_of\n";
        std::cout << "\tint\n\t\t";
        std::cout << RNAlgorithm::any_of<int>(a.begin(), a.end(), RNPredicate::LessThan(2)) << " – 1\n\t\t";
        std::cout << RNAlgorithm::any_of<int>(a.begin(), a.end(), RNPredicate::MoreThan(4)) << " – 0\n\t\t";
        std::cout << RNAlgorithm::any_of<int>(a.begin(), a.end(), RNPredicate::EqualTo(2)) << " – 0\n\t\t";
        std::cout << RNAlgorithm::any_of<int>(g.begin(), g.end(), RNPredicate::EqualTo(3)) << " – 1\n\n";

        std::cout << "\tstd::string\n\t\t";
        std::cout << RNAlgorithm::any_of<std::string>(s.begin(), s.end(), RNPredicate::EqualTo('b')) << " – 0\n\t\t";
        std::cout << RNAlgorithm::any_of<std::string>(s1.begin(), s1.end(), RNPredicate::EqualTo('b')) << " – 1\n\n";

        std::cout << "\tCPoint\n\t\t";
        std::cout << RNAlgorithm::any_of<CPoint>(p.begin(), p.end(), RNPredicate::MoreThan(CPoint(1, 0)))
                  << " – 1\n\t\t";
        std::cout << RNAlgorithm::any_of<CPoint>(p1.begin(), p1.end(), RNPredicate::LessThan(CPoint(1, 0)))
                  << " – 0\n\n\n";
    }

    /** none_of **/
    {
        std::cout << "none_of\n";
        std::cout << "\tint\n\t\t";
        std::cout << RNAlgorithm::none_of<int>(a.begin(), a.end(), RNPredicate::MoreThan(4)) << " – 1\n\t\t";
        std::cout << RNAlgorithm::none_of<int>(a.begin(), a.end(), RNPredicate::LessThan(2)) << " – 0\n\t\t";
        std::cout << RNAlgorithm::none_of<int>(a.begin(), a.end(), RNPredicate::EqualTo(1)) << " – 0\n\t\t";
        std::cout << RNAlgorithm::none_of<int>(g.begin(), g.end(), RNPredicate::EqualTo(2)) << " – 1\n\n";

        std::cout << "\tstd::string\n\t\t";
        std::cout << RNAlgorithm::none_of<std::string>(s.begin(), s.end(), RNPredicate::EqualTo('b')) << " – 1\n\t\t";
        std::cout << RNAlgorithm::none_of<std::string>(s1.begin(), s1.end(), RNPredicate::EqualTo('b')) << " – 0\n\n";

        std::cout << "\tCPoint\n\t\t";
        std::cout << RNAlgorithm::none_of<CPoint>(p.begin(), p.end(), RNPredicate::EqualTo(CPoint(1, 0)))
                  << " – 0\n\t\t";
        std::cout << RNAlgorithm::none_of<CPoint>(p1.begin(), p1.end(), RNPredicate::LessThan(CPoint(0, 0)))
                  << " – 1\n\n\n";
    }

    /** one_of **/
    {
        std::cout << "one_of\n";
        std::cout << "\tint\n\t\t";
        std::cout << RNAlgorithm::one_of<int>(a.begin(), a.end(), RNPredicate::LessThan(2)) << " – 1\n\t\t";
        std::cout << RNAlgorithm::one_of<int>(a.begin(), a.end(), RNPredicate::MoreThan(2)) << " – 0\n\t\t";
        std::cout << RNAlgorithm::one_of<int>(a.begin(), a.end(), RNPredicate::EqualTo(3)) << " – 0\n\t\t";
        std::cout << RNAlgorithm::one_of<int>(g.begin(), g.end(), RNPredicate::EqualTo(3)) << " – 1\n\n";

        std::cout << "\tstd::string\n\t\t";
        std::cout << RNAlgorithm::one_of<std::string>(s.begin(), s.end(), RNPredicate::EqualTo('b')) << " – 0\n\t\t";
        std::cout << RNAlgorithm::one_of<std::string>(s1.begin(), s1.end(), RNPredicate::EqualTo('b')) << " – 1\n\n";

        std::cout << "\tCPoint\n\t\t";
        std::cout << RNAlgorithm::one_of<CPoint>(p.begin(), p.end(), RNPredicate::EqualTo(CPoint(1, 0)))
                  << " – 1\n\t\t";
        std::cout << RNAlgorithm::one_of<CPoint>(p1.begin(), p1.end(), RNPredicate::LessThan(CPoint(0, 0)))
                  << " – 0\n\n\n";
    }

    /** is_sorted **/
    {
        std::cout << "is_sorted\n";
        std::cout << "\tint\n\t\t";
        std::cout << RNAlgorithm::is_sorted<int>(inc.begin(), inc.end(), RNPredicate::Increasing<int>())
                  << " – 1\n\t\t";
        std::cout << RNAlgorithm::is_sorted<int>(dec.begin(), dec.end(), RNPredicate::Decreasing<int>())
                  << " – 1\n\t\t";
        std::cout << RNAlgorithm::is_sorted<int>(a.begin(), a.end(), RNPredicate::Increasing<int>()) << " – 0\n\t\t";
        std::cout << RNAlgorithm::is_sorted<int>(g.begin(), g.end(), RNPredicate::Decreasing<int>()) << " – 1\n\n";

        std::cout << "\tstd::string\n\t\t";
        std::cout << RNAlgorithm::is_sorted<std::string>(s.begin(), s.end(), RNPredicate::Increasing<char>())
                  << " – 1\n\t\t";
        std::cout << RNAlgorithm::is_sorted<std::string>(s1.begin(), s1.end(), RNPredicate::Decreasing<char>())
                  << " – 0\n\n";

        std::cout << "\tCPoint\n\t\t";
        std::cout << RNAlgorithm::is_sorted<CPoint>(p.begin(), p.end(), RNPredicate::Decreasing<CPoint>())
                  << " – 0\n\t\t";
        std::cout << RNAlgorithm::is_sorted<CPoint>(p1.begin(), p1.end(), RNPredicate::Increasing<CPoint>())
                  << " – 1\n\n\n";
    }

    /** is_partitioned **/
    {
        std::cout << "is_partitioned\n";
        std::cout << "\tint\n\t\t";
        std::cout << RNAlgorithm::is_partitioned<int>(h.begin(), h.end(), RNPredicate::MoreThan(4)) << " – 1\n\t\t";
        std::cout << RNAlgorithm::is_partitioned<int>(g.begin(), g.end(), RNPredicate::MoreThan(2)) << " – 0\n\n";

        std::cout << "\tstd::string\n\t\t";
        std::cout << RNAlgorithm::is_partitioned<std::string>(s2.begin(), s2.end(), RNPredicate::MoreThan('b'))
                  << " – 1\n\t\t";
        std::cout << RNAlgorithm::is_partitioned<std::string>(s.begin(), s.end(), RNPredicate::MoreThan('b'))
                  << " – 0\n\n";

        std::cout << "\tCPoint\n\t\t";
        std::cout << RNAlgorithm::is_partitioned<CPoint>(p.begin(), p.end(), RNPredicate::EqualTo(CPoint(0, 1)))
                  << " – 0\n\t\t";
        std::cout << RNAlgorithm::is_partitioned<CPoint>(p1.begin(), p1.end(), RNPredicate::MoreThan(CPoint(2, 1)))
                  << " – 1\n\n\n";
    }


    std::cout << RNAlgorithm::is_palindrome<int>(a.begin(), a.end()) << std::endl;
    std::cout << RNAlgorithm::is_palindrome<int>(b.begin(), b.end()) << std::endl;
    std::cout << RNAlgorithm::is_palindrome<int>(c.begin(), c.end()) << std::endl;
    std::cout << RNAlgorithm::is_palindrome<int>(d.begin(), d.end()) << std::endl;
    std::cout << RNAlgorithm::is_palindrome<int>(e.begin(), e.end()) << std::endl;
    std::cout << RNAlgorithm::is_palindrome<int>(f.begin(), f.end()) << std::endl;
    std::cout << RNAlgorithm::is_palindrome<int>(g.begin(), g.end()) << std::endl;

    return 0;
}
