#pragma once

#include "CoreMinimal.h"
#include "Runtime/CoreUObject/Public/UObject/Class.h"

#define MAX_ADJACENT 4

namespace PathfindingAlgorithm
{
	struct FAstarPathNode
	{
	public:
		FAstarPathNode();
		~FAstarPathNode();

		void CalcGScalarValue();
		void CalcHScalarValue(TSharedPtr<FAstarPathNode> goal);
		FVector2D GetNodePosition();
		void SetNodePosition(int32 x, int32 y);
		void SetNodePosition(FVector2D pos);
		TSharedPtr<FAstarPathNode> GetParentNode();
		int32 GetGScalarValue();
		int32 GetHScalarValue();
		void SetParentNode(TSharedPtr<FAstarPathNode> node);
	public:
		TWeakPtr<FAstarPathNode> AdjacentNodes[MAX_ADJACENT] = { nullptr, nullptr, nullptr, nullptr };
		bool bGoalNode = false;
		bool bStartNode = false;
		bool bVisited = false;
		int32 NodeIndex;
	private:
		TWeakPtr<FAstarPathNode> ParentNode;
		FVector2D NodePosition = FVector2D::ZeroVector;
		int32 GScalarValue = 0;
		int32 HScalarValue = 0;
	};
	class FPathFinderAStar
	{
	public:
		FPathFinderAStar();
		~FPathFinderAStar();
		
		//길찾기를 시작합니다.
		bool Pathfinding(TArray<int32>& navigateList, int32 startIndex, int32 goalIndex);
	public:
		int32 StartFloorID;
		int32 GoalFloorID;
	
	private:
		TMap<int32, TSharedPtr<FAstarPathNode>> CreatedNodes;
		TArray<TWeakPtr<FAstarPathNode>> OpenList;
		TArray<TWeakPtr<FAstarPathNode>> ClosedList;
		TWeakPtr<FAstarPathNode> TraverseNode = nullptr; // 시작점으로 시작해서 목표까지 찾아가는 노드.
		TWeakPtr<FAstarPathNode> GoalNode = nullptr;
		TSharedPtr<FAstarPathNode> RootNode = nullptr;

	private:
		void InitPathFinding();
		void CurNodeToClosedList();
		bool IsInClosedList(FVector2D searchPos);
		bool IsInOpenList(FVector2D searchPos);
		bool IsGoalInOpenList();
		void SettingOpenList();
		void SearchAdjacentNodes(TSharedPtr<FAstarPathNode> selectNode);
		bool ExtractNavigatePath(TArray<int32>& navigatePath);
		void MakePathNodesRecursive(TSharedPtr<FAstarPathNode> rootNode);
		void ReleasePathNodesRecursive(TSharedPtr<FAstarPathNode> rootNode);
		void MakeObstaclePathNodesRecursive(TSharedPtr<FAstarPathNode> rootNode);
		TSharedPtr<FAstarPathNode> SelectLowCostPath();
		void ReleaseAllData();
		void InitalizeVisitedFlag();
	};

	struct FPriorityPredicate
	{
		//bool operator()(const FPathNodeDijkstra& A, const FPathNodeDijkstra& B) const
		//{
		//	// Inverted compared to std::priority_queue - higher priorities float to the top
		//	return A.Distance > B.Distance;
		//};
	};

	struct FPathNodeDijkstra
	{
	public:
		int32 Distance;
		FVector2D Position;
	};

	class FPathFinderDijkstra
	{
	public:
		TArray<FPathNodeDijkstra> Pathfinding(FVector2D start, FVector2D goal);
	private:
	};
}