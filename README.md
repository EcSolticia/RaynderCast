# RaynderCast

RaynderCast is a C++ library for creating raycasting-based minimal walking simulators. Like traditional raycasting-based games, such as Wolfenstein3D, it creates an image based on the magnitude of arrays of rays projected into a 2D grid. This magnitude corresponds to a distance: the distance from the "camera" to each **ray-to-*solid grid cell wall*** collision point. These distance values are usually taken to be inversely proportional to the line-height of the rectangle that is drawn to represent the area around the collision point. 
RaynderCast differs in allowing for different render distance functions. While the space and geometry remains Euclidean, the distance to each point specifically for rendering purposes can be custom-set by a C++ lambda. Consequently, this need not be Euclidean at all.

> [!WARNING]
> RaynderCast has mainly been a "learning project". Due to initial idiosyncratic choices regarding the modificaton of the raycasting-based rendering method clashing with what I later decided would be needed for the project, I decided to call it a sunk cost. This is the reason the README is not as detailed, and it completely lacks documentation. I may approach the same idea later on.
