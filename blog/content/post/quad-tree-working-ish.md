+++
date = "2016-05-30T13:15:34+02:00"
draft = false
title = "Quad tree working (ish)"
+++

We got the quadtree to work but without any restructuring. ie we do not remove empty nodes. Performance is not the priority right now so we ignore that for now. We solved the issue by rewriting the insert and update-node functions from scratch and made them much more clean. The search for beak neighbors are the next thing to do.

![Drawing debug, showing node/entity relation](/img/quadtree-working-5.PNG)

![1936 boids](/img/quadtree-working-3.PNG)

![Closeup](/img/quadtree-working-4.PNG)
