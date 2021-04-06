# Computational Logistics

## This Repository accompanies our Master-Thesis at the [Institute of Production and Logistics Managment](https://www.jku.at/en/institute-of-production-and-logistics-management/) @ [JKU](http://www.jku.at)

The pdf of the thesis is available [here!](http://google.com) (Coming soon!)

## Directory Tree

* **MasterProject**: Code used to compute the VND solution for the Combined Multi-Trip Capacitated Vehicle Routing Problem with Time Windows (MTCVRPTW) with Dock Scheduling Problem (DSP)

* **Simplified_A**:  VND Code for the simplified DSP

* **Simplified_B**:  VND Code for the simplified MTCVRPTW

* **Daten_*'**:  Test Data for Example in Chapters 3, 4 and 5 and accompanying Python Programs 

## Abstract
<img src="https://user-images.githubusercontent.com/21077042/94988819-52e2bf80-0570-11eb-8ab5-aed473967617.png" width="600" height="400">

Retail companies face a two-fold optimization problem, if they deliver goods from a central warehouse to their stores by a set of delivery trucks: on the one hand delivery trips have to be planned, such that they can be completed in as little time as possible, but on the other hand the loading operation of goods for the delivery trips at the warehouse docks need to be scheduled, such that only a minimal waiting time occurs for the trucks. In the literature the routing part of this problem is known as the multi-trip capacitated vehicle routing problem with time windows (MTCVRPTW) and the dock scheduling problem is abbreviated as DSP.

In this work, we develop a method based on variable neighbourhood descent (VND) metaheuristics to compute an optimized solution for this combined MTCVRPTW and DSP problem by utilizing a solution strategy, in which the MTCVRPTW problem is firstly solved independently and the obtained delivery trips are afterwards scheduled in the DSP, which allows solutions at a very cheap computational cost. Our 5-level VND algorithm is validated on several [Solomon benchmark instances](https://www.sintef.no/projectweb/top/vrptw/solomon-benchmark/) and applied afterwards to a real-world data-set, consisting of 736 stores, to investigate the relation between the number of trucks in the delivery fleet and the number of warehouse docks for a possible extension of the retail company's warehouse docks or delivery truck fleet.
