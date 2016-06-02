+++
date = "2016-06-02T14:10:48+02:00"
draft = true
title = "floats"

+++

### Floats, floats, why art though keff

Since starting to implement our boids behaviour the program has
showing wierd behaviour. Sometimes the quadtree insert goes into an infinite
loop. Sometimes we get read-access violation. Sometimes we get wired null
references. At first we thought our cleanup of the tree might be wrong.
Maybe we deleted nodes that were in use? That would explain the read-access
violation at least, but not all our problems. After some serious debuging
we noticed something.

```
 size = 9.697e-42
```

When we implemented the behaviour, we didn't finish with the seperation behaviour,
to prevent the boids from being on the same spot. They do however have cohesion,
so the will move towords each other. So they all ocupied the same space. Mother fuckers.

This might cause our floating precision to be off, since floats are **really** bad
at comparisons. So when the areas are that small we are unable to decide where to
place a boid in the tree, leading to unpredictable behaviour.