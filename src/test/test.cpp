#include "CppUnitTest.h"
#include <iostream>

#include <nepd_arranger.hpp>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft {namespace VisualStudio {namespace CppUnitTestFramework {
    template<typename T> 
    static std::wstring ToString(const std::vector<T>& arr) {
        std::wstringstream ws;
        for (const auto& c : arr) ws << c << " ";
        return ws.str();
    }
}}}

namespace test
{

TEST_CLASS(test_nepd_arranger)
{
public:

    TEST_METHOD(test_dist2) {
        nepd_arranger arr(7);

        Assert::AreEqual(0, arr.dist2(0, 0));
        Assert::AreEqual(1, arr.dist2(0, 1));
        Assert::AreEqual(1, arr.dist2(1, 0));
        Assert::AreEqual(1, arr.dist2(1, 8));

        Assert::AreEqual(4, arr.dist2(1, 3));
        Assert::AreEqual(4, arr.dist2(0, 14));

        Assert::AreEqual(2, arr.dist2(8, 0));
    }

    TEST_METHOD(test_mask_with_distance) {
        nepd_arranger arr(3);
        std::vector<char> mask(3*3, 0);
        arr.mask_with_distance(4, 2, mask);
        Assert::AreEqual(mask, {0, 0, 0, 0, 0, 0, 1, 0, 1});

        arr.mask_with_distance(0, 1, mask);
        Assert::AreEqual(mask, {0, 1, 0, 1, 0, 0, 1, 0, 1});
    }

    TEST_METHOD(test_mask_equidist) {
        nepd_arranger arr(3);
        std::vector<char> mask(3*3, 0);
        arr.mask_equidist(0, 2, mask);
        Assert::AreEqual(mask, {0, 0, 0, 0, 1, 0, 0, 1, 0});

        arr.mask_equidist(0, 6, mask);
        Assert::AreEqual(mask, {0, 0, 0, 0, 1, 0, 0, 1, 0});

        arr.mask_equidist(2, 6, mask);
        Assert::AreEqual(mask, {0, 0, 0, 0, 1, 0, 0, 1, 1});

    }

    TEST_METHOD(test_layout_print) {
        nepd_arranger::layout t = {3, {0, 1, 7}};
        std::stringstream ss;
        t.print(ss);
        Assert::AreEqual(std::string("oo.\n...\n.o.\n"), ss.str());
    }

    TEST_METHOD(test_layouts_equal) {
        typedef nepd_arranger::layout layout;
        layout t = {3, {0, 1, 6}};
        Assert::IsTrue(t == layout{3, {0, 1, 6}});
        Assert::IsTrue(t == layout{3, {0, 2, 5}});
        Assert::IsTrue(t == layout{3, {2, 7, 8}});
        Assert::IsTrue(t == layout{3, {3, 6, 8}});

        Assert::IsTrue(t == layout{3, {1, 2, 8}});
        Assert::IsTrue(t == layout{3, {0, 2, 3}});
        Assert::IsTrue(t == layout{3, {0, 6, 7}});
        Assert::IsTrue(t == layout{3, {5, 6, 8}});

        Assert::IsFalse(t == layout{3, {1, 2}});
        Assert::IsFalse(t == layout{3, {0, 1, 7}});
        Assert::IsFalse(t == layout{3, {2, 5, 6}});
    }

    TEST_METHOD(test_find_layouts) {
        {
            typedef nepd_arranger::layout layout;

            nepd_arranger arr(3);
            std::vector<layout> lt;
            arr.find_conforming_layouts(lt);
            Assert::AreEqual(5, (int)lt.size());

            Assert::IsFalse(std::find(lt.begin(), lt.end(), layout{3, {0, 1, 5}}) == lt.end());
            Assert::IsFalse(std::find(lt.begin(), lt.end(), layout{3, {0, 1, 6}}) == lt.end());
            Assert::IsFalse(std::find(lt.begin(), lt.end(), layout{3, {0, 1, 7}}) == lt.end());
            Assert::IsFalse(std::find(lt.begin(), lt.end(), layout{3, {0, 1, 8}}) == lt.end());
            Assert::IsFalse(std::find(lt.begin(), lt.end(), layout{3, {0, 4, 5}}) == lt.end());
        }
        {
            nepd_arranger arr(4);
            std::vector<nepd_arranger::layout> layouts;
            arr.find_conforming_layouts(layouts);
            Assert::AreEqual(16, (int)layouts.size());
        }
        {
            nepd_arranger arr(5);
            std::vector<nepd_arranger::layout> layouts;
            arr.find_conforming_layouts(layouts);
            Assert::AreEqual(28, (int)layouts.size());
        }
        {
            nepd_arranger arr(6);
            std::vector<nepd_arranger::layout> layouts;
            arr.find_conforming_layouts(layouts);
            Assert::AreEqual(2, (int)layouts.size());
        }
    }

};

}