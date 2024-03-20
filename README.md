## bat-time-fit

### Author: Toby Dixon (toby.dixon.23@ucl.ac.uk)

This code provides a fit of the rate of events to an analytic function.
Given a set of "runs" (time-intervals) where for each `N_i` events where measured we can model the data as:

$$ r(t)=f(t;\vec{\theta})M(t),$$
where $r(t)$ is the rate of measured as a function of time, $f(t)$ is the rate (counts/time) with some parameters $\theta$, and $M(t)is the active mass as a function of time.
The likelihood is given by a product of Poisson distributions:

$$\mathcal{L}=\prod_{i=1}^N \text{Pois}(N_i|\int_{t_i}^{t_i+1}r(t)dt)$$

This code perform this fit using BAT, currently only an exponetial is implemented.