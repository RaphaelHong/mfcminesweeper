#include "pch.h"

#include "GameCore.h"
#include "utils.h"
#include "MainFrm.h"

#include <memory>

//#define DEBUG_MINE_INIT

using namespace Gdiplus;
using namespace MineCraftUtils;


#define SendMessageToMainView(message_id, wparam, lparam) \
{ \
::SendMessage(((CMainFrame*)(AfxGetApp()->GetMainWnd()))->GetMainView()->m_hWnd, \
    message_id, \
    WPARAM(wparam), \
    LPARAM(lparam)); \
}

#define SendMessageToStatusView(message_id, wparam, lparam) \
{ \
::SendMessage(((CMainFrame*)(AfxGetApp()->GetMainWnd()))->GetStatusView()->m_hWnd, \
    message_id, \
    WPARAM(wparam), \
    LPARAM(lparam)); \
}

#define IMG_MINE_1 (0)  // mine_1.png
#define IMG_MINE_2 (1)  // mine_2.png
#define IMG_MINE_3 (2)  // mine_3.png
#define IMG_MINE_4 (3)  // mine_4.png
#define IMG_MINE_5 (4)  // mine_5.png
#define IMG_MINE_6 (5)  // mine_6.png
#define IMG_MINE_7 (6)  // mine_7.png
#define IMG_MINE_8 (7)  // mine_8.png
#define IMG_MINE_NEX  (8)   // mine_unexplored.png
#define IMG_MINE_MEE  (9)   // mine_minee.png
#define IMG_MINE_WRO  (10)  // mine_wrong.png
#define IMG_MINE_FLA  (11)  // mine_flag.png
#define IMG_MINE_HOL  (12)  // mine_hollow.png
#define IMG_MINE_EXP  (13)  // mine_exploded.png
#define IMG_MINE_UNE  (14)  // mine_uncertain_emboss.png
#define IMG_MINE_UNF  (15)  // mine_uncertain_flat.png
#define IMG_MINE_INDEX_END (16)

#ifndef DEBUG_MEM

Gdiplus::Image* _imgs[16]{};

BOOL InitImages()
{
    if (_imgs[IMG_MINE_1] != nullptr)
        return TRUE;

    _imgs[IMG_MINE_1] = (new Gdiplus::Image(L"res/mine_1.png"));
    _imgs[IMG_MINE_2] = (new Gdiplus::Image(L"res/mine_2.png"));
    _imgs[IMG_MINE_3] = (new Gdiplus::Image(L"res/mine_3.png"));
    _imgs[IMG_MINE_4] = (new Gdiplus::Image(L"res/mine_4.png"));
    _imgs[IMG_MINE_5] = (new Gdiplus::Image(L"res/mine_5.png"));
    _imgs[IMG_MINE_6] = (new Gdiplus::Image(L"res/mine_6.png"));
    _imgs[IMG_MINE_7] = (new Gdiplus::Image(L"res/mine_7.png"));
    _imgs[IMG_MINE_8] = (new Gdiplus::Image(L"res/mine_8.png"));
    _imgs[IMG_MINE_NEX] = (new Gdiplus::Image(L"res/mine_unexplored.png"));
    _imgs[IMG_MINE_MEE] = (new Gdiplus::Image(L"res/mine_minee.png"));
    _imgs[IMG_MINE_WRO] = (new Gdiplus::Image(L"res/mine_wrong.png"));
    _imgs[IMG_MINE_FLA] = (new Gdiplus::Image(L"res/mine_flag.png"));
    _imgs[IMG_MINE_HOL] = (new Gdiplus::Image(L"res/mine_hollow.png"));
    _imgs[IMG_MINE_EXP] = (new Gdiplus::Image(L"res/mine_exploded.png"));
    _imgs[IMG_MINE_UNE] = (new Gdiplus::Image(L"res/mine_uncertain_emboss.png"));
    _imgs[IMG_MINE_UNF] = (new Gdiplus::Image(L"res/mine_uncertain_flat.png"));

    for (int i = 0; i < IMG_MINE_INDEX_END; i++) {
        if (_imgs[i] == nullptr) {
            TRACE(L"image index %d load fail.\n", i);
        }
    }

    return _imgs[IMG_MINE_1] != nullptr;
}

#endif



/////////////////////////////////////////////////////////
//                     GameControl                     //
/////////////////////////////////////////////////////////
void GameControl::Start()
{
	_status = GS_Running;
    _time_elapsed = 0;
    _start_time = time(NULL);

    SendMessageToStatusView(WM_USER_GAME_START, 0, 0);
}

void GameControl::Reset()
{
	_status = GS_Ready;
    _time_elapsed = 0;
}

void GameControl::Win()
{
	_status = GS_Win;
    SendMessageToStatusView(WM_USER_GAME_WIN,0,0);
}

void GameControl::Fail()
{
	_status = GS_Fail;
	SendMessageToStatusView(WM_USER_GAME_OVER, 0, 0);
}

int GameControl::TimeElapsed()
{
    if (_start_time == 0)
        return 0;

    time_t now = time(NULL);
    _time_elapsed = (int)(now - _start_time);
    return _time_elapsed;
}




/////////////////////////////////////////////////////////
//                      GameCore                       //
/////////////////////////////////////////////////////////
GameCore* GameCore::_pInst = nullptr;
std::mutex GameCore::_lock;

GameCore::GameCore()
 : _gameLevel(GL_Easy)
{

}

GameCore::~GameCore()
{
}

BOOL GameCore::SetGameLevel(GameLevel lvl)
{
    if (lvl < GL_Easy || lvl > GL_Custom)
        return FALSE;

    if (_game_controller.IsStart()) {
        if (IDYES != MessageBox(NULL, L"要放弃当前游戏，重新开始吗？", L"扫雷", MB_YESNO)) {
            return FALSE;
        }
    }

    // reset.
    _gameLevel = lvl;

    Init();

    return TRUE;
}

GameLevel GameCore::GetGameLevel() const
{
    return _gameLevel;
}

int GameCore::GetTotalMines() const
{
    GameAttributes const& prop = gameAttributes[_gameLevel];
    return prop.nMines;
}

int GameCore::Init()
{
    _mine_field.SetFieldPos();
    _mine_field.Reset();

    _game_controller.Reset();

    return 0;
}


void GameCore::DrawMineField(CDC* pDC)
{
    _mine_field.Draw(pDC);
}

void GameCore::DrawCell(MineCell* cell, CDC* pDC)
{
    if (cell) {
        Graphics gp(pDC->GetSafeHdc());
        MineCell::GetHandler(cell->Surface())->Draw(cell, &gp);
    }
}

CRect GameCore::GetMineFieldPos() const
{
    return _mine_field.GetFieldPos();
}


void GameCore::Customize(int r, int c, int m)
{
    GameAttributes& prop = gameAttributes[3];
    prop.nRows = r;
    prop.nColumns = c;
    prop.nMines = m;
}

MineField* GameCore::GetMineFieldPtr()
{
    return &_mine_field;
}



/////////////////////////////////////////////////////////
//                 cell status handler                 //
/////////////////////////////////////////////////////////
// CellStatusUnexploredHandler
void CellStatusUnexploredHandler::HandleLBtnClick(MineCell* cell)
{
	if (cell->Substance() == CellSubstanceStatus::minee) {
		// explode the minee cell.
		GetGameCore->GetMineFieldPtr()->Explode(cell);
		GetGameCore->GameController().Fail();
	}
	else if (cell->Substance() == CellSubstanceStatus::hollow) {
		// expand the hollow cell.
		GetGameCore->GetMineFieldPtr()->Expand(cell);
	}
	else if (cell->Substance() == CellSubstanceStatus::nearby) {
        cell->Surface() = CellSurfaceStatus::nearby;
		SendMessageToMainView(WM_USER_REDRAW_SINGLE_CELL, (WPARAM)cell, 0);
        GetGameCore->GetMineFieldPtr()->IncreaseCellExplored();
	}

	if (GetGameCore->GetMineFieldPtr()->IsClean()) {
		GetGameCore->GameController().Win();
	}
}

void CellStatusUnexploredHandler::HandleRBtnClick(MineCell* cell)
{
	GetGameCore->GetMineFieldPtr()->Mark(cell);
}

void CellStatusUnexploredHandler::Draw(MineCell* cell, Graphics* gp)
{
    if (_imgs[IMG_MINE_NEX] != nullptr) {
        gp->DrawImage(_imgs[IMG_MINE_NEX], \
            Rect(cell->Pos().left,\
                cell->Pos().top,\
                cell->Pos().Width(),\
                cell->Pos().Height()));
    }
}

// CellStatusHollowHandler
void CellStatusHollowHandler::HandleLBtnClick(MineCell* cell)
{
	// do nothing.
}

void CellStatusHollowHandler::HandleRBtnClick(MineCell* cell)
{
	// do nothing.
}

void CellStatusHollowHandler::Draw(MineCell* cell, Graphics* gp)
{
    Gdiplus::SolidBrush br(Gdiplus::Color(255, 255, 255, 255));
    gp->FillRectangle(&br, \
        Rect(cell->Pos().left, \
            cell->Pos().top, \
            cell->Pos().Width(), \
            cell->Pos().Height()));
}

// CellStatusNearbyHandler
void CellStatusNearbyHandler::HandleLBtnClick(MineCell* cell)
{
    GetGameCore->GetMineFieldPtr()->NearbyClear(cell);
}

void CellStatusNearbyHandler::HandleRBtnClick(MineCell* cell)
{
    GetGameCore->GetMineFieldPtr()->NearbyClear(cell);
}

void CellStatusNearbyHandler::Draw(MineCell* cell, Graphics* gp)
{
    Gdiplus::Image *img = nullptr;
    if (cell->Surrounded() <= 8 && cell->Surrounded() >= 1) {
        img = _imgs[cell->Surrounded() - 1];
    }

    if (img != nullptr) {
        gp->DrawImage(img, \
            Rect(cell->Pos().left, \
                cell->Pos().top, \
                cell->Pos().Width(), \
                cell->Pos().Height()));
    }
}

// CellStatusMineeHandler
void CellStatusMineeHandler::HandleLBtnClick(MineCell* cell)
{
	// do nothing.
}

void CellStatusMineeHandler::HandleRBtnClick(MineCell* cell)
{
	// do nothing.
}

void CellStatusMineeHandler::Draw(MineCell* cell, Graphics* gp)
{
    if (_imgs[IMG_MINE_MEE] != nullptr) {
        gp->DrawImage(_imgs[IMG_MINE_MEE], \
            Rect(cell->Pos().left, \
                cell->Pos().top, \
                cell->Pos().Width(), \
                cell->Pos().Height()));
    }
}

// CellStatusWrongHandler
void CellStatusWrongHandler::HandleLBtnClick(MineCell* cell)
{
	// do nothing.
}

void CellStatusWrongHandler::HandleRBtnClick(MineCell* cell)
{
	//  do nothing.
}

void CellStatusWrongHandler::Draw(MineCell* cell, Graphics* gp)
{
    if (_imgs[IMG_MINE_MEE] != nullptr) {
        gp->DrawImage(_imgs[IMG_MINE_WRO], \
            Rect(cell->Pos().left, \
                cell->Pos().top, \
                cell->Pos().Width(), \
                cell->Pos().Height()));
    }
}

// CellStatusExplodedHandler
void CellStatusExplodedHandler::HandleLBtnClick(MineCell* cell)
{
	// do nothing.
}

void CellStatusExplodedHandler::HandleRBtnClick(MineCell* cell)
{
	// do nothing.
}

void CellStatusExplodedHandler::Draw(MineCell* cell, Graphics* gp)
{
    if (_imgs[IMG_MINE_MEE] != nullptr) {
        gp->DrawImage(_imgs[IMG_MINE_EXP], \
            Rect(cell->Pos().left, \
                cell->Pos().top, \
                cell->Pos().Width(), \
                cell->Pos().Height()));
    }
}

// CellStatusFlagHandler
void CellStatusFlagHandler::HandleLBtnClick(MineCell* cell)
{
	// do nothing. 
}

void CellStatusFlagHandler::HandleRBtnClick(MineCell* cell)
{
	GetGameCore->GetMineFieldPtr()->Mark(cell);
}

void CellStatusFlagHandler::Draw(MineCell* cell, Graphics* gp)
{
    if (_imgs[IMG_MINE_MEE] != nullptr) {
        gp->DrawImage(_imgs[IMG_MINE_FLA], \
            Rect(cell->Pos().left, \
                cell->Pos().top, \
                cell->Pos().Width(), \
                cell->Pos().Height()));
    }
}

// CellStatusUncertainHandler
void CellStatusUncertainHandler::HandleLBtnClick(MineCell* cell)
{
	// do nothing.
}

void CellStatusUncertainHandler::HandleRBtnClick(MineCell* cell)
{
	GetGameCore->GetMineFieldPtr()->Mark(cell);
}

void CellStatusUncertainHandler::Draw(MineCell* cell, Graphics* gp)
{
    if (_imgs[IMG_MINE_MEE] != nullptr) {
        gp->DrawImage(_imgs[IMG_MINE_UNE], \
            Rect(cell->Pos().left, \
                cell->Pos().top, \
                cell->Pos().Width(), \
                cell->Pos().Height()));
    }
}

// CellStatusUncertainFlatHandler
void CellStatusUncertainFlatHandler::HandleLBtnClick(MineCell* cell)
{
	// do nothing.
}

void CellStatusUncertainFlatHandler::HandleRBtnClick(MineCell* cell)
{
	// do nothing.
}

void CellStatusUncertainFlatHandler::Draw(MineCell* cell, Graphics* gp)
{
    if (_imgs[IMG_MINE_MEE] != nullptr) {
        gp->DrawImage(_imgs[IMG_MINE_UNF], \
            Rect(cell->Pos().left, \
                cell->Pos().top, \
                cell->Pos().Width(), \
                cell->Pos().Height()));
    }
}

/////////////////////////////////////////////////////////
//                      MineCell                       //
/////////////////////////////////////////////////////////
map<MineCellStatus, ICellStatusHandler*> MineCell::_handlers =
{
    {MineCellStatus::unexplored, new CellStatusUnexploredHandler},
    {MineCellStatus::hollow, new CellStatusHollowHandler},
    {MineCellStatus::nearby, new CellStatusNearbyHandler},
    {MineCellStatus::minee, new CellStatusMineeHandler},
    {MineCellStatus::wrong, new CellStatusWrongHandler},
    {MineCellStatus::exploded, new CellStatusExplodedHandler},
    {MineCellStatus::flag, new CellStatusFlagHandler},
    {MineCellStatus::uncertain, new CellStatusUncertainHandler},
    {MineCellStatus::uncertain_flat, new CellStatusUncertainFlatHandler},
};

MineCell::MineCell() :
    _pos(0, 0, 0, 0),
    _surface(CellSurfaceStatus::unexplored),
    _substance(CellSubstanceStatus::hollow),
    _surrouned(0)
{

}

ICellStatusHandler* MineCell::GetHandler(CellSurfaceStatus s)
{
    return _handlers.at(s);
}

void MineCell::ReleaseHandlers()
{
    for (auto& it : _handlers) {
        SAFE_DEL(it.second);
    }
}

void MineCell::Draw(CDC* pDC)
{
}


MineField::MineField()
{

}


void MineField::SetFieldPos()
{
    GameAttributes const& prop = gameAttributes[GetGameCore->GetGameLevel()];

    _pos.left = 0;
    _pos.top = 0;
    _pos.right = prop.nColumns * MineCell::_cell_size + prop.nColumns + 1;
    _pos.bottom = prop.nRows * MineCell::_cell_size + prop.nRows + 1;
}

void MineField::Reset()
{
    // reset flags.
    //_mine_explored = 0;
	_cell_explored = 0;

    CellsRealloc();


#ifdef DEBUG_MINE_INIT
    for (int i = 0; i < prop.nRows; i++) {
        for (int j = 0; j < prop.nColumns; j++) {
            _cells[i][j]._surface = _cells[i][j]._substance;
        }
    }
#endif 
}


void MineField::CellsRealloc()
{
    // reset cells.
    int lvl = int(GameCore::GetInstance()->GetGameLevel());
    GameAttributes const &prop = gameAttributes[lvl];

    // clear cells.
    decltype(_cells) v;
    _cells.swap(v);

    decltype(_mine_cells) mc;
    _mine_cells.swap(mc);

    _cells.resize(prop.nRows);
    for (auto & row : _cells) {
        row.resize(prop.nColumns);
    }

    int l{}, t{};
    for (int i = 0; i < prop.nRows; i++) {
        for (int j = 0; j < prop.nColumns; j++) {
            auto& cell = _cells[i][j];
            l = j * (MineCell::_cell_size + 1) + _pos.left;
            t = i * (MineCell::_cell_size + 1) + _pos.top;
            cell._pos = CRect(l, t, l+MineCell::_cell_size, t+MineCell::_cell_size);
        }
    }
}


void MineField::SetupCellsSubstance(int clickX, int clickY)
{
    int lvl = int(GameCore::GetInstance()->GetGameLevel());
    GameAttributes const& prop = gameAttributes[lvl];

    // random mine index
    auto minecells = MineSamplingWithFirstClickAtHollow(prop.nColumns * prop.nRows, prop.nMines, prop.nRows, prop.nColumns, clickX, clickY);

    // init mine cells.
    for (auto i : minecells)
    {
        int x = i / prop.nColumns;
        int y = i % prop.nColumns;

        _cells[x][y]._substance = CellSubstanceStatus::minee;
        _mine_cells.push_back(std::pair<int, int>{x, y});
    }

    for (int i = 0; i < prop.nRows; i++) {
        for (int j = 0; j < prop.nColumns; j++) {
            auto& cell = _cells[i][j];
            if (cell._substance == CellSubstanceStatus::minee) {
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int nx = i + dx;
                        int ny = j + dy;

                        if (nx == i && ny == j)
                            continue;

                        if (nx < 0 || nx >= prop.nRows || ny < 0 || ny >= prop.nColumns)
                            continue;

                        if (_cells[nx][ny]._substance == CellSubstanceStatus::minee)
                            continue;

                        _cells[nx][ny]._surrouned++;
                        _cells[nx][ny]._substance = CellSubstanceStatus::nearby;
                    }
                }
            }
        }
    }
}


void MineField::Draw(CDC* pDC)
{
    Graphics gp(pDC->GetSafeHdc());
    for (auto& row : _cells) {
        for (auto& cell : row) {
            MineCell::GetHandler(cell.Surface())->Draw(&cell, &gp);
            //cell.Draw(pDC);
        }
    }
}

MineCell* MineField::At(int x, int y)
{
    if (_cells.empty())
        return nullptr;

    if (x >= 0 && x < _cells.size() && y >= 0 && y < _cells.front().size())
    {
        return &_cells[x][y];
    }

    return nullptr;
}


void MineField::Mark(MineCell* cell)
{
    if (cell->_surface == CellSurfaceStatus::unexplored) {
        cell->_surface = CellSurfaceStatus::flag;
		//IncreaseMineExplored();
        //更新状态 扫雷+1
        SendMessageToStatusView(WM_USER_MINE_COUNT_UP, 0, 0);        
    }
    else if (cell->_surface == CellSurfaceStatus::flag) {
        cell->_surface = CellSurfaceStatus::uncertain;
		//DecreaseMineExplored();
        SendMessageToStatusView(WM_USER_MINE_COUNT_DOWN, 0, 0);
    }
    else if (cell->_surface == CellSurfaceStatus::uncertain) {
        cell->_surface = CellSurfaceStatus::unexplored;
    }
    
    SendMessageToMainView(WM_USER_REDRAW_SINGLE_CELL, cell, 0);
}

bool MineField::IsClean() const
{
    TRACE(L"explored:%d\n", _cell_explored);

	// 所有非地雷格子都被清理，便可判胜
	GameAttributes const& prop = gameAttributes[GetGameCore->GetGameLevel()];
    if (_cell_explored == (prop.nColumns * prop.nRows - prop.nMines)) {
        return true;
    }

    return false;
}

void MineField::Explode(MineCell* failCell)
{
	// 显示所有地雷格
	for (auto& cellIdx : _mine_cells) {
		auto cell = &_cells[cellIdx.first][cellIdx.second];
		if (cell->Surface() == CellSurfaceStatus::unexplored) {
			cell->Surface() = CellSurfaceStatus::minee;
			//SendMessageToMainView(WM_USER_REDRAW_SINGLE_CELL, cell, 0);
		}
	}
	// 显示错误标记的格子
	for (auto& row : _cells) {
		for (auto& cell : row) {
			if (cell.Surface() == CellSurfaceStatus::flag && cell.Substance() != CellSubstanceStatus::minee) {
				cell.Surface() = CellSurfaceStatus::wrong;
				//SendMessageToMainView(WM_USER_REDRAW_SINGLE_CELL, &cell, 0);
			}
		}
	}
	// 显示爆炸的地雷格
	failCell->Surface() = CellSurfaceStatus::exploded;
	//SendMessageToMainView(WM_USER_REDRAW_SINGLE_CELL, failCell, 0);

	((CMainFrame*)(AfxGetApp()->GetMainWnd()))->GetMainView()->Invalidate();
}


void MineField::Expand(MineCell* hollowCell)
{
	IncreaseCellExplored();

    hollowCell->Surface() = CellSurfaceStatus::hollow;
	SendMessageToMainView(WM_USER_REDRAW_SINGLE_CELL, hollowCell, 0);
	GameAttributes const& prop = gameAttributes[GetGameCore->GetGameLevel()];
	int x = (hollowCell->Pos().top - _pos.top) / (MineCell::_cell_size + 1);
	int y = (hollowCell->Pos().left - _pos.left) / (MineCell::_cell_size + 1);
	// expand the surrounding cells.
    
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			if (i < 0 || i >= prop.nRows)
				continue;
			if (j < 0 || j >= prop.nColumns)
				continue;
			if (i == x && j == y)
				continue;
			auto& cell = _cells[i][j];
			if (cell.Surface() == CellSurfaceStatus::unexplored) {
				if (cell.IsHollow()) {
					Expand(&cell);
				}
				else if (cell.IsNearby()) {
					cell.Surface() = CellSurfaceStatus::nearby;
					SendMessageToMainView(WM_USER_REDRAW_SINGLE_CELL, &cell, 0);
					IncreaseCellExplored();
				}
			}
		}
	}
}

void MineField::NearbyClear(MineCell* nearbyCell)
{
    int x = (nearbyCell->Pos().top - _pos.top) / (MineCell::_cell_size + 1);
    int y = (nearbyCell->Pos().left - _pos.left) / (MineCell::_cell_size + 1);

	GameAttributes const& prop = gameAttributes[GetGameCore->GetGameLevel()];

	bool hasMinesUnexplored = false;
	vector<MineCell*> unexploredCells;
	for (int i = x - 1; i <= x + 1 ; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
            if (i < 0 || i >= prop.nRows)
                continue;
            if (j < 0 || j >= prop.nColumns)
                continue;
            if (i == x && j == y)
                continue;

			if (_cells[i][j].Surface() == CellSurfaceStatus::unexplored) {
				unexploredCells.push_back(&_cells[i][j]);

                if (_cells[i][j].Substance() == CellSubstanceStatus::minee)
					hasMinesUnexplored = true;
			}
		}
	}

	if (hasMinesUnexplored) {
        SendMessageToMainView(WM_USER_BLINK_CELLS, WPARAM(&unexploredCells), 0);
	}
    else {
		for (auto cell : unexploredCells) {
			MineCell::GetHandler(cell->Surface())->HandleLBtnClick(cell);
		}
    }
}

/////////////////////////////////////////////////////////
//                MineMouseManager                     //
/////////////////////////////////////////////////////////
MineMouseManager::MineMouseManager(MineField* p)
{
    _owner = p;
}

void MineMouseManager::SetOwner(MineField* p)
{
    _owner = p;
}


void MineMouseManager::OnMouseLBtnDown(CPoint pt)
{
    // game ended.
    if (GetGameCore->GameController().IsWin() || GetGameCore->GameController().IsFail())
        return;

    int clickX{}, clickY{};
    auto cell = SelectCell(pt, clickX, clickY);

    if (cell == nullptr) return;

    if (cell->Pos().PtInRect(pt)) {

        if (GetGameCore->GameController().IsReady()) {
            GetGameCore->GetMineFieldPtr()->SetupCellsSubstance(clickX, clickY);
            GetGameCore->GameController().Start();
        }

        // actions on cell.
        MineCell::GetHandler(cell->Surface())->HandleLBtnClick(cell);
    }
}


void MineMouseManager::OnMouseRBtnDown(CPoint pt)
{
    // game ended.
    if (GetGameCore->GameController().IsWin() || GetGameCore->GameController().IsFail())
        return;

    int clickX{}, clickY{};
    auto cell = SelectCell(pt, clickX, clickY);
    if (cell == nullptr) return;

    if (cell->Pos().PtInRect(pt)) {
		MineCell::GetHandler(cell->Surface())->HandleRBtnClick(cell);

        if (GetGameCore->GameController().IsReady()) {
            GetGameCore->GameController().Start();
        }
    }
}


MineCell* MineMouseManager::SelectCell(CPoint pt, int& x, int& y)
{
    auto rc = _owner->GetFieldPos();
    //int x = -1;
    //int y = -1;

    x = (pt.y - rc.top) / (MineCell::_cell_size + 1);
    y = (pt.x - rc.left) / (MineCell::_cell_size + 1);
    
    GameAttributes const & prop = gameAttributes[GameCore::GetInstance()->GetGameLevel()];
    if (x < 0 || x >= prop.nRows)
        return nullptr;
    if (y < 0 || y >= prop.nColumns)
        return nullptr;

    TRACE(L"x:%d, y:%d\n", x, y);

    auto cell = _owner->At(x, y);

    return cell;
}