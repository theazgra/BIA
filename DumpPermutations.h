#pragma once

#include <azgra/collection/vector_linq.h>
#include <azgra/collection/set_utilities.h>
#include <azgra/collection/linq.h>
#include <boost/optional.hpp>

template<typename T>
class DumpPermutations
{
private:
    std::vector<T> m_items;
    size_t m_itemSize;

    void swap(const size_t &from, const size_t &to)
    {
        T tmp = m_items[from];
        m_items[from] = m_items[to];
        m_items[to] = tmp;
    }

    bool find_k(size_t &k)
    {
//        1. Find the largest index k such that a[k] < a[k + 1]. If no such index exists, the permutation is the last permutation.
        k = 0;
        bool found = false;
        for (size_t i = 0; i < m_items.size() - 1; ++i)
        {
            if (m_items[i] < m_items[i + 1])
            {
                k = i;
                found = true;
            }
        }
        return found;
    }

    bool find_l(const size_t &k, size_t &l)
    {
        l = 0;
        bool found = false;
//      2. Find the largest index l greater than k such that a[k] < a[l].
        for (size_t i = (k + 1); i < m_items.size(); ++i)
        {
            if (m_items[k] < m_items[i])
            {
                found = true;
                l = i;
            }
        }
        return found;
    }

    void reverse(const size_t from)
    {
        if (from == (m_itemSize - 1))
            return;

        /*
            i = len - 1;
          j = 0;
          while(i > j)
          {
            int temp = a[i];
            a[i] = a[j];
            a[j] = temp;
            i--;
            j++;
          }
         */

        int front = from;
        int back = m_itemSize - 1;
        while (front < back)
        {
            swap(front++,back--);
        }
//        size_t backIndex = m_itemSize - 1;
//        for (int i = from; i < ((m_itemSize + 2 - 1) / 2); ++i)
//        {
//            swap(i, backIndex--);
//        }
    }


public:
    explicit DumpPermutations(std::set<T> source)
    {
        m_items = std::vector<T>(source.begin(), source.end());
        m_itemSize = m_items.size();
    }

    std::vector<T> const &get_current_permutation() const
    {
        return m_items;
    }

    bool next_permutation()
    {
        size_t k = 0;
        size_t l = 0;

//        1. Find the largest index k such that a[k] < a[k + 1]. If no such index exists, the permutation is the last permutation.
        if (!find_k(k))
            return false;

//        2. Find the largest index l greater than k such that a[k] < a[l].
        if (!find_l(k, l))
            return false;

//        3. Swap the value of a[k] with that of a[l].
        swap(k, l);

//        4. Reverse the sequence from a[k + 1] up to and including the final element a[n].
        reverse(k + 1);
        return true;
    }


};
