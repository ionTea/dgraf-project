+++
date = "2016-05-27T13:08:56+02:00"
draft = false
title = "Quadtree - for great success"

+++

We wanted our Boids (currently meaning *Beak-oid like objects*) to have high
performance when looking for other Boids in proximity, both for collision
detection and for our grouping algorithm. So we started implementing a Quadtree
structure to partition our 2D space into areas, and storing the entities in the
Quadtree. Below is an image on the current crude Quadtree (it's not 100% bug free. yet.).

![Quadtree](/img/quadtree-initial.PNG)

As you can see, the area is partitioned into smaller areas where each contain a entity. The upside
of using this partitioning is that the entities that are close to each other in the tree-structure
will be close to each other in real-space as well. This means that the searching for neighbors will be fast. 

![Quadtree again](/img/quadtree-2.png)

### Current Objective
We are currenlty working on getting the Quadtree working as intended. It is having some trouble with null-pointers,
and *"loosing"* entities. But it's just another days work in computer science. 

**WE EAT BUGS FOR BREAKFAST**.

Yolo.
Peace out.

Jonathan and Mathias