#pragma once

#include "CoreMinimal.h"
#include "Runtime/CoreUObject/Public/UObject/Class.h"
#define MAP_SIZE_X 10
#define MAP_SIZE_Y 10
#define INFINITY_VALUE 9999999
#define MAX_ADJACENT 4

namespace PathfindingAlgorithm
{
	struct FSortiePathNodeInformation
	{
	public:
		int32 FloorIndex;
		FName FloorCode;
		FName AdjacentLinkCodes[MAX_ADJACENT];
	};

	struct FAstarPathNode
	{
	public:
		FAstarPathNode();
		~FAstarPathNode();

		void CalcGValue();
		void CalcHValue(FAstarPathNode* goal);
		FVector2D GetPosition();
		void SetPosition(int x, int y);
		void SetPosition(FVector2D pos);
		FAstarPathNode* GetParentNode();
		int32 GetGValue();
		int32 GetHValue();
		void SetParentNode(FAstarPathNode* node);
	public:
		TSharedPtr<FAstarPathNode> AdjacentNodes[MAX_ADJACENT];
		FSortiePathNodeInformation SoritePathNodeInfo;
		bool bGoalNode = false;
	private:
		TSharedPtr<FAstarPathNode> ParentNode;
		FVector2D Position;
		int32 GValue = 0;
		int32 HValue = 0;
	};

	class FPathFinderAStar
	{
	public:
		FPathFinderAStar();
		~FPathFinderAStar();
		
		//길찾기를 시작합니다.
		bool Pathfinding(TArray<int32>& navigateList, int32 startIndex, int32 goalIndex);
	private:
		TArray<FAstarPathNode*> OpenList;
		TArray<FAstarPathNode*> ClosedList;
		TSharedPtr<FAstarPathNode> CurrentNode; // 시작점으로 시작해서 목표까지 찾아가는 노드.
		TSharedPtr<FAstarPathNode> GoalNode;
		TSharedPtr<FAstarPathNode> RootNode;
	private:
		void InitPathFinding();
		void CurNodeToClosedList();
		bool IsInClosedList(int searchPosX, int searchPosY);
		bool IsInOpenList(int searchPosX, int searchPosY);
		bool IsGoalInOpenList();
		void SetOpenList();
		void SearchAdjacentNodes(FAstarPathNode* selectNode);
		bool ExtractNavigatePath(TArray<int32>& navigatePath);
		void MakePathNode(FAstarPathNode* rootNode);
		void ReleasePathNode(FAstarPathNode* rootNode);
		FAstarPathNode* SelectLowCostPath();
	};

	struct FPriorityPredicate
	{
		bool operator()(const FPathNodeDijkstra& A, const FPathNodeDijkstra& B) const
		{
			// Inverted compared to std::priority_queue - higher priorities float to the top
			return A.Distance > B.Distance;
		}
	};

	struct FPathNodeDijkstra
	{
	public:
		int32 Distance;
		FVector2D Position;
		TArray<FPathNodeDijkstra> Childs;
	};

	class FPathFinderDijkstra
	{
	public:
		TArray<FPathNodeDijkstra> Pathfinding(FVector2D start, FVector2D goal);
	private:
	};
}