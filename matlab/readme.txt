FastSLAM Simulator (version 1.0)
------------------

This simulator demonstrates simple implementations of FastSLAM versions 1.0 and 2.0.
It permits simple configuration via 'configfile.m' to perform SLAM with various 
control parameters, noises, etc. The simulator assumes known data association.

The key files in this simulator are called 'fastslam1_sim.m' and 'fastslam2_sim.m'
for FastSLAM 1.0 and 2.0, respectively. These files are in the two subdirectories
in this collection.

The runtime of this simulator is very slow due, in part, to the graphics of the
simulator animations, and also to the use of 'for'-loops. Loops in Matlab are very 
inefficient, but it is difficult to avoid them for this task without making the
code very obscure. Hopefully, however, the basic algorithms are clear enough to
assist development in a more suitable language.

Tim Bailey and Juan Nieto
2004.


