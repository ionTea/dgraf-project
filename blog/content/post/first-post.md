+++
date = "2016-05-24T14:21:19+02:00"
draft = false
title = "Project Specification"
+++

# Project Specification

## Background
Simulation of group behaviour can take many forms, and can be done in both sophisticated as well as simple ways. Allowing the different agents to interact with each other as well as other objects in an environment can give rise to interesting behaviour. There is however lots of things to take into account when building this kind of grouping behaviour, for example grouping of actors, collision detection as well as collision avoidance (actively moving out of the way of something).

## Problem
We are interested in implementing a boid system, where we simulate the behaviour of predator and prey, where the prays exhibit a grouping behavior of staying together while avoiding the predator.

[Example of fishes + shark](https://www.youtube.com/watch?v=FQ4YJbD0zRM)

[Example of birds + evil birds](https://www.youtube.com/watch?v=rN8DzlgMt3M)

## Implementation
We are not very interested in the aesthetics, we just want to model the behaviour. So if we render fishes or if we render dots, it doesn’t make a difference. So we are unsure if we should use Unity at all, since it might just be unnecessary overhead, and we can’t leverage any of the knowledge from the labs. We are currenly thinking of using SFML (http://www.sfml-dev.org/) just to render stuff, and implement the actor behaviour in C++.

## Evaluation
Does it look bonkes siq nice? Then we are happy.

## Contingency Plan
We will try to implement the behaviour ourselves but using a library such as RVO is not out of the question.
