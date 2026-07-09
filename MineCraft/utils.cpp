#include "pch.h"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <assert.h>
#include <random>
#include "utils.h"

namespace MineCraftUtils {
    // randomly pick up M numbers from a integer array.
    std::vector<int> knuth1(const std::vector<int> &numbers, int m)
    {
        if (numbers.empty() || m > numbers.size())
            return {};

        srand((unsigned int)time(0));
        int n = int(numbers.size());
        std::vector<int> ret;
        for (int i = 0; i < numbers.size(); i++)
        {
            if (rand() % (n - i) < m)
            {
                m--;
                ret.push_back(numbers[i]);
            }
        }

        return ret;
    }


    // randomly pick up M numbers from a consecutive number array.
    std::vector<int> MineSample(int max, int m)
    {
        std::vector<int> continuousNums;
        continuousNums.resize(max);
        for (int i = 0; i < max; i++) {
            continuousNums[i] = i;
        }

        return std::move(knuth1(continuousNums, m));
    } 

    inline int randEx()
    {
        std::random_device rd;
        std::mt19937 generator(rd());
        return abs((int)generator());
    }

    // generate hollow cells at the click cell.
    void HollowGen(std::map<int, int>& dest, int max, int cx, int cy, int X, int Y)
    {
        int key = X * cy + Y;
        dest[key] = key;
        while ((int)dest.size() < max) {
            int idx = randEx() % ((int)dest.size());
            auto itr = dest.begin();
            std::advance(itr, idx);
            int x1 = itr->first / cy;
            int y1 = itr->first % cy;

            int branch = randEx() % 4;
            if (branch == 0) {
                if (x1 > 0 && dest.find((x1 - 1) * cy + y1) == dest.end()) { dest[(x1 - 1) * cy + y1] = (x1 - 1) * cy + y1; }
            }
            else if (branch == 1) {
                if (x1 + 1 < cx && dest.find((x1 + 1) * cy + y1) == dest.end()) { dest[(x1 + 1) * cy + y1] = (x1 + 1) * cy + y1; }
            }
            else if (branch == 2) {
                if (y1 > 0 && dest.find(x1 * cy + y1 - 1) == dest.end()) { dest[x1 * cy + y1 - 1] = x1 * cy + y1 - 1; }
            }
            else if (branch == 3) {
                if (y1 + 1 < cy && dest.find(x1 * cy + y1 + 1) == dest.end()) { dest[x1 * cy + y1 + 1] = x1 * cy + y1 + 1; }
            }
        }
    }

    // generate nearby cells wrapped the entire hollow area.
    void NearbyGen(std::map<int, int>& hollows, int cx, int cy)
    {
        int dx[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
        int dy[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };

        int x{ 0 }, y{ 0 }, idx{ 0 };
        std::map<int, int> nearbys;
        for (auto& it : hollows) {
            x = it.first / cy;
            y = it.first % cy;
            for (int i = 0; i < 8; i++) {
                idx = (x + dx[i]) * cy + (y + dy[i]);
                if (hollows.find(idx)!=hollows.end() || nearbys.find(idx)!=nearbys.end() || idx < 0)
                    continue;
                nearbys[idx] = idx;
            }
        }

        //hollows.merge(nearbys);
        hollows.insert(nearbys.begin(), nearbys.end());
    }


    // generate mine cells with a fisrt click at hollow area.
    std::vector<int> MineSamplingWithFirstClickAtHollow(int max_cell_num, int mine_num, int cx, int cy, int X, int Y)
    {
        assert(max_cell_num > 0 && mine_num > 0 && max_cell_num > mine_num);
        assert(cx > 0 && cy > 0 && cx * cy == max_cell_num);
        assert(X < cx && X >= 0 && Y < cy && Y >= 0);

        // generate coninuous numbers.
        std::vector<int> continuousNums;
        continuousNums.resize(max_cell_num);
        for (int i = 0; i < max_cell_num; i++) {
            continuousNums[i] = i;
        }
        
        std::map<int, int> hollows;
        int min_ = (max_cell_num - mine_num) /20;
        int max_ = (max_cell_num - mine_num) * 3/20 ;
        min_ = min(1, min_);
        max_ = min(2, max_);
        if (min_ >= max_) max_ = min_ + 5;
        int hollowNum = randEx() % (max_ - min_ + 1) + min_; // [min, max]

        HollowGen(hollows, hollowNum, cx, cy, X, Y);

        NearbyGen(hollows, cx, cy);

        //std::sort(hollows.begin(), hollows.end());
        for (auto itr = hollows.rbegin(); itr != hollows.rend(); itr++) {
            continuousNums.erase(continuousNums.begin() + itr->first);
        }

        return std::move(knuth1(continuousNums, mine_num));
    }
}