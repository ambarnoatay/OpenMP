# OpenMP
To understand and apply the principles of OpenMP for parallel calculations in a computationally-intensive problem related to the Electric Field calculations.
Create an application that uses OpenMP for multithreading to calculate the electric field at a point in space.
• Your program should query the user for how many threads to run concurrently when doing the calculation.
• Your program needs to query the user for the size of the N x M array and the charge q.
• Your program should continuously prompt the user if a new calculation is wanted.
• Your program should use OpenMP to reduce the processing time taken.
• After calculating the resultant electric field your program should output the Ex, Ey, Ez values and the resultant electric field strength.
• Your program should also output how long it takes in microseconds from the time the user inputs the xyz location to just before the results are output to the screen.
• Spaces separate the values when entering multiple inputs.
• Make sure you check for valid inputs
o N and M should be natural numbers.
o Separation distances should be > 0.0 and be valid numerical values
o Charge should be a valid numerical value.
o Point location should be made up of valid numerical values.
• Make sure that your code checks for a location that may be the same as a point charge location.

