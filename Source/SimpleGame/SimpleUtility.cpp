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
	// �ʱ�ȭ��, ��ֹ� ����.
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

// ��ã�� ���� ȣ��Ǿ� �ʱ�ȭ�� �����մϴ�.
void FPathFinderAStar::InitPathFinding()
{
	OpenList.Empty();
	ClosedList.Empty();
	// �� Ʈ�� ����.
	ReleasePathNode(RootNode.Get());
	RootNode.Reset();
	CurrentNode.Reset();
	GoalNode.Reset();

	// ��ũ�� ����Ʈ�� ����. ( = �� ������ �����Ѵ�(Ʈ������).
	//
	// to do here.... 
	// �������� Current Node, ��ǥ������ Goal Node�� �����ȴ�. 
	// �������� Current Node�� OpenList�� �߰��Ǿ�� ��.
	// �������� ������ ClosedList�� �߰��Ǿ�� ��.
	
	// ��� ���̺� �����Ͱ� ����Ʈ ���·� ���´ٸ�
	// ù��° Row�� �����͸� �̿���
	// TSharedPtr<FAstarPathNode> root = MakeShared<FAstarPathNode>();
	// ~> ��ũ���� Code������ root->SoritePathNodeInfo.PathLinkCodes[MAX_ADJACENT] �� �Ҵ�.
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
			// ���� ��ȿ�� PathNode�� ��ġ���� �Ҵ��������. 
			// rootNode->AdjacentNodes[loop]->SetPosition(FVector2D::ZeroVector); 
			// ���� PathNode�� Position ��, �׸��� Code���� Start Ȥ�� Goal ���� �����Ѱ�쿡 ���� ó���� �ʿ�.
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
		// �밢�� �̵�.
		GValue = (FMath::Abs(diffX) + FMath::Abs(diffY)) * 14 + ParentNode->GetGValue();
	}
	else
	{
		// �¿���� �̵�.
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
	TArray<FPathNodeDijkstra> MapList; // ��.
	TArray<FPathNodeDijkstra> PriortyQueue;
	//
	PriortyQueue.HeapPush(MapList[0], FPriorityPredicate());
	for (int32 loopCnt = 0; loopCnt < MapList.Num(); loopCnt++)
	{
		PriortyQueue.HeapPush(MapList[loopCnt], FPriorityPredicate());
	}
}
