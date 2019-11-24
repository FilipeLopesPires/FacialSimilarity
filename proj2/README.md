# Directory description

 - **wavFiles:** examples of original songs, in stereo

 - **wavMonoFiles:** conversion of the original songs to mono and quantized songs

 - **sampleTests:** music segments already pre-processed and ready to be used as query musics

 - **codebook:** codebook examples

 - **report:** written report explaining our design choices, presenting results from
 parameter variation and conclusions of those results

 - **src:** source code

 - **build:** compiled files

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

  Run:     `$ ./build/wavhist wavMonoFiles/sample01.wav 0`

- **Compare sample01 with sample04:**

  Run:     `$ ./build/wavcmp wavMonoFiles/sample01.wav wavMonoFiles/sample04.wav`

- **Quantize sample01 with 8 bits and a reduction sampling rate factor of 4:**

  Run:     `$ ./build/wavquant -q 8 -r 4 wavFiles/sample01.wav wavMonoFiles/sample01.wav`

- **Generate the codebook for sample01 with block size of 800 samples, overlap factor of 20% and error threshold of 1%:**

  Run:     `$ ./build/wavcb wavMonoFiles/sample01.wav 800 0.2 0.01 2 codebook/sample01`

- **Generate the same codebook as the last example, but with parallelism(2 external threads and 2 internal threads):**

  Run:     `$ ./build/wavcb wavMonoFiles/sample01.wav 800 0.2 0.01 2 codebook/sample01 2 2`

- **Verify to which music does a a given music segment belong to:**

  Run:     `$ ./build/wavfind sampleTests/sampleTest.wav 200 0.1 codebook`
  
  
Majority of the scripts have mandatory arguments. To have information about them call them without parameters,like:

`$ ./build/<script_name>`