+++
date = "2016-06-01T10:37:18+02:00"
draft = false
title = "Eureka!"
+++

### Today is a good day

Our quadtree is fully operational. The tree updates the nodes on the fly, and adapts to the movement of the boids.
A demonstration can be seen by pressing the beak bellow.

{{< youtube Y1zuuNbmelA >}}
<br>
The following picture shows which boids are found by our quadtree search. The search finds all boids which resides inside a quad that intersects the circle. After finding these **potential** close boids, we can check if they are actually inside the desired search range.

![Quadtree search](/img/quadtree-search-1.PNG)

A more clear visualisation of what the search considers.

![Clear visualisation](/img/geohash-query.png)

### Current TODO list
* Implementing the behaviour of our Boids.
* Implement a "find n closest neighbors" search.
