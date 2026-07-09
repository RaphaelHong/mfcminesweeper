#pragma once

#include "utils.h"

#include <mutex>
#include <vector>
#include <map>


//#define DEBUG_MEM


using namespace std;

enum GameLevel
{
    GL_Easy   = 0, // 9(column) x 9(row) grid, 10 mines
    GL_Middle = 1, // 16(column) x 16(row) grid, 40 mines
    GL_Expert = 2, // 30(column) x 16(row) grid, 99 mines
    GL_Custom = 3  // M(column) x N(row) grid, X mines. [M,N,X] is customized.
};

struct GameAttributes
{
    int nColumns; // 地雷区列数
    int nRows;    // 地雷区行数
    int nMines;   // 总地雷数
};

static GameAttributes gameAttributes[4] = {
    {9, 9, 10},
    {16, 16, 40},
    {30, 16, 99},
    {0, 0, 0}
};


/**
 * 格子所有状态
 */
typedef enum MineCellStatus
{
    unexplored, //  未探索
    hollow,     //  空心格，周围格子没有地雷
    nearby,     //  格子周围有地雷，并标记了周围地雷数量
    minee,      //  地雷格
    wrong,      //  错误地雷标记
    exploded,   //  已爆炸地雷格
    flag,       //  旗帜标记（标记此格为地雷）
    uncertain,  //  问号标记(标记此格未确定)
    uncertain_flat, // 问号标记(结束统计) 
    reserve

} CellSurfaceStatus,CellSubstanceStatus;

class MineCell;
class ICellStatusHandler
{
public:    
    virtual void HandleLBtnClick(MineCell* cell) = 0;
    virtual void HandleRBtnClick(MineCell* cell) = 0;
    virtual void Draw(MineCell* cell, Gdiplus::Graphics* gp) = 0;
};

// unexplored.
class CellStatusUnexploredHandler : public ICellStatusHandler
{
public:
    void HandleLBtnClick(MineCell* cell) override;
    void HandleRBtnClick(MineCell* cell) override;
    void Draw(MineCell* cell, Gdiplus::Graphics* gp) override;
};

// hollow.
class CellStatusHollowHandler : public ICellStatusHandler
{
public:
    void HandleLBtnClick(MineCell* cell) override;
    void HandleRBtnClick(MineCell* cell) override;
    void Draw(MineCell* cell, Gdiplus::Graphics* gp) override;
};

// nearby.
class CellStatusNearbyHandler : public ICellStatusHandler
{
public:
    void HandleLBtnClick(MineCell* cell) override;
    void HandleRBtnClick(MineCell* cell) override;
    void Draw(MineCell* cell, Gdiplus::Graphics* gp) override;
};

// minee.
class CellStatusMineeHandler : public ICellStatusHandler
{
public:
    void HandleLBtnClick(MineCell* cell) override;
    void HandleRBtnClick(MineCell* cell) override;
    void Draw(MineCell* cell, Gdiplus::Graphics* gp) override;
};

// wrong.
class CellStatusWrongHandler : public ICellStatusHandler
{
public:
    void HandleLBtnClick(MineCell* cell) override;
    void HandleRBtnClick(MineCell* cell) override;
    void Draw(MineCell* cell, Gdiplus::Graphics* gp) override;
};

// exploded.
class CellStatusExplodedHandler : public ICellStatusHandler
{
public:
    void HandleLBtnClick(MineCell* cell) override;
    void HandleRBtnClick(MineCell* cell) override;
    void Draw(MineCell* cell, Gdiplus::Graphics* gp) override;
};

// flag.
class CellStatusFlagHandler : public ICellStatusHandler
{
public:
    void HandleLBtnClick(MineCell* cell) override;
    void HandleRBtnClick(MineCell* cell) override;
    void Draw(MineCell* cell, Gdiplus::Graphics* gp) override;
};

// uncertain.
class CellStatusUncertainHandler : public ICellStatusHandler
{
public:
    void HandleLBtnClick(MineCell* cell) override;
    void HandleRBtnClick(MineCell* cell) override;
    void Draw(MineCell* cell, Gdiplus::Graphics* gp) override;
};

// uncertain flat.
class CellStatusUncertainFlatHandler : public ICellStatusHandler
{
public:
    void HandleLBtnClick(MineCell* cell) override;
    void HandleRBtnClick(MineCell* cell) override;
    void Draw(MineCell* cell, Gdiplus::Graphics* gp) override;
};


class MineCell
{
    friend class MineField;
    friend class GameCore;
public:
    MineCell();

    static ICellStatusHandler* GetHandler(CellSurfaceStatus s);
    static void ReleaseHandlers();

    inline bool IsHollow() const noexcept { return _substance == CellSubstanceStatus::hollow; }
    inline bool IsNearby() const noexcept { return _substance == CellSubstanceStatus::nearby; }
    inline bool IsMinee() const noexcept { return _substance == CellSubstanceStatus::minee; }
    
    CellSurfaceStatus Surface() const { return _surface; }
    CellSurfaceStatus& Surface() { return _surface; }
    CellSubstanceStatus Substance() const { return _substance; }

    CRect Pos() const { return _pos; }
    int Surrounded() const { return _surrouned; }
   
    static const int _cell_size{ 16 };

protected:
    CRect _pos; // cell position.
    CellSurfaceStatus _surface; // 格子表征态，用于状态控制
    CellSubstanceStatus _substance; // 格子真实态，（地雷、空心、数字三种状态）
    int _surrouned; // cell has been surrounded with.

    static map<MineCellStatus, ICellStatusHandler*> _handlers;

    void Draw(CDC* pDC);
};


extern BOOL InitImages();

///////////////////////////////////////////////////////////////
// class MineField
//
class MineField
{
public:
    MineField();

    void SetFieldPos();
    CRect GetFieldPos() const { return _pos; };

    void Draw(CDC*  pDC);

    MineCell* At(int x, int y);

    void Explode(MineCell* mineeCell);
	void Expand(MineCell* hollowCell);
	void NearbyClear(MineCell* nearbyCell);

    void Mark(MineCell* cell);

	inline void IncreaseCellExplored() { _cell_explored++; }
	inline void DecreaseCellExplored() { _cell_explored--; }
	//inline void IncreaseMineExplored() { _mine_explored++; }
	//inline void DecreaseMineExplored() { _mine_explored--; }

    bool IsClean() const;

    void Reset();
    void SetupCellsSubstance(int clickX, int clickY);

private:
    void CellsRealloc();

protected:
    CRect _pos;
	//int _mine_explored{ 0 }; // 标记的地雷格子数
	int _cell_explored{ 0 }; // 已清理的格子数
    vector<vector<MineCell>> _cells; // 所有格子
    vector<std::pair<int, int>> _mine_cells; // 地雷格索引
};


class MineMouseManager
{
    friend class MineField;
    friend class MineCell;
public:    
    MineMouseManager() : _owner(nullptr) {}
    MineMouseManager(MineField* p);
    void SetOwner(MineField* p);
    void OnMouseLBtnDown(CPoint pt);
    void OnMouseRBtnDown(CPoint pt);

protected:
    MineField* _owner;

    MineCell* SelectCell(CPoint pt, int& x, int& y);
};


///////////////////////////////////////////////////////////////
//  Class GameCore
//
class GameControl
{
public:
	enum GameStatus
	{
		GS_Ready,
		GS_Running,
		GS_Win,
		GS_Fail
	};

    void Start();
    void Win();
    void Fail();
    void Reset();

    int TimeElapsed();

	inline bool IsReady() const { return _status == GS_Ready; }
    inline bool IsStart() const { return _status == GS_Running; }
	inline bool IsFail() const { return _status == GS_Fail; }
	inline bool IsWin() const { return _status == GS_Win; }

protected:
	GameStatus _status{ GS_Ready };
    //bool _is_game_started{ false };
	//bool _is_game_over{ false };
    time_t _start_time{ 0 };
    int _time_elapsed{ 0 };
};

class GameCore : public Noncopyable<GameCore>
{
private:
    GameCore();
 
public:
    static GameCore* GetInstance()
    {
        if (_pInst == nullptr) {
            std::lock_guard<std::mutex> lock(_lock);
            if (_pInst == nullptr) {
                _pInst = new GameCore();
            }
        }

        return _pInst;
    }

    static void ReleaseInstance()
    {
        if (_pInst != nullptr) {
            std::lock_guard<std::mutex> lock(_lock);
            SAFE_DEL(_pInst);
            MineCell::ReleaseHandlers();
        }
    }

    ~GameCore();

    int Init();

    GameControl& GameController() { return _game_controller; }

    BOOL SetGameLevel(GameLevel lvl);
    GameLevel GetGameLevel() const;
    void Customize(int r, int c, int m);

    int GetTotalMines() const;
    
    void DrawMineField(CDC* pDC);
    void DrawCell(MineCell* cell, CDC* pDC);
    CRect GetMineFieldPos() const;
    
    MineField* GetMineFieldPtr();

protected:
   

private:
    static GameCore* _pInst;
    static std::mutex _lock;
    GameLevel _gameLevel;
    MineField _mine_field;
    GameControl _game_controller;
};

#define GetGameCore (GameCore::GetInstance())