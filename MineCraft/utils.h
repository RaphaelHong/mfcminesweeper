#pragma once

#include <vector>
#include <map>

#define WM_USER_REDRAW_SINGLE_CELL  WM_USER+1
#define WM_USER_GAME_START          WM_USER+2
#define WM_USER_GAME_OVER           WM_USER+3
#define WM_USER_GAME_WIN            WM_USER+4
#define WM_USER_MINE_COUNT_UP       WM_USER+5
#define WM_USER_MINE_COUNT_DOWN     WM_USER+6
#define WM_USER_GAME_RESET          WM_USER+7
#define WM_USER_BLINK_CELLS		    WM_USER+8


#define SAFE_DEL(ptr) { delete (ptr); (ptr) = nullptr; }
#define SAFE_DEL_ARR(arr_ptr) { delete [] (arr_ptr); (arr_ptr) = nullptr; }

/**
 * template for all noncopyable class.
 */
template <typename T>
class Noncopyable {
protected:
    Noncopyable() = default;
    Noncopyable(Noncopyable const &) = delete;
    Noncopyable& operator=(Noncopyable const &) = delete;
};

namespace MineCraftUtils
{
    std::vector<int> knuth1(const std::vector<int> &numbers, int m);

    std::vector<int> MineSample(int max, int m);

    inline int randEx();

    void HollowGen(std::map<int, int>& dest, int max, int cx, int cy, int X, int Y);

    void NearbyGen(std::map<int, int>& hollows, int cx, int cy);

    std::vector<int> MineSamplingWithFirstClickAtHollow(int max_cell_num, int mine_num, int cx, int cy, int X, int Y);
};