# Directory description

 - **wavFiles:** examples of original songs, in stereo

 - **wavMonoFiles:** conversion of the original songs to mono and quantized songs

 - **sampleTests:** music segments already pre-processed and ready to be used as query musics

 - **codebook:** codebook examples

 - **report:** written report explaining our design choices, presenting results from
 parameter variation and conclusions of those results

 - **src:** source code

 - **build:** compiled files
 
 - **docs:** html documentation

# Main programs description

 - **wavhist:** generates a histogram of a channel from the signal provided, possibly being the channel 0 or 1 if the signal is stereo or 0 if the signal is mono

 - **wavcmp:** given two songs, it's able to calculate the distance/error between them

 - **wavquant:** converts a stereo signal into a mono one, quantizes and reduces the sampling size if necessary

 - **wavcb:** from a mono file, generates the representing codebook

 - **wavfind:** given a query music segment, it's predicted the music to which it belongs

# How to run and build

In the root folder of this project("proj2"), following command will compile all the scripts and store them in the "build" folder:

`$ make`

Some examples are :

- **Visualize the frequency signature of the sample01 in mono:**

  Run:     `./build/wavhist wavMonoFiles/sample01.wav 0`
  
  
Majority of the scripts have mandatory arguments. To have information about them call them without parameters,like:

`$ ./build/<script_name>`

To generate the html documentation run `make docs` and visit the file "docs/index.html".