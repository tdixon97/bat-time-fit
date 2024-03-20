## bat-time-fit

### Author: Toby Dixon (toby.dixon.23@ucl.ac.uk)

This code provides a fit of the rate of events to an analytic function.
Given a set of "runs" (time-intervals) where for each `N_i` events where measured we can model the data as:

$$ r(t)=f(t;\vec{\theta})M(t),$$

where $r(t)$ is the rate of measured as a function of time, $f(t)$ is the rate (counts/time) with some parameters $\theta$, and $M(t)is the active mass as a function of time.
Thus the predicton for a time interval $t_i$ to $t_{i+1}$ is (assuming $M(t)$ is consant over this interval)

$$N_{\text{pred}}=M(t_i)\int_{t_i}^{t_i+1}f(t)dt = M(t_i)[F(t_{i+1})-F(t_i)],$$

where $F(t)$ is the cummulative distribution function (CDF) of $f(t)$.

The likelihood is given by a product of Poisson distributions:

$$\mathcal{L}=\prod_{i=1}^N \text{Pois}\Big(N_i|M(t_i)\int_{t_i}^{t_i+1}r(t)dt\Big)$$

This code perform this fit using BAT.
The user should supply
* The CDF of the function to fit (currently an exponenital is hardcoded to be updated),
* A histogram of the active mass in each run,
* A histogram of the counts in each run.

The code will then compute the posterior distribution on the parameters of the function $F$.
Currently the code is run by compiling with `make` (or `make clean` then `make`) and then running with:

    Usage: ./runexpo_fit -i <inputfile> -o <outputDir> 

Where `<inputfile>` is a ROOT file containing two histograms `counts` and `mass`, this is produced by the python script [time-analysis.py in legend-bkg-scripts](https://github.com/tdixon97/legend-bkg-scripts/).
This will then save the BAT output files in `<outputDir>` this files can be read back into `legend-bkg-scripts` to plot the fit result.

