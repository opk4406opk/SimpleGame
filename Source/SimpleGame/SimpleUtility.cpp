#include "SimpleUtility.h"

using namespace PathfindingAlgorithm;

FPathFinderAStar::FPathFinderAStar()
{
}


FPathFinderAStar::~FPathFinderAStar()
{
}

bool PathfindingAlgorithm::FPathFinderAStar::Pathfinding(TArray<int32>& navigateList, int32 startIndex, int32 goalIndex)
{
	InitPathFinding();
	// 초기화후, 장애물 설정.
	// to do here..
	//
	//goalNode = MakeShareable<FAstarPathNode>(pathMap[(int32)goal.X][(int32)goal.Y].Get());
	//goalNode->bGoalNode = true;
	////
	//curNode = MakeShareable<FAstarPathNode>(pathMap[(int32)start.X][(int32)start.Y].Get());
	//curNode->SetParentNode(nullptr);
	//curNode->CalcHValue(goalNode.Get());
	while ((OpenList.Num() != 0) && (!IsGoalInOpenList()))
	{
		SetOpenList();
		FAstarPathNode* selectNode = SelectLowCostPath();
		if (selectNode != nullptr)
		{
			SearchAdjacentNodes(selectNode);
		}
	}
	return ExtractNavigatePath(navigateList);
}

// 길찾기 전에 호출되어 초기화를 진행합니다.
void FPathFinderAStar::InitPathFinding()
{
	OpenList.Empty();
	ClosedList.Empty();
	// 맵 트리 해제.
	ReleasePathNode(RootNode.Get());
	RootNode.Reset();
	CurrentNode.Reset();
	GoalNode.Reset();

	// 링크드 리스트를 구성. ( = 맵 노드들을 구성한다(트리형태).
	//
	// to do here.... 
	// 시작점은 Current Node, 목표지점은 Goal Node로 설정된다. 
	// 시작점인 Current Node는 OpenList에 추가되어야 함.
	// 갈수없는 지점은 ClosedList에 추가되어야 함.
	
	// 출격 테이블 데이터가 리스트 형태로 들어온다면
	// 첫번째 Row의 데이터를 이용해
	// TSharedPtr<FAstarPathNode> root = MakeShared<FAstarPathNode>();
	// ~> 링크들의 Code값들을 root->SoritePathNodeInfo.PathLinkCodes[MAX_ADJACENT] 에 할당.
	// RootNode = root;
	// MakePathNode(root);
	

	OpenList.Add(CurrentNode.Get());
}

void FPathFinderAStar::CurNodeToClosedList()
{
	ClosedList.Add(CurrentNode.Get());
	for (FAstarPathNode* pathNode : OpenList)
	{
		if (pathNode->GetPosition() == CurrentNode->GetPosition())
		{
			OpenList.Remove(pathNode);
			break;
		}
	}
}

bool FPathFinderAStar::IsInClosedList(int searchPosX, int searchPosY)
{
	bool bFind = false;
	for(auto node : ClosedList)
	{
		if ((node->GetPosition().X == searchPosX) &&
			(node->GetPosition().Y == searchPosY))
			bFind = true;
	}
	return bFind;
}

bool FPathFinderAStar::IsInOpenList(int searchPosX, int searchPosY)
{
	bool bFind = false;
	for(auto node : OpenList)
	{
		if ((node->GetPosition().X == searchPosX) &&
			(node->GetPosition().Y == searchPosY))
			bFind = true;
	}
	return bFind;
}

bool FPathFinderAStar::IsGoalInOpenList()
{
	bool bFind = false;
	for(auto node : OpenList)
	{
		if (node->bGoalNode)
		{
			bFind = true;
		}
	}
	return bFind;
}

void FPathFinderAStar::SetOpenList()
{
	for (TSharedPtr<FAstarPathNode> adjacentNode : CurrentNode->AdjacentNodes)
	{
		if (adjacentNode.IsValid() == false) continue;

		if ((IsInClosedList(adjacentNode->GetPosition().X, adjacentNode->GetPosition().Y) == false) &&
			(IsInOpenList(adjacentNode->GetPosition().X, adjacentNode->GetPosition().Y) == false))
		{
			adjacentNode->SetParentNode(CurrentNode.Get());
			adjacentNode->CalcHValue(GoalNode.Get());
			adjacentNode->CalcGValue();
			OpenList.Add(adjacentNode.Get());
		}
	}
	//
	/*for(FVector2D pos : fourDir)
	{
		int searchPosX = curNode->GetPosition().X + pos.X;
		int searchPosY = curNode->GetPosition().Y + pos.Y;
		if ((searchPosX >= 0 && searchPosX < MAP_SIZE_X) && (searchPosY >= 0 && searchPosY < MAP_SIZE_Y))
		{
			if ((!IsInClosedList(searchPosX, searchPosY)) && (!IsInOpenList(searchPosX, searchPosY)))
			{
				pathMap[searchPosX][searchPosY]->SetParentNode(curNode.Get());
				pathMap[searchPosX][searchPosY]->CalcHValue(goalNode.Get());
				pathMap[searchPosX][searchPosY]->CalcGValue();
				openList.Add(pathMap[searchPosX][searchPosY].Get());
			}
		}
	}*/
	CurNodeToClosedList();
}

void FPathFinderAStar::SearchAdjacentNodes(FAstarPathNode * selectNode)
{
	for (TSharedPtr<FAstarPathNode> adjacentNode : selectNode->AdjacentNodes)
	{
		if (adjacentNode.IsValid() == false) continue;

		if (IsInClosedList(adjacentNode->GetPosition().X, adjacentNode->GetPosition().Y) == false)
		{
			if (IsInOpenList(adjacentNode->GetPosition().X, adjacentNode->GetPosition().Y) == false)
			{
				OpenList.Add(adjacentNode.Get());
				adjacentNode->SetParentNode(CurrentNode.Get());
				adjacentNode->CalcHValue(GoalNode.Get());
				adjacentNode->CalcGValue();
			}
			else
			{
				if ((adjacentNode->GetGValue() < selectNode->GetGValue()))
				{
					selectNode->SetParentNode(adjacentNode.Get());
					selectNode->CalcHValue(GoalNode.Get());
					selectNode->CalcGValue();
				}
			}
		}
	}
	//
	//
	/*for (FVector2D pos : fourDir)
	{
		int searchPosX = selectNode->GetPosition().X;
		int searchPosY = selectNode->GetPosition().Y;
		if ((searchPosX >= 0 && searchPosX < MAP_SIZE_X) && (searchPosY >= 0 && searchPosY < MAP_SIZE_Y))
		{
			if (!IsInClosedList(searchPosX, searchPosY))
			{
				if (!IsInOpenList(searchPosX, searchPosY))
				{
					openList.Add(pathMap[searchPosX][searchPosY].Get());
					pathMap[searchPosX][searchPosY]->SetParentNode(curNode.Get());
					pathMap[searchPosX][searchPosY]->CalcHValue(goalNode.Get());
					pathMap[searchPosX][searchPosY]->CalcGValue();
				}
				else
				{
					if ((pathMap[searchPosX][searchPosY]->GetGValue() < selectNode->GetGValue()))
					{
						selectNode->SetParentNode(pathMap[searchPosX][searchPosY].Get());
						selectNode->CalcHValue(goalNode.Get());
						selectNode->CalcGValue();
					}
				}
			}
		}
	}*/
	
}

bool FPathFinderAStar::ExtractNavigatePath(TArray<int32>& navigatePath)
{
	FAstarPathNode* path = GoalNode.Get();
	while (path->GetParentNode() != nullptr)
	{
		navigatePath.Add(path->SoritePathNodeInfo.FloorIndex);
		path = path->GetParentNode();
	}
	if (navigatePath.Num() == 0)
	{
		return false;
	}
	return true;
}

FAstarPathNode* FPathFinderAStar::SelectLowCostPath()
{
	int minCost = 0;
	FAstarPathNode* lowCostPath = nullptr;
	if (OpenList.Num() == 0) return nullptr;

	minCost = OpenList[0]->GetGValue() + OpenList[0]->GetHValue();
	lowCostPath = OpenList[0];
	for (int i = 1; i < OpenList.Num(); i++)
	{
		int cost = OpenList[i]->GetGValue() + OpenList[i]->GetHValue();
		if (minCost > cost)
		{
			minCost = cost;
			lowCostPath = OpenList[i];
		}
	}

	for (FAstarPathNode* pathNode : OpenList)
	{
		if (pathNode->GetPosition() == lowCostPath->GetPosition())
		{
			OpenList.Remove(pathNode);
			break;
		}
	}

	ClosedList.Add(lowCostPath);
	CurrentNode = MakeShareable<FAstarPathNode>(lowCostPath);

	return lowCostPath;
}

void FPathFinderAStar::MakePathNode(FAstarPathNode* rootNode)
{
	if (rootNode == nullptr) return;

	for (int32 loop = 0; loop < MAX_ADJACENT; loop++)
	{
		if (rootNode->AdjacentNodes[loop].IsValid() == false && rootNode->SoritePathNodeInfo.AdjacentLinkCodes[loop] != NAME_None)
		{
			rootNode->AdjacentNodes[loop] = MakeShared<FAstarPathNode>();
			rootNode->AdjacentNodes[loop]->SetParentNode(rootNode);
			// 실제 유효한 PathNode의 위치값을 할당해줘야함. 
			// rootNode->AdjacentNodes[loop]->SetPosition(FVector2D::ZeroVector); 
			// 만약 PathNode의 Position 값, 그리고 Code값이 Start 혹은 Goal 노드와 동일한경우에 대해 처리가 필요.
			MakePathNode(rootNode->AdjacentNodes[loop].Get());
		}
	}
}

void FPathFinderAStar::ReleasePathNode(FAstarPathNode* rootNode)
{
	if (rootNode == nullptr) return;

	for (int32 loop = 0; loop < MAX_ADJACENT; loop++)
	{
		if (rootNode->AdjacentNodes[loop].IsValid() == true)
		{
			ReleasePathNode(rootNode->AdjacentNodes[loop].Get());
			rootNode->AdjacentNodes[loop].Reset();
		}
	}
}


FAstarPathNode::FAstarPathNode()
{
}


FAstarPathNode::~FAstarPathNode()
{
}

void FAstarPathNode::CalcGValue()
{
	if (ParentNode.Get() == nullptr)
	{
		GValue = 0;
		return;
	}
	int diffX = Position.X - ParentNode->GetPosition().X;
	int diffY = Position.Y - ParentNode->GetPosition().Y;
	if (((diffX == -1) && (diffY == 1))
		|| ((diffX == 1) && (diffY == 1))
		|| ((diffX == -1) && (diffY == -1))
		|| ((diffX == 1) && (diffY == -1)))
	{
		// 대각선 이동.
		GValue = (FMath::Abs(diffX) + FMath::Abs(diffY)) * 14 + ParentNode->GetGValue();
	}
	else
	{
		// 좌우상하 이동.
		GValue = (FMath::Abs(diffX) + FMath::Abs(diffY)) * 10 + ParentNode->GetGValue();
	}
}

void FAstarPathNode::CalcHValue(FAstarPathNode* goal)
{
	int diffX = goal->GetPosition().X - Position.X;
	int diffY = goal->GetPosition().Y - Position.Y;

	HValue = (FMath::Abs(diffX) + FMath::Abs(diffY)) * 10;
}

FVector2D FAstarPathNode::GetPosition()
{
	return Position;
}

void FAstarPathNode::SetPosition(int x, int y)
{
	Position.X = x;
	Position.Y = y;
}

void FAstarPathNode::SetPosition(FVector2D pos)
{
	Position = pos;
}

FAstarPathNode* FAstarPathNode::GetParentNode()
{
	return ParentNode.Get();
}

int FAstarPathNode::GetGValue()
{
	return GValue;
}

int FAstarPathNode::GetHValue()
{
	return HValue;
}

void FAstarPathNode::SetParentNode(FAstarPathNode * node)
{
	ParentNode = MakeShareable<FAstarPathNode>(node);
}

TArray<FPathNodeDijkstra> FPathFinderDijkstra::Pathfinding(FVector2D start, FVector2D goal)
{
	TArray<FPathNodeDijkstra> MapList; // 맵.
	TArray<FPathNodeDijkstra> PriortyQueue;
	//
	PriortyQueue.HeapPush(MapList[0], FPriorityPredicate());
	for (int32 loopCnt = 0; loopCnt < MapList.Num(); loopCnt++)
	{
		PriortyQueue.HeapPush(MapList[loopCnt], FPriorityPredicate());
	}
}
