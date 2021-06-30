# TkLayout_FCCee
Description

Some basic description and information is already available at: https://github.com/tkLayout/tkLayout/
This document’s goal is mostly to add some precision on specific parameters, as well as list the edits and changes I have brought.

Compilation/Install

source setup_centos7.sh
mkdir build
cd build
cmake ..
make -j8
make install
cd ..
cp config/* $TKG_STANDARDDIRECTORY/config/.

Run

cd tkLayout/geometries/FCC
tklayout --geometry --material -r FCChh_Option4 ls FCChh_Option4.cfg

Some useful additional command options are:
-n arg
-N arg
to set the number of tracks for geometry and material & resolution calculations respectively, with ‘arg’ the number of tracks (100 by defaults)


Source code edits

Polar angle

The default FCChh installation of tkLayout uses the pseudorapidity eta. The  material budget and resolution plots in terms of the polar angle theta has been implemented.

Disc spacing

By default, tkLayout has its own formula for z-axis disc placement in the endcaps. In the current version,  equally-spaced discs between the innerZ and outerZ values specified in your configuration file has been implemented

Resolution comparison plots

The ROOT macro sigma.cc allows to compare resolution plots between two runs by automatically calculating and graphing the ratio of all corresponding resolutions files one by one, and saving the output plots in the .root and .png format. The macro has to be edited beforehand in order to specify the path to the result folders of the two runs and to the output folder.


Example sample file (cld.cfg)

The basic configuration file architecture (see cld.cfg “CLD reference config_results” for reference) is:
Simulation Parameters
Beampipe Parameters
Tracker 1
	Barrel 1
	Barrel 2…
	Endcap 1
	Endcap 2…
Tracker 2
	Barrel 1
etc.
Any setting defined within a section is applied to all branching subsections. If the setting is reiterated with a different value in a following subsection, it will overwrite the setting with the new value for the rest of the subsection.

Simulation Parameters

Just any other section of the code, simulation parameters can be defined in an annex .cfg file and referenced in the main configuration file (ex: material files).
Define general simulation settings such as magnetic field value and eta regions.

Beampipe Parameters
Radius, thickness, radiation and interaction lengths. The values indicated for these last two items are not defined in g/cm2 or even cm, but are unit-less. They correspond to X/X0 value of the beampipe at eta=0 (theta=90°), and therefore have to be calculated manually by the user based on the beampipe thickness and material.
Tracker
	bigDelta: shift in r between neighbor ladders in the same (x,y) plane (barrel), or two rings of the same disc (endcap)
smallDelta:  shift in r between neighbor ladders along the z-axis (barrel), or two adjacent modules in the same ring (endcap). Needs to be inferior or equal to bigDelta
Barrel

	length: ladder length (z-axis)
width: ladder width ((x,y) plane)
numberRods: number of ladders for a given layer (along phi, i.e. in the (x,y) plane)
startZMode: moduleedge or modulecenter, position of the first module at z=0
Endcap
	
	moduleShape: rectangular or wedge. If rectangular, specify width. If wedge, specify waferDiameter.
	
	waferDiameter = twice the distance between any corner of the trapezoid and the center of the trapezoid (crossing point between all perpendicular bisectors)
