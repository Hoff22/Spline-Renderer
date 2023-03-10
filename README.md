
# Spline Renderer

Bezier curves or Splines are interpolated paths defined by a sequence of nodes. 

# What Is It Made Of?

A curve can be defined by the interpolation of all the points along a start and end point.

![](https://i.imgur.com/dTeP3G4.png)

    p3 ~= p1 + (p2 - p1) * 0.3

In the equation above 0.3 is a value that defines where along the curve segment we want our point.

We define this interpolation function as a **linear** interpolation and will denote it as:

    lerp(p1,p2,t)

Now if we add one more point to our segment and perform the following:

    p4 = lerp(p1,p3,0.3)
    p5 = lerp(p3,p2,0.3)

We'll have:

![](https://i.imgur.com/QPnVqXD.png)

and now if we continuously linearly interpolate between (p1 and p3), (p3 and p2) and (p4 and p5) we'll have a curve that looks like this:

![](https://i.imgur.com/Kidb6Lg.png)

this is what is called a quadratic interpolation. Remember:

    lerp(p1,p2,t) =>
    p1 + (p2 - p1) * t =>
    p1 + p2t - p1t

so...

    lerp(lerp(p1,p3,t), lerp(p3,p2,t), t) =>
    lerp(p1 + p3t - p1t, p3 + p2t - p3t, t) =>
    (p1 + p3t - p1t) + (p3 + p2t - p3t)*t - (p1 + p3t - p1t)*t =>
    p1 - 2*p1*t + 2*p3*t - p1*t^2 + p2*t^2

as you can see the interpolations give rise to a quadratic equation.

As you can imagine this recursive process has no reason to end at 2 iterations, the most used type of bezier curve in the industry is a cubic bezier curve (one step after the quadratic just described) and is this that I choose to implement.

As a linear curve has zero so called 'control points' and a quadratic curve has one the cubic bezier curve has two.

![](https://i.imgur.com/P515Wg2.gif)