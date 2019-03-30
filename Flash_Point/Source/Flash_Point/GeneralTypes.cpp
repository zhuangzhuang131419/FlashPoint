// Fill out your copyright notice in the Description page of Project Settings.

#include "GeneralTypes.h"
#include "Tile.h"

GeneralTypes::GeneralTypes()
{
}

int32 GeneralTypes::AStarShotest(int32 maxDepth, ATile * start, ATile * goal, TArray<ATile*>& trace, bool countSmoke)
{
	if (start == goal) {
		trace.Add(start);
		return 0;
	}
	// use a tarray as heap for finding the min node each time
	bool success = false;
	TArray<FSearchNode> searchNodes;
	FSearchNode startNode(0, 0, start);
	searchNodes.Add(FSearchNode(0, 0, start));
	ATile* current = nullptr;
	// check on each of the 4 neighbouring nodes
	TArray<AEdgeUnit*> towardsDirection;
	ATile* temp = nullptr;
	FSearchNode tempNode;
	int32 cost = 0;
	int32 tempCost = 0;
	while (searchNodes.Num() > 0) {
		UE_LOG(LogTemp, Warning, TEXT("Before expansion"));
		// get the min node
		searchNodes.HeapPop(tempNode);
		current = tempNode.nodeTile;
		current->SetExpanded(true);
		// front neighbour
		towardsDirection.Add(current->GetFront());
		towardsDirection.Add(current->GetBack());
		towardsDirection.Add(current->GetLeft());
		towardsDirection.Add(current->GetRight());
		// check on all 4 nodes to see if they can be inserted to the search Nodes
		for (AEdgeUnit* edge : towardsDirection) {
			if (edge && !edge->IsBlocked()) {
				temp = edge->GetOtherNeighbour(current);
				if (temp && !temp->IsExpanded()) {			
					cost = tempNode.cost + 1;
					// if the tile is on fire, it cost extra ap
					if (temp->GetFireStatus() == EFireStatus::Fire) {
						cost++;
					}
					// if we also count smoke, add another cost
					if (countSmoke) {
						if (temp->GetFireStatus() == EFireStatus::Smoke) {
							cost++;
						}
					}
					// check if this path is better
					tempCost = temp->GetPathCost();
					if (tempCost == -1 || cost < tempCost) {
						// set the new path cost for the tile
						temp->SetpathCost(cost);
						// mark prev tile node
						temp->SetPrev(current);
						// insert a new node to the heap
						searchNodes.HeapPush(FSearchNode(GetHeuristic(temp, goal) + cost, cost, temp));
					}
				}
			}
		}
		UE_LOG(LogTemp, Warning, TEXT("After expansion"));
		UE_LOG(LogTemp, Warning, TEXT("Heap size: %d"), searchNodes.Num());
		// check if min of the heap is a goal state
		if (searchNodes.Num() > 0 && searchNodes.HeapTop().nodeTile == goal) {
			success = true;
			break;
		}
	}
	if (searchNodes.Num() > 0 && success) {
		// use the first node of the searchNodes to fillup trace
		temp = goal;
		while (temp && temp != start && trace.Num() < 80) {
			// insert at begining each time
			UE_LOG(LogTemp, Warning, TEXT("trace adding"));
			trace.Add(temp);
			temp = temp->GetPrev();
		}
		trace.Add(start);
		// return the cost to the goal node
		return searchNodes.HeapTop().cost;
	}
	else {
		return 0;
	}
}

int32 GeneralTypes::GetHeuristic(ATile * cur, ATile * goal)
{
	int32 value = 0;
	int32 curX, curY, goalX, goalY;
	if (cur->GetLocation(curX, curY) && goal->GetLocation(goalX, goalY)) {
		value =  FMath::Abs<int32>(curX - goalX) + FMath::Abs<int32>(curY - goalY);
	}
	return value;
}

GeneralTypes::~GeneralTypes()
{
}
