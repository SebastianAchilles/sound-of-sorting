# The Sound of Sorting

The Sound of Sorting is an "audibilization" of common sorting algorithms. The
idea is that you can listen to the sorting algorithm as it works its magic.

### WARNING

Before running the program, **turn down the system volume to the lowest
non-zero setting**! `sound-of-sorting` outputs at maximum amplitude. Not
complying may not only annoy your neighbors but also damage your speakers!
This is especially true when using the `--mode waveform`.

## System requirements

The program should run on any Linux system. To play sounds it uses the SDL2
library but that is linked statically, so you'll only need to install it if you
intend to compile from source; but not to run the executable. On Fedora, you
can install SDL2 static libs by running

    sudo dnf install SDL2-static

## Running the program

Download the tarball from the *Releases* tab. Navigate to the download location,
extract it and run. In the terminal:

    cd ~/Downloads
    tar xzf sound-of-silence.tar.gz
    ./sound-of-silence -s quick
    
## Usage

**SYNOPSIS**

    ./sound-of-sorting  [-N <#samples>]
                        -s <bubble|insertion|merge|quick|selection|...>
                        [-m <swap-pitch|waveform>]
                        [-i <random|sine|falling-edge>]
                        [--] [--version] [-h]

**DESCRIPTION**

 * `-N`, `--number <#samples>` Number of samples to sort. This
   defaults to 50. For efficient algorithms (quick or merge sort) you may want
   to up this.
 * `-s`, `--sort <bubble|insertion|...>` The sorting algorithm that is used.
   This argument is non-optional. Or is it?
 * `-m`, `--mode <swap-pitch|waveform>` Choose the scheme to be used for the
   audibilization. In `swap-pitch` mode, each time two elements are swapped
   (or each time they are compared, depending on the algorithm), a superposition
   of two beeps is played whose frequency (pitch) corresponds to the value or
   elements being swapped (compared). In `waveform` mode, the current state of
   the complete (partially) sorted array will be used as a waveform that is
   repeatedly looped while its altered by the algorithm. At the end when the
   array is (almost) sorted, it will resemble a sawtooth wave. _Turn down your
   volume before trying this mode!_
 * `-i`, `--input <random|sine|falling-edge>` Specify the input array that the
   sorting algorithm is to be applied to. `random` initializes the array with
   random values (different each time.) `sine` starts out from a sinsodial wave.
   This is especially fun when used in `waveform` mode as the pure tone will be
   transformed to a sawtooth wave. `falling-edge` starts out from a sawtooth
   wave with a falling-edge, i.e. the elements are sorted in decreasing order.
   For the present implementation of quick sort, this is the worst case.
 * `-h`, `--help` Print a help message to remind you of these options.
 * `No arguments at all`: You'll have to find out.

## Contribute more sorting algorithms

You are welcome to add further sorting algorithms! Obvious choices would be
heap sort (hard) and radix sort (easy). The program is designed to be extended it
with new sorting algorithms in a modular way. Use one of the existing algorithms as
a template (preferrably one of the simple ones like `sorts/insertion_sort.hpp`).
Each algorithm defines its own namespace through which it exposes its `sort`
function. Two iterators marking the begin and end of the sequence to be sorted
are passed in. No access to the container itself should be needed. The
container's type is used as the template parameter though; the iterator type
`T::iterator` or value type `T::value_type` may be derived from it as necessary.

Additional, `sort` is passed a callback function of type `callback_type<T>`.
This callback is what triggers the production of actual sounds.
Call it whenever you operate on two elements of the array in a
characteristic way. Ususally, this would be when you either swap two elements or
when you compare two element. As a rule of thumb, the number of `callback` calls
should scale according to the complexity of the sorting algorithm. Pass `callback`
the two _values_ (not the iterators) of the elements being operated on.

New sorting algorithms need to be `#include`'d and added to a `std::map` in the
header `sort.hpp`. The two positions are marked with comments. No modification
of any other sources should be required.

Feel free to fork this repository and submit a pull request.

### Testing new sorting algorithms

To ensure the algorithm works properly, after adding to `sorts.hpp`, you should
compile the test program using `make test` and run it (`./test`). It'll attempt
to sort a random sequence of integers with each of the sorting algorithms and
compare the results with that from using `std::sort`. It's also useful for
debugging.
