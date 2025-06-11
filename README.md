# SandpileModel
 A sandpile model that can save its states to a BMP image.

## Objective

This project implements a simplified version of the Abelian Sandpile Model, a cellular automaton that models the dynamic behavior of sand grains on a grid. The simulation allows for dynamic grid resizing and saves its states as BMP images. The program is a console application written in C++ and supports various command-line arguments for customization.

The Abelian Sandpile Model, also known as the Bak-Tang-Wiesenfeld model, is a mathematical model used to study self-organized criticality. It simulates a grid where each cell can hold a certain number of "sand grains." When a cell contains more than a specific threshold of grains (typically 4), it becomes unstable and "topples," distributing one grain to each of its four neighboring cells. This process can trigger further topplings, leading to avalanches that propagate across the grid. The model is notable for its Abelian property, meaning the final stable state is independent of the order in which topplings occur. The sandpile model is widely studied in fields like physics, mathematics, and computer science due to its emergent complex patterns and critical behavior.

![Example of concluding state of sandpile with input (0 0 100000)](https://raw.githubusercontent.com/AnastasiaLeon/SandpileModel/refs/heads/main/outputDir/sandpile100K/final_state.bmp)


## Features

### Dynamic Grid: 

The grid size adjusts dynamically if sand grains attempt to topple beyond the current boundaries.

### BMP Output: 

Saves the state of the sandpile as BMP images with a 4-bit color depth, mapping the number of grains to specific colors.

### Customizable Colors: 

Users can specify colors for different grain counts (0, 1, 2, 3, and >3 grains) via command-line arguments.

### Input File Support: 

Initial conditions are read from a tab-separated values (TSV) file.

### Iteration Control: 

Supports a maximum number of iterations or runs until the model reaches a stable state.

### Save Frequency: 

Allows saving intermediate states at a specified frequency, or only the final state.

### No STL Containers:

The implementação avoids using standard library containers, relying on raw arrays for memory management.

## Command-Line Arguments
The program accepts the following command-line arguments:

**-i, --input <file>**: Path to the TSV file containing the initial state of the sandpile (required).

**-o, --output <directory>**: Path to the directory where BMP images will be saved (required).

**-m, --max-iter <number>**: Maximum number of iterations (optional; if not specified, runs until stable).

**-f, --freq <number>**: Frequency of saving BMP images (e.g., every nth iteration; if 0, only the final state is saved) (required).

**-c0, --color0 <color>**: Color for cells with 0 grains (default: white).

**-c1, --color1 <color>**: Color for cells with 1 grain (default: green).

**-c2, --color2 <color>**: Color for cells with 2 grains (default: purple).

**-c3, --color3 <color>**: Color for cells with 3 grains (default: yellow).

**-c4, --color4 <color>**: Color for cells with >3 grains (default: black).

## Supported Colors

The following colors are available for the -c0 to -c4 arguments:

- white
- green
- purple
- yellow
- black
- maroon
- pink
- red
- orange
- beige
- grey
- violet
- cyan
- blue
- navy
- lime

## Input Format

The initial state of the sandpile is specified in a TSV file. Each line represents a cell and contains three values separated by tabs:

x-coordinate: The x-coordinate of the cell (int16_t).
y-coordinate: The y-coordinate of the cell (int16_t).
Grain count: The number of sand grains in the cell (uint64_t).

### Example TSV file (input.tsv):

0   0   5

1   0   2

0   1   3

This example specifies a grid with cells at (0,0) with 5 grains, (1,0) with 2 grains, and (0,1) with 3 grains. The grid size is automatically calculated as the smallest rectangle encompassing all specified cells.

## Output

The program generates BMP images in the specified output directory, named as follows:

- state_0.bmp: Initial state (if --freq is not 0).
- state_n.bmp: Intermediate states at iterations divisible by the frequency value (if applicable).
- final_state.bmp: Final stable state or state at the maximum iteration.

### Each BMP image:

- Has dimensions equal to the current grid size.
- Uses a 4-bit color depth (16 colors).
- Maps grain counts to colors as specified by the -c0 to -c4 arguments (default: 0=white, 1=green, 2=purple, 3=yellow, >3=black).
- Encodes two pixels per byte, with the left pixel in the high 4 bits and the right pixel in the low 4 bits.

## Model Details

### Initialization:

Sand grains are only added during initialization from the input file.

### Update Rule:
A cell with more than 3 grains topples, distributing one grain to each of its four neighbors (up, down, left, right). The next state depends only on the previous state.

### Grid Expansion: 

If grains attempt to topple beyond the grid boundaries, the grid expands in the corresponding direction (up, down, left, or right).

### Stability: 

The simulation stops when the model reaches a stable state (no cell has more than 3 grains) or the maximum iteration limit is reached.

## Building and Running

### Prerequisites

A C++ compiler (e.g., g++).
A system supporting file I/O and directory creation.

### Compilation

g++ -o SandpileModel main.cpp sandpile.cpp bmp.cpp

### Example Usage

```./SandpileModel -i input.tsv -o outputDir -m 100 -f 10 -c0 white -c1 green -c2 purple -c3 yellow -c4 black```

This command runs the simulation with:

- Input file input.tsv.

- Output directory outputDir (a new directory will be created if it doesn't exist; if it exists, a suffix like (1) is added).
- Maximum 100 iterations.
- BMP images saved every 10 iterations.
- Default color mapping.

## Examples

Example input files are located in the inputExamples directory, and their corresponding results can be found in the outputDir directory.

1. Running for sandpile100K.tsv:

```./SandpileModel -i ../inputExamples/sandpile100K.tsv -o ../outputDir -f 50000```

(All color arguments are set to default values.) 

![Final state)](https://raw.githubusercontent.com/AnastasiaLeon/SandpileModel/refs/heads/main/outputDir/sandpile100K/final_state.bmp)

2. Running for sandpileFive10K.tsv:

```./SandpileModel -i ../inputExamples/sandpileFive10K -o ../outputDir -f 10000 -c0 beige -c1 maroon -c2 red -c3 pink```

(All color arguments except for -c4 (--color4) are specified; -c4 is set to its default state.)

![Final state)](https://raw.githubusercontent.com/AnastasiaLeon/SandpileModel/refs/heads/main/outputDir/sandpileFive10K/final_state.bmp)

## Notes

- The program ensures memory is managed manually without STL containers.
- The BMP format uses a 16-color palette, with each pixel encoded in 4 bits.
- The grid size is dynamically calculated based on the input file and may grow during simulation.
- Error handling includes checks for invalid input files, missing arguments, and directory creation issues.

