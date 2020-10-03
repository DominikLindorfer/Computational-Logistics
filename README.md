# Computational Logistics

## This Repository accompanies our Master-Thesis at the [Institute of Production and Logistics Managment](https://www.jku.at/en/institute-of-production-and-logistics-management/) @ [JKU](http://www.jku.at)

The pdf of the thesis is available [here!](http://google.com) (Coming soon!)

## Directory Tree

* **MasterProject**: Code used to compute the VND solution for the Combined Multi-Trip Capacitated Vehicle Routing Problem with Time Windows (MTCVRPTW) with Dock Scheduling Problem (DSP)

* **Simplified_A**:  VND Code for the simplified DSP

* **Simplified_B**:  VND Code for the simplified MTCVRPTW

* **Daten_*'**:  Test Data for Example in Chapters 3, 4 and 5 and accompanying Python Programs 

## Abstract

![Abstract](https://user-images.githubusercontent.com/21077042/94988819-52e2bf80-0570-11eb-8ab5-aed473967617.png)

When a retail company tries to optimize and extend their distribution operation it often faces a two-fold problem, if they deliver goods from a central warehouse to their stores by a set of delivery trucks: on the one hand delivery trips have to be planned, such that they can be completed in as little time as possible, but on the other hand the loading operation of goods for the delivery trips at the warehouse docks need to be scheduled, such that only a minimal waiting time occurs for the trucks. The latter is because each dock can only  load one truck at a time and thus a waiting time occurs, if a truck has to wait for a dock to become free for loading. Additionally, since the stores have opening hours during which the store is open to receive deliveries from trucks, a waiting time occurs if a truck arrives to early at a store. In the literature the routing part of this problem is known as the multi-trip capacitated vehicle routing problem with time windows (MTCVRPTW) and the dock scheduling problem is abbreviated as DSP.

In this work, we develop a method based on metaheuristics to compute an optimized solution for this combined MTCVRPTW and DSP problem by utilizing a solution strategy, in which the MTCVRPTW problem is firstly solved independently and the obtained delivery trips are afterwards scheduled in the DSP. We show that such a relaxation of the interdependence between the MTCVRPTW and the DSP reduces the size of the solution space significantly and thus allows a solution at a very cheap computational cost. Our 5-level variable neighbourhood descent algorithm is validated on several Solomon benchmark instances and applied afterwards to a real-world data-set, consisting of 736 stores, to investigate the relation between the number of trucks in the delivery fleet and the number of warehouse docks, in order to provide assistance for the possible extension of the company's warehouse docks or delivery truck fleet.
