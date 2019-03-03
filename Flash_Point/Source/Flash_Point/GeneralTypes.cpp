// Fill out your copyright notice in the Description page of Project Settings.

#include "GeneralTypes.h"
#include "Tile.h"

GeneralTypes::GeneralTypes()
{
}

int32 GeneralTypes::AStarShotest(ATile * start, ATile * goal, TArray<ATile*>& trace)
{
	if (start == goal) {
		trace.Add(start);
		return 0;
	}
	// use a tarray as heap for finding the min node each time
	bool success = false;
	TArray<FSearchNode> searchNodes;
	searchNodes.Add(FSearchNode(0, 0, start));
	ATile* current = nullptr;
	// check on each of the 4 neighbouring nodes
	AEdgeUnit *front, *back, *left, *right = nullptr;
	ATile* temp = nullptr;
	FSearchNode tempNode;
	int32 cost = 0;
	while (searchNodes.Num() > 0) {
		UE_LOG(LogTemp, Warning, TEXT("Before expansion"));
		// get the min node
		searchNodes.HeapPop(tempNode);
		current = tempNode.nodeTile;
		current->SetExpanded(true);
		// front neighbour
		front = current->GetFront();
		back = current->GetBack();
		left = current->GetLeft();
		right = current->GetRight();
		// check on all 4 nodes to see if they can be inserted to the search Nodes
		// do front node first
		if (front) {
			temp = front->GetOtherNeighbour(current);
			if (temp && !temp->IsExpanded()) {
				// mark prev tile node
				temp->SetPrev(current);
				cost = tempNode.cost + 1;
				// if the tile is on fire, it cost extra ap
				if (temp->GetFireStatus() == EFireStatus::Fire) {
					cost++;
				}
				// if the wall is blocked, use very high cost to represent unlikely way
				if (front->IsBlocked()) {
					cost += 200;
				}
				// insert a new node to the heap
				searchNodes.HeapPush(FSearchNode(GetHeuristic(temp, goal) + cost, cost, temp));
			}
		}
		// do back node
		if (back) {
			temp = back->GetOtherNeighbour(current);
			if (temp && !temp->IsExpanded()) {
				// mark prev tile node
				temp->SetPrev(current);
				cost = tempNode.cost + 1;
				// if the tile is on fire, it cost extra ap
				if (temp->GetFireStatus() == EFireStatus::Fire) {
					cost++;
				}
				// if the wall is blocked, use very high cost to represent unlikely way
				if (back->IsBlocked()) {
					cost += 200;
				}
				// insert a new node to the heap
				searchNodes.HeapPush(FSearchNode(GetHeuristic(temp, goal) + cost, cost, temp));
			}
		}
		// do left node
		if (left) {
			temp = left->GetOtherNeighbour(current);
			if (temp && !temp->IsExpanded()) {
				// mark prev tile node
				temp->SetPrev(current);
				cost = tempNode.cost + 1;
				// if the tile is on fire, it cost extra ap
				if (temp->GetFireStatus() == EFireStatus::Fire) {
					cost++;
				}
				// if the wall is blocked, use very high cost to represent unlikely way
				if (left->IsBlocked()) {
					cost += 200;
				}
				// insert a new node to the heap
				searchNodes.HeapPush(FSearchNode(GetHeuristic(temp, goal) + cost, cost, temp));
			}
		}
		// do right node
		if (right) {
			temp = right->GetOtherNeighbour(current);
			if (temp && !temp->IsExpanded()) {
				// mark prev tile node
				temp->SetPrev(current);
				cost = tempNode.cost + 1;
				// if the tile is on fire, it cost extra ap
				if (temp->GetFireStatus() == EFireStatus::Fire) {
					cost++;
				}
				// if the wall is blocked, use very high cost to represent unlikely way
				if (right->IsBlocked()) {
					cost += 200;
				}
				// insert a new node to the heap
				searchNodes.HeapPush(FSearchNode(GetHeuristic(temp, goal) + cost, cost, temp));
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
		while (temp && temp != start) {
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
