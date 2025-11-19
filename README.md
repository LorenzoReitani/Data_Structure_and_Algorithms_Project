# Data Structure and Algorithms Final Exam @ Polimi 🧑‍💻

Impletentation of an algorithm (in C) to fulfill correctness and efficiency specs

## Goal 🏎️

The project aims to plan the shortest route with the fewest stops between two given service stations on a highway. Each service station has a unique distance from the start of the highway and offers electric rental vehicles with a specified range. A journey consists of consecutive stops at service stations, with the driver unable to backtrack and renting a new vehicle at each stop. The objective is to find the route with the minimum number of stops, favoring routes with shorter distances from the start of the highway in case of ties. Routes are represented as sequences of distances from the start of the highway, and the chosen route must not have another route with the same final sequence of stops preceded by a station with a shorter distance.

## Implementation 

The solution structures the problem as a layered search over a dynamically maintained highway model, using two key data abstractions: a sorted doubly linked list of service stations and, inside each station, a BST that stores available vehicles by autonomy. The smart idea is to exploit the maximum available autonomy at each station to prune the search space aggressively: instead of exploring arbitrary sequences of stops, the algorithm expands only the stations reachable with the local maximum range, generating a tree of “tappe” (stages) and stopping as soon as the destination becomes reachable. This effectively performs a breadth-first expansion constrained by vehicle autonomy, guaranteeing the minimum number of stops and naturally favoring routes closer to the start due to the ordered structure of the station list. An additional enhancement is the dual-direction support: the algorithm automatically switches between forward and backward search depending on start/end ordering, reusing the same logic with minimal duplication. The combination of ordered station traversal, BST-based autonomy lookup, and incremental pruning yields an efficient and elegant solution to the shortest-route-with-fewest-stops problem.

## Grade 🏆

30/30 cum laude
