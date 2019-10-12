# Directory description

 - **input:** texts to use as training data for our model

 - **output:** files with data relative to the output of the main programs (graphs,
 entropy values for different parameters, generated texts)

 - **report:** written report explaining our design choices, presenting results from
 parameter variation and conclusions of those results

 - **src:** source code

 - **build:** compiled files
 
 - **docs:** html documentation

# Main programs description

 - **fcm:** Parses a list of files to train the model and
 presents the entropy of the texts on those files
 
 - **generator:** Parses a list of files to train the model and
 generates text to a file based on the occurrences
 statistics of contexts and letters after a context

# How to run and build

On the same folder of this readme file, to compile and run the program:

- **fcm**

  Compile: `make fcm`
  
  Run:     `./fcm ...`
  
- **generator**

  Compile: `make generator`
  
  Run:     `./generator ...`
  
Both *fcm* and *generator* programs have mandatory arguments. To have information
about them use the `-h` option.

To generate the html documentation run `make docs`.