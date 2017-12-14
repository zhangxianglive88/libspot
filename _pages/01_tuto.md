---
layout: tuto
title: Tutorial
permalink: /tuto/
---

<a name="intro"></a>
<a name="buildspot"></a>
### INTRODUCTION
**libspot** is based on the core algorithm **SPOT** which detects anomalies. This algorithm is a part of the main object `Spot`.

### CONSTRUCT A SPOT OBJECT

Here is the full constructor of the `SPOT` object:

```cpp
	Spot(double q, int n_init, double level, 
		bool up, bool down, bool alert, 
		bool bounded, int max_excess);
```

The main parameter is `q`. It defines the probability of an abnormal event. For example, if `q = 0.001`, it means that the algorithm will consider events with a probability lower than `q` as abnormal.

The parameters `n_init` and `level` are involved in the calibration step of the algorithm. `n_init` is the number of data required to calibrate and `level` is the proportion of these initial data not involved in the tail distribution fit. For example, let us use `n_init = 1000` and `level = 0.99`. The algorithm will drop the 990 lowest data and will keep the 10 highest to make a first fit. 

<div class="alert">
In practice, the user must ensure that <code>n_init * (1 - level) > 10</code> to have enough data for the fit (at least 10). Furthermore, the <code>level</code> must be high enough to reduce the bias of the algorithm. We may advice to have the <code>level</code> as close to 1 as possible.
</div>

The booleans `up` and `down` define whether the algorithm computes upper/lower thresholds or not.

The boolean `alert = true` enables alert triggering. If `alert = false`, it means that all the data (even those out of the bounds) will be taken into account.

The boolean `bounded = true` enables the memory bounding. The bound is given by `max_excess` which is the maximum number of data stored to perform the tail fit.
<a name="buildotherspot"></a>
<div class="tip">
	If the memory is not bounded, the algorithm will store more and more data to perform the tail fit. However, the computation time will cause more problems than the memory growth because the tail fit complexity depends on this number of stored data.
</div>


### OTHER CONSTRUCTORS
**libspot** provides other constructors. For instance rather than specifying `n_init` (the number of data to calibrate), the user can provide an initial batch of data `init_data`. This batch will directly be used to perform the calibration.

```cpp
	Spot(double q, vector<double> init_data, double level, 
	     bool up, bool down, bool alert, 
	     bool bounded, int max_excess);
```
Finally, shorter constructors can also be used:
```cpp
	Spot(double q = 1e-3, int n_init = 1000);
	Spot(double q, vector<double> init_data);
```


The default values of the parameters are the following:
<a name="runspot"></a>
```cpp
	q = 1e-3;		down = true;
	n_init = 1000;		alert = true;
	level = 0.99;		bounded = true;
	up = true;		max_excess = 200;
```


### RUN SPOT

Once a `SPOT` object is created you can feed it with data through the `step` method.
```cpp
	int Spot.step(double x)
```
According to the state of the algorithm or the detected event, this method returns a corresponding integer.
<center>
<table align="center" width="40%" style="margin: auto auto;">
   <TR ALIGN="CENTER">
		<TD width="20%"><h3>Code</h3></TD>
		<TD width="80%"><h3>Meaning</h3></TD>
   </TR>
   <TR ALIGN="CENTER">
		<TD width="20%">0</TD>
		<TD width="80%">Normal data</TD>
   </TR>
   <TR ALIGN="CENTER">
		<TD width="20%">-1/1</TD>
		<TD width="80%">Down/Up alert</TD>
   </TR>
   <TR ALIGN="CENTER">
		<TD width="20%">-2/2</TD>
		<TD width="80%">Down/Up excess</TD>
   </TR>
   <TR ALIGN="CENTER">
		<TD width="20%">3</TD>
		<TD width="80%">Data for inital batch</TD>
   </TR>
   <TR ALIGN="CENTER">
		<TD width="20%">4</TD>
		<TD width="80%">Calibration step</TD>
   </TR>
</table>
</center>

<a name="example"></a>
### EXAMPLE

Here we give an example where Spot is applied on a Gaussian white noise of 10000 values.
```cpp
// main.cpp
#include "spot.h"
#include <random>
#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>

using namespace std;

vector<double> gaussian_white_noise(double mu, double sigma, int N)
{
	vector<double> v(N);
	random_device rd;
	default_random_engine gen(rd());
	normal_distribution<double> gaussian(mu,sigma);

	for (int i = 0; i < N; i++)
	{
		v[i] = gaussian(gen);
	}
	return v;
}

int main(int argc, const char * argv[])
{
	int N = 10000;
	vector<double> data = gaussian_white_noise(0,1,N);
	Spot S;
	int output;
	for(auto & x : data)
	{
		output = S.step(x);
		if (abs(output) == 1)
		{
			cout << "Alarm!" << endl;
		}
	}
	return 0;
}
```


