Atharva Pandkar.

Operating System :- Windows 11
IDE:- Visual Studio 

The following files should be present at the time of running the code:-
olympus(Dir for the photos)
Driver.cpp
Filter.cpp
Filter.h
baseline.cpp
baseline.h
histogram.cpp
histogram.h
law.cpp
law.h
multihistogram.cpp
multihistogram.h
textureColor.cpp
textureCOlor.h

For running the program we should run the Driver.cpp

Driver.cpp takes in command line arguments.
1.<Target_File> :- This is the image loaction for the target to be searched
2.<Dir_name> :- This is the directory that contains the database with which the target image is checked against
3.<Type> :- There are 5 types avaliable based on their numbers
		1. Baseline matching 9x9
		2. Histogram matching
		3. Multi Histogram matchinng
		4. Texture and Color matching
		5. Law filters
4.<Number_of_results> :- A number representing the total number of images to display.
