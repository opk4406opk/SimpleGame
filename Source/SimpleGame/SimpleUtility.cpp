#include "SimpleUtility.h"

using namespace PathfindingAlgorithm;

bool FPathFinderAStar::Pathfinding(TArray<int32>& navigateList, int32 startIndex, int32 goalIndex)
{
	InitPathFinding();
	//
	while ((OpenList.Num() != 0) && (IsGoalInOpenList() == false))
	{
		SettingOpenList();
		TSharedPtr<FAstarPathNode> selectNode = SelectLowCostPath();
		if (selectNode.Get() != nullptr)
		{
			SearchAdjacentNodes(selectNode);
		}
	}
	return ExtractNavigatePath(navigateList);
}

// 길찾기 전에 호출되어 초기화를 진행합니다.
void FPathFinderAStar::InitPathFinding()
{
	// 사용했던 노드들을 모두 릴리즈한다.
	ReleaseAllData();
	// 1. Path tree 구성
	// 2. Path tree를 순회하며 장애물 설정 ( setting closed list )
	RootNode = MakeShared<FAstarPathNode>();
	RootNode->SetNodePosition(FVector2D(0, 0));
	// 맵 트리 구성.
	MakePathNodesRecursive(RootNode);
	InitalizeVisitedFlag();
	// 장애물 설정.
	MakeObstaclePathNodesRecursive(RootNode);
	InitalizeVisitedFlag();
	OpenList.Add(TraverseNode);
}

void FPathFinderAStar::CurNodeToClosedList()
{
	ClosedList.Add(TraverseNode);
	for (TWeakPtr<FAstarPathNode> pathNode : OpenList)
	{
		if (pathNode.Pin().IsValid() == false) continue;

		if (pathNode.Pin()->GetNodePosition() == TraverseNode.Pin()->GetNodePosition())
		{
			OpenList.Remove(pathNode);
			break;
		}
	}
}

bool FPathFinderAStar::IsInClosedList(FVector2D searchPos)
{
	bool bFind = false;
	for (TWeakPtr<FAstarPathNode> node : ClosedList)
	{
		if (node.Pin().IsValid() == false) continue;

		if ((node.Pin()->GetNodePosition().X == searchPos.X) &&
			(node.Pin()->GetNodePosition().Y == searchPos.Y))
		{
			bFind = true;
		}
	}
	return bFind;
}

bool FPathFinderAStar::IsInOpenList(FVector2D searchPos)
{
	bool bFind = false;
	for (TWeakPtr<FAstarPathNode> node : OpenList)
	{
		if (node.Pin().IsValid() == false) continue;

		if ((node.Pin()->GetNodePosition().X == searchPos.X) &&
			(node.Pin()->GetNodePosition().Y == searchPos.Y))
		{
			bFind = true;
		}
	}
	return bFind;
}

bool FPathFinderAStar::IsGoalInOpenList()
{
	bool bFind = false;
	for (TWeakPtr<FAstarPathNode> node : OpenList)
	{
		if (node.Pin().IsValid() == true && node.Pin()->bGoalNode)
		{
			bFind = true;
		}
	}
	return bFind;
}

void FPathFinderAStar::SettingOpenList()
{
	for (TWeakPtr<FAstarPathNode> adjacentNode : TraverseNode.Pin()->AdjacentNodes)
	{
		if (adjacentNode.IsValid() == false) continue;

		if ((IsInClosedList(adjacentNode.Pin()->GetNodePosition()) == false) &&
			(IsInOpenList(adjacentNode.Pin()->GetNodePosition()) == false))
		{
			adjacentNode.Pin()->SetParentNode(TraverseNode.Pin());
			adjacentNode.Pin()->CalcHScalarValue(GoalNode.Pin());
			adjacentNode.Pin()->CalcGScalarValue();
			OpenList.Add(adjacentNode);
		}
	}
	CurNodeToClosedList();
}

void FPathFinderAStar::SearchAdjacentNodes(TSharedPtr<FAstarPathNode> selectNode)
{
	for (TWeakPtr<FAstarPathNode> adjacentNode : selectNode->AdjacentNodes)
	{
		if (adjacentNode.IsValid() == false) continue;

		if (IsInClosedList(adjacentNode.Pin()->GetNodePosition()) == false)
		{
			if (IsInOpenList(adjacentNode.Pin()->GetNodePosition()) == false)
			{
				OpenList.Add(adjacentNode);
				adjacentNode.Pin()->SetParentNode(TraverseNode.Pin());
				adjacentNode.Pin()->CalcHScalarValue(GoalNode.Pin());
				adjacentNode.Pin()->CalcGScalarValue();
			}
			else
			{
				if ((adjacentNode.Pin()->GetGScalarValue() < selectNode->GetGScalarValue()))
				{
					selectNode->SetParentNode(adjacentNode.Pin());
					selectNode->CalcHScalarValue(GoalNode.Pin());
					selectNode->CalcGScalarValue();
				}
			}
		}
	}
}

bool FPathFinderAStar::ExtractNavigatePath(TArray<int32>& navigatePath)
{
	TSharedPtr<FAstarPathNode> path = GoalNode.Pin();
	while (path->GetParentNode().IsValid() != false)
	{
		navigatePath.Add(path->NodeIndex);
		path = path->GetParentNode();
	}
	if (navigatePath.Num() == 0)
	{
		return false;
	}
	// 목표노드로부터 백트래킹을 한 결과이므로 반대로 정렬해준다.
	Algo::Reverse(navigatePath);
	return true;
}

TSharedPtr<FAstarPathNode> FPathFinderAStar::SelectLowCostPath()
{
	int32 minCost = 0;
	TSharedPtr<FAstarPathNode> lowCostPath = nullptr;
	if (OpenList.Num() == 0) return nullptr;

	minCost = OpenList[0].Pin()->GetGScalarValue() + OpenList[0].Pin()->GetHScalarValue();
	lowCostPath = OpenList[0].Pin();
	for (int32 i = 1; i < OpenList.Num(); i++)
	{
		int32 cost = OpenList[i].Pin()->GetGScalarValue() + OpenList[i].Pin()->GetHScalarValue();
		if (minCost > cost)
		{
			minCost = cost;
			lowCostPath = OpenList[i].Pin();
		}
	}

	for (TWeakPtr<FAstarPathNode> pathNode : OpenList)
	{
		if (pathNode.Pin()->GetNodePosition() == lowCostPath->GetNodePosition())
		{
			OpenList.Remove(pathNode);
			break;
		}
	}

	ClosedList.Add(lowCostPath);
	TraverseNode = lowCostPath;

	return lowCostPath;
}

void FPathFinderAStar::ReleaseAllData()
{
	// 맵 트리 해제.
	ReleasePathNodesRecursive(RootNode);
	//
	for (TPair<int32, TSharedPtr<FAstarPathNode>> createdNode : CreatedNodes)
	{
		if (createdNode.Value.IsValid() == true)
		{
			createdNode.Value.Reset();
		}
	}
	CreatedNodes.Empty();
	//
	for (TWeakPtr<FAstarPathNode> openNode : OpenList)
	{
		if (openNode.Pin().IsValid() == true)
		{
			openNode.Reset();
		}
	}
	OpenList.Empty();
	//
	for (TWeakPtr<FAstarPathNode> closedNode : ClosedList)
	{
		if (closedNode.Pin().IsValid() == true)
		{
			closedNode.Reset();
		}
	}
	ClosedList.Empty();
	//
	//
	if (RootNode.IsValid() == true)
	{
		RootNode.Reset();
	}
	//
	if (TraverseNode.IsValid() == true)
	{
		TraverseNode.Reset();
	}
	//
	if (GoalNode.IsValid() == true)
	{
		GoalNode.Reset();
	}
}

void FPathFinderAStar::InitalizeVisitedFlag()
{
	for (TPair<int32, TSharedPtr<FAstarPathNode>> node : CreatedNodes)
	{
		node.Value->bVisited = false;
	}
}


void FPathFinderAStar::MakePathNodesRecursive(TSharedPtr<FAstarPathNode> rootNode)
{
	// base case.
	if (rootNode.IsValid() == false) return;
	if (rootNode->bVisited == true) return;
	rootNode->bVisited = true;

	if (rootNode->NodeIndex == StartFloorID)
	{
		rootNode->bStartNode = true;
		TraverseNode = rootNode;
	}
	else if (rootNode->NodeIndex == GoalFloorID)
	{
		rootNode->bGoalNode = true;
		GoalNode = rootNode;
	}

	// 주변 노드를 생성.
	for (int32 loop = 0; loop < MAX_ADJACENT; loop++)
	{
		TWeakPtr<FAstarPathNode> adjacentNode = rootNode->AdjacentNodes[loop];
		if (adjacentNode.IsValid() == true)
		{
			TSharedPtr<FAstarPathNode>* visitedNode = CreatedNodes.Find(adjacentNode.Pin()->NodeIndex);
			if (visitedNode == nullptr)
			{
				if (CreatedNodes.Contains(adjacentNode.Pin()->NodeIndex) == false)
				{
					CreatedNodes.Add(adjacentNode.Pin()->NodeIndex, adjacentNode.Pin());
				}
			}
		}
		else
		{
			TSharedPtr<FAstarPathNode> newNode = MakeShared<FAstarPathNode>();
			newNode->SetNodePosition(FVector2D(FMath::Rand(), FMath::Rand()));
			rootNode->AdjacentNodes[loop] = newNode;
			//
			newNode->AdjacentNodes[loop] = rootNode;
			//
			if (CreatedNodes.Contains(newNode->NodeIndex) == false)
			{
				CreatedNodes.Add(newNode->NodeIndex, newNode);
			}
		}
	}

	//
	if (CreatedNodes.Contains(rootNode->NodeIndex) == false)
	{
		CreatedNodes.Add(rootNode->NodeIndex, rootNode);
	}
	// Traverse adjacent nodes.
	for (TWeakPtr<FAstarPathNode> adjacentNode : rootNode->AdjacentNodes)
	{
		MakePathNodesRecursive(adjacentNode.Pin());
	}
}

void FPathFinderAStar::ReleasePathNodesRecursive(TSharedPtr<FAstarPathNode> rootNode)
{
	// base case.
	if (rootNode.IsValid() == false) return;
	if (rootNode->bVisited == true) return;
	rootNode->bVisited = true;

	for (TWeakPtr<FAstarPathNode> node : rootNode->AdjacentNodes)
	{
		if (node.IsValid() == true)
		{
			ReleasePathNodesRecursive(node.Pin());
			node.Reset();
		}
	}
}

void FPathFinderAStar::MakeObstaclePathNodesRecursive(TSharedPtr<FAstarPathNode> rootNode)
{
	// base case.
	if (rootNode.IsValid() == false) return;
	if (rootNode->bVisited == true) return;
	rootNode->bVisited = true;
	// debug
#if WITH_EDITOR
	int32 DebugNodeIndex = rootNode->NodeIndex;
#endif
	if (IsInClosedList(rootNode->GetNodePosition()) == false)
	{
		ClosedList.Add(rootNode);
	}
	// base case and Traverse other nodes.
	for (TWeakPtr<FAstarPathNode> node : rootNode->AdjacentNodes)
	{
		MakeObstaclePathNodesRecursive(node.Pin());
	}
}

FAstarPathNode::FAstarPathNode()
{
}


FAstarPathNode::~FAstarPathNode()
{
	for (TWeakPtr<FAstarPathNode> node : AdjacentNodes)
	{
		if (node.IsValid() == true)
		{
			node.Reset();
		}
	}
}

void FAstarPathNode::CalcGScalarValue()
{
	if (ParentNode.Pin().IsValid() == false)
	{
		GScalarValue = 0;
		return;
	}

	const int32 diffX = FMath::Abs(NodePosition.X - ParentNode.Pin()->GetNodePosition().X);
	const int32 diffY = FMath::Abs(NodePosition.Y - ParentNode.Pin()->GetNodePosition().Y);
	// 대각선
	if (diffX == 1 && diffY == 1)
	{
		GScalarValue = (diffX + diffY) * 14 + ParentNode.Pin()->GetGScalarValue();
	}
	else // 4방향.
	{
		GScalarValue = (diffX + diffY) * 10 + ParentNode.Pin()->GetGScalarValue();
	}
	
}

void FAstarPathNode::CalcHScalarValue(TSharedPtr<FAstarPathNode> goal)
{
	const int32 diffX = goal->GetNodePosition().X - NodePosition.X;
	const int32 diffY = goal->GetNodePosition().Y - NodePosition.Y;

	HScalarValue = (FMath::Abs(diffX) + FMath::Abs(diffY)) * 10;
}

FVector2D FAstarPathNode::GetNodePosition()
{
	return NodePosition;
}

void FAstarPathNode::SetNodePosition(int32 x, int32 y)
{
	NodePosition.X = x;
	NodePosition.Y = y;
}

void FAstarPathNode::SetNodePosition(FVector2D pos)
{
	NodePosition = pos;
}

TSharedPtr<FAstarPathNode> FAstarPathNode::GetParentNode()
{
	return ParentNode.Pin();
}

int32 FAstarPathNode::GetGScalarValue()
{
	return GScalarValue;
}

int32 FAstarPathNode::GetHScalarValue()
{
	return HScalarValue;
}

void FAstarPathNode::SetParentNode(TSharedPtr<FAstarPathNode> node)
{
	ParentNode = TWeakPtr<FAstarPathNode>(node);
}