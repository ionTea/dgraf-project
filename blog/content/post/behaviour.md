+++
date = "2016-06-09T13:42:18+02:00"
draft = false
title = "Working boids"

+++

We have implemented the behavior of the boids. They follow a simple set of rules, inspired from 
[*Craig Reynolds*](http://www.red3d.com/cwr/boids/) work in the 80's. The basic rules the boids follow are:

**Separation**: steer to avoid crowding local 

**Alignment**: steer towards the average heading of local flockmates

**Cohesion**: steer to move toward the average position of local flockmates

**Target**: steer towords a general target location

<br>
#### Pseudocode outlining the behavior
Here is pseudocode outlining the behavior mechanism, 
provided by [*Conrad Parker*](http://www.kfish.org/boids/pseudocode.html).

```
Separation(boid bJ)

    Vector c = 0;

    FOR EACH BOID b
        IF b != bJ THEN
            IF |b.position - bJ.position| < 100 THEN
                c = c - (b.position - bJ.position)
            END IF
        END IF
    END

    RETURN c * SEPERATION_BIAS
```

```
Alignment(boid bJ)

    Vector pvJ

    FOR EACH BOID b
        IF b != bJ THEN
            pvJ = pvJ + b.velocity
        END IF
    END

    pvJ = pvJ / N-1

    RETURN (pvJ - bJ.velocity) * ALIGNMENT_BIAS

```

```
 Cohesion(boid bJ)

    Vector pcJ

    FOR EACH BOID b
        IF b != bJ THEN
            pcJ = pcJ + b.position
        END IF
    END

    pcJ = pcJ / N-1

    RETURN (pcJ - bJ.position) * COHESION_BIAS
``` 

```
Target(Boid b)
    Vector place

    RETURN (place - b.position) * TARGET_BIAS
```

The different bias values we use can be altered to change the way
the boids group and move together.

### Last but lot least
A video demonstration
As target we have the big boid, so all other boids will want to move
towards this boid. Later in the video you can see the underlying Quadtree
we use, and how it adapts to the boids positions and how the proximity
algorithm limits the number of potential neighboring boids.

{{< youtube jSBxFq6pMIE >}}
